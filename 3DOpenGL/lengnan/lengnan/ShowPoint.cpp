// ShowPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "ShowPoint.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Lena.h"
#include "ADOConn.h"


 UINT dis_dataptr=0;
 extern int sensors_num;
 extern int ShowPoint[1000];
 LONG starttime;
 
 //当前实验进行了多少秒
 extern int nowtime[10000];

 int m_tempValue;
//单点曲线显示线程
 CWinThread* ThreadShowLine;
 CWinThread* ThreadShowPoint;

extern double DatatNum[1000][10000];
 int ThreadShowPointFlag=0;
 int ThreadShowLineFlag=0;

 CEvent ShowLineEvent;
 CEvent ShowPointTEvent;

 CWnd* StaticWnd;
 int bShow=0;
 //CShowPoint 对话框

IMPLEMENT_DYNAMIC(CShowPoint, CDialogEx)


CShowPoint::CShowPoint(CWnd* pParent /*=NULL*/)
	: CDialogEx(CShowPoint::IDD, pParent)
	, m_Point(0)
	, m_timeintervalue(50)
{
	point_num=0;
}

CShowPoint::~CShowPoint()
{
}

void CShowPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Point);
	DDX_Control(pDX, IDC_LIST1, m_List);
	DDX_Text(pDX, IDC_EDIT2, m_timeintervalue);
}


BEGIN_MESSAGE_MAP(CShowPoint, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CShowPoint::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CShowPoint::OnBnClickedButton2)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON3, &CShowPoint::OnBnClickedButton3)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON4, &CShowPoint::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CShowPoint::OnBnClickedButton5)
	ON_BN_CLICKED(IDOK, &CShowPoint::OnOK)
// 	ON_BN_CLICKED(IDCANCEL, &CShowPoint::OnCancel)
ON_EN_CHANGE(IDC_EDIT2, &CShowPoint::OnEnChangeEdit2)
END_MESSAGE_MAP()


// CShowPoint 消息处理程序


void CShowPoint::OnBnClickedButton1()//增
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CString tempstr;
	tempstr.Format(_T("%d"),point_num+1);
	m_List.InsertItem(point_num,tempstr);
	tempstr.Format(_T("%d"),m_Point);
	m_List.SetItemText(point_num,1,tempstr);
	ShowPoint[m_Point-1]=1;
	point_num++;
}
void CShowPoint::OnBnClickedButton2()//减
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	for (int i=0;i<m_List.GetItemCount();i++)
	{
		if (m_List.GetItemState(i,LVIS_SELECTED)==LVIS_SELECTED)
		{
			CString str=m_List.GetItemText(i,1);
			//AfxMessageBox(str);
			int j=_wtoi(str);
			ShowPoint[j-1]=0;
			m_List.DeleteItem(i);
			point_num--;
		}
		
	}
}
BOOL CShowPoint::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	TODO:  在此添加额外的初始化
		//控件初始化
		LONG lStyle;
		lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);//获取当前窗口style
		lStyle &= ~LVS_TYPEMASK; //清除显示方式位
		lStyle |= LVS_REPORT; //设置style
		SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);//设置style
	
		DWORD dwStyle;
		dwStyle =m_List.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
		dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
		//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
		dwStyle	|= LVS_EX_ONECLICKACTIVATE;//单击激活
		dwStyle	|= LVS_EX_TRACKSELECT;//自动换行
		m_List.SetExtendedStyle(dwStyle); //设置扩展风格
		m_List.InsertColumn(0,_T("序号"),LVCFMT_LEFT,60);
		m_List.InsertColumn(1,_T("值"),LVCFMT_LEFT,70);
		return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
void CShowPoint::OnClose()
{
	

	bShow=0;
	dis_dataptr=0;
	if (ThreadShowLineFlag)
	{
		ThreadShowLine->SuspendThread();
	}
	if (ThreadShowPointFlag)
	{
		ThreadShowPoint->SuspendThread();
	}	
	for (int i=0;i<1000;i++)
	{
		ShowPoint[i]=0;
		for (int j=0;j<10000;j++)
		{
			DatatNum[i][j]=0;
		}
	}
	CDialog::OnClose();  
}
// 曲线绘制函数
void CShowPoint::fndiscurve(double  maxvalue,double  internaltime,CWnd* Ctlwnd)
{
	CDC* Pdc=Ctlwnd->GetDC();
	Pdc->SetBkMode(TRANSPARENT);
	CRect mydlgrect;
	double  xrt=0,yrt=0;
	CBrush mybrush(RGB(125,125,125)),*oldbrush;
	CPen mypen1,mypen2,*oldpen;
	CFont myfont,*oldfont;
	CString tempstr;
	CRect temprect;
	Ctlwnd->GetClientRect(&mydlgrect);
	double  volrt,voltimrt;
	CRect volrect(mydlgrect.left+45,mydlgrect.top+40,mydlgrect.right-5,mydlgrect.bottom-15);
	CRect redrawrect(mydlgrect.left+5,mydlgrect.top+35,mydlgrect.right,mydlgrect.bottom);
	//计算装换系数
	volrt=(volrect.bottom-volrect.top)*1.0/maxvalue;
	voltimrt=(volrect.right-volrect.left)*1.0/internaltime;
	//绘制显示框
	myfont.CreatePointFont(90, _T("Arial"), NULL);
	oldfont=Pdc->SelectObject(&myfont);
	//填充底色
	Pdc->SelectObject(&mybrush);
	Pdc->FillRect(redrawrect,&mybrush);
	//画出显示框
	mypen1.CreatePen(PS_SOLID,1,RGB(0,255,0));
	mypen2.CreatePen(PS_DOT,1,RGB(0,255,0));
	oldpen=Pdc->SelectObject(&mypen1);
	//压力显示框
	Pdc->MoveTo(volrect.left,volrect.bottom);
	Pdc->LineTo(volrect.right,volrect.bottom);
	Pdc->LineTo(volrect.right,volrect.top);
	Pdc->LineTo(volrect.left,volrect.top);
	Pdc->LineTo(volrect.left,volrect.bottom);
	//压力显示框画边刻度框
	xrt=(volrect.right-volrect.left+2)*1.0/30;
	yrt=(volrect.bottom-volrect.top+2)*1.0/30;
	for (int i=0;i<=30;i++)
	{
		Pdc->MoveTo(volrect.left+int(i*xrt),volrect.bottom);
		Pdc->LineTo(volrect.left+int(i*xrt),volrect.bottom-3);
// 		if (i%2==0)
// 		{
// 
// 			tempstr.Format(_T("-%.0f"),internaltime*(1-1.0*i/30));
// 			Pdc->TextOutW(volrect.left-tempstr.GetLength()+int(i*xrt),volrect.bottom+20,tempstr);
// 		}
	}
	for (int i=0;i<=30;i++)
	{
		Pdc->MoveTo(volrect.left,volrect.top+int(i*yrt));
		Pdc->LineTo(volrect.left+3,volrect.top+int(i*yrt));
		if (i%2==0)
		{
			if (maxvalue>10)
			{
				tempstr.Format(_T("%.2f"),1.0*i*maxvalue/30);
			}
			else
			{
				tempstr.Format(_T("%.3f"),1.0*i*maxvalue/30);
			}
			Pdc->TextOutW(volrect.left-7*tempstr.GetLength(),volrect.bottom-int(i*yrt)-5,tempstr);
		}

	}
	//绘制粘度框刻度线
	Pdc->SelectObject(&mypen2);
	xrt=(volrect.right-volrect.left+2)*1.0/30;
	yrt=(volrect.bottom-volrect.top+2)*1.0/30;
	for (int i=0;i<=30;i++)
	{
		Pdc->MoveTo(volrect.left+int(i*xrt),volrect.bottom);
		Pdc->LineTo(volrect.left+int(i*xrt),volrect.top);
	}
	for (int i=0;i<=30;i++)
	{
		Pdc->MoveTo(volrect.left,volrect.top+int(i*yrt));
		Pdc->LineTo(volrect.right,volrect.top+int(i*yrt));
	}
	//恢复初始设置
	Pdc->SelectObject(&oldbrush);
	Pdc->SelectObject(&oldpen);
	Pdc->SelectObject(&oldfont);
	mypen1.DeleteObject();
	mypen2.DeleteObject();
	myfont.DeleteObject();
	Ctlwnd->ReleaseDC(Pdc);
}

// 定义曲线绘制函数
void CShowPoint::fndrawcurve(double  maxvalue, double  internaltime,double  data[],CWnd* Ctlwnd,unsigned short* color)
{
	CPen mypen,*oldpen;
	CRect mydlgrect;
	CDC* Pdc=Ctlwnd->GetDC();
	double  tempvalue; 
	double  volrt,voltimrt;
	//选择画笔
	mypen.CreatePen(PS_SOLID,2,RGB(color[0],color[1],color[2]));
	oldpen=Pdc->SelectObject(&mypen);
	//进行数据的显示
	Ctlwnd->GetClientRect(&mydlgrect);
	//确定重画区
	CRect volrect(mydlgrect.left+45,mydlgrect.top+40,mydlgrect.right-5,mydlgrect.bottom-15);
	//计算转换系数
	volrt=(volrect.bottom-volrect.top)*1.0/maxvalue;
	voltimrt=(volrect.right-volrect.left)*1.0/internaltime;
	UINT dis_zero=(dis_dataptr-1+MAXNUM)%MAXNUM;
	//绘制粘度曲线
	tempvalue=data[dis_zero];
	if (tempvalue==-1000)
	{
		return;
	}
	tempvalue=(tempvalue>maxvalue?maxvalue:tempvalue);
	tempvalue=(tempvalue>0?tempvalue:0);
	Pdc->MoveTo(volrect.right,volrect.bottom-int(tempvalue*volrt));
	for (int i=1;i<MAXNUM;i++)
	{
		bool gotoend=false;
		//向前寻找
		int num=(dis_zero-i+MAXNUM)%MAXNUM;
		//时间有效性判断
		if (nowtime[num]==-1000)
		{
			return;
		}
		//x轴边界判断
		int next_x=volrect.right-int((nowtime[dis_zero]-nowtime[num])*voltimrt);
		next_x=(next_x<volrect.left?volrect.left:next_x);
		if (next_x==volrect.left)
		{
			gotoend=true;
		}
		next_x=(next_x>volrect.right?volrect.right:next_x);
		//数值有效性判断
		if (data[num]==-1000)
		{
			return;
		}
		//y值计算
		int next_y=volrect.bottom-int(data[num]*volrt);
		next_y=(next_y<volrect.top?volrect.top:next_y);
		next_y=(next_y>volrect.bottom?volrect.bottom:next_y);
		Pdc->LineTo(next_x,next_y);
		if (gotoend)
		{
			break;
		}
	}
	Pdc->SelectObject(oldpen);
	mypen.DeleteObject();
	Ctlwnd->ReleaseDC(Pdc);
}


void CShowPoint::OnPaint()
{
	CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
	int m_maxp=pMainFrame->pLenaView->m_temmax;
	
	fndiscurve(m_maxp,360,GetDlgItem(IDC_STATIC));
	CDialog::OnPaint();  
}


 UINT CShowPoint::ShowLine(LPVOID pPrama)
 {
 	CShowPoint* pmydlg=(CShowPoint *)pPrama;
 	while(bShow)
 	{
 		ShowLineEvent.Lock();
 		CMainFrame* pMainFrame=(CMainFrame*)AfxGetApp()->GetMainWnd();
 		int m_maxp=pMainFrame->pLenaView->m_temmax;
 		pmydlg->fndiscurve(m_maxp,360,StaticWnd);
 		for (int i=0;i<sensors_num;i++)
 		{
 			if (ShowPoint[i])
 			{
 				unsigned short color[3];
				pmydlg->ChangeValue2Color(25,1,i,color);
				pmydlg->fndrawcurve(m_maxp,360,DatatNum[i],StaticWnd,color);
 			}	
 		}
 	}
	return 1;
}
BOOL CShowPoint::ChangeValue2Color(float Max,float Min,float currvalue,unsigned short* color)
{
 	if (Max<Min)
 	{
 		return FALSE;
 	}
 	if (currvalue>=Max)
 	{
 		color[0]=255;
 		color[1]=0;
 		color[2]=0;
 		return TRUE;
 	}
 	if (currvalue<=Min)
 	{
 		color[0]=0;
 		color[1]=0;
 		color[2]=255;
 		return TRUE;
 	}
 	float curr=currvalue-Min;
 	float range=Max-Min;
 	if (curr>range*3/4)
 	{
 		color[0]=255;
 		color[1]=(range-curr)*4/range*255;
 		color[2]=0;
 		return TRUE;
 	}
 	if (curr<=range*3/4&&curr>range/2)
 	{
 		color[0]=((curr-range/2)*4/range)*255;
 		color[1]=255;
 		color[2]=0;
 		return TRUE;
 	}
 	if (curr<=range/2&&curr>range/4)
 	{
 		color[0]=0;
 		color[1]=255;
 		color[2]=((range/2-curr)*4/range)*255;
 		return TRUE;
 	}
 	if (curr<=range/4)
 	{
 		color[0]=0;
 		color[1]=(curr*4/range)*255;
 		color[2]=255;
 		return TRUE;
 	}
 	return TRUE;
}

void CShowPoint::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	CDialogEx::OnOK();
}


void CShowPoint::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (ThreadShowPointFlag)
	{
		ThreadShowPoint->SuspendThread();
		ThreadShowLine->SuspendThread();
	}
	UpdateData(TRUE);
	StaticWnd=GetDlgItem(IDC_STATIC);
	bShow=1;
	m_tempValue=m_timeintervalue;
	ThreadShowPoint=AfxBeginThread(ShowPointT,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	ThreadShowPointFlag=1;
	ThreadShowLine=AfxBeginThread(ShowLine,this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	ThreadShowLineFlag=1;
	starttime=GetTickCount();
	
	
}
UINT CShowPoint::ShowPointT(LPVOID pPrama)
{
	//CShowPoint* pmydlg=(CShowPoint*)pPrama;
	//pmydlg->UpdateData(TRUE);
	//LONG tcurrent=0;
	//LONG tim=0;
	//if (!sensors_num)
	//{
	//	AfxMessageBox(_T("请先载入配置文件"));
	//	return 0;
	//}

	//CFileDialog filedlg(TRUE,_T(".accdb"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("实验数据 (*.accdb;*.mdb)|*.accdb;*.mdb|All Files(*.*)|*.*||"),0);
	//if (IDOK==filedlg.DoModal())
	//{
	//	memset(DatatNum,0,sizeof(DatatNum));
	//	//unsigned int i=sizeof(DatatNum);
	//	
	//	dis_dataptr=0;
	//	CString sDriver;
	//	CString dbName;
	//	CString tempstr,datatempstr;
	//	//连接数据库
	//	CADOConn adoconn;
	//	_bstr_t bstrSQL;
	//	_RecordsetPtr m_pRecordset;//指向Recordset对象指针
	//	_RecordsetPtr m_pRecordset1;
	//	LONG tim=0;
	//	int index;
	//	dbName=filedlg.GetPathName();
	//	if (adoconn.m_pConnection==NULL)
	//	{
	//		adoconn.OninitADOConn(dbName,_T(""),_T(""));
	//	}
	//	bstrSQL=_T("select *from [传感器数据]");
	//	//打开并获取记录集
	//	try{
	//		if (adoconn.m_pConnection==NULL)
	//		{
	//			adoconn.OninitADOConn(tempstr,_T(""),_T(""));
	//		}
	//		m_pRecordset=adoconn.GetRecordSet(bstrSQL);
	//		//创建记录集对象
	//		m_pRecordset.CreateInstance(__uuidof(Recordset));
	//		//取得表中记录
	//		m_pRecordset->Open(bstrSQL,adoconn.m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);

	//	}
	//	catch(_com_error e)
	//	{
	//		AfxMessageBox(e.Description());
	//		return 0;
	//	}
	//	if(sensors_num>250)
	//	{
	//		bstrSQL=_T("select *from [传感器数据续]");
	//		//打开并获取记录集
	//		try{
	//			if (adoconn.m_pConnection==NULL)
	//			{
	//				adoconn.OninitADOConn(tempstr,_T(""),_T(""));
	//			}
	//			m_pRecordset1=adoconn.GetRecordSet(bstrSQL);
	//			//创建记录集对象
	//			m_pRecordset1.CreateInstance(__uuidof(Recordset));
	//			//取得表中记录
	//			m_pRecordset1->Open(bstrSQL,adoconn.m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);

	//		}
	//		catch(_com_error e)
	//		{
	//			AfxMessageBox(e.Description());
	//			return 0;
	//		}
	//	}

	//	datatempstr=m_pRecordset->GetCollect(_T("序号"));
	//	index=_wtoi(datatempstr);
	//	if(sensors_num<251)
	//	{
	//		int i=0;
	//		while (!m_pRecordset->adoEOF)
	//		{
	//			tim=GetTickCount();
	//			for(int i=0;i<sensors_num;i++)
	//			{
	//				tempstr.Format(_T("采集点%d"),i+1);
	//				datatempstr=m_pRecordset->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}
	//			//写入采集时间
	//			//nowtime[dis_dataptr]=(GetTickCount()-starttime)/1000;
	//			nowtime[dis_dataptr]=i;
	//			//更新数据游标
	//			dis_dataptr=(dis_dataptr+1)%4096;
	//			//启用画线线程
	//			ShowLineEvent.SetEvent();

	//			tcurrent=GetTickCount();
	//			if ((tcurrent-tim)/1000.0<0.5)
	//			{
	//				Sleep(m_tempValue-tcurrent+tim);
	//			}
	//			i++;
	//			m_pRecordset->MoveNext();
	//		}


	//	}else{
	//		while (!m_pRecordset->adoEOF)
	//		{
	//			for(int i=0;i<250;i++)
	//			{
	//				tempstr.Format(_T("采集点%d"),i+1);
	//				datatempstr=m_pRecordset->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}


	//			for(int i=250;i<sensors_num;i++)
	//			{
	//				tempstr.Format(_T("采集点%d"),i+1);
	//				datatempstr=m_pRecordset1->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}
	//			nowtime[dis_dataptr]=dis_dataptr;
	//			//更新数据游标
	//			dis_dataptr=(dis_dataptr+1)%4096;
	//			//启用画线线程
	//			ShowLineEvent.PulseEvent();
	//			m_pRecordset->MoveNext();
	//			m_pRecordset1->MoveNext();
	//		}
	//	}
	//	//关闭记录集
	//	if(m_pRecordset!=NULL)
	//	{
	//		m_pRecordset->Close();
	//		m_pRecordset=NULL;
	//		if (sensors_num>250)
	//		{
	//			m_pRecordset1->Close();
	//			m_pRecordset1=NULL;
	//		}	
	//	}
	//	//关闭连接
	//	adoconn.m_pConnection->Close();
	//	adoconn.m_pConnection=NULL;
	//	::CoUninitialize();

	//}
return 0;
}

void CShowPoint::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	ThreadShowPoint->SuspendThread();
}


void CShowPoint::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	ThreadShowPoint->ResumeThread();
}
void CShowPoint::OnOK()
{
	GetNextDlgTabItem(GetFocus())->SetFocus();
}
// void CShowPoint::OnCancel()
// {
// 	SendMessage(WM_CLOSE);
// }

void CShowPoint::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	m_tempValue=m_timeintervalue;
	// TODO:  在此添加控件通知处理程序代码
}

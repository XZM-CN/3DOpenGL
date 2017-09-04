// ShowPoint.cpp : ʵ���ļ�
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
 
 //��ǰʵ������˶�����
 extern int nowtime[10000];

 int m_tempValue;
//����������ʾ�߳�
 CWinThread* ThreadShowLine;
 CWinThread* ThreadShowPoint;

extern double DatatNum[1000][10000];
 int ThreadShowPointFlag=0;
 int ThreadShowLineFlag=0;

 CEvent ShowLineEvent;
 CEvent ShowPointTEvent;

 CWnd* StaticWnd;
 int bShow=0;
 //CShowPoint �Ի���

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


// CShowPoint ��Ϣ�������


void CShowPoint::OnBnClickedButton1()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CString tempstr;
	tempstr.Format(_T("%d"),point_num+1);
	m_List.InsertItem(point_num,tempstr);
	tempstr.Format(_T("%d"),m_Point);
	m_List.SetItemText(point_num,1,tempstr);
	ShowPoint[m_Point-1]=1;
	point_num++;
}
void CShowPoint::OnBnClickedButton2()//��
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

//	TODO:  �ڴ���Ӷ���ĳ�ʼ��
		//�ؼ���ʼ��
		LONG lStyle;
		lStyle = GetWindowLong(m_List.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
		lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
		lStyle |= LVS_REPORT; //����style
		SetWindowLong(m_List.m_hWnd, GWL_STYLE, lStyle);//����style
	
		DWORD dwStyle;
		dwStyle =m_List.GetExtendedStyle();
		dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
		dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
		//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
		dwStyle	|= LVS_EX_ONECLICKACTIVATE;//��������
		dwStyle	|= LVS_EX_TRACKSELECT;//�Զ�����
		m_List.SetExtendedStyle(dwStyle); //������չ���
		m_List.InsertColumn(0,_T("���"),LVCFMT_LEFT,60);
		m_List.InsertColumn(1,_T("ֵ"),LVCFMT_LEFT,70);
		return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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
// ���߻��ƺ���
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
	//����װ��ϵ��
	volrt=(volrect.bottom-volrect.top)*1.0/maxvalue;
	voltimrt=(volrect.right-volrect.left)*1.0/internaltime;
	//������ʾ��
	myfont.CreatePointFont(90, _T("Arial"), NULL);
	oldfont=Pdc->SelectObject(&myfont);
	//����ɫ
	Pdc->SelectObject(&mybrush);
	Pdc->FillRect(redrawrect,&mybrush);
	//������ʾ��
	mypen1.CreatePen(PS_SOLID,1,RGB(0,255,0));
	mypen2.CreatePen(PS_DOT,1,RGB(0,255,0));
	oldpen=Pdc->SelectObject(&mypen1);
	//ѹ����ʾ��
	Pdc->MoveTo(volrect.left,volrect.bottom);
	Pdc->LineTo(volrect.right,volrect.bottom);
	Pdc->LineTo(volrect.right,volrect.top);
	Pdc->LineTo(volrect.left,volrect.top);
	Pdc->LineTo(volrect.left,volrect.bottom);
	//ѹ����ʾ�򻭱߿̶ȿ�
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
	//����ճ�ȿ�̶���
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
	//�ָ���ʼ����
	Pdc->SelectObject(&oldbrush);
	Pdc->SelectObject(&oldpen);
	Pdc->SelectObject(&oldfont);
	mypen1.DeleteObject();
	mypen2.DeleteObject();
	myfont.DeleteObject();
	Ctlwnd->ReleaseDC(Pdc);
}

// �������߻��ƺ���
void CShowPoint::fndrawcurve(double  maxvalue, double  internaltime,double  data[],CWnd* Ctlwnd,unsigned short* color)
{
	CPen mypen,*oldpen;
	CRect mydlgrect;
	CDC* Pdc=Ctlwnd->GetDC();
	double  tempvalue; 
	double  volrt,voltimrt;
	//ѡ�񻭱�
	mypen.CreatePen(PS_SOLID,2,RGB(color[0],color[1],color[2]));
	oldpen=Pdc->SelectObject(&mypen);
	//�������ݵ���ʾ
	Ctlwnd->GetClientRect(&mydlgrect);
	//ȷ���ػ���
	CRect volrect(mydlgrect.left+45,mydlgrect.top+40,mydlgrect.right-5,mydlgrect.bottom-15);
	//����ת��ϵ��
	volrt=(volrect.bottom-volrect.top)*1.0/maxvalue;
	voltimrt=(volrect.right-volrect.left)*1.0/internaltime;
	UINT dis_zero=(dis_dataptr-1+MAXNUM)%MAXNUM;
	//����ճ������
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
		//��ǰѰ��
		int num=(dis_zero-i+MAXNUM)%MAXNUM;
		//ʱ����Ч���ж�
		if (nowtime[num]==-1000)
		{
			return;
		}
		//x��߽��ж�
		int next_x=volrect.right-int((nowtime[dis_zero]-nowtime[num])*voltimrt);
		next_x=(next_x<volrect.left?volrect.left:next_x);
		if (next_x==volrect.left)
		{
			gotoend=true;
		}
		next_x=(next_x>volrect.right?volrect.right:next_x);
		//��ֵ��Ч���ж�
		if (data[num]==-1000)
		{
			return;
		}
		//yֵ����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CDialogEx::OnOK();
}


void CShowPoint::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	//	AfxMessageBox(_T("�������������ļ�"));
	//	return 0;
	//}

	//CFileDialog filedlg(TRUE,_T(".accdb"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("ʵ������ (*.accdb;*.mdb)|*.accdb;*.mdb|All Files(*.*)|*.*||"),0);
	//if (IDOK==filedlg.DoModal())
	//{
	//	memset(DatatNum,0,sizeof(DatatNum));
	//	//unsigned int i=sizeof(DatatNum);
	//	
	//	dis_dataptr=0;
	//	CString sDriver;
	//	CString dbName;
	//	CString tempstr,datatempstr;
	//	//�������ݿ�
	//	CADOConn adoconn;
	//	_bstr_t bstrSQL;
	//	_RecordsetPtr m_pRecordset;//ָ��Recordset����ָ��
	//	_RecordsetPtr m_pRecordset1;
	//	LONG tim=0;
	//	int index;
	//	dbName=filedlg.GetPathName();
	//	if (adoconn.m_pConnection==NULL)
	//	{
	//		adoconn.OninitADOConn(dbName,_T(""),_T(""));
	//	}
	//	bstrSQL=_T("select *from [����������]");
	//	//�򿪲���ȡ��¼��
	//	try{
	//		if (adoconn.m_pConnection==NULL)
	//		{
	//			adoconn.OninitADOConn(tempstr,_T(""),_T(""));
	//		}
	//		m_pRecordset=adoconn.GetRecordSet(bstrSQL);
	//		//������¼������
	//		m_pRecordset.CreateInstance(__uuidof(Recordset));
	//		//ȡ�ñ��м�¼
	//		m_pRecordset->Open(bstrSQL,adoconn.m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);

	//	}
	//	catch(_com_error e)
	//	{
	//		AfxMessageBox(e.Description());
	//		return 0;
	//	}
	//	if(sensors_num>250)
	//	{
	//		bstrSQL=_T("select *from [������������]");
	//		//�򿪲���ȡ��¼��
	//		try{
	//			if (adoconn.m_pConnection==NULL)
	//			{
	//				adoconn.OninitADOConn(tempstr,_T(""),_T(""));
	//			}
	//			m_pRecordset1=adoconn.GetRecordSet(bstrSQL);
	//			//������¼������
	//			m_pRecordset1.CreateInstance(__uuidof(Recordset));
	//			//ȡ�ñ��м�¼
	//			m_pRecordset1->Open(bstrSQL,adoconn.m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);

	//		}
	//		catch(_com_error e)
	//		{
	//			AfxMessageBox(e.Description());
	//			return 0;
	//		}
	//	}

	//	datatempstr=m_pRecordset->GetCollect(_T("���"));
	//	index=_wtoi(datatempstr);
	//	if(sensors_num<251)
	//	{
	//		int i=0;
	//		while (!m_pRecordset->adoEOF)
	//		{
	//			tim=GetTickCount();
	//			for(int i=0;i<sensors_num;i++)
	//			{
	//				tempstr.Format(_T("�ɼ���%d"),i+1);
	//				datatempstr=m_pRecordset->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}
	//			//д��ɼ�ʱ��
	//			//nowtime[dis_dataptr]=(GetTickCount()-starttime)/1000;
	//			nowtime[dis_dataptr]=i;
	//			//���������α�
	//			dis_dataptr=(dis_dataptr+1)%4096;
	//			//���û����߳�
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
	//				tempstr.Format(_T("�ɼ���%d"),i+1);
	//				datatempstr=m_pRecordset->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}


	//			for(int i=250;i<sensors_num;i++)
	//			{
	//				tempstr.Format(_T("�ɼ���%d"),i+1);
	//				datatempstr=m_pRecordset1->GetCollect((_variant_t)tempstr);
	//				DatatNum[i][dis_dataptr]=_wtof(datatempstr);
	//			}
	//			nowtime[dis_dataptr]=dis_dataptr;
	//			//���������α�
	//			dis_dataptr=(dis_dataptr+1)%4096;
	//			//���û����߳�
	//			ShowLineEvent.PulseEvent();
	//			m_pRecordset->MoveNext();
	//			m_pRecordset1->MoveNext();
	//		}
	//	}
	//	//�رռ�¼��
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
	//	//�ر�����
	//	adoconn.m_pConnection->Close();
	//	adoconn.m_pConnection=NULL;
	//	::CoUninitialize();

	//}
return 0;
}

void CShowPoint::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	ThreadShowPoint->SuspendThread();
}


void CShowPoint::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	UpdateData(TRUE);
	m_tempValue=m_timeintervalue;
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

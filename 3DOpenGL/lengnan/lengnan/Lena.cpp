// Lena.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Lena.h"
#include "lengnanView.h"
#include "AddIso.h"
#include "ShowPoint.h"

bool cutxflag=true;
bool cutyflag=true;
bool cutzflag=true;


bool numshowflag;//显示数据（true）,显示序号（false）
CLena* pLena;


extern GLint viewport[4]; 

extern int glvolume,glvolume_total;
extern float isosurface_data[10];
extern int isosurface_num;
//滑块裁剪模型在三轴上的值
float x_cutout=0,y_cutout=0,z_cutout=0;
//各个面的值
extern float z_down,z_up;//上、下
extern float y_front,y_back;//前、后
extern float x_left,x_right;//左、右

extern GLdouble eqnx[4];
extern GLdouble eqny[4];
extern GLdouble eqnz[4];
//消息控制标志位：
UINT CtrlFlag;
extern int nSlider;
// 温度最小值
extern int mymin;
// 温度最大值
extern int mymax;
int nradio;//选中的层
BOOL bperspective;//是否透视
BOOL bshow;//是否网格显示
int n3showmode;//显示模式，单层、多层、实体
int MulFloorFlag;	//多层显示时，控制显示的层数标志量
/////////保存图片标志
extern bool bpicture;
bool initflag(false);

int IsoSurface_num;
bool diswellflag;//显示放置井标志
extern int well_num;
float Alpha_Mixing;		//Alpha混合系数
// CLena

IMPLEMENT_DYNCREATE(CLena, CFormView)

CLena::CLena()
	: CFormView(CLena::IDD)
	, m_nSlider(80)
	, m_radio(0)
	, m_bshow(FALSE)
	, m_temmax(mymax)
	, m_temmin(mymin)
	, m_nSlider2(10)
	, m_nSlider3(20)
	, m_xmin(0)
	, m_ymin(0)
	, m_zmin(0)
	, m_diswellflag(FALSE)
{
	tempflag1=false;
	tempflag2=false;
	diswellflag=false;
}

CLena::~CLena()
{
}

void CLena::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER1, m_SliderCtrl);
	DDX_Slider(pDX, IDC_SLIDER1, m_nSlider);
	DDV_MinMaxInt(pDX, m_nSlider, 0, 100);
	DDX_Radio(pDX, IDC_RADIO1, m_radio);
	DDX_Check(pDX, IDC_SHOWMODE, m_bshow);
	DDX_Text(pDX, IDC_EDIT1, m_temmax);
	DDX_Text(pDX, IDC_EDIT2, m_temmin);
	DDX_Control(pDX, IDC_EDIT1, m_temedit1);
	DDX_Control(pDX, IDC_EDIT2, m_temedit2);
	DDX_Control(pDX, IDC_SLIDER2, m_SliderCtrl2);
	DDX_Slider(pDX, IDC_SLIDER2, m_nSlider2);
	DDV_MinMaxInt(pDX, m_nSlider2, 5, 25);
	DDX_Control(pDX, IDC_SLIDER3, m_SliderCtrl3);
	DDX_Slider(pDX, IDC_SLIDER3, m_nSlider3);
	DDX_Text(pDX, IDC_EDIT4, m_xmin);
	DDX_Text(pDX, IDC_EDIT5, m_ymin);
	DDX_Text(pDX, IDC_EDIT6, m_zmin);
	DDX_Control(pDX, IDC_SLIDER4, m_SliderCtrl4);
	DDX_Slider(pDX, IDC_SLIDER4, m_nSlider4);
	DDV_MinMaxInt(pDX, m_nSlider4, 0, 100);
	DDX_Control(pDX, IDC_SLIDER5, m_SliderCtrl5);
	DDX_Slider(pDX, IDC_SLIDER5, m_nSlider5);
	DDV_MinMaxInt(pDX, m_nSlider5, 0, 100);
	DDX_Control(pDX, IDC_SLIDER6, m_SliderCtrl6);
	DDX_Slider(pDX, IDC_SLIDER6, m_nSlider6);
	DDV_MinMaxInt(pDX, m_nSlider6, 0, 100);
	DDX_Control(pDX, IDC_SHOWDATA, m_ctrlcheck1);
	DDX_Control(pDX, IDC_SHOWDATA2, m_ctrlcheck2);
	DDX_Check(pDX, IDC_CHECK_DISWELL, m_diswellflag);
}

BEGIN_MESSAGE_MAP(CLena, CFormView)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CLena::OnNMCustomdrawSlider1)
	ON_BN_CLICKED(IDC_SHOWPOINT, &CLena::OnBnClickedShowpoint)
	ON_BN_CLICKED(IDC_SHOWDATA, &CLena::OnBnClickedShowdata)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO1, IDC_RADIO10, &CLena::OnRadio)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO17, IDC_RADIO19, &CLena::OnRadioShow)
	ON_BN_CLICKED(IDC_SHOWMODE, &CLena::OnBnClickedShowmode)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1, &CLena::OnDeltaposSpin1)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &CLena::OnDeltaposSpin2)
	ON_EN_CHANGE(IDC_EDIT1, &CLena::OnEnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, &CLena::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_CHECK1, &CLena::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CLena::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CLena::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CLena::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK5, &CLena::OnBnClickedCheck5)
	ON_BN_CLICKED(IDC_CHECK6, &CLena::OnBnClickedCheck6)
	ON_BN_CLICKED(IDC_SHOW_ISOSURFACE, &CLena::OnBnClickedShowIsosurface)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CLena::OnNMCustomdrawSlider2)
	ON_BN_CLICKED(IDC_CHECK_PERSPECTIVE, &CLena::OnBnClickedCheckPerspective)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER3, &CLena::OnNMCustomdrawSlider3)
	ON_BN_CLICKED(IDC_CHECK7, &CLena::OnBnClickedCheck7)
	ON_BN_CLICKED(IDC_CHECK8, &CLena::OnBnClickedCheck8)
	ON_BN_CLICKED(IDC_CHECK9, &CLena::OnBnClickedCheck9)
	ON_BN_CLICKED(IDC_CHECK10, &CLena::OnBnClickedCheck10)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SHOWDATA2, &CLena::OnBnClickedShowdata2)
	ON_BN_CLICKED(IDC_CHECK_DISWELL, &CLena::OnBnClickedCheckDiswell)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CLena 诊断

#ifdef _DEBUG
void CLena::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CLena::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CLena 消息处理程序

//void CLena::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl.GetPos();
//	//UpdateData(TRUE);
//	////UpdateWindow();
//	//nSlider=(int)(m_nSlider/6);
//	//pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
//	*pResult = 0;
//}

void CLena::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	pLena=this;
	CMDIFrameWnd *pFrame; 
	CMDIChildWnd *pChild; 
	//获取主窗口指针 
	pFrame =(CMDIFrameWnd*)AfxGetApp()->m_pMainWnd; 
	// 获取子窗口指针 
	pChild = (CMDIChildWnd *) pFrame->GetActiveFrame(); 
	//获取视类指针 
	pActiveView = pChild->GetActiveView(); 

	m_SliderCtrl.SetPos(m_nSlider);
	// TODO: 在此添加专用代码和/或调用基类
	GetDlgItem(IDC_SHOWDATA)->EnableWindow(FALSE);
	GetDlgItem(IDC_SHOWDATA2)->EnableWindow(FALSE);

	((CButton*)GetDlgItem(IDC_RADIO19))->SetCheck(1);
	n3showmode=2;
	bperspective=FALSE;
	((CButton*)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK2))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK3))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK4))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK5))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK6))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK7))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK8))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK9))->SetCheck(1);
	((CButton*)GetDlgItem(IDC_CHECK10))->SetCheck(1);

	GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_RADIO10)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK3)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK4)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK5)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK6)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK7)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK8)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK9)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK10)->ShowWindow(SW_HIDE);

	m_xmin=(float)m_nSlider4*(x_right-x_left)+x_left;
	m_ymin=(float)m_nSlider5*(y_back-y_front)+y_front;
	m_zmin=(float)m_nSlider6*(z_up-z_down)+z_down;

	//读ini文件
	CFile file;
	CFileException e;
	CString FileName = _T("lengnan.ini");
	CString FilePath;
	CString TcMax,TcMin;
	if(!file.Open(FileName, CFile::modeRead,&e))
	{
#ifdef _DEBUG
		afxDump<<"File could not be opened"<<e.m_cause<<"\n";
#endif
	}
	FilePath=file.GetFilePath();
	file.Close();

	::GetPrivateProfileString(_T("温度"),_T("最大值"),TcMax,TcMax.GetBuffer(MAX_PATH),MAX_PATH,FilePath);
	::GetPrivateProfileString(_T("温度"),_T("最小值"),TcMin,TcMin.GetBuffer(MAX_PATH),MAX_PATH,FilePath);

	mymax=_wtoi(TcMax);
	mymin=_wtoi(TcMin);

	CSpinButtonCtrl *pSpinCtrl1 = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN1);
	pSpinCtrl1->SetRange(0,1000);
	pSpinCtrl1->SetBuddy(GetDlgItem(IDC_EDIT1));
	pSpinCtrl1->SetBase(10);
	pSpinCtrl1->SetPos(mymax);
	CSpinButtonCtrl *pSpinCtrl2 = (CSpinButtonCtrl *)GetDlgItem(IDC_SPIN2);
	pSpinCtrl2->SetRange(-50,200);
	pSpinCtrl2->SetBuddy(GetDlgItem(IDC_EDIT2));
	pSpinCtrl2->SetBase(10);
	pSpinCtrl2->SetPos(mymin);

	m_SliderCtrl2.SetRangeMin(5);//设置控件最小值
	m_SliderCtrl2.SetRangeMax(25);//设置控件最大值,可用
	m_SliderCtrl2.SetPos(10);//设置初始位置

	m_SliderCtrl3.SetRangeMin(0);//设置控件最小值
	m_SliderCtrl3.SetRangeMax(100);//设置控件最大值,可用
	m_SliderCtrl3.SetPos(100);//设置初始位置

	MulFloorFlag=0xffff;

	UpdateData(TRUE);
	mymax=m_temmax;
	mymin=m_temmin;

	nSlider=(int)(m_nSlider/6);
	IsoSurface_num=m_nSlider2;
	Alpha_Mixing=(float)m_nSlider3/100;
}

void CLena::OnBnClickedShowpoint()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOWPOINT))
	
	{
		CtrlFlag|=0x01;
		GetDlgItem(IDC_SHOWDATA)->EnableWindow(TRUE);
		GetDlgItem(IDC_SHOWDATA2)->EnableWindow(TRUE);
		pActiveView->SendMessage(WM_PAINT);
	}else{
		CtrlFlag&=0xfffffffe;
		GetDlgItem(IDC_SHOWDATA)->EnableWindow(FALSE);
		GetDlgItem(IDC_SHOWDATA2)->EnableWindow(FALSE);
		pActiveView->SendMessage(WM_PAINT);
	}	
}

void CLena::OnBnClickedShowdata()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOWDATA))
	{
		numshowflag=true;
		m_ctrlcheck2.SetCheck(0);
		UpdateData(FALSE);
		CtrlFlag|=0x02;
		pActiveView->SendMessage(WM_PAINT);
	}else if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOWDATA2))
	{
		numshowflag=false;
		CtrlFlag|=0x02;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		CtrlFlag&=0xfffffffd;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnRadio(UINT uID)
{
	UINT nID=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO10);
	switch(nID)
	{
	case IDC_RADIO1:
		m_radio=0;

		break;
	case IDC_RADIO2:
		m_radio=1;

		break;
	case IDC_RADIO3:
		m_radio=2;

		break;
	case IDC_RADIO4:
		m_radio=3;

		break;
	case IDC_RADIO5:
		m_radio=4;

		break;
	case IDC_RADIO6:
		m_radio=5;

		break;
	case IDC_RADIO7:
		m_radio=6;

		break;
	case IDC_RADIO8:
		m_radio=7;

		break;
	case IDC_RADIO9:
		m_radio=8;

		break;
	case IDC_RADIO10:
		m_radio=9;

		break;
	default:
		break;
	}
	nradio=m_radio;
	pActiveView->SendMessage(WM_PAINT);
}
void CLena::OnBnClickedShowmode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (IsDlgButtonChecked(IDC_SHOWMODE)==BST_CHECKED)
	{
		//网格线显示
		m_bshow=TRUE;
	}else{
		//曲面显示
		m_bshow=FALSE;
	}
	bshow=m_bshow;
	pActiveView->SendMessage(WM_PAINT);
}

void CLena::OnRadioShow(UINT ucmdID)
{
	UINT nID=GetCheckedRadioButton(IDC_RADIO17,IDC_RADIO19);
	switch(nID)
	{
	case IDC_RADIO19:
		n3showmode=2;
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK10)->ShowWindow(SW_HIDE);
		break;
	case IDC_RADIO18:
		n3showmode=1;
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_RADIO10)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK10)->ShowWindow(SW_SHOW);
		break;
	case IDC_RADIO17:
		n3showmode=0;
		GetDlgItem(IDC_RADIO1)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO2)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO3)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO4)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO5)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO6)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO7)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO8)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO9)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_RADIO10)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_CHECK1)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK2)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK3)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK4)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK5)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK6)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK7)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK8)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK9)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CHECK10)->ShowWindow(SW_HIDE);
		break;
	default:
		break;
	}
	pActiveView->SendMessage(WM_PAINT);
}
void CLena::OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT1);    //得到指向编辑框的窗口指针
	UpdateData(TRUE);
	if(pNMUpDown->iDelta == 1)                    //如果点击的是Spin中的往上按钮
	{
		m_temmax++;
	}
	else if(pNMUpDown->iDelta == - 1)    //如果点击的是Spin中往下按钮
	{
		m_temmax--;
	}
	mymax=m_temmax;
	pActiveView->SendMessage(WM_PAINT);
	*pResult = 0;
}

void CLena::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	//CWnd* pWnd = (CWnd*)GetDlgItem(IDC_EDIT2);    //得到指向编辑框的窗口指针
	UpdateData(TRUE);
	if(pNMUpDown->iDelta == 1)                    //如果点击的是Spin中的往上按钮
	{
		m_temmin++;
	}
	else if(pNMUpDown->iDelta == - 1)    //如果点击的是Spin中往下按钮
	{
		m_temmin--;
	}
	mymin=m_temmin;
	pActiveView->SendMessage(WM_PAINT);
	*pResult = 0;
}

void CLena::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!tempflag1)
	{
		tempflag1=!tempflag1;
	}else{
		UpdateData(TRUE);
		mymax=m_temmax;
		//pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnEnChangeEdit2()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CFormView::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	if (!tempflag2)
	{
		tempflag2=!tempflag2;
	}else{
		UpdateData(TRUE);
		mymin=m_temmin;
		//pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck1()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK1))
	{
		MulFloorFlag|=0x001;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xffe;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK2))
	{
		MulFloorFlag|=0x002;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xffd;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck3()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK3))
	{
		MulFloorFlag|=0x004;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xffb;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck4()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK4))
	{
		MulFloorFlag|=0x008;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xff7;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK5))
	{
		MulFloorFlag|=0x010;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xfef;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck6()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK6))
	{
		MulFloorFlag|=0x020;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xfdf;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedShowIsosurface()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOW_ISOSURFACE))
	{
		CtrlFlag|=0x04;
		IsoSurface_num=m_nSlider2;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		CtrlFlag&=0xfffffffb;
		IsoSurface_num=m_nSlider2;
		pActiveView->SendMessage(WM_PAINT);
	}
}

//void CLena::OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl2.GetPos();
//	//UpdateData(TRUE);
//	////UpdateWindow();
//	//IsoSurface_num=m_nSlider2;
//	//pActiveView->SendMessage(WM_PAINT);
//	*pResult = 0;
//}

void CLena::OnBnClickedCheckPerspective()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_PERSPECTIVE))
	{
		bperspective=TRUE;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		bperspective=FALSE;
		pActiveView->SendMessage(WM_PAINT);
	}
}

//void CLena::OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl3.GetPos();
//	//UpdateData(TRUE);
//	//Alpha_Mixing=(float)m_nSlider3/100;
//	//pActiveView->SendMessage(WM_PAINT);
//	*pResult = 0;
//}
void CLena::OnBnClickedCheck7()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK7))
	{
		MulFloorFlag|=0x040;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xfbf;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck8()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK8))
	{
		MulFloorFlag|=0x080;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xf7f;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck9()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK9))
	{
		MulFloorFlag|=0x100;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xeff;
		pActiveView->SendMessage(WM_PAINT);
	}
}

void CLena::OnBnClickedCheck10()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK10))
	{
		MulFloorFlag|=0x200;
		pActiveView->SendMessage(WM_PAINT);
	}else{
		MulFloorFlag&=0xdff;
		pActiveView->SendMessage(WM_PAINT);
	}
}

//void CLena::OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl4.GetPos();
//	//UpdateData(TRUE);
//	//float temp=(float)m_nSlider4/100*(x_right-x_left)+x_left;
//	//m_xmin=temp;
//	//x_cutout=m_xmin;
//	//eqnx[0]=1/temp;
//	//eqnx[1]=0;
//	//eqnx[2]=0;
//	//eqnx[3]=-0.999;
//	//pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
//	//UpdateData(FALSE);
//	*pResult = 0;
//}

//void CLena::OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl5.GetPos();
//	//UpdateData(TRUE);
//	//float temp=(float)m_nSlider5/100*(y_back-y_front)+y_front;
//	//m_ymin=temp;
//	//y_cutout=m_ymin;
//	//eqny[0]=0;
//	//eqny[1]=1/temp;
//	//eqny[2]=0;
//	//eqny[3]=-0.999;
//	//pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
//	//UpdateData(FALSE);
//	*pResult = 0;
//}

//void CLena::OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	//m_SliderCtrl6.GetPos();
//	//UpdateData(TRUE);
//	//float temp=(float)m_nSlider6/100*(z_up-z_down)+z_down;
//	//m_zmin=temp;
//	//z_cutout=m_zmin;
//	//eqnz[0]=0;
//	//eqnz[1]=0;
//	//eqnz[2]=1/temp;
//	//eqnz[3]=-0.999;
//	//pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
//	//UpdateData(FALSE);
//	*pResult = 0;
//}

//void CLena::OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}

void CLena::OnBnClickedShowdata2()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOWDATA2))
	{
		numshowflag=false;
		m_ctrlcheck1.SetCheck(0);
		UpdateData(FALSE);
		CtrlFlag|=0x02;
		pActiveView->SendMessage(WM_PAINT);
	}else if (BST_CHECKED == IsDlgButtonChecked(IDC_SHOWDATA))
	{
		numshowflag=true;
		CtrlFlag|=0x02;
		pActiveView->SendMessage(WM_PAINT);
	}
	else{
		CtrlFlag&=0xfffffffd;
		pActiveView->SendMessage(WM_PAINT);
	}
}


void CLena::OnBnClickedCheckDiswell()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	diswellflag=m_diswellflag;
}


void CLena::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (pScrollBar!=NULL)
	{
		if( pScrollBar->GetDlgCtrlID() == IDC_SLIDER1 ) 
		{

			m_SliderCtrl.GetPos();
			UpdateData(TRUE);
			nSlider=(int)(m_nSlider/6);
			pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
		}else if(pScrollBar->GetDlgCtrlID() == IDC_SLIDER2){
			m_SliderCtrl2.GetPos();
			UpdateData(TRUE);
			IsoSurface_num=m_nSlider2;
			pActiveView->SendMessage(WM_PAINT);
		}else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER3)
		{
			m_SliderCtrl3.GetPos();
			UpdateData(TRUE);
			Alpha_Mixing=(float)m_nSlider3/100;
			pActiveView->SendMessage(WM_PAINT);
		}else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER4)
		{
			m_SliderCtrl4.GetPos();
			UpdateData(TRUE);
			float temp=(float)m_nSlider4/100*(x_right-x_left)+x_left;
			m_xmin=temp*100.0;
			x_cutout=m_xmin/100.0;
			if (temp==0)
			{
				cutxflag=true;
				eqnx[0]=0;
			}else{
				cutxflag=false;
				eqnx[0]=1/temp;
			}		
			eqnx[1]=0.0;
			eqnx[2]=0.0;
			eqnx[3]=-0.999;
			pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
			UpdateData(FALSE);
		}else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER5)
		{
			m_SliderCtrl5.GetPos();
			UpdateData(TRUE);
			float temp=(float)m_nSlider5/100*(y_back-y_front)+y_front;
			m_ymin=temp*100.0;
			y_cutout=m_ymin/100.0;
			if (temp==0)
			{
				cutyflag=true;
				eqny[1]=0;
			}else{
				cutyflag=false;
				eqny[1]=1/temp;
			}
			eqny[0]=0;
			eqny[2]=0;
			eqny[3]=-0.999;
			pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
			UpdateData(FALSE);
		}else if (pScrollBar->GetDlgCtrlID() == IDC_SLIDER6)
		{
			m_SliderCtrl6.GetPos();
			UpdateData(TRUE);
			float temp=(float)m_nSlider6/100.0*(z_up-z_down)+z_down;
			m_zmin=temp*100.0;
			z_cutout=m_zmin/100.0;
			if (temp==0)
			{
				cutzflag=true;
				eqnz[2]=0;
			}else{
				cutzflag=false;
				eqnz[2]=1/temp;
			}
			eqnz[0]=0;
			eqnz[1]=0;
			eqnz[3]=-0.999;
			pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
			UpdateData(FALSE);
		}
	}
	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}

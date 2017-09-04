// Cnewtest.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cnewtest.h"
#include "afxdialogex.h"


// Cnewtest 对话框

IMPLEMENT_DYNAMIC(Cnewtest, CDialogEx)

Cnewtest::Cnewtest(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cnewtest::IDD, pParent)
	, mytestname(_T(""))
	, myinetnaltime(10)
	, m_TSensorCom(_T(""))
	, m_ConCtral_com(_T(""))
	, m_classname(_T(""))
	, m_pcom(_T(""))
{
	mypictpath=_T("C:\\");
	mydatapath=_T("C:\\");
}

Cnewtest::~Cnewtest()
{
}

void Cnewtest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mytestname);
	DDX_Text(pDX, IDC_EDIT7, myinetnaltime);
	DDX_CBString(pDX, IDC_COMBO1, m_TSensorCom);
	DDX_CBString(pDX, IDC_COMBO2, m_ConCtral_com);
	DDX_CBString(pDX, IDC_COMBO3, m_classname);
	DDX_CBString(pDX, IDC_COMBO4, m_pcom);
}


BEGIN_MESSAGE_MAP(Cnewtest, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &Cnewtest::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cnewtest::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &Cnewtest::OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO3, &Cnewtest::OnCbnSelchangeCombo3)
END_MESSAGE_MAP()


// Cnewtest 消息处理程序


void Cnewtest::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}
	::SHGetPathFromIDList(pItemIDList,strDirName);
	mydatapath.Format(_T("%s"),strDirName);
	mydatapath.TrimRight('\\');
	mydatapath+=_T("\\");
}


void Cnewtest::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}
	::SHGetPathFromIDList(pItemIDList,strDirName);
	mypictpath.Format(_T("%s"),strDirName);
	mypictpath.TrimRight('\\');
	mypictpath+=_T("\\");
}


void Cnewtest::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (mytestname==_T(""))
	{
		MessageBox(_T("请输入实验名称！"));
		return;
	}
	if (myinetnaltime<=0)
	{
		MessageBox(_T("请输入采集实验间隔！"));
		return;
	}
	CDialogEx::OnOK();
}


void Cnewtest::OnCbnSelchangeCombo3()
{
	// TODO: 在此添加控件通知处理程序代码
	CComboBox *pselect=(CComboBox*)GetDlgItem(IDC_COMBO3);
	int myselectnum=pselect->GetCurSel();
	if (myselectnum==0)
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);	
	}
	else
	{
		GetDlgItem(IDC_COMBO1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO4)->EnableWindow(TRUE);
	}
}


BOOL Cnewtest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CComboBox *pcombox=(CComboBox*)GetDlgItem(IDC_COMBO3);
	pcombox->AddString(_T("温度场"));
	pcombox->AddString(_T("压力场"));
	m_classname=_T("温度场");
	pcombox=(CComboBox*)GetDlgItem(IDC_COMBO1);
	pcombox->AddString(_T("COM1"));
	pcombox->AddString(_T("COM2"));
	pcombox->AddString(_T("COM3"));
	pcombox->AddString(_T("COM4"));
	pcombox->AddString(_T("COM5"));
	pcombox->AddString(_T("COM6"));
	m_TSensorCom=_T("COM1");
	pcombox=(CComboBox*)GetDlgItem(IDC_COMBO2);
	pcombox->AddString(_T("COM1"));
	pcombox->AddString(_T("COM2"));
	pcombox->AddString(_T("COM3"));
	pcombox->AddString(_T("COM4"));
	pcombox->AddString(_T("COM5"));
	pcombox->AddString(_T("COM6"));
	m_ConCtral_com=_T("COM2");
	pcombox=(CComboBox*)GetDlgItem(IDC_COMBO4);
	pcombox->AddString(_T("COM1"));
	pcombox->AddString(_T("COM2"));
	pcombox->AddString(_T("COM3"));
	pcombox->AddString(_T("COM4"));
	pcombox->AddString(_T("COM5"));
	pcombox->AddString(_T("COM6"));
	m_pcom=_T("COM3");
	UpdateData(FALSE);
	GetDlgItem(IDC_COMBO4)->EnableWindow(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// AVIPrama.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "AVIPrama.h"
#include "afxdialogex.h"


// CAVIPrama 对话框

IMPLEMENT_DYNAMIC(CAVIPrama, CDialogEx)

CAVIPrama::CAVIPrama(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAVIPrama::IDD, pParent)
	, mywidth(740)
	, myheiht(680)
	, myfps(10)
	, myname(_T(""))
{
	myfilepath=_T("C:\\");
}

CAVIPrama::~CAVIPrama()
{
}

void CAVIPrama::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mywidth);
	DDX_Text(pDX, IDC_EDIT7, myheiht);
	DDX_Text(pDX, IDC_EDIT2, myfps);
	DDX_Text(pDX, IDC_EDIT9, myname);
}


BEGIN_MESSAGE_MAP(CAVIPrama, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAVIPrama::OnBnClickedButton1)
	ON_BN_CLICKED(IDOK, &CAVIPrama::OnBnClickedOk)
END_MESSAGE_MAP()


// CAVIPrama 消息处理程序


void CAVIPrama::OnBnClickedButton1()
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
	myfilepath.Format(_T("%s"),strDirName);
}


void CAVIPrama::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (myheiht<0||myfps<0||mywidth<0||myname==_T(""))
	{
		MessageBox(_T("请输入正确的参数！"));
		return;
	}
	CDialogEx::OnOK();
}

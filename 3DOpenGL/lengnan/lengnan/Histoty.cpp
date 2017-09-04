// Histoty.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Histoty.h"
#include "afxdialogex.h"


// CHistoty 对话框

IMPLEMENT_DYNAMIC(CHistoty, CDialogEx)

CHistoty::CHistoty(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoty::IDD, pParent)
{
	mypathfilename=_T("");
	mypictpath=_T("C:\\");
}

CHistoty::~CHistoty()
{
}

void CHistoty::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHistoty, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CHistoty::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CHistoty::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CHistoty::OnBnClickedOk)
END_MESSAGE_MAP()


// CHistoty 消息处理程序


void CHistoty::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog filedlg(TRUE,_T(".accdb"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("实验数据 (*.accdb;*.mdb)|*.accdb;*.mdb|All Files(*.*)|*.*||"),0);
	if (IDOK!=filedlg.DoModal())
	{
		mypathfilename=_T("");
	}
	else
	{
     	mypathfilename=filedlg.GetPathName();
	}
}


void CHistoty::OnBnClickedButton2()
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


void CHistoty::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if (mypathfilename==_T(""))
	{
		MessageBox(_T("请打开有效数据文件！"));
		return;
	}
	CDialogEx::OnOK();
}

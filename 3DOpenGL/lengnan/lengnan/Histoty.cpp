// Histoty.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Histoty.h"
#include "afxdialogex.h"


// CHistoty �Ի���

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


// CHistoty ��Ϣ�������


void CHistoty::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CFileDialog filedlg(TRUE,_T(".accdb"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("ʵ������ (*.accdb;*.mdb)|*.accdb;*.mdb|All Files(*.*)|*.*||"),0);
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("ѡ��һ��Դ�ļ����ļ���");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (mypathfilename==_T(""))
	{
		MessageBox(_T("�����Ч�����ļ���"));
		return;
	}
	CDialogEx::OnOK();
}

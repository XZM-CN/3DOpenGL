// AutoPlay.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "AutoPlay.h"



// CAutoPlay 对话框

IMPLEMENT_DYNAMIC(CAutoPlay, CDialog)

CAutoPlay::CAutoPlay(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoPlay::IDD, pParent)
	, m_nspace(20)
	, m_nFrameRate(10)
{
	m_aviflag=false;
	m_AviFileName=_T("");
}

CAutoPlay::~CAutoPlay()
{
}

void CAutoPlay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_nspace);
	DDX_Text(pDX, IDC_EDIT2, m_nFrameRate);
}


BEGIN_MESSAGE_MAP(CAutoPlay, CDialog)
	ON_BN_CLICKED(IDOK, &CAutoPlay::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_AVI, &CAutoPlay::OnBnClickedCheckAvi)
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoPlay::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAutoPlay 消息处理程序

void CAutoPlay::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	if (m_AviFileName==_T("")&&m_aviflag)
	{
		MessageBox(_T("请输入Avi文件名称"));
		return;
	}
	OnOK();
}

void CAutoPlay::OnBnClickedCheckAvi()
{
	// TODO: 在此添加控件通知处理程序代码
	if (BST_CHECKED == IsDlgButtonChecked(IDC_CHECK_AVI))
	{
		m_aviflag=true;
	}else{
		m_aviflag=false;
	}
}

void CAutoPlay::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog filedlg(FALSE,_T(".avi"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,_T("(*.avi)|*.avi|All Files(*.*)|*.*||"),0);
	if (IDOK==filedlg.DoModal())
	{
		m_AviFileName=filedlg.GetPathName();
	}
}

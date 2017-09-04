// SetServer.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "SetServer.h"
#include "afxdialogex.h"


// CSetServer 对话框

IMPLEMENT_DYNAMIC(CSetServer, CDialogEx)

CSetServer::CSetServer(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetServer::IDD, pParent)
	, m_Port(0)
	, m_ServerIp(_T(""))
{

}

CSetServer::~CSetServer()
{
}

void CSetServer::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_Port);
	DDX_Text(pDX, IDC_EDIT2, m_ServerIp);
}


BEGIN_MESSAGE_MAP(CSetServer, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetServer::OnBnClickedOk)
END_MESSAGE_MAP()


// CSetServer 消息处理程序


void CSetServer::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	UpdateData(TRUE);

	CDialogEx::OnOK();
}

BOOL CSetServer::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Client=socket(AF_INET,SOCK_STREAM,0);//创建套接字

	return TRUE;
}
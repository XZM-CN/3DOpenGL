// TextToAccess.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "TextToAccess.h"
#include "afxdialogex.h"


// CTextToAccess 对话框

IMPLEMENT_DYNAMIC(CTextToAccess, CDialogEx)

CTextToAccess::CTextToAccess(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTextToAccess::IDD, pParent)
	, tsensor_num(0)
	, tcount(0)
{

}

CTextToAccess::~CTextToAccess()
{
}

void CTextToAccess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, tsensor_num);
	DDV_MinMaxInt(pDX, tsensor_num, 1, 1000);
	DDX_Text(pDX, IDC_EDIT7, tcount);
	DDV_MinMaxInt(pDX, tcount, 1, 1000000);
}


BEGIN_MESSAGE_MAP(CTextToAccess, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTextToAccess::OnBnClickedOk)
END_MESSAGE_MAP()


// CTextToAccess 消息处理程序


void CTextToAccess::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

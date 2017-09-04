// TextToAccess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "TextToAccess.h"
#include "afxdialogex.h"


// CTextToAccess �Ի���

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


// CTextToAccess ��Ϣ�������


void CTextToAccess::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}

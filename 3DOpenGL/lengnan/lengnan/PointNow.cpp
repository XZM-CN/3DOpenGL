// PointNow.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "PointNow.h"
#include "afxdialogex.h"
#include "IA.h"

extern Vector3D Pt3d;

// CPointNow �Ի���

IMPLEMENT_DYNAMIC(CPointNow, CDialogEx)

CPointNow::CPointNow(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPointNow::IDD, pParent)
	, m_point_x(0)
{

}

CPointNow::~CPointNow()
{
}

void CPointNow::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_point_x);
	DDX_Text(pDX, IDC_EDIT7, m_point_y);
	DDX_Text(pDX, IDC_EDIT8, m_point_z);
}


BEGIN_MESSAGE_MAP(CPointNow, CDialogEx)
	ON_BN_CLICKED(IDOK, &CPointNow::OnBnClickedOk)
END_MESSAGE_MAP()


// CPointNow ��Ϣ�������


void CPointNow::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	CDialogEx::OnOK();
}


BOOL CPointNow::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_point_x=Pt3d.x;
	m_point_y=Pt3d.y;
	m_point_z=Pt3d.z;
	UpdateData(FALSE);

	return TRUE;
}

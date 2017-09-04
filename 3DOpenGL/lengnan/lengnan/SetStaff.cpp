// SetStaff.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "SetStaff.h"
#include "afxdialogex.h"


// CSetStaff 对话框
float color1[3]={0.0f,0.0f,1.0f};
float color2[3]={0.0f,1.0f,0.0f};
float color3[3]={1.0f,1.0f,0.0f};
float color4[3]={1.0f,0.0f,0.0f};
extern BOOL SetStaff;

IMPLEMENT_DYNAMIC(CSetStaff, CDialogEx)


CSetStaff::CSetStaff(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetStaff::IDD, pParent)
	, m_value1(100)
	, m_value2(200)
	, m_value3(300)
	, m_value4(500)
	, m_value22(200)
	, m_value33(300)
	, m_value44(400)
	, m_value55(400)
{

}

CSetStaff::~CSetStaff()
{
}

void CSetStaff::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_value1);
	DDX_Text(pDX, IDC_EDIT7, m_value2);
	DDX_Text(pDX, IDC_EDIT2, m_value3);
	DDX_Text(pDX, IDC_EDIT9, m_value4);
	DDX_Text(pDX, IDC_EDIT10, m_value22);
	DDX_Text(pDX, IDC_EDIT11, m_value33);
	DDX_Text(pDX, IDC_EDIT12, m_value44);
	DDX_Text(pDX, IDC_EDIT13, m_value55);
	DDX_Control(pDX, IDC_EDIT7, m_edit1);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT13, m_edit3);
}


BEGIN_MESSAGE_MAP(CSetStaff, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSetStaff::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT10, &CSetStaff::OnEnChangeEdit10)
	ON_EN_CHANGE(IDC_EDIT11, &CSetStaff::OnEnChangeEdit11)
	ON_EN_CHANGE(IDC_EDIT12, &CSetStaff::OnEnChangeEdit12)
END_MESSAGE_MAP()


// CSetStaff 消息处理程序

BOOL CSetStaff::OnInitDialog()
{
	CDialogEx::OnInitDialog();
// 	((CButton*)GetDlgItem(IDC_RADIO1))->SetCheck(BST_CHECKED);
// 	((CButton*)GetDlgItem(IDC_RADIO6))->SetCheck(BST_CHECKED);
// 	((CButton*)GetDlgItem(IDC_RADIO11))->SetCheck(BST_CHECKED);
// 	((CButton*)GetDlgItem(IDC_RADIO16))->SetCheck(BST_CHECKED);
	return TRUE;
}
void CSetStaff::OnRadioShow(UINT ucmdID)
{
// 	UINT nID=GetCheckedRadioButton(IDC_RADIO1,IDC_RADIO4);
// 	switch(nID)
// 	{
// 	case IDC_RADIO1:
// 		color1[0]=0.0f;
// 		color1[1]=0.0f;
// 		color1[2]=1.0f;
// 		break;
// 	case IDC_RADIO2:
// 		color1[0]=0.0f;
// 		color1[1]=1.0f;
// 		color1[2]=0.0f;
// 		break;
// 	case IDC_RADIO3:
// 		color1[0]=1.0f;
// 		color1[1]=1.0f;
// 		color1[2]=0.0f;
// 		break;
// 	case IDC_RADIO4:
// 		color1[0]=1.0f;
// 		color1[1]=0.0f;
// 		color1[2]=0.0f;
// 		break;
// 	default:
// 		break;
// 
// 	}
	
}
void CSetStaff::OnRadioShow1(UINT ucmdID)
{
// 	UINT nID=GetCheckedRadioButton(IDC_RADIO5,IDC_RADIO8);
// 	switch(nID)
// 	{
// 		case IDC_RADIO5:
// 			color2[0]=0.0f;
// 			color2[1]=0.0f;
// 			color2[2]=1.0f;
// 			break;
// 		case IDC_RADIO6:
// 			color2[0]=0.0f;
// 			color2[1]=1.0f;
// 			color2[2]=0.0f;
// 			break;
// 		case IDC_RADIO7:
// 			color2[0]=1.0f;
// 			color2[1]=1.0f;
// 			color2[2]=0.0f;
// 			break;
// 		case IDC_RADIO8:
// 			color2[0]=1.0f;
// 			color2[1]=0.0f;
// 			color2[2]=0.0f;
// 			break;
// 		default:
// 			break;
// 	}
// 	

}
void CSetStaff::OnRadioShow2(UINT ucmdID)
{
// 	UINT nID=GetCheckedRadioButton(IDC_RADIO9,IDC_RADIO12);
// 	switch(nID)
// 	{
// 	case IDC_RADIO9:
// 		color3[0]=0.0f;
// 		color3[1]=0.0f;
// 		color3[2]=1.0f;
// 		break;
// 	case IDC_RADIO10:
// 		color3[0]=0.0f;
// 		color3[1]=1.0f;
// 		color3[2]=0.0f;
// 		break;
// 	case IDC_RADIO11:
// 		color3[0]=1.0f;
// 		color3[1]=1.0f;
// 		color3[2]=0.0f;
// 		break;
// 	case IDC_RADIO12:
// 		color3[0]=1.0f;
// 		color3[1]=0.0f;
// 		color3[2]=0.0f;
// 		break;
// 	default:
// 		break;
// 	}
	

}
void CSetStaff::OnRadioShow3(UINT ucmdID)
{
// 	UINT nID=GetCheckedRadioButton(IDC_RADIO13,IDC_RADIO16);
// 	switch(nID)
// 	{
// 	case IDC_RADIO13:
// 		color4[0]=0.0f;
// 		color4[1]=0.0f;
// 		color4[2]=1.0f;
// 		break;
// 	case IDC_RADIO14:
// 		color4[0]=0.0f;
// 		color4[1]=1.0f;
// 		color4[2]=0.0f;
// 		break;
// 	case IDC_RADIO15:
// 		color4[0]=1.0f;
// 		color4[1]=1.0f;
// 		color4[2]=0.0f;
// 		break;
// 	case IDC_RADIO16:
// 		color4[0]=1.0f;
// 		color4[1]=0.0f;
// 		color4[2]=0.0f;
// 		break;
// 	default:
// 		break;
// 	}
}


void CSetStaff::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	CDialogEx::OnOK();
}



void CSetStaff::OnEnChangeEdit10()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	UpdateData(TRUE);
	CString str;str.Format(_T("%d"),m_value22);
	m_edit1.SetWindowText(str);
	// TODO:  在此添加控件通知处理程序代码
}


void CSetStaff::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	CString str;str.Format(_T("%d"),m_value33);
	m_edit2.SetWindowText(str);
	// TODO:  在此添加控件通知处理程序代码
}


void CSetStaff::OnEnChangeEdit12()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	CString str;str.Format(_T("%d"),m_value44);
	m_edit3.SetWindowText(str);
	// TODO:  在此添加控件通知处理程序代码
}

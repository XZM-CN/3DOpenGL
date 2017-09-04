// AddIso.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "AddIso.h"
#include "afxdialogex.h"


extern float isosurface_data[10];
extern int isosurface_num;

// CAddIso 对话框

IMPLEMENT_DYNAMIC(CAddIso, CDialogEx)

CAddIso::CAddIso(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAddIso::IDD, pParent)
	, m_isofacedata(0)
{

}

CAddIso::~CAddIso()
{
}

void CAddIso::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_isofacedata);
	DDX_Control(pDX, IDC_LIST2, m_list2);
}


BEGIN_MESSAGE_MAP(CAddIso, CDialogEx)
	ON_BN_CLICKED(IDOK, &CAddIso::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CAddIso::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CAddIso::OnBnClickedButtonSub)
	ON_BN_CLICKED(IDCANCEL, &CAddIso::OnBnClickedCancel)
END_MESSAGE_MAP()


// CAddIso 消息处理程序


void CAddIso::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CAddIso::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	if (isosurface_num==4)
	{
		AfxMessageBox(_T("等值面最多显示4个"));
	}
	UpdateData(TRUE);
	CString tempstr;
	tempstr.Format(_T("%d"),isosurface_num+1);
	m_list2.InsertItem(isosurface_num,tempstr);
	tempstr.Format(_T("%.1f"),m_isofacedata);
	m_list2.SetItemText(isosurface_num,1,tempstr);
	isosurface_data[isosurface_num]=m_isofacedata;
	isosurface_num++;
}


void CAddIso::OnBnClickedButtonSub()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	for(int i=0; i<m_list2.GetItemCount(); i++) 
	{ 
		if( m_list2.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
		{ 
			m_list2.DeleteItem(i);
			for (int j=i;j<isosurface_num;j++)
			{
				isosurface_data[j]=isosurface_data[j+1];
			}
			isosurface_num--;			
		} 
	}
}

BOOL CAddIso::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}
		if (pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

//BOOL CAddIso::OnInitDialog()
//{
//	CDialogEx::OnInitDialog();
//
//	// TODO:  在此添加额外的初始化
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// 异常: OCX 属性页应返回 FALSE
//}


BOOL CAddIso::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//控件初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_list2.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_list2.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle;
	dwStyle =m_list2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle	|= LVS_EX_ONECLICKACTIVATE;//单击激活
	dwStyle	|= LVS_EX_TRACKSELECT;//自动换行
	m_list2.SetExtendedStyle(dwStyle); //设置扩展风格

	m_list2.InsertColumn(0,_T("序号"),LVCFMT_LEFT,60);
	m_list2.InsertColumn(1,_T("值"),LVCFMT_LEFT,60);

	CString tempstr;

	for (int i=0;i<isosurface_num;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		m_list2.InsertItem(i,tempstr);
		tempstr.Format(_T("%.1f"),isosurface_data[i]);
		m_list2.SetItemText(i,1,tempstr);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CAddIso::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}

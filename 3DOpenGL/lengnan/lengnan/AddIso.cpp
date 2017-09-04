// AddIso.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "AddIso.h"
#include "afxdialogex.h"


extern float isosurface_data[10];
extern int isosurface_num;

// CAddIso �Ի���

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


// CAddIso ��Ϣ�������


void CAddIso::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CAddIso::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (isosurface_num==4)
	{
		AfxMessageBox(_T("��ֵ�������ʾ4��"));
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ����ר�ô����/����û���
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
//	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
//
//	return TRUE;  // return TRUE unless you set the focus to a control
//	// �쳣: OCX ����ҳӦ���� FALSE
//}


BOOL CAddIso::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�ؼ���ʼ��
	LONG lStyle;
	lStyle = GetWindowLong(m_list2.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_list2.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle;
	dwStyle =m_list2.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	dwStyle	|= LVS_EX_ONECLICKACTIVATE;//��������
	dwStyle	|= LVS_EX_TRACKSELECT;//�Զ�����
	m_list2.SetExtendedStyle(dwStyle); //������չ���

	m_list2.InsertColumn(0,_T("���"),LVCFMT_LEFT,60);
	m_list2.InsertColumn(1,_T("ֵ"),LVCFMT_LEFT,60);

	CString tempstr;

	for (int i=0;i<isosurface_num;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		m_list2.InsertItem(i,tempstr);
		tempstr.Format(_T("%.1f"),isosurface_data[i]);
		m_list2.SetItemText(i,1,tempstr);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CAddIso::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}

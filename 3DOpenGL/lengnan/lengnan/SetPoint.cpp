// SetPoint.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "SetPoint.h"
#include "ADOConn.h"
#include "myheader.h"
//����������
extern int sensors_num;
_RecordsetPtr m_pRecordset0;//ָ��Recordset����ָ��
int nPointNum=0;				//������Ŀ
int nPoints[100]={0};			//�����������
extern float Sensors_Array[3000];
// CSetPoint �Ի���

IMPLEMENT_DYNAMIC(CSetPoint, CDialog)

CSetPoint::CSetPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPoint::IDD, pParent)
	, myselectnum(_T(""))
{

}

CSetPoint::~CSetPoint()
{
}

void CSetPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	DDX_Control(pDX, IDC_COMBO1, mysensorcombo);
	DDX_CBString(pDX, IDC_COMBO1, myselectnum);
}


BEGIN_MESSAGE_MAP(CSetPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CSetPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSetPoint::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CSetPoint::OnBnClickedButtonSub)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSetPoint::OnNMRClickList1)
END_MESSAGE_MAP()


// CSetPoint ��Ϣ�������

void CSetPoint::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

BOOL CSetPoint::PreTranslateMessage(MSG* pMsg)
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
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetPoint::OnBnClickedButtonAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int n;
	CString tempstr;
	UpdateData(TRUE);
	if (myselectnum==_T(""))
	{
		MessageBox(_T("��ѡ�񻵵㴫������"));
		return;
	}
	n=_wtoi(myselectnum);
	nPoints[nPointNum]=n;
	//����λ��
	int i=0;
	for (;i<sensors_num;i++)
	{
		if (sensornumber[i]==n)
		{
			break;
		}
		
	}
	m_listctrl.InsertItem(nPointNum,myselectnum);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i]);
	m_listctrl.SetItemText(nPointNum,1,tempstr);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i+1]);
	m_listctrl.SetItemText(nPointNum,2,tempstr);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i+2]);
	m_listctrl.SetItemText(nPointNum,3,tempstr);
	nPointNum++;
}

void CSetPoint::OnBnClickedButtonSub()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	for(int i=0; i<m_listctrl.GetItemCount(); i++) 
	{ 
		if( m_listctrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
		{ 
			m_listctrl.DeleteItem(i);
			for (int j=i;j<nPointNum;j++)
			{
				nPoints[j]=nPoints[j+1];
			}
			nPointNum--;			
		} 
	}
}

BOOL CSetPoint::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString tempstr;
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//�ؼ���ʼ��
	LONG lStyle;
	lStyle = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);//��ȡ��ǰ����style
	lStyle &= ~LVS_TYPEMASK; //�����ʾ��ʽλ
	lStyle |= LVS_REPORT; //����style
	SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, lStyle);//����style

	DWORD dwStyle;
	dwStyle =m_listctrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//ѡ��ĳ��ʹ���и�����ֻ������report����listctrl��
	dwStyle |= LVS_EX_GRIDLINES;//�����ߣ�ֻ������report����listctrl��
	//dwStyle |= LVS_EX_CHECKBOXES;//itemǰ����checkbox�ؼ�
	dwStyle	|= LVS_EX_ONECLICKACTIVATE;//��������
	dwStyle	|= LVS_EX_TRACKSELECT;//�Զ�����
	m_listctrl.SetExtendedStyle(dwStyle); //������չ���

	m_listctrl.InsertColumn(0,_T("���"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(1,_T("����X"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(2,_T("����Y"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(3,_T("����Z"),LVCFMT_LEFT,60);

	for (int i=0;i<nPointNum;i++)
	{
		tempstr.Format(_T("%d"),nPoints[i]);
		m_listctrl.InsertItem(i,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-3]);
		m_listctrl.SetItemText(i,1,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-2]);
		m_listctrl.SetItemText(i,2,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-1]);
		m_listctrl.SetItemText(i,3,tempstr);
	}

	//��Ӵ��������
	for (int i=0;i<sensors_num;i++)
	{
		if (sensornumber[i]>0)
		{
			CString tempstr;
			tempstr.Format(_T("%d"),sensornumber[i]);
			mysensorcombo.AddString(tempstr);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CSetPoint::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(IDNO==::MessageBox(NULL,_T("���Ҫɾ������㣿"),_T("OK"),MB_YESNO|MB_ICONWARNING)) 
		return;
	for(int i=0; i<m_listctrl.GetItemCount(); i++) 
	{ 
		if( m_listctrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
		{ 
			m_listctrl.DeleteItem(i);
			for (int j=i;j<nPointNum;j++)
			{
				nPoints[j]=nPoints[j+1];
			}
			nPointNum--;			
		} 
	}
	*pResult = 0;
}

// SetWellDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "SetWellDlg.h"


//�������ֵ
extern float z_down,z_up;//�ϡ���
extern float y_front,y_back;//ǰ����
extern float x_left,x_right;//����
// CSetWellDlg �Ի���

IMPLEMENT_DYNAMIC(CSetWellDlg, CDialog)

CSetWellDlg::CSetWellDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSetWellDlg::IDD, pParent)
	, m_nwellmode(0)
	, m_nwellorientations(0)
	, x_start(0)
	, y_start(0)
	, Z_start(0)
{
	m_start=0;
	m_end=0;
}

CSetWellDlg::~CSetWellDlg()
{
}

void CSetWellDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_WELL_EDIT1, m_start);
	DDX_Text(pDX, IDC_WELL_EDIT2, m_end);
	DDX_Radio(pDX, IDC_WELL_RADIO1, m_nwellmode);
	DDX_Radio(pDX, IDC_WELL_RADIO5, m_nwellorientations);
	DDX_Text(pDX, IDC_EDIT1, x_start);
	DDX_Text(pDX, IDC_EDIT2, y_start);
	DDX_Text(pDX, IDC_EDIT9, Z_start);
}


BEGIN_MESSAGE_MAP(CSetWellDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CSetWellDlg::OnBnClickedOk)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_WELL_RADIO1, IDC_WELL_RADIO2, &CSetWellDlg::OnWellRadio)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_WELL_RADIO5, IDC_WELL_RADIO7, &CSetWellDlg::OnWellRadio2)
END_MESSAGE_MAP()


// CSetWellDlg ��Ϣ�������

void CSetWellDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(TRUE);
	switch(m_nwellmode)
	{
	case 0:
		if (m_nwellorientations==0)
		{
			m_wellflag=0;
		}else if(m_nwellorientations==1)
		{
			m_wellflag=1;
		}else{
			m_wellflag=4;
		}
		break;
	case 1:
		if (m_nwellorientations==0)
		{
			m_wellflag=2;
		}else if (m_nwellorientations==1)
		{
			m_wellflag=3;
		}else{
			m_wellflag=5;
		}
		break;
	default:
		break;
	}
	OnOK();
}

void CSetWellDlg::OnWellRadio(UINT ucmdID)
{
	UINT nID=GetCheckedRadioButton(IDC_WELL_RADIO1,IDC_WELL_RADIO2);
	switch(nID)
	{
	case IDC_WELL_RADIO1:
		m_nwellmode=0;
		break;
	case IDC_WELL_RADIO2:
		m_nwellmode=1;
		break;
	default:
		break;
	}
}

void CSetWellDlg::OnWellRadio2(UINT ucmdID)
{
	UINT nID=GetCheckedRadioButton(IDC_WELL_RADIO5,IDC_WELL_RADIO7);
	switch(nID)
	{
	case IDC_WELL_RADIO5:
		m_nwellorientations=0;
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("��ʼx����"));
		GetDlgItem(IDC_STATIC2)->SetWindowText(_T("��ֹx����"));
		UpdateData(FALSE);
		break;
	case IDC_WELL_RADIO6:
		m_nwellorientations=1;
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("��ʼy����"));
		GetDlgItem(IDC_STATIC2)->SetWindowText(_T("��ֹy����"));
		UpdateData(FALSE);
		break;
	case IDC_WELL_RADIO7:
		m_nwellorientations=2;
		GetDlgItem(IDC_STATIC1)->SetWindowText(_T("��ʼz����"));
		GetDlgItem(IDC_STATIC2)->SetWindowText(_T("��ֹz����"));
		UpdateData(FALSE);
		break;
	default:
		break;
	}
}
BOOL CSetWellDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

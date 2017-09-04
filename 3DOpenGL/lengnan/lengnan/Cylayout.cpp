// Cylayout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cylayout.h"
#include "afxdialogex.h"


// Cylayout �Ի���

IMPLEMENT_DYNAMIC(Cylayout, CPropertyPage)

Cylayout::Cylayout()
	: CPropertyPage(Cylayout::IDD)
	, myynum(0)
	, myStrYselectnum(_T(""))
	, InternalSpace(0)
{

}

Cylayout::~Cylayout()
{
}

void Cylayout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myynum);
	DDX_CBString(pDX, IDC_COMBO1, myStrYselectnum);
	DDX_Text(pDX, IDC_EDIT7, InternalSpace);
	DDX_Control(pDX, IDC_COMBO1, mycombobox);
	DDX_Control(pDX, IDC_LIST2, mylist);
}


BEGIN_MESSAGE_MAP(Cylayout, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &Cylayout::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cylayout::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cylayout ��Ϣ�������


BOOL Cylayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	////////////��ʼ���б��
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString tempsting;
	//��ʼ������ؼ�
	mylist.InsertColumn(0,_T("Y���к�"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("����һ�м��"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void Cylayout::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	mylist.DeleteAllItems();
	mycombobox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Ydistances[i]=-1;
	}
	//����������ռ䷽λ
	for (int z=0;z<SENSORLAYNUM;z++)
	{
		for (int y=0;y<SENSORLAYNUM;y++)
		{
			for (int x=0;x<SENSORLAYNUM;x++)
			{
				sensor_pos[x][y][z]=-1;
			}
		}
	}
	UpdateData(TRUE);
	if (myynum<=1)
	{
		MessageBox(_T("Y���������������1��"));
		return;
	}
	Ynum=myynum;
	//��ʼ���б�������б�
	for (int i=0;i<Ynum;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		mylist.InsertItem(i,tempstr);
		mycombobox.AddString(tempstr);
		if (i==0)
		{
			tempstr=_T("0");
			mylist.SetItemText(i,1,tempstr);
		}
	}
	//��ӵ�һ��������
	Ydistances[0]=0;
}


void Cylayout::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	UpdateData(TRUE);
	if (myStrYselectnum==_T(""))
	{
		MessageBox(_T("��ѡ���кţ�"));
		return;
	}
	if (myStrYselectnum!=_T("1"))
	{
		if (InternalSpace<=0)
		{
			MessageBox(_T("�������м�࣡"));
			return;
		}
	}
	else
	{
		InternalSpace=0;
		UpdateData(FALSE);
	}
	int num=_wtoi(myStrYselectnum);
	//��ӵ��б���
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//��ӵ�������
	Ydistances[num-1]=InternalSpace;
}


BOOL Cylayout::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// �������Ա�ֻ�С���һ������ť    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


LRESULT Cylayout::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (myynum==0)
	{
		MessageBox(_T("���趨Y�򴫸�������"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Ynum;i++)
	{
		if (Ydistances[i]==-1)
		{
			tempstr.Format(_T("������Y���%d���м��"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

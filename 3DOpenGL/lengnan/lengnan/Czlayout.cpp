// Czlayout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Czlayout.h"
#include "afxdialogex.h"
#include "myheader.h"

// Czlayout �Ի���

IMPLEMENT_DYNAMIC(Czlayout, CPropertyPage)

Czlayout::Czlayout()
	: CPropertyPage(Czlayout::IDD)
	, myznum(0)
	, myStrZselectnum(_T(""))
	, InternalSpace(0)
{

}

Czlayout::~Czlayout()
{
}

void Czlayout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myznum);
	DDX_CBString(pDX, IDC_COMBO1, myStrZselectnum);
	DDX_Control(pDX, IDC_COMBO1, mycombox);
	DDX_Text(pDX, IDC_EDIT2, InternalSpace);
	DDX_Control(pDX, IDC_LIST1, mylist);
}


BEGIN_MESSAGE_MAP(Czlayout, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &Czlayout::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Czlayout::OnBnClickedButton2)
END_MESSAGE_MAP()


// Czlayout ��Ϣ�������


void Czlayout::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	mylist.DeleteAllItems();
	mycombox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Zdistances[i]=-1;
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
	if (myznum<=0)
	{
		MessageBox(_T("������Z����������"));
		return;
	}
	Znum=myznum;
	//��ʼ���б�������б�
	for (int i=0;i<Znum;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		mylist.InsertItem(i,tempstr);
		mycombox.AddString(tempstr);
		if (i==0)
		{
			tempstr=_T("0");
			mylist.SetItemText(i,1,tempstr);
		}
	}
	//��ӵ�һ��������
	Zdistances[0]=0;
}


void Czlayout::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	UpdateData(TRUE);
	if (myStrZselectnum==_T(""))
	{
		MessageBox(_T("��ѡ���ţ�"));
		return;
	}
	if (myStrZselectnum!=_T("1"))
	{
		if (InternalSpace<=0)
		{
			MessageBox(_T("��������࣡"));
			return;
		}
	}
	else
	{
		InternalSpace=0;
		UpdateData(FALSE);
	}
	int num=_wtoi(myStrZselectnum);
	//��ӵ��б���
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//��ӵ�������
	Zdistances[num-1]=InternalSpace;
}


BOOL Czlayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	////////////��ʼ���б��
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString tempsting;
	//��ʼ������ؼ�
	mylist.InsertColumn(0,_T("Z����"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("����һ����"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


BOOL Czlayout::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// �������Ա�ֻ�С���һ������ť    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


LRESULT Czlayout::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (myznum==0)
	{
		MessageBox(_T("���趨Z�򴫸�������"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Znum;i++)
	{
		if (Zdistances[i]==-1)
		{
			tempstr.Format(_T("������Z���%d�����"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

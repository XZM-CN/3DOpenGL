// Cxlayout.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cxlayout.h"
#include "afxdialogex.h"
#include "myheader.h"

// Cxlayout �Ի���

IMPLEMENT_DYNAMIC(Cxlayout, CPropertyPage)

Cxlayout::Cxlayout()
	: CPropertyPage(Cxlayout::IDD)
	, myxnum(0)
	, InternalSpace(0)
	, myStrXselectnum(_T(""))
{

}

Cxlayout::~Cxlayout()
{
}

void Cxlayout::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, myxnum);
	DDX_Text(pDX, IDC_EDIT2, InternalSpace);
	DDX_CBString(pDX, IDC_COMBO1, myStrXselectnum);
	DDX_Control(pDX, IDC_LIST1, mylist);
	DDX_Control(pDX, IDC_COMBO1, mycombobox);
}


BEGIN_MESSAGE_MAP(Cxlayout, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &Cxlayout::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cxlayout::OnBnClickedButton2)
END_MESSAGE_MAP()


// Cxlayout ��Ϣ�������


BOOL Cxlayout::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// �������Ա�ֻ�С���һ������ť    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


void Cxlayout::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	mylist.DeleteAllItems();
	mycombobox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Xdistances[i]=-1;
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
	if (myxnum<=1)
	{
		MessageBox(_T("X���������������1"));
		return;
	}
	Xnum=myxnum;
	//��ʼ���б�������б�
    for (int i=0;i<Xnum;i++)
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
	Xdistances[0]=0;
}


void Cxlayout::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString tempstr;
	UpdateData(TRUE);
	if (myStrXselectnum==_T(""))
	{
		MessageBox(_T("��ѡ���кţ�"));
		return;
	}
	if (myStrXselectnum!=_T("1"))
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
	int num=_wtoi(myStrXselectnum);
	//��ӵ��б���
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//��ӵ�������
	Xdistances[num-1]=InternalSpace;
}


BOOL Cxlayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	////////////��ʼ���б��
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString tempsting;
	//��ʼ������ؼ�
	mylist.InsertColumn(0,_T("X���к�"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("����һ�м��"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


LRESULT Cxlayout::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	if (myxnum==0)
	{
		MessageBox(_T("���趨X�򴫸�������"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Xnum;i++)
	{
		if (Xdistances[i]==-1)
		{
			tempstr.Format(_T("������X���%d���м��"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

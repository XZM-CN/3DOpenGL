// Cselectnum.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cselectnum.h"
#include "afxdialogex.h"
#include "myheader.h"


// Cselectnum �Ի���

IMPLEMENT_DYNAMIC(Cselectnum, CDialogEx)

Cselectnum::Cselectnum(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cselectnum::IDD, pParent)
{
	Xposnum=0;
	Yposnum=0;
	Zposnum=0;
	SelectNum=0;
}

Cselectnum::~Cselectnum()
{
}

void Cselectnum::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, mycombox);
}


BEGIN_MESSAGE_MAP(Cselectnum, CDialogEx)
	ON_BN_CLICKED(IDOK, &Cselectnum::OnBnClickedOk)
END_MESSAGE_MAP()


// Cselectnum ��Ϣ�������


BOOL Cselectnum::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString tempstr;
/////��λ��Ϣ
	tempstr.Format(_T("X���%d��\nY���%d��\nZ���%d��"),Xposnum+1,Yposnum+1,Zposnum+1);
	GetDlgItem(IDC_STATIC_FANG)->SetWindowText(tempstr);
/////ԭ��������Ϣ
	double xspace=0;
	for (int xx=0;xx<=Xposnum;xx++)
	{
		xspace+=Xdistances[xx];
	}
	double yspace=0;
	for (int yy=0;yy<=Yposnum;yy++)
	{
		yspace+=Ydistances[yy];
	}
	double zspace=0;
	for (int zz=0;zz<=Zposnum;zz++)
	{
		zspace+=Zdistances[zz];
	}
	tempstr.Format(_T("X=%.1fcm\nY=%.1fcm\nZ=%.1fcm"),xspace,yspace,zspace);
	GetDlgItem(IDC_STATIC_POS)->SetWindowText(tempstr);
/////��Ӵ��������
	for (int i=0;i<Xnum*Ynum*Znum;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		mycombox.AddString(tempstr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void Cselectnum::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	SelectNum=mycombox.GetCurSel()+1;
	CDialogEx::OnOK();
}

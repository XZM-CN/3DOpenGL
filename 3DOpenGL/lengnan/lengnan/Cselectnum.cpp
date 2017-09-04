// Cselectnum.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cselectnum.h"
#include "afxdialogex.h"
#include "myheader.h"


// Cselectnum 对话框

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


// Cselectnum 消息处理程序


BOOL Cselectnum::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  在此添加额外的初始化
	CString tempstr;
/////方位信息
	tempstr.Format(_T("X向第%d行\nY向第%d列\nZ向第%d层"),Xposnum+1,Yposnum+1,Zposnum+1);
	GetDlgItem(IDC_STATIC_FANG)->SetWindowText(tempstr);
/////原点坐标信息
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
/////添加传感器编号
	for (int i=0;i<Xnum*Ynum*Znum;i++)
	{
		tempstr.Format(_T("%d"),i+1);
		mycombox.AddString(tempstr);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Cselectnum::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	SelectNum=mycombox.GetCurSel()+1;
	CDialogEx::OnOK();
}

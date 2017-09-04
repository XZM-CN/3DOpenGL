// Czlayout.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Czlayout.h"
#include "afxdialogex.h"
#include "myheader.h"

// Czlayout 对话框

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


// Czlayout 消息处理程序


void Czlayout::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	mylist.DeleteAllItems();
	mycombox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Zdistances[i]=-1;
	}
	//清除传感器空间方位
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
		MessageBox(_T("请输入Z向网格数！"));
		return;
	}
	Znum=myznum;
	//初始化列表和下拉列表
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
	//添加第一行数组间距
	Zdistances[0]=0;
}


void Czlayout::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	UpdateData(TRUE);
	if (myStrZselectnum==_T(""))
	{
		MessageBox(_T("请选择层号！"));
		return;
	}
	if (myStrZselectnum!=_T("1"))
	{
		if (InternalSpace<=0)
		{
			MessageBox(_T("请输入层间距！"));
			return;
		}
	}
	else
	{
		InternalSpace=0;
		UpdateData(FALSE);
	}
	int num=_wtoi(myStrZselectnum);
	//添加到列表中
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//添加到数组中
	Zdistances[num-1]=InternalSpace;
}


BOOL Czlayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	////////////初始化列表框
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  在此添加额外的初始化
	CString tempsting;
	//初始化报表控件
	mylist.InsertColumn(0,_T("Z向层号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("到上一层间距"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


BOOL Czlayout::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// 设置属性表只有“下一步”按钮    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


LRESULT Czlayout::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (myznum==0)
	{
		MessageBox(_T("请设定Z向传感器个数"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Znum;i++)
	{
		if (Zdistances[i]==-1)
		{
			tempstr.Format(_T("请输入Z向第%d层层间距"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

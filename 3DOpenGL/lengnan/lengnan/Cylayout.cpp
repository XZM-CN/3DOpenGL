// Cylayout.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cylayout.h"
#include "afxdialogex.h"


// Cylayout 对话框

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


// Cylayout 消息处理程序


BOOL Cylayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	////////////初始化列表框
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  在此添加额外的初始化
	CString tempsting;
	//初始化报表控件
	mylist.InsertColumn(0,_T("Y向列号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("到上一列间距"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void Cylayout::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	mylist.DeleteAllItems();
	mycombobox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Ydistances[i]=-1;
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
	if (myynum<=1)
	{
		MessageBox(_T("Y向网格数必须大于1！"));
		return;
	}
	Ynum=myynum;
	//初始化列表和下拉列表
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
	//添加第一行数组间距
	Ydistances[0]=0;
}


void Cylayout::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	UpdateData(TRUE);
	if (myStrYselectnum==_T(""))
	{
		MessageBox(_T("请选择列号！"));
		return;
	}
	if (myStrYselectnum!=_T("1"))
	{
		if (InternalSpace<=0)
		{
			MessageBox(_T("请输入列间距！"));
			return;
		}
	}
	else
	{
		InternalSpace=0;
		UpdateData(FALSE);
	}
	int num=_wtoi(myStrYselectnum);
	//添加到列表中
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//添加到数组中
	Ydistances[num-1]=InternalSpace;
}


BOOL Cylayout::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// 设置属性表只有“下一步”按钮    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


LRESULT Cylayout::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (myynum==0)
	{
		MessageBox(_T("请设定Y向传感器个数"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Ynum;i++)
	{
		if (Ydistances[i]==-1)
		{
			tempstr.Format(_T("请输入Y向第%d列列间距"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

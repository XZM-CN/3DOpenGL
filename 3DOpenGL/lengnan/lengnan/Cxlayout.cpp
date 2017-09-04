// Cxlayout.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Cxlayout.h"
#include "afxdialogex.h"
#include "myheader.h"

// Cxlayout 对话框

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


// Cxlayout 消息处理程序


BOOL Cxlayout::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// 设置属性表只有“下一步”按钮    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_NEXT);  
	return CPropertyPage::OnSetActive();
}


void Cxlayout::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	mylist.DeleteAllItems();
	mycombobox.ResetContent();
	for (int i=0;i<SENSORLAYNUM;i++)
	{
		Xdistances[i]=-1;
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
	if (myxnum<=1)
	{
		MessageBox(_T("X向网格数必须大于1"));
		return;
	}
	Xnum=myxnum;
	//初始化列表和下拉列表
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
	//添加第一行数组间距
	Xdistances[0]=0;
}


void Cxlayout::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString tempstr;
	UpdateData(TRUE);
	if (myStrXselectnum==_T(""))
	{
		MessageBox(_T("请选择行号！"));
		return;
	}
	if (myStrXselectnum!=_T("1"))
	{
		if (InternalSpace<=0)
		{
			MessageBox(_T("请输入行间距！"));
			return;
		}
	}
	else
	{
		InternalSpace=0;
		UpdateData(FALSE);
	}
	int num=_wtoi(myStrXselectnum);
	//添加到列表中
	tempstr.Format(_T("%.3f"),InternalSpace);
	mylist.SetItemText(num-1,1,tempstr);
	//添加到数组中
	Xdistances[num-1]=InternalSpace;
}


BOOL Cxlayout::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	////////////初始化列表框
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  在此添加额外的初始化
	CString tempsting;
	//初始化报表控件
	mylist.InsertColumn(0,_T("X向行号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("到上一行间距"),LVCFMT_LEFT,160);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


LRESULT Cxlayout::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (myxnum==0)
	{
		MessageBox(_T("请设定X向传感器个数"));
		return -1;
	}
	CString tempstr;
	for (int i=0;i<Xnum;i++)
	{
		if (Xdistances[i]==-1)
		{
			tempstr.Format(_T("请输入X向第%d行行间距"),i+1);
			MessageBox(tempstr);
			return -1;
		}
	}
	return CPropertyPage::OnWizardNext();
}

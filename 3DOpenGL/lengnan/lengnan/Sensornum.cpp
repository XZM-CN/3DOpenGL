// Sensornum.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "Sensornum.h"
#include "afxdialogex.h"
#include "Cselectnum.h"

// CSensornum 对话框

IMPLEMENT_DYNAMIC(CSensornum, CPropertyPage)

CSensornum::CSensornum()
	: CPropertyPage(CSensornum::IDD)
	, myxnum(0)
	,myynum(0)
	,myznum(0)
	,redrawflag(TRUE)
	,finishflag(FALSE)
{

}

CSensornum::~CSensornum()
{
}

void CSensornum::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSENSORNUM, mylist);
}


BEGIN_MESSAGE_MAP(CSensornum, CPropertyPage)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LSENSORNUM, &CSensornum::OnNMDblclkLsensornum)
END_MESSAGE_MAP()


// CSensornum 消息处理程序


BOOL CSensornum::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 获得父窗口，即属性表CPropertySheet类    
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// 设置属性表只有“下一步”按钮    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);   
	return CPropertyPage::OnSetActive();
}


void CSensornum::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (!redrawflag)
	{
		return;
	}
	myxnum=Xnum;
	myynum=Ynum;
	myznum=Znum;
////////////添加行、列、层和传感器编号
	CString tempsting;
	int num=0;
	for (int z=0;z<Znum;z++)
	{
		for (int y=0;y<Ynum;y++)
		{
			for (int x=0;x<Xnum;x++)
			{
				//X
				tempsting.Format(_T("%d"),x+1);
				mylist.InsertItem(num,tempsting);
				//Y
				tempsting.Format(_T("%d"),y+1);
				mylist.SetItemText(num,1,tempsting);
				//Z
				tempsting.Format(_T("%d"),z+1);
				mylist.SetItemText(num,2,tempsting);
				//传感器编号
				if (sensor_pos[x][y][z]!=-1)
				{
					tempsting.Format(_T("%d"),sensor_pos[x][y][z]+1);
					mylist.SetItemText(num,3,tempsting);
				}
				//X向空间位置
				double xspace=0;
				for (int xx=0;xx<=x;xx++)
				{
					xspace+=Xdistances[xx];
				}
				tempsting.Format(_T("%.3f"),xspace);
				mylist.SetItemText(num,4,tempsting);
				//Y向空间位置
				double yspace=0;
				for (int yy=0;yy<=y;yy++)
				{
					yspace+=Ydistances[yy];
				}
				tempsting.Format(_T("%.3f"),yspace);
				mylist.SetItemText(num,5,tempsting);
				//Z向空间位置
				double zspace=0;
				for (int zz=0;zz<=z;zz++)
				{
					zspace+=Zdistances[zz];
				}
				tempsting.Format(_T("%.3f"),zspace);
				mylist.SetItemText(num,6,tempsting);
				num++;
			}
		}
	}
	//清除更新标志
	redrawflag=false;
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CPropertyPage::OnPaint()
}


BOOL CSensornum::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//初始化列表类
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  在此添加额外的初始化
	CString tempsting;
	//初始化报表控件
	mylist.InsertColumn(0,_T("X向序号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("Y向序号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(2,_T("Z向层号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(3,_T("传感器编号"),LVCFMT_LEFT,80);
	mylist.InsertColumn(4,_T("X向位置"),LVCFMT_LEFT,80);
	mylist.InsertColumn(5,_T("Y向位置"),LVCFMT_LEFT,80);
	mylist.InsertColumn(6,_T("Z向位置"),LVCFMT_LEFT,80);
	// TODO:  在此添加额外的初始化
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CSensornum::OnNMDblclkLsensornum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	UINT numofselect=mylist.GetSelectedCount();
	if (numofselect==0)
	{
		return;
	}
	POSITION posofselect=mylist.GetFirstSelectedItemPosition();
	if (posofselect==NULL)
	{
		return;
	}
	int index = mylist.GetNextSelectedItem(posofselect);
	//获取选择项的名称
	CString tempx=mylist.GetItemText(index,0);
	CString tempy=mylist.GetItemText(index,1);
	CString tempz=mylist.GetItemText(index,2);
	unsigned int x=_wtoi(tempx);
	x-=1;
	unsigned int y=_wtoi(tempy);
	y-=1;
	unsigned int z=_wtoi(tempz);
	z-=1;
	//弹出传感器选择对话框
	Cselectnum myselectnum;
	myselectnum.Xposnum=x;
	myselectnum.Yposnum=y;
	myselectnum.Zposnum=z;
	if (IDOK==myselectnum.DoModal())
	{  
		CString tempnum;
		//清除原有配置
		if (myselectnum.SelectNum==-1)
		{
		    tempnum=mylist.GetItemText(index,3);	
			if (tempnum!=_T(""))
			{
				unsigned int numofsensor=_wtoi(tempnum);
				//清除传感器方位
				sensor_pos[x][y][z]=-1;
			}
			//清除显示
			tempnum.Format(_T(""));
			mylist.SetItemText(index,3,tempnum);
		}
		else//设置传感器空间方位
		{
			myselectnum.SelectNum;
			//检查传感器是否设置相冲
			//for (int xx=0;xx<Xnum;xx++)
			//{
			//	for (int yy=0;yy<Ynum;yy++)
			//	{
			//		for (int zz=0;zz<Znum;zz++)
			//		{
			//			if (sensor_pos[xx][yy][zz]==myselectnum.SelectNum)
			//			{
			//				tempnum.Format(_T("第%d号传感器已经配置到：\nX向第%d行，Y向第%d列，Z向第%d层"),myselectnum.SelectNum+1,xx+1,yy+1,zz+1);
			//				MessageBox(tempnum);
			//				return;
			//			}
			//		}
			//	}
			//}
			//记录传感器方位
			sensor_pos[x][y][z]=myselectnum.SelectNum;
			//加入列表
			tempnum.Format(_T("%d"),sensor_pos[x][y][z]+1);
			mylist.SetItemText(index,3,tempnum);
		}
	}
}


LRESULT CSensornum::OnWizardBack()
{
	// TODO: 在此添加专用代码和/或调用基类
	//删除所有
	mylist.DeleteAllItems();
	//设置更新标志
	redrawflag=TRUE;
	return CPropertyPage::OnWizardBack();
}


BOOL CSensornum::OnWizardFinish()
{
	// TODO: 在此添加专用代码和/或调用基类
	//计算传感器总量
	totalnum=Xnum*Ynum*Znum;
	finishflag=true;
	return CPropertyPage::OnWizardFinish();
}

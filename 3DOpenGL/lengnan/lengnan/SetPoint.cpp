// SetPoint.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "SetPoint.h"
#include "ADOConn.h"
#include "myheader.h"
//传感器数量
extern int sensors_num;
_RecordsetPtr m_pRecordset0;//指向Recordset对象指针
int nPointNum=0;				//坏点数目
int nPoints[100]={0};			//坏点序号数组
extern float Sensors_Array[3000];
// CSetPoint 对话框

IMPLEMENT_DYNAMIC(CSetPoint, CDialog)

CSetPoint::CSetPoint(CWnd* pParent /*=NULL*/)
	: CDialog(CSetPoint::IDD, pParent)
	, myselectnum(_T(""))
{

}

CSetPoint::~CSetPoint()
{
}

void CSetPoint::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listctrl);
	DDX_Control(pDX, IDC_COMBO1, mysensorcombo);
	DDX_CBString(pDX, IDC_COMBO1, myselectnum);
}


BEGIN_MESSAGE_MAP(CSetPoint, CDialog)
	ON_BN_CLICKED(IDOK, &CSetPoint::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSetPoint::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_SUB, &CSetPoint::OnBnClickedButtonSub)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSetPoint::OnNMRClickList1)
END_MESSAGE_MAP()


// CSetPoint 消息处理程序

void CSetPoint::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

BOOL CSetPoint::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_ESCAPE)
		{
			return TRUE;
		}
		if (pMsg->wParam==VK_RETURN)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSetPoint::OnBnClickedButtonAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int n;
	CString tempstr;
	UpdateData(TRUE);
	if (myselectnum==_T(""))
	{
		MessageBox(_T("请选择坏点传感器！"));
		return;
	}
	n=_wtoi(myselectnum);
	nPoints[nPointNum]=n;
	//检索位置
	int i=0;
	for (;i<sensors_num;i++)
	{
		if (sensornumber[i]==n)
		{
			break;
		}
		
	}
	m_listctrl.InsertItem(nPointNum,myselectnum);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i]);
	m_listctrl.SetItemText(nPointNum,1,tempstr);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i+1]);
	m_listctrl.SetItemText(nPointNum,2,tempstr);
	tempstr.Format(_T("%.2f"),Sensors_Array[3*i+2]);
	m_listctrl.SetItemText(nPointNum,3,tempstr);
	nPointNum++;
}

void CSetPoint::OnBnClickedButtonSub()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0; i<m_listctrl.GetItemCount(); i++) 
	{ 
		if( m_listctrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
		{ 
			m_listctrl.DeleteItem(i);
			for (int j=i;j<nPointNum;j++)
			{
				nPoints[j]=nPoints[j+1];
			}
			nPointNum--;			
		} 
	}
}

BOOL CSetPoint::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString tempstr;
	// TODO:  在此添加额外的初始化
	//控件初始化
	LONG lStyle;
	lStyle = GetWindowLong(m_listctrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listctrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle;
	dwStyle =m_listctrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	dwStyle	|= LVS_EX_ONECLICKACTIVATE;//单击激活
	dwStyle	|= LVS_EX_TRACKSELECT;//自动换行
	m_listctrl.SetExtendedStyle(dwStyle); //设置扩展风格

	m_listctrl.InsertColumn(0,_T("序号"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(1,_T("坐标X"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(2,_T("坐标Y"),LVCFMT_LEFT,60);
	m_listctrl.InsertColumn(3,_T("坐标Z"),LVCFMT_LEFT,60);

	for (int i=0;i<nPointNum;i++)
	{
		tempstr.Format(_T("%d"),nPoints[i]);
		m_listctrl.InsertItem(i,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-3]);
		m_listctrl.SetItemText(i,1,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-2]);
		m_listctrl.SetItemText(i,2,tempstr);
		tempstr.Format(_T("%.2f"),Sensors_Array[3*nPoints[i]-1]);
		m_listctrl.SetItemText(i,3,tempstr);
	}

	//添加传感器编号
	for (int i=0;i<sensors_num;i++)
	{
		if (sensornumber[i]>0)
		{
			CString tempstr;
			tempstr.Format(_T("%d"),sensornumber[i]);
			mysensorcombo.AddString(tempstr);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSetPoint::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	// TODO: 在此添加控件通知处理程序代码
	if(IDNO==::MessageBox(NULL,_T("真的要删除这个点？"),_T("OK"),MB_YESNO|MB_ICONWARNING)) 
		return;
	for(int i=0; i<m_listctrl.GetItemCount(); i++) 
	{ 
		if( m_listctrl.GetItemState(i, LVIS_SELECTED) == LVIS_SELECTED ) 
		{ 
			m_listctrl.DeleteItem(i);
			for (int j=i;j<nPointNum;j++)
			{
				nPoints[j]=nPoints[j+1];
			}
			nPointNum--;			
		} 
	}
	*pResult = 0;
}

// MOdelSet.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "MOdelSet.h"
#include "afxdialogex.h"


// CMOdelSet 对话框

IMPLEMENT_DYNAMIC(CMOdelSet, CPropertyPage)

CMOdelSet::CMOdelSet()
	: CPropertyPage(CMOdelSet::IDD)
	, mymodellongth(0)
	, mymodelwith(0)
	, mymodelheigh(0)
	, mycardnum(0)
	, myfilename(_T(""))
	, pathname(_T("C:\\"))
{

}

CMOdelSet::~CMOdelSet()
{
}

void CMOdelSet::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, mymodellongth);
	DDX_Text(pDX, IDC_EDIT7, mymodelwith);
	DDX_Text(pDX, IDC_EDIT2, mymodelheigh);
	DDX_Text(pDX, IDC_EDIT9, mycardnum);
	DDX_Text(pDX, IDC_EDIT3, myfilename);
}


BEGIN_MESSAGE_MAP(CMOdelSet, CPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON1, &CMOdelSet::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMOdelSet 消息处理程序


BOOL CMOdelSet::OnSetActive()
{
	// TODO: 在此添加专用代码和/或调用基类

	// 获得父窗口，即属性表CPropertySheet类    
	CPropertySheet* psheet = (CPropertySheet*) GetParent();    
	// 设置属性表只有“下一步”按钮    
	psheet->SetWizardButtons(PSWIZB_NEXT); 
	return CPropertyPage::OnSetActive();
}


LRESULT CMOdelSet::OnWizardNext()
{
	// TODO: 在此添加专用代码和/或调用基类
	UpdateData(TRUE);
	if (mymodelheigh<=0&&mymodellongth<=0&&mymodelwith<=0)
	{
		MessageBox(_T("请输入正确的模型尺寸信息！"));
		return -1;
	}
	if (mycardnum<=0)
	{
		MessageBox(_T("请输入正确的采集卡信息！"));
		return -1;
	}
	if (myfilename==_T(""))
	{
		MessageBox(_T("请输入实验名称！"));
		return -1;
	}
	//保存信息
	modelH=mymodelheigh;
	modelL=mymodellongth;
	modelW=mymodelwith;
	cardnum=mycardnum;
	return CPropertyPage::OnWizardNext();
}


void CMOdelSet::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("选择一个源文件子文件夹");
	bi.hwndOwner = ::GetFocus();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = strDirName;
	bi.lpszTitle = szString;
	bi.ulFlags = BIF_BROWSEFORCOMPUTER | BIF_DONTGOBELOWDOMAIN | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;
	LPITEMIDLIST pItemIDList = ::SHBrowseForFolder(&bi);
	if(pItemIDList == NULL)
	{
		return ;
	}
	::SHGetPathFromIDList(pItemIDList,strDirName);
	pathname.Format(_T("%s"),strDirName);
	pathname.TrimRight('\\');
	pathname+=_T("\\");
}

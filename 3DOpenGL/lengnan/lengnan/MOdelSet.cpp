// MOdelSet.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "MOdelSet.h"
#include "afxdialogex.h"


// CMOdelSet �Ի���

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


// CMOdelSet ��Ϣ�������


BOOL CMOdelSet::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ��ø����ڣ������Ա�CPropertySheet��    
	CPropertySheet* psheet = (CPropertySheet*) GetParent();    
	// �������Ա�ֻ�С���һ������ť    
	psheet->SetWizardButtons(PSWIZB_NEXT); 
	return CPropertyPage::OnSetActive();
}


LRESULT CMOdelSet::OnWizardNext()
{
	// TODO: �ڴ����ר�ô����/����û���
	UpdateData(TRUE);
	if (mymodelheigh<=0&&mymodellongth<=0&&mymodelwith<=0)
	{
		MessageBox(_T("��������ȷ��ģ�ͳߴ���Ϣ��"));
		return -1;
	}
	if (mycardnum<=0)
	{
		MessageBox(_T("��������ȷ�Ĳɼ�����Ϣ��"));
		return -1;
	}
	if (myfilename==_T(""))
	{
		MessageBox(_T("������ʵ�����ƣ�"));
		return -1;
	}
	//������Ϣ
	modelH=mymodelheigh;
	modelL=mymodellongth;
	modelW=mymodelwith;
	cardnum=mycardnum;
	return CPropertyPage::OnWizardNext();
}


void CMOdelSet::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	TCHAR strDirName[MAX_PATH];
	BROWSEINFO bi;
	CString szString = TEXT("ѡ��һ��Դ�ļ����ļ���");
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

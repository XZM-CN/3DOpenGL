#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// Czlayout �Ի���

class Czlayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Czlayout)

public:
	Czlayout();
	virtual ~Czlayout();

// �Ի�������
	enum { IDD = IDD_ZLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	UINT myznum;
	CString myStrZselectnum;
	CComboBox mycombox;
	double InternalSpace;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
	CListCtrl mylist;
};

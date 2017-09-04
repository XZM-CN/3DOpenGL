#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// Czlayout 对话框

class Czlayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Czlayout)

public:
	Czlayout();
	virtual ~Czlayout();

// 对话框数据
	enum { IDD = IDD_ZLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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

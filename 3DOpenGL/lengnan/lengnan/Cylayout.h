#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "myheader.h"

// Cylayout 对话框

class Cylayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Cylayout)

public:
	Cylayout();
	virtual ~Cylayout();

// 对话框数据
	enum { IDD = IDD_YLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	UINT myynum;
	CString myStrYselectnum;
	double InternalSpace;
	CComboBox mycombobox;
	CListCtrl mylist;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnSetActive();
	virtual LRESULT OnWizardNext();
};

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Cxlayout 对话框

class Cxlayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Cxlayout)

public:
	Cxlayout();
	virtual ~Cxlayout();

// 对话框数据
	enum { IDD = IDD_XSENSORLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// X向传感器个数
	UINT myxnum;
	// X向行间距
	double InternalSpace;
	CString myStrXselectnum;
	CListCtrl mylist;
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	virtual BOOL OnInitDialog();
	CComboBox mycombobox;
	virtual LRESULT OnWizardNext();
};

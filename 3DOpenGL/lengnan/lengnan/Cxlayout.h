#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// Cxlayout �Ի���

class Cxlayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Cxlayout)

public:
	Cxlayout();
	virtual ~Cxlayout();

// �Ի�������
	enum { IDD = IDD_XSENSORLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// X�򴫸�������
	UINT myxnum;
	// X���м��
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

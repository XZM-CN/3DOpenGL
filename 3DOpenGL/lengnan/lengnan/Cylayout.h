#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "myheader.h"

// Cylayout �Ի���

class Cylayout : public CPropertyPage
{
	DECLARE_DYNAMIC(Cylayout)

public:
	Cylayout();
	virtual ~Cylayout();

// �Ի�������
	enum { IDD = IDD_YLAYOUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

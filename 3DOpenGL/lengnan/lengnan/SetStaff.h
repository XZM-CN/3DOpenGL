#pragma once
#include "afxwin.h"


// CSetStaff 对话框

class CSetStaff : public CDialogEx
{
	DECLARE_DYNAMIC(CSetStaff)

public:
	CSetStaff(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetStaff();

// 对话框数据
	enum { IDD = IDD_SETSTAFF };
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnRadioShow(UINT uID);
	afx_msg void OnRadioShow1(UINT uID);
	afx_msg void OnRadioShow2(UINT uID);
	afx_msg void OnRadioShow3(UINT uID);

	DECLARE_MESSAGE_MAP()
public:
	int m_value1;
	int m_value2;
	int m_value3;
	int m_value4;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	int m_value22;
	int m_value33;
	int m_value44;
	int m_value55;
	CEdit m_edit1;
	CEdit m_edit2;
	CEdit m_edit3;
	afx_msg void OnEnChangeEdit10();
	afx_msg void OnEnChangeEdit11();
	afx_msg void OnEnChangeEdit12();
};

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSetPoint 对话框

class CSetPoint : public CDialog
{
	DECLARE_DYNAMIC(CSetPoint)

public:
	CSetPoint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetPoint();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSub();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	// 传感器选择下拉列表
	CComboBox mysensorcombo;
	// 传感器选择编号
	CString myselectnum;
};

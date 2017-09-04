#pragma once
#include "afxcmn.h"


// CAddIso 对话框

class CAddIso : public CDialogEx
{
	DECLARE_DYNAMIC(CAddIso)

public:
	CAddIso(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAddIso();

// 对话框数据
	enum { IDD = IDD_DIALOG_ISO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	float m_isofacedata;
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSub();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnInitDialog();
	virtual BOOL OnInitDialog();
	CListCtrl m_list2;
	afx_msg void OnBnClickedCancel();
};

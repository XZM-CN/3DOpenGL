#pragma once
#include "afxcmn.h"


// CAddIso �Ի���

class CAddIso : public CDialogEx
{
	DECLARE_DYNAMIC(CAddIso)

public:
	CAddIso(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAddIso();

// �Ի�������
	enum { IDD = IDD_DIALOG_ISO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

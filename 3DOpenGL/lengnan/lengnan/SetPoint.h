#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSetPoint �Ի���

class CSetPoint : public CDialog
{
	DECLARE_DYNAMIC(CSetPoint)

public:
	CSetPoint(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetPoint();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETPOINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listctrl;
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonSub();
	virtual BOOL OnInitDialog();
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	// ������ѡ�������б�
	CComboBox mysensorcombo;
	// ������ѡ����
	CString myselectnum;
};

#pragma once


// CTextToAccess �Ի���

class CTextToAccess : public CDialogEx
{
	DECLARE_DYNAMIC(CTextToAccess)

public:
	CTextToAccess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTextToAccess();

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	int tsensor_num;
	int tcount;
	afx_msg void OnBnClickedOk();
};

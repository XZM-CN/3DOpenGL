#pragma once


// CSetServer �Ի���

class CSetServer : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServer)

public:
	CSetServer(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSetServer();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	SOCKET m_Client;
	// �˿ں�
	int m_Port;
	CString m_ServerIp;
};

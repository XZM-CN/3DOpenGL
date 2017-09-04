#pragma once


// CSetServer 对话框

class CSetServer : public CDialogEx
{
	DECLARE_DYNAMIC(CSetServer)

public:
	CSetServer(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetServer();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	SOCKET m_Client;
	// 端口号
	int m_Port;
	CString m_ServerIp;
};

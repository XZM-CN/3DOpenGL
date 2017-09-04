#pragma once


// CTextToAccess 对话框

class CTextToAccess : public CDialogEx
{
	DECLARE_DYNAMIC(CTextToAccess)

public:
	CTextToAccess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTextToAccess();

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int tsensor_num;
	int tcount;
	afx_msg void OnBnClickedOk();
};

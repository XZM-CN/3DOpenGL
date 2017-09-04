#pragma once


// CAVIPrama 对话框

class CAVIPrama : public CDialogEx
{
	DECLARE_DYNAMIC(CAVIPrama)

public:
	CAVIPrama(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAVIPrama();

// 对话框数据
	enum { IDD = IDD_DAVI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int mywidth;
	int myheiht;
	int myfps;
	CString myname;
	afx_msg void OnBnClickedButton1();
	CString myfilepath;
	afx_msg void OnBnClickedOk();
};

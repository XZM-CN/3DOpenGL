#pragma once


// CAutoPlay 对话框

class CAutoPlay : public CDialog
{
	DECLARE_DYNAMIC(CAutoPlay)

public:
	CAutoPlay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAutoPlay();

// 对话框数据
	enum { IDD = IDD_DIALOG_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckAvi();
	bool m_aviflag;
	int m_nspace;
	// 帧率
	int m_nFrameRate;
	CString m_AviFileName;
	afx_msg void OnBnClickedButton1();
};

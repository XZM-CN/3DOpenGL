#pragma once


// CAutoPlay �Ի���

class CAutoPlay : public CDialog
{
	DECLARE_DYNAMIC(CAutoPlay)

public:
	CAutoPlay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAutoPlay();

// �Ի�������
	enum { IDD = IDD_DIALOG_PLAY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCheckAvi();
	bool m_aviflag;
	int m_nspace;
	// ֡��
	int m_nFrameRate;
	CString m_AviFileName;
	afx_msg void OnBnClickedButton1();
};

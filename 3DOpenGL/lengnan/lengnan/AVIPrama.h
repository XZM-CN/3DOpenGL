#pragma once


// CAVIPrama �Ի���

class CAVIPrama : public CDialogEx
{
	DECLARE_DYNAMIC(CAVIPrama)

public:
	CAVIPrama(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAVIPrama();

// �Ի�������
	enum { IDD = IDD_DAVI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

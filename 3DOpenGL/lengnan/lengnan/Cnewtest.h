#pragma once


// Cnewtest �Ի���

class Cnewtest : public CDialogEx
{
	DECLARE_DYNAMIC(Cnewtest)

public:
	Cnewtest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~Cnewtest();

// �Ի�������
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString mytestname;
	CString mydatapath;
	CString mypictpath;
	double myinetnaltime;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	// �¶ȴ���������
	CString m_TSensorCom;
	// �¶ȿ���������
	CString m_ConCtral_com;
	// ����ͨ������
	CString m_classname;
	afx_msg void OnCbnSelchangeCombo3();
	// ѹ���ɼ�����
	CString m_pcom;
	virtual BOOL OnInitDialog();
};

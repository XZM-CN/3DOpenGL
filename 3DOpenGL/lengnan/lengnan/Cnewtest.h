#pragma once


// Cnewtest 对话框

class Cnewtest : public CDialogEx
{
	DECLARE_DYNAMIC(Cnewtest)

public:
	Cnewtest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cnewtest();

// 对话框数据
	enum { IDD = IDD_DIALOG6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString mytestname;
	CString mydatapath;
	CString mypictpath;
	double myinetnaltime;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedOk();
	// 温度传感器串口
	CString m_TSensorCom;
	// 温度控制器串口
	CString m_ConCtral_com;
	// 串口通信种类
	CString m_classname;
	afx_msg void OnCbnSelchangeCombo3();
	// 压力采集串口
	CString m_pcom;
	virtual BOOL OnInitDialog();
};

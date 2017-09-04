#pragma once


// CPointNow 对话框

class CPointNow : public CDialogEx
{
	DECLARE_DYNAMIC(CPointNow)

public:
	CPointNow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPointNow();

// 对话框数据
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	float m_point_x;
	float m_point_y;
	float m_point_z;
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};

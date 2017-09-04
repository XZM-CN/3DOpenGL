#pragma once


// CSetWellDlg 对话框

class CSetWellDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetWellDlg)

public:
	CSetWellDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSetWellDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WELL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnWellRadio(UINT ucmdID);
	afx_msg void OnWellRadio2(UINT ucmdID);
	// 起始坐标
	float m_start;
	// 终止坐标
	float m_end;
	int m_nwellmode;
	int m_nwellorientations;
	int m_wellflag;
	virtual BOOL OnInitDialog();
	// x向起点坐标
	double x_start;
	// Y向起点坐标
	double y_start;
	// Z向起点坐标
	double Z_start;
};

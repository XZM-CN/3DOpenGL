#pragma once
#include "afxwin.h"
#include "myheader.h"

// Cselectnum 对话框

class Cselectnum : public CDialogEx
{
	DECLARE_DYNAMIC(Cselectnum)

public:
	Cselectnum(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~Cselectnum();

// 对话框数据
	enum { IDD = IDD_DIALOG5 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 传感器下拉列表
	CComboBox mycombox;
	virtual BOOL OnInitDialog();
///选择的空间点的方位坐标
	unsigned int Xposnum;
	unsigned int Yposnum;
	unsigned int Zposnum;
	unsigned int SelectNum;
	afx_msg void OnBnClickedOk();
};

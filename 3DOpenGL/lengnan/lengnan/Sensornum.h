#pragma once
#include "myheader.h"
#include "afxcmn.h"

// CSensornum 对话框

class CSensornum : public CPropertyPage
{
	DECLARE_DYNAMIC(CSensornum)

public:
	CSensornum();
	virtual ~CSensornum();

// 对话框数据
	enum { IDD = IDD_DSENORNUM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	afx_msg void OnPaint();
	// X向传感器个数
	int myxnum;
	// Y向传感器个数
	int myynum;
	// Z向层数
	int myznum;
	//重绘更新标志
	bool redrawflag;
	virtual BOOL OnInitDialog();
	// 传感器序号列表
	CListCtrl mylist;
	afx_msg void OnNMDblclkLsensornum(NMHDR *pNMHDR, LRESULT *pResult);
	virtual LRESULT OnWizardBack();
	virtual BOOL OnWizardFinish();
	//向导完成标志位
	bool finishflag;
};

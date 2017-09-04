#pragma once
#include "myheader.h"

// CMOdelSet 对话框

class CMOdelSet : public CPropertyPage
{
	DECLARE_DYNAMIC(CMOdelSet)

public:
	CMOdelSet();
	virtual ~CMOdelSet();

// 对话框数据
	enum { IDD = IDD_MODELPARAM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	// 模型长度
	double mymodellongth;
	// 模型宽度
	double mymodelwith;
	// 模型高度
	double mymodelheigh;
	// 采集卡数量
	int mycardnum;
	virtual LRESULT OnWizardNext();
	// 配置文件名称
	CString myfilename;
	//配置文件保存位置
	CString pathname;
	afx_msg void OnBnClickedButton1();
};

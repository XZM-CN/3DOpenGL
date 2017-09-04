#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CLena 窗体视图

class CLena : public CFormView
{
	DECLARE_DYNCREATE(CLena)

protected:
	CLena();           // 动态创建所使用的受保护的构造函数
	virtual ~CLena();

public:
	enum { IDD = IDD_DIALOG_LENA };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	// 透明度滑动条
	CSliderCtrl m_SliderCtrl;
	// 透明度
	int m_nSlider;
	//afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void OnInitialUpdate();
	CView *pActiveView; 
	afx_msg void OnBnClickedShowpoint();
	afx_msg void OnBnClickedShowdata();
	afx_msg void OnRadio(UINT uID);
	afx_msg void OnRadioShow(UINT uID);
	int m_radio;
	afx_msg void OnBnClickedShowmode();
	BOOL m_bshow;
	afx_msg void OnDeltaposSpin1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	// 温度最大值
	int m_temmax;
	// 温度最小值
	int m_temmin;
	CEdit m_temedit1;
	CEdit m_temedit2;
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnEnChangeEdit2();
	bool tempflag1;
	bool tempflag2;
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnBnClickedCheck2();
	afx_msg void OnBnClickedCheck3();
	afx_msg void OnBnClickedCheck4();
	afx_msg void OnBnClickedCheck5();
	afx_msg void OnBnClickedCheck6();
	afx_msg void OnBnClickedShowIsosurface();
	//afx_msg void OnNMCustomdrawSlider2(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderCtrl2;
	int m_nSlider2;
	afx_msg void OnBnClickedCheckPerspective();
	//afx_msg void OnNMCustomdrawSlider3(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderCtrl3;
	// 透视参数
	int m_nSlider3;
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	// 剖分x轴向最小值
	float m_xmin;
	// 剖分y轴向最小值
	float m_ymin;
	// 剖分z轴向最小值
	float m_zmin;
	//afx_msg void OnNMCustomdrawSlider4(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMCustomdrawSlider5(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMCustomdrawSlider6(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMCustomdrawSlider7(NMHDR *pNMHDR, LRESULT *pResult);
	CSliderCtrl m_SliderCtrl4;
	CSliderCtrl m_SliderCtrl5;
	CSliderCtrl m_SliderCtrl6;
	int m_nSlider4;
	int m_nSlider5;
	int m_nSlider6;
	afx_msg void OnBnClickedShowdata2();
	CButton m_ctrlcheck1;
	CButton m_ctrlcheck2;
	// 显示井标志
	BOOL m_diswellflag;
	afx_msg void OnBnClickedCheckDiswell();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};



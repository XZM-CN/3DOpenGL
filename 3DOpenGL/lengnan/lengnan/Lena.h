#pragma once
#include "afxcmn.h"
#include "afxwin.h"



// CLena ������ͼ

class CLena : public CFormView
{
	DECLARE_DYNCREATE(CLena)

protected:
	CLena();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	// ͸���Ȼ�����
	CSliderCtrl m_SliderCtrl;
	// ͸����
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
	// �¶����ֵ
	int m_temmax;
	// �¶���Сֵ
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
	// ͸�Ӳ���
	int m_nSlider3;
	afx_msg void OnBnClickedCheck7();
	afx_msg void OnBnClickedCheck8();
	afx_msg void OnBnClickedCheck9();
	afx_msg void OnBnClickedCheck10();
	// �ʷ�x������Сֵ
	float m_xmin;
	// �ʷ�y������Сֵ
	float m_ymin;
	// �ʷ�z������Сֵ
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
	// ��ʾ����־
	BOOL m_diswellflag;
	afx_msg void OnBnClickedCheckDiswell();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};



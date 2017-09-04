#pragma once


// CMyToolBar

class CMyToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CMyToolBar)

public:
	CMyToolBar();
	virtual ~CMyToolBar();

protected:
	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_MySlider;
	CScrollBar m_MyScroll;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//更新标志
	bool Setpos();
};



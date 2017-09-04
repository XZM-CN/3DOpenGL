#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CShowPoint 对话框

class CShowPoint : public CDialogEx
{
	DECLARE_DYNAMIC(CShowPoint)

public:
	CShowPoint(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowPoint();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOWPOINT };
public:
	int point_num;
	//double DatatNum[1000][10000];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	int m_Point;
	CListCtrl m_List;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnPaint(); 
	void fndiscurve(double  maxvalue,double  internaltime,CWnd* Ctlwnd);
	void fndrawcurve(double  maxvalue, double  internaltime,double  data[],CWnd* Ctlwnd,unsigned short* color);
	BOOL ChangeValue2Color(float Max,float Min,float currvalue,unsigned short* color);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnClose();
 	afx_msg void OnOK();
// 	afx_msg void OnCancel();
public:
	static UINT ShowPointT(LPVOID pPrama);
	static UINT ShowLine(LPVOID pPrama);
	int m_timeintervalue;
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnEnChangeEdit2();
};

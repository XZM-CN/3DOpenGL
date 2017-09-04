// MainFrm.h : CMainFrame ��Ľӿ�
//


#pragma once
#include "afxdb.h"
#include "lengnanView.h"
#include "MyToolBar.h"
#include "afxcmn.h"
#include "winsock2.h"
#include "Lena.h"
#include "AVIPrama.h"
#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#define MAXPOLLTAB 50
#define USER_PROGRESS WM_USER+0x0002



class CMainFrame : public CFrameWnd
{
	
protected: // �������л�����
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ʵ��
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // �ؼ���Ƕ���Ա
// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
public:
	CMyToolBar	m_wndExtendBar;
	CStatusBar  m_wndStatusBar;
	CSplitterWnd spw;
	afx_msg void OnLoadfile();
protected:
	virtual void PostNcDestroy();
	void DockControlBarLeftOf(CToolBar *Bar,CToolBar *LeftOf);
public:
	afx_msg void OnClose();
	afx_msg void OnHistory();
	afx_msg void OnStart();
	afx_msg void OnSetpoint();
	ClengnanView *pActiveView; 
	CLena* pLenaView;
	CString DatabaseName;
	CString mysavepath;//���ݿⱣ��·��
	virtual BOOL DestroyWindow();
	afx_msg void OnShowLine();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetBadPoints();
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	// ���Ż�����
	CSliderCtrl m_SliderCtrl_x;
	// ����
	int m_nSlider_x;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGoon();
	afx_msg void OnSuspend();
	afx_msg void OnDestroy();
	CProgressCtrl m_progress;
	afx_msg LRESULT OnProgress(WPARAM wParam,LPARAM lParam);
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClearlast();
	afx_msg void OnPause();
	afx_msg BOOL OnToolTipNotify( UINT id, NMHDR * pNMHDR, LRESULT * pResult );
	int IMPInit(void);
	int m_IMPNum;	//IMP����
	//int LoadIMPDrv(HWND hWnd);
	int LocateCard();
	afx_msg void OnSetmodel();
	SOCKET m_Client;
	// �˿ں�
	int m_Port;
	CString m_ServerIp;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ReceiveData(void);
	
	void SaveData(void);
	afx_msg void OnBtoa();
	afx_msg void OnDbcompose();
	afx_msg void OnSetstaff();
	afx_msg void OnSetstaff1();
////��ȡ�����ļ�
	bool ReadfromFile(CString& filename);
////д�������ļ�
	bool WritetoFile(CString& filename);
	afx_msg void OnMsarttest();
	afx_msg void OnMstoptest();
	afx_msg void OnMstartavi();
	afx_msg void OnMstopavi();
	afx_msg void OnMstophistry();
//////////AVI�ļ��������:��Ƶ����
private:
	//��Ƶд����
	CvVideoWriter *Pvideowrite;
	//��Ƶ���
	int Xwidth;
	//��Ƶ�߶�
	int Yheigth;
	//֡��
	int FPS;
	//��Ƶ�����λ��
	CString Avipathname;
public:
	//OPenCV��ƵAVI�ļ�����
	bool SaveBMPToAVI();
	bool InitAVI();
	bool CloseAvi();
///////////////////////////////////////
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMsvpicttest();
	afx_msg void OnMsvpicthistory();
};



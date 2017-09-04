// MainFrm.h : CMainFrame 类的接口
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
	
protected: // 仅从序列化创建
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

// 操作
public:

// 重写
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 实现
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // 控件条嵌入成员
// 生成的消息映射函数
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
	CString mysavepath;//数据库保存路径
	virtual BOOL DestroyWindow();
	afx_msg void OnShowLine();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void SetBadPoints();
	afx_msg void OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult);
	// 播放滑动条
	CSliderCtrl m_SliderCtrl_x;
	// 进度
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
	int m_IMPNum;	//IMP数量
	//int LoadIMPDrv(HWND hWnd);
	int LocateCard();
	afx_msg void OnSetmodel();
	SOCKET m_Client;
	// 端口号
	int m_Port;
	CString m_ServerIp;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void ReceiveData(void);
	
	void SaveData(void);
	afx_msg void OnBtoa();
	afx_msg void OnDbcompose();
	afx_msg void OnSetstaff();
	afx_msg void OnSetstaff1();
////读取配置文件
	bool ReadfromFile(CString& filename);
////写入配置文件
	bool WritetoFile(CString& filename);
	afx_msg void OnMsarttest();
	afx_msg void OnMstoptest();
	afx_msg void OnMstartavi();
	afx_msg void OnMstopavi();
	afx_msg void OnMstophistry();
//////////AVI文件保存变量:视频变量
private:
	//视频写入类
	CvVideoWriter *Pvideowrite;
	//视频宽度
	int Xwidth;
	//视频高度
	int Yheigth;
	//帧率
	int FPS;
	//视频保存的位置
	CString Avipathname;
public:
	//OPenCV视频AVI文件保存
	bool SaveBMPToAVI();
	bool InitAVI();
	bool CloseAvi();
///////////////////////////////////////
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnMsvpicttest();
	afx_msg void OnMsvpicthistory();
};



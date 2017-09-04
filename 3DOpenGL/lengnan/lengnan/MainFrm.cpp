// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "lengnan.h"
#include "ModelSheet.h"
#include "lengnanView.h"
#include "Cnewtest.h"
#include "MainFrm.h"

#include "Lena.h"
#include "ADOConn.h"
#include "Shlwapi.h"
#include "SetPoint.h"
#include "SplashWnd.h"
#include <string>
#include "afxmt.h"
//#include "ToAvi.h"
#include "SetServer.h"
#include "ShowPoint.h"
#include "TextToAccess.h"
#include "SetStaff.h"
#include "myheader.h"
#include "Histoty.h"
#include "Com.h"
//#include "SetModel.h"

BOOL SetStaff=TRUE;
extern int temvalue1;
extern int temvalue2;
extern int temvalue3;
extern int temvalue4;
extern int temvalue5;
#define MSG_CLIENT 1000
#define Collect_Num 48
#define ChanNum 20

using namespace std;

#pragma comment(lib,"Shlwapi.lib") 
#pragma comment(lib,"ws2_32.lib")

#import "C:\Program Files\Common Files\System\ado\msadox.dll"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int ShowPoint[1000];

//当前实验进行了多少秒
int nowtime[10000];
double DatatNum[1000][10000];


CADOConn adoconn;
//读取数据线程
CWinThread *ThreadBToA=NULL;
//线程同步控制事件
CEvent DataEvent3;
CEvent DataSend;

int sjbc_socond=5;
//采集板数量
int IMP_Num=0;
int cjbNum;
HINSTANCE hDllInst=NULL;		//输力强dll实例句柄
long data_index=0;

//模型信息显示
BOOL m_bModleInfo=FALSE;
//已开始记录标志位
bool startwriteflag=false;//实验开始控制位
bool recordflag=false;//开始记录控制位
//压力温度显示选择标准：false压力显示，true温度显示
bool Tempflag=false;
//井线条数
extern int well_num;
extern CMyToolBar* pMyToolBar;
bool bSliderflag=false;
bool threadstarrtflag=false;
extern GLint viewport[4];
CEvent g_eventKill;
bool threadoverflag=false;
bool opengliniflag=false;	//opengl需要初始化标志位
BOOL FolderExist(CString strPath);
bool DeleteDirectory(char* sDirName);
extern CLena* pLena;//面板指针
extern void ImageToAVI( LPCTSTR pAviName, LPCTSTR pDir, int videoWidth, int videoHeight, int bpp );
int nAviFrameRate=20;
//读取历史数据标志
bool bAviFlag=false;//导出AVI文件标志
bool ThreadReadDataflag=false; //读取历史记录标志
bool ThreadReadDataflag0=false; //自动播放标志
bool bSuspendThreadflag=false;	//线程即将挂起标志
bool m_playmoveflag=false; //播放滑动条有移动标志量
bool bpicture=false;//保存图片标志
int m_PlayPos=1;
//串口通讯线程
CWinThread *ThreadReadCom;
//读取串口线程函数
static UINT ReadCom(LPVOID *pPrama);
//读取数据线程
CWinThread *ThreadReadData;
//读取数据线程函数
static UINT ReadData(LPVOID *pPrama);

//线程同步控制事件
CEvent DataEvent;
CEvent DataEvent2;
//回放模式标志位
bool Historyflag(false);
//数据采集的时间点
CString timestring;

CString mypicsavepath;//截图视频保存路径

double OldDataNumbers[1000];//模型数据
extern int nPointNum;				//坏点数目
extern int nPoints[100];			//坏点序号数组

//模型参数
float Model_Length;
float Model_Width;
float Model_Height;
UINT Length_Num,Width_Num,Height_Num;
float DataNumbers[1000];
float Sensors_Array[3000];
//传感器数量
int sensors_num;
//各个面的值
float z_down,z_up;//上、下
float y_front,y_back;//前、后
float x_left,x_right;//左、右
/////////////用于记录的数据表
_RecordsetPtr Pmyrecordset[10]={NULL};

//数据的组数
int order_number_total=1000;

// 温度最小值
extern int mymin;
// 温度最大值
extern int mymax;

extern Vector3D Pt3d;
extern bool show_coordinateflag;
//串口通信类
CCom *m_com_Tsensor;
CCom *m_com_TConctral;
CCom *m_com_Pre;
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(IDM_LOADFILE, &CMainFrame::OnLoadfile)
	ON_WM_CLOSE()
	ON_COMMAND(IDM_HISTORY, &CMainFrame::OnHistory)
	ON_COMMAND(IDM_START, &CMainFrame::OnStart)
	ON_COMMAND(IDM_SETPOINT, &CMainFrame::OnSetpoint)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDERCTRL, &CMainFrame::OnNMCustomdrawSlider)
	ON_WM_HSCROLL()
	ON_COMMAND(IDM_GOON, &CMainFrame::OnGoon)
	ON_COMMAND(IDM_SUSPEND, &CMainFrame::OnSuspend)
	ON_WM_DESTROY()
	ON_MESSAGE(USER_PROGRESS,&CMainFrame::OnProgress)
	ON_WM_PAINT()
	ON_COMMAND(IDM_CLEARLAST, &CMainFrame::OnClearlast)
	ON_COMMAND(IDM_PAUSE, &CMainFrame::OnPause)
	ON_NOTIFY_EX(TTN_NEEDTEXT, 0, &CMainFrame::OnToolTipNotify)
	ON_COMMAND(IDM_SETMODEL, &CMainFrame::OnSetmodel)
	ON_COMMAND(IDM_SHOWLINE, &CMainFrame::OnShowLine)
	ON_COMMAND(IDM_SETSTAFF, &CMainFrame::OnSetstaff)
	ON_COMMAND(IDM_SETSTAFF1, &CMainFrame::OnSetstaff1)
	ON_COMMAND(ID_MSARTTEST, &CMainFrame::OnMsarttest)
	ON_COMMAND(ID_MSTOPTEST, &CMainFrame::OnMstoptest)
	ON_COMMAND(ID_MSTARTAVI, &CMainFrame::OnMstartavi)
	ON_COMMAND(ID_MSTOPAVI, &CMainFrame::OnMstopavi)
	ON_COMMAND(ID_MSTOPHISTRY, &CMainFrame::OnMstophistry)
	ON_WM_SYSCOMMAND()
	ON_COMMAND(ID_MSVPICTTEST, &CMainFrame::OnMsvpicttest)
	ON_COMMAND(ID_MSVPICTHISTORY, &CMainFrame::OnMsvpicthistory)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	IDS_PROGRESS,			// 进度条
	IDS_PRONUM,
	IDS_COORDINATE,
	IDS_TIMER,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
	DatabaseName=_T("");
	mypicsavepath=_T("C:\\");
	hDllInst=NULL;
	m_IMPNum=0;
	m_Client=socket(AF_INET,SOCK_STREAM,0);
	Pvideowrite=NULL;
	m_bAutoMenuEnable=FALSE;
	m_com_TConctral=NULL;
	m_com_Tsensor=NULL;
	m_com_Pre=NULL;
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	//定义扩展工具框
	if (!m_wndExtendBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndExtendBar.LoadToolBar(IDR_EXTEND_BAR))
	{
		TRACE(_T("未能创建工具栏\n"));
		return -1;
	}
	m_wndExtendBar.SetWindowText(_T("扩展工具栏"));
	CRect rect;
	//设置组合框宽度
	GetWindowRect(&rect);
	//返回屏幕宽度
	int   nWidth   =   GetSystemMetrics(SM_CXSCREEN);
	m_wndExtendBar.SetButtonInfo(1,IDC_SRICROLLPLAY,TBBS_SEPARATOR,nWidth-40);
	//得到组合框位置
	m_wndExtendBar.GetItemRect(1,&rect);
	rect.top-=6;
	rect.bottom-=6;
	//创建滚动条
	if (!m_wndExtendBar.m_MyScroll.Create(TBS_HORZ|WS_VISIBLE|TBS_TOP|TBS_BOTTOM,rect,&m_wndExtendBar,IDC_SRICROLLPLAY))
		return -1;
	//order_number_total=1000;
	m_wndExtendBar.m_MyScroll.SetScrollRange(0,order_number_total,TRUE);//设置控件最小最大值
	m_wndExtendBar.m_MyScroll.SetScrollPos(1,TRUE);//设置初始位置
	m_wndExtendBar.m_MyScroll.ShowScrollBar(TRUE);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	CTime t=CTime::GetCurrentTime();
	CString strr=t.Format(_T("%Y-%m-%d %H:%M:%S"));
	//CClientDC dc(this);
	//CWindowDC dc(this);
	//CSize sz=dc.GetTextExtent(strr);
	CSize sz=200;
	int index=m_wndStatusBar.CommandToIndex(IDS_TIMER);
	m_wndStatusBar.SetPaneInfo(index,IDS_TIMER,SBPS_NORMAL,sz.cx);
	m_wndStatusBar.SetPaneText(index,strr);

	////工具条停靠
	////调用CToolBar中的成员函数设置工具栏可停靠的位置 
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	////调用框架类中的成员函数设置哪里可以被停靠  
	//EnableDocking(CBRS_ALIGN_ANY);  
	//m_wndToolBar.GetWindowRect(&rect);
	////指向停靠的工具条.  
	//DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	//停靠新工具条
	m_wndExtendBar.EnableDocking(CBRS_ALIGN_ANY);

	//DockControlBarLeftOf(&m_wndExtendBar,&m_wndToolBar);

	//CSplashWnd::ShowSplashScreen(this);
	//设定定时器
	SetTimer(1, 1000, NULL); 
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	//cs.style &= ~WS_MAXIMIZEBOX; //禁止窗口最大化
	//cs.style &= ~WS_MINIMIZEBOX; //禁止窗口最小化

	return TRUE;
}


// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame 消息处理程序




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	 //TODO: 在此添加专用代码和/或调用基类
	CRect rect;
	GetWindowRect(&rect);
	int   nWidth   =   GetSystemMetrics(SM_CXSCREEN);    
	int   nHeight=   GetSystemMetrics(SM_CYSCREEN);

	//将主窗口一分为二
	if (!spw.CreateStatic(this,1,2))
	{
		return FALSE;
	}	
	
	spw.CreateView(0,1,RUNTIME_CLASS(CLena),CSize(nWidth*0.17,nHeight),pContext);
	spw.CreateView(0,0,RUNTIME_CLASS(ClengnanView),CSize(nWidth*0.83,nHeight),pContext);
	pActiveView=(ClengnanView*)spw.GetPane(0,0);
	pLenaView=(CLena*)spw.GetPane(0,1);
	return TRUE;


	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

void CMainFrame::OnLoadfile()
{
    if (sensors_num>0)
    {
		MessageBox(_T("模型已经建立！"));
		return;
    }
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CString filepathname;
	CFileDialog hFileDlg(FALSE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,TEXT("TXT(*.txt)|*.txt||"),NULL);

	hFileDlg.m_ofn.nFilterIndex=1; //
	hFileDlg.m_ofn.hwndOwner=m_hWnd;  //
	hFileDlg.m_ofn.lStructSize=sizeof(OPENFILENAME);   //
	hFileDlg.m_ofn.lpstrTitle=TEXT("打开配置文件");  //CFileDialog 对话框标题 
	hFileDlg.m_ofn.nMaxFile=MAX_PATH;		 // 增加文件名缓冲，以便多选

	if(hFileDlg.DoModal() !=IDOK)
	{
		::MessageBox(NULL, hFileDlg.GetPathName(),NULL,MB_OK ); // 显示所选文件的详细路径
		return;
	} 

    CString myfilename=hFileDlg.GetPathName();
	if (!ReadfromFile(myfilename))
	{
		MessageBox(_T("配置文件载入失败！"));
		return;
	}
	for (int i=0;i<(sizeof(Sensors_Array)/sizeof(float));i++)
	{
		Sensors_Array[i]/=100;
	}
	//各个面的值
	x_left=Sensors_Array[0];
	y_front=Sensors_Array[1];
	z_down=Sensors_Array[2];
	x_right=Sensors_Array[3*Length_Num*Width_Num*Height_Num-3];
	y_back=Sensors_Array[3*Length_Num*Width_Num*Height_Num-2];
	z_up=Sensors_Array[3*Length_Num*Width_Num*Height_Num-1];
	Model_Length=x_right-x_left;
	Model_Width=y_back-y_front;
	Model_Height=z_up-z_down;
	//操作区更新
	pLenaView->m_ymin=y_front*100;
	pLenaView->m_xmin=x_left*100;
	pLenaView->m_zmin=z_down*100;
	pLenaView->UpdateData(FALSE);
	//坏点处理
	SetBadPoints();
	pActiveView->Visual_depth*=Model_Length/2;
	pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
}

void CMainFrame::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类
	CFrameWnd::PostNcDestroy();
}

void CMainFrame::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (startwriteflag)
	{
		MessageBox(_T("请先关闭实验！"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("请先关闭历史记录！"));
		return;
	}
// 	if (MessageBox(_T("是否关闭程序?"),_T("提示"),MB_OKCANCEL|MB_ICONINFORMATION)==IDCANCEL)
// 	{
// 		return;
// 	}

	//Historyflag=false;
	CFile file;
	CFileException e;
	CString FileName = _T("lengnan.ini");
	CString FilePath;
	CString TcMax,TcMin;
	TcMax.Format(_T("%d"),mymax);
	TcMin.Format(_T("%d"),mymin);
	if(!file.Open(FileName, CFile::modeCreate | CFile::modeWrite,&e))
	{
#ifdef _DEBUG
		afxDump<<"File could not be opened"<<e.m_cause<<"\n";
#endif
	}
	FilePath=file.GetFilePath();
	file.Close();

	::WritePrivateProfileString(_T("温度"),_T("最大值"),TcMax,FilePath);
	::WritePrivateProfileString(_T("温度"),_T("最小值"),TcMin,/*TcMin.GetBuffer(MAX_PATH),MAX_PATH,*/FilePath);

	WaitForSingleObject(&ReadData,INFINITE); 
	KillTimer(1);

	typedef  short(WINAPI  Imp_CardInUse)();	//Call this function before	inspecting the return value and unloading the IMP DLL
	Imp_CardInUse *pImp_CardInUse=(Imp_CardInUse* )GetProcAddress(hDllInst,"Imp_CardInUse");
	if (hDllInst)
	{	
		if (!pImp_CardInUse())
		{
			FreeLibrary(hDllInst);
			hDllInst=NULL;
		}
	}
	CFrameWnd::OnClose();
}

//历史记录
void CMainFrame::OnHistory()
{
	if (!sensors_num)
	{
		AfxMessageBox(_T("请先载入配置文件"));
		return;
	}
	if (startwriteflag)
	{
		MessageBox(_T("实验进行中！"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("已经打开历史数据！"));
		return;
	}
	CHistoty myhistoty;
	if (myhistoty.DoModal()!=IDOK)
	{
		return;
	}
	//清除数据库连接
	mypicsavepath=myhistoty.mypictpath;
	for (int i=0;i<10;i++)
	{
		if (Pmyrecordset[i]!=NULL)
		{
			adoconn.ADO_CloseRecord(Pmyrecordset[i]);
			Pmyrecordset[i]=NULL;
		}
	}
	adoconn.ADO_ExitConnect();
	//打开历史数据库
	if (!adoconn.ADO_Connection(myhistoty.mypathfilename))
	{
		MessageBox(_T("数据库连接失败！"));
		return;
	}
	//打开数据表
	//int numofrecords=sensors_num/250;
	  int numofrecords=sensors_num/230;
         	if (sensors_num%230>0)
	{
		numofrecords+=1;
	}


	for (int i=0;i<numofrecords;i++)
	{
		CString tablename;
		//tablename.Format(_T("传感器数据%d"),i+1);
		
	     if(i == 2)
		 {
			 tablename.Format(_T("数据%d"),i+4);
		 }
		 else
		 {
			 tablename.Format(_T("数据%d"),i+1);
		 }
	     	
	   CString str=tablename;
	

	  
		Pmyrecordset[i]=adoconn.ADO_GetRecordSet2(tablename);
		if (Pmyrecordset[i]==NULL)
		{
			MessageBox(_T("数据表打开失败！"));
			for(int j=0;j<i;j++)
			{
				adoconn.ADO_CloseRecord(Pmyrecordset[j]);
			}
			adoconn.ADO_ExitConnect();
			return;
		}
	}
	
	m_PlayPos=1;
	//删除视图类定时回放功能
	pActiveView->SetTimer(5,1000,NULL);
	//设置数据读取标志
	ThreadReadDataflag=true;
    //开启数据读取线程
	ThreadReadData=AfxBeginThread(AFX_THREADPROC(ReadData),(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_HISTORY,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPHISTRY,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_ENABLED);
}

//开始记录
void CMainFrame::OnStart()
{
	// TODO: 在此添加命令处理程序代码
	if (!startwriteflag)
	{
		return;
	}
	recordflag=true;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_START,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_PAUSE,MF_BYCOMMAND|MF_ENABLED);
}


//测点设置
void CMainFrame::OnSetpoint()
{
	// TODO: 在此添加命令处理程序代码
	CSetPoint SPdlg;
	SPdlg.DoModal();
}
void CMainFrame::DockControlBarLeftOf(CToolBar *Bar,CToolBar *LeftOf)
{
	// 设 置 工 具 条 并 列 停 靠 在 同 一 条 边 上
	CRect rect; // 矩 形 区 域 定 义
	DWORD dw;
	UINT n=0;
	RecalcLayout(); // 重 新 显 示
	LeftOf ->GetWindowRect(&rect);
	rect.OffsetRect(1,0); // 设 置 偏 移 植 以 停 靠 在 同 一 边 上
	dw=LeftOf ->GetBarStyle();
	n=(dw &CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
	n=(dw &CBRS_ALIGN_BOTTOM &&n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
	n=(dw &CBRS_ALIGN_LEFT &&n==0)?AFX_IDW_DOCKBAR_LEFT:n;
	n=(dw &CBRS_ALIGN_RIGHT &&n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
	DockControlBar(Bar,n, &rect);
}
BOOL CMainFrame::DestroyWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	return CFrameWnd::DestroyWindow();
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	CTime t=CTime::GetCurrentTime();
	CString strr=t.Format(_T("%Y-%m-%d %H:%M:%S"));
	//CClientDC dc(this);
	////CWindowDC dc(this);
	//CSize sz=dc.GetTextExtent(strr);
	CSize sz=150;
	int index=m_wndStatusBar.CommandToIndex(IDS_TIMER);
	CRect rect;
	static int number=0;
	static CString str;

	switch(nIDEvent)
	{
	case 1:
		m_wndStatusBar.SetPaneInfo(index,IDS_TIMER,SBPS_NORMAL,sz.cx);
		m_wndStatusBar.SetPaneText(index,strr);
		break;
	case 2:
		DataEvent.PulseEvent();
		break;
	case 3:
		m_progress.SetPos(number); //设定进度信息
		number+=1;
		str.Format(_T("%d"),number);
		index=m_wndStatusBar.CommandToIndex(IDS_PRONUM);
		
		m_wndStatusBar.GetItemRect(index,&rect);
		if(m_progress.m_hWnd)
			m_progress.MoveWindow(rect);//将进度栏移到新位置 
		m_wndStatusBar.SetPaneText(index,str); //显示进度信息
		if(number>=99)
			number=98;
		break;
	case 4:
		number=0;
		m_progress.SetPos(number); //设定进度信息
		str.Format(_T("%d"),number);
		index=m_wndStatusBar.CommandToIndex(IDS_PRONUM);
		m_wndStatusBar.GetItemRect(index,&rect);
		if(m_progress.m_hWnd)
			m_progress.MoveWindow(rect);//将进度栏移到新位置
		m_wndStatusBar.SetPaneText(index,str); //显示进度信息
		break;
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

//坏点插值
void CMainFrame::SetBadPoints()
{
	int tempsensornum[1024]={-1};
	//缓存
	for (int q=0;q<sensors_num;q++)
	{
		DataNumbers[q]=OldDataNumbers[q];
		tempsensornum[q]=sensornumber[q];
	}
	//利用同一层的X向或Y向进行内插值
	for (int j=0;j<Height_Num;j++)
	{
		//标志坏点
		for (int i=0;i<(int)(Length_Num*Width_Num);i++)
		{
			for (int p=0;p<nPointNum;p++)
			{
				if (tempsensornum[i+j*(Length_Num*Width_Num)]==nPoints[p])
				{
					tempsensornum[i+j*(Length_Num*Width_Num)]=-1;
				}
			}
		}
		//插值
		for (int i=0;i<(int)(Length_Num*Width_Num);i++)
		{	
			if (tempsensornum[i+j*(Length_Num*Width_Num)]<0)
			{	
				//提取改点自X、Y坐标
				double xpos=Sensors_Array[(i+j*(Length_Num*Width_Num))*3];
			    double ypos=Sensors_Array[(i+j*(Length_Num*Width_Num))*3+1];
		        double mindistance=0;
				double addvalue=0;
				int addnum=0;
				for (int k=0;k<(int)(Length_Num*Width_Num);k++)
				{
					if (tempsensornum[k+j*(Length_Num*Width_Num)]>0)
					{
						double tempdistance=0;
						tempdistance+=(Sensors_Array[3*(k+j*(Length_Num*Width_Num))]-xpos)*(Sensors_Array[3*(k+j*(Length_Num*Width_Num))]-xpos);
						tempdistance+=(Sensors_Array[3*(k+j*(Length_Num*Width_Num))+1]-ypos)*(Sensors_Array[3*(k+j*(Length_Num*Width_Num))+1]-ypos);
						if (mindistance==0||mindistance>tempdistance)
						{
							mindistance=tempdistance;
							addvalue=DataNumbers[k+j*(Length_Num*Width_Num)];
							addnum=1;
						}
						else
						{
						 	if (abs((mindistance-tempdistance)/mindistance)<0.01)
							{
								addvalue+=DataNumbers[k+j*(Length_Num*Width_Num)];
								addnum++;
							}
						}
					}
				}
				//计算
				if (addnum==0)//一层都没有传感器
				{
                   DataNumbers[i+j*(Length_Num*Width_Num)]=0;
				}
				else
				{
				   DataNumbers[i+j*(Length_Num*Width_Num)]=addvalue/addnum;
				}
		    }
		}
	}
}

void CMainFrame::OnNMCustomdrawSlider(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

BOOL FolderExist(CString strPath)
{
	WIN32_FIND_DATA wfd;
	BOOL rValue = FALSE;
	HANDLE hFind = FindFirstFile(strPath, &wfd);
	if ((hFind!=INVALID_HANDLE_VALUE) &&
		(wfd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY))
	{
		rValue = TRUE;
	}
	FindClose(hFind);
	return rValue;
}
//删除文件夹目录(非空)

bool DeleteDirectory(CString sDirName) 
{ 
	CFileFind tempFind; 
	CString sTempFileFind;
	sTempFileFind=sDirName+_T("\\*.*");
	BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
	while (IsFinded) 
	{ 
		IsFinded = tempFind.FindNextFile(); 

		if (!tempFind.IsDots()) 
		{ 
			CString sFoundFileName; 
			sFoundFileName=tempFind.GetFilePath();

			if (tempFind.IsDirectory()) 
			{ 
				CString sTempDir; 
				sTempDir=/*sDirName+_T("\\")+*/sFoundFileName;
				DeleteDirectory(sTempDir); 
			} 
			else 
			{ 
				CString sTempFileName; 
				sTempFileName=/*sDirName+_T("\\")+*/sFoundFileName;
				DeleteFile(sTempFileName); 
			} 
		} 
	} 
	tempFind.Close(); 
	if(!RemoveDirectory(sDirName)) 
	{ 
		return FALSE; 
	} 
	return TRUE; 
} 

//读取数据线程
UINT ReadData(LPVOID *pPrama)
{
	int nphotoframe=0;	//pic帧数
	CDBVariant myvariant;
	CString sDriver;
	CString dbName;
	CString tempstr,datatempstr;
	CString tempFileName=_T("D:\\Temp\\");
	DeleteDirectory(tempFileName);
	if (!FolderExist(tempFileName))
	{
		CreateDirectory(tempFileName, NULL);
	}
	CMainFrame* pMainFram=(CMainFrame *)pPrama;
	CToolBar* pToolBar = (CToolBar*)AfxGetMainWnd()->GetDescendantWindow(IDR_EXTEND_BAR);//工具栏指针
	//数据读取点位置
	long nCounttotal=Pmyrecordset[0]->GetRecordCount();
	long nCount=0;
	//移动到文件头
	for (int i=0;Pmyrecordset[i]!=NULL;i++)
	{
		Pmyrecordset[i]->MoveFirst();
	}
	//数据读取过程
	while(ThreadReadDataflag)
	{
//根据滑动条位置定位
		if (!ThreadReadDataflag0)//手动移动滑块
		{
			if (m_playmoveflag)
				{
					nCount=(1.0*nCounttotal*m_PlayPos)/order_number_total;
					for (int i=0;Pmyrecordset[i]!=NULL;i++)
					{
						Pmyrecordset[i]->MoveFirst();
						Pmyrecordset[i]->Move(nCount);
					}
					m_playmoveflag=false;
				}
		}
		else
		{
			//自动播放延时
			Sleep(500);
			//调整下一次读取位置
			nCount+=20;
			if (nCount>nCounttotal)//已经到头
			{
				nCount=nCounttotal;
				//结束自动播放和AVI记录
				ThreadReadDataflag0=false;
				bAviFlag=false;
				//更新状态栏
				pMainFram->pActiveView->SendMessage(WM_MY_MESSAGE,5,0);
			}
			m_PlayPos=nCount*order_number_total/(nCounttotal);
			//定位滑块
			pMyToolBar->Setpos();
			//数据库后移
			for (int i=0;Pmyrecordset[i]!=NULL;i++)
			{
				Pmyrecordset[i]->Move(20);
			}
		}	
/////////读取数据
		//数据超范围约束
		if (Pmyrecordset[0]->adoEOF)
		{
			for (int i=0;Pmyrecordset[i]!=NULL;i++)
			{
				Pmyrecordset[i]->MoveLast();
			}
		}
		//读取数据
		int tablenum=0;
		for (;Pmyrecordset[tablenum+1]!=NULL;tablenum++)
		{
			if (tablenum==0)
			{
				//数据采集的时间点
				timestring=Pmyrecordset[0]->GetCollect(_T("时间"));
			}
			for(int i=0;i<250;i++)
			{
				tempstr.Format(_T("采集点%d"),tablenum*250+i+1);
				datatempstr=Pmyrecordset[tablenum]->GetCollect((_variant_t)tempstr);
				OldDataNumbers[tablenum*250+i]=_wtof(datatempstr);
			}
		}
		for (int i=tablenum*250;i<sensors_num;i++)
		{
			tempstr.Format(_T("采集点%d"),i+1);
			datatempstr=Pmyrecordset[tablenum]->GetCollect((_variant_t)tempstr);
			OldDataNumbers[i]=_wtof(datatempstr);
		}
		//准备数据
		pMainFram->SetBadPoints();
		//显示
		pMainFram->pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
		//自动播放时记录视频数据
		if (bAviFlag)
		{
			//将画面保存到视频
			pMainFram->SaveBMPToAVI();
		}
		else
		{
		   	pMainFram->CloseAvi();
		}
	}
	//关闭程序集
	for (int i=0;Pmyrecordset[i]!=NULL;i++)
	{
		adoconn.ADO_CloseRecord(Pmyrecordset[i]);
		Pmyrecordset[i]=NULL;
	}
	adoconn.ADO_ExitConnect();
	return 1;
}

void CMainFrame::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类
	DDX_Control(pDX, IDC_SLIDERCTRL, m_SliderCtrl_x);
	DDX_Slider(pDX, IDC_SLIDERCTRL, m_nSlider_x);
	DDV_MinMaxInt(pDX, m_nSlider_x, 1, order_number_total);
	CFrameWnd::DoDataExchange(pDX);
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnGoon()
{
	// TODO: 在此添加命令处理程序代码
	if (!ThreadReadDataflag)
	{
		return;
		
	}
	ThreadReadDataflag0=true;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_ENABLED);
}

void CMainFrame::OnSuspend()
{
	// TODO: 在此添加命令处理程序代码
	ThreadReadDataflag0=false;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	g_eventKill.SetEvent();
	// TODO: 在此处添加消息处理程序代码
}


afx_msg LRESULT CMainFrame::OnProgress(WPARAM wParam,LPARAM lParam)
{
	CRect rect;
	int index=m_wndStatusBar.CommandToIndex(IDS_PROGRESS);
	m_wndStatusBar.GetItemRect(index,&rect);
	m_progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,rect,&m_wndStatusBar,150);//创建进度栏
	if(m_progress.m_hWnd)
		m_progress.MoveWindow(rect);//将进度栏移到新位置 
	return 1;
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CFrameWnd::OnPaint()
	CRect rect;
	int index=m_wndStatusBar.CommandToIndex(IDS_PROGRESS);
	m_wndStatusBar.GetItemRect(index,&rect);
	if(m_progress.m_hWnd)
		m_progress.MoveWindow(rect);//将进度栏移到新位置 
}
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnClearlast()
{
	// TODO: 在此添加命令处理程序代码
	well_num--;
	if (well_num<0)
	{
		well_num=0;
	}
	pActiveView->SendMessage(WM_PAINT);
}

void CMainFrame::OnPause()
{
	// TODO: 在此添加命令处理程序代码
	recordflag=false;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_START,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_PAUSE,MF_BYCOMMAND|MF_GRAYED);
}

UINT ReadCom(LPVOID *pPrama)
{
	CMainFrame* pMainFram=(CMainFrame *)pPrama;
	double tempdatavalue[1000]={0}; 
	unsigned char writebuffer_3038[20]={0};
	unsigned char readbuffer_3038[40]={0};
	LONGLONG lasttime=0;
	LONGLONG curtime=0;
	while(startwriteflag)
	{
        ////////实验数据采集
		if (Tempflag)
		{
		}
		////获得3038热电偶采集数据
		
	    /////获取仪表采集实验数据
		//实验数据调整，将采集数据调整为按配置表顺序排列的数据
		for (int iii=0;iii<sensors_num;iii++)
		{
			OldDataNumbers[iii]=tempdatavalue[sensornumber[iii]-1];
		}
		//数据记录
		CTime time=CTime::GetCurrentTime();
		CString strtime=time.Format("%Y-%m-%d %H:%M:%S");
		if (recordflag)
		{
			curtime=::GetTickCount();
			if (curtime-lasttime>sjbc_socond*1000)
			{
				adoconn.ADO_Insertdata(Pmyrecordset,strtime,OldDataNumbers,sensors_num);
				lasttime=curtime;
			}
		}
		//坏点处理
		pMainFram->SetBadPoints();
		pMainFram->pActiveView->PostMessageW(WM_MY_MESSAGE,2,NULL);
	}
	for (int iii=0;iii<sensors_num;iii++)
	{
		OldDataNumbers[iii]=tempdatavalue[sensornumber[iii]-1];
	}
	//清除数据库资源
	for (int i=0;i<10;i++)
	{
		if (Pmyrecordset[i]!=NULL)
		{
			adoconn.ADO_CloseRecord(Pmyrecordset[i]);
		}
	}
	adoconn.ADO_ExitConnect();
	return 1;
}
BOOL CMainFrame::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
{
	ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

	// UNICODE消息
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFrom为工具条的HWND 
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) //不为分隔符
	{
		strTipText.LoadString(nID);
		strTipText = strTipText.Mid(strTipText.Find('\n',0)+1);

#ifndef _UNICODE
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
		}
		else
		{
			_mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#else
		if (pNMHDR->code == TTN_NEEDTEXTA)
		{
			_wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
		}
		else
		{
			lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
		}
#endif

		*pResult = 0;

		// 使工具条提示窗口在最上面
		::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
			SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); 
		return TRUE;
	}
	return TRUE;
}

int CMainFrame::IMPInit(void)
{
	int IMPCardNum=0;
	short Poll_Tab[MAXPOLLTAB];
	short ErrorFlag = 0;
	short CardHandle = -1;
	unsigned short CardAddress = 0xc800;

	//Load IMP driver
	hDllInst=LoadLibrary(_T("IMPDRVR.DLL"));
	if (!hDllInst)
	{
		AfxMessageBox(_T("DLL加载失败"));
		return -1;
	}
	
	//声明IMP库函数
	typedef  void(WINAPI  Imp_Connect)(unsigned short *CardAddress,short *CardHandle);
	Imp_Connect *pImp_Connect=(Imp_Connect* )GetProcAddress(hDllInst,"Imp_Connect");

	typedef  void(WINAPI  Imp_Init)(short *Poll_Tab,short *ErrorFlag,short *CardHandle);
	Imp_Init *pImp_Init=(Imp_Init* )GetProcAddress(hDllInst,"Imp_Init");

	typedef  void(WINAPI  Imp_Disconnect)(short *CardHandle);
	Imp_Disconnect *pImp_Disconnect=(Imp_Disconnect* )GetProcAddress(hDllInst,"Imp_Disconnect");

	//typedef  short(WINAPI  Imp_CardInUse)();	//Call this function before	inspecting the return value and unloading the IMP DLL
	//Imp_CardInUse *pImp_CardInUse=(Imp_CardInUse* )GetProcAddress(hDllInst,"Imp_CardInUse");

	do 
	{	
		//Connect IMP
		pImp_Connect(&CardAddress,&CardHandle);
		if (CardHandle>=0)	//链接成功
		{	
			//Initialize the IMP Interface card
			pImp_Init(&Poll_Tab[0],&ErrorFlag,&CardHandle);

			//DisConnect IMP
			//pImp_Disconnect(&CardHandle);

			CardAddress	+= 0x0100;
			IMPCardNum++;
		}
	} while (CardHandle>=0);
	
	return IMPCardNum;
}

//int CMainFrame::LoadIMPDrv(HWND hWnd) /** Load IMP Driver **/
//{
//	char MsgText[80]; /** Error Message Box Text **/
//	char MsgTitle[80]; /** Error Message Box Title **/
//	int LoadStat = 0; /** DLL Load Status 1=OK **/
//	int MsgStat = 0; /** Error Message Box Status **/
//	HMODULE hImpDLL = LoadLibrary(_T("IMPDRVR.DLL"));
//	//定义ＩＭＰ卡的库函数
//	void (FAR PASCAL *Imp_init) (short* far,short* far,short* far); /** Address of Imp_Init **/
//	void (FAR PASCAL *Imp_tx) (short *far,char *far,short *far,short *far,short *far);
//	/** Address of Imp_Tx **/
//	void (FAR PASCAL *Imp_test) (short *far,short *far,short *far,short *far);
//	/** Address of Imp_Test **/
//	void (FAR PASCAL *Imp_string) (short *far,short *far,char *far,short *far,short *far,short *far);
//	/** Imp_String **/
//	void (FAR PASCAL *Imp_numeric) (short *far,short *far,short *far,short *far,float *far,short *far,short *far); /** Imp_Numeric **/
//	void (FAR PASCAL *Imp_valToByte) (float *far,short *far); /** Address of Imp_ValToByte **/
//	void (FAR PASCAL *Imp_connect) (unsigned short *far,short *far);/** Address of Imp_Connect **/
//	void (FAR PASCAL *Imp_disconnect) (short *far); /** Address of Imp_Disconnect **/
//	short (FAR PASCAL *Imp_cardInUse) (); /** Address of Imp_CardInUse **/
//	void (FAR PASCAL *Imp_dLLVersion) (char *far); /** Address of Imp_DLLVersion **/
//	void (FAR PASCAL *Imp_dLLDelay) (unsigned short); /** Address of Imp_DLLDelay **/
//	void (FAR PASCAL *Imp_powerOff) (short *far); /** Address of Imp_PowerOff **/
//	if (hImpDLL != NULL) /** Loaded OK ? **/
//	{
//		(FARPROC) Imp_Init = GetProcAddress(hImpDLL,"Imp_Init");
//		if (Imp_Init != NULL) /** Address Valid ? **/
//		{
//			(FARPROC) Imp_Tx = GetProcAddress(hImpDLL,"Imp_Tx");
//			if (Imp_Tx != NULL) /** Address Valid ? **/
//			{
//				(FARPROC) Imp_Test = GetProcAddress(hImpDLL,"Imp_Test");
//				if (Imp_Test != NULL) /** Address Valid ? **/
//				{
//					(FARPROC) Imp_String = GetProcAddress(hImpDLL,"Imp_String");
//					if (Imp_String != NULL) /** Address Valid ? **/
//					{
//						(FARPROC) Imp_Numeric = GetProcAddress(hImpDLL,"Imp_Numeric");
//						if (Imp_Numeric != NULL) /** Address Valid ? **/
//						{
//							(FARPROC) Imp_ValToByte =GetProcAddress(hImpDLL,"Imp_ValToByte");
//							if (Imp_ValToByte != NULL) /** Address Valid ? **/
//							{
//								(FARPROC) Imp_Connect = GetProcAddress(hImpDLL,"Imp_Connect");
//								if (Imp_Connect != NULL) /** Address Valid ? **/
//								{
//									(FARPROC) Imp_Disconnect =GetProcAddress(hImpDLL,"Imp_Disconnect");
//									if (Imp_Disconnect != NULL) /** Address Valid ? **/
//									{
//										(FARPROC) Imp_CardInUse =GetProcAddress(hImpDLL,"Imp_CardInUse");
//										if (Imp_CardInUse != NULL) /** Address Valid ? **/
//										{
//											(FARPROC) Imp_DLLVersion =GetProcAddress(hImpDLL,"Imp_DLLVersion");
//											if (Imp_DLLVersion != NULL) /** Address Valid ? **/
//											{
//												(FARPROC) Imp_DLLDelay =GetProcAddress(hImpDLL,"Imp_DLLDelay");
//												if (Imp_DLLDelay != NULL) /** Address Valid ? **/
//												{
//													(FARPROC) Imp_PowerOff =GetProcAddress(hImpDLL,"Imp_PowerOff");
//													if (Imp_PowerOff != NULL) /** Address Valid ? **/
//													{
//														LoadStat = 1; /** DLL Loaded OK **/
//													}
//												}
//											}
//										}
//									}
//								}
//							}
//						}
//					}
//				}
//			}
//		}
//		if (!LoadStat) /** IMP DLL Initialise Error **/
//		{
//			wsprintf(MsgText, "Could Not Initialise IMPDRVR.DLL");
//			wsprintf(MsgTitle, "Test IMP DLL ERROR");
//			MsgStat = MessageBox(hWnd,(LPSTR)MsgText,(LPSTR)MsgTitle,MB_OK |
//				MB_ICONEXCLAMATION);
//		}
//		else /** IMP DLL Load Error **/
//		{
//			wsprintf(MsgText, "Could Not Load IMPDRVR.DLL");
//			wsprintf(MsgTitle, "Test IMP DLL ERROR");
//			MsgStat = MessageBox(hWnd,(LPSTR)MsgText,(LPSTR)MsgTitle,MB_OK |MB_ICONEXCLAMATION);
//		}
//	}
//	return (LoadStat); /** Return Load Status **/
//}

/******************** LOCATECARD ******************************************/
int  CMainFrame::LocateCard() /** Locate IMP Adaptor Card **/
{
	char LpExit = 0; /** Loop Exit Flag **/
	int  AdapFound = 0; /** Adaptor Card Found 0=No 1=Yes **/
	short  InitStat = 0; /** Initialise Status **/
	short CardHandle = -1;
	short Poll_Tab[MAXPOLLTAB];

	//声明IMP库函数
	typedef  void(WINAPI  Imp_Connect)(unsigned short *CardAddress,short *CardHandle);
	Imp_Connect *pImp_Connect=(Imp_Connect* )GetProcAddress(hDllInst,"Imp_Connect");
	typedef  void(WINAPI  Imp_Init)(short *Poll_Tab,short *ErrorFlag,short *CardHandle);
	Imp_Init *pImp_Init=(Imp_Init* )GetProcAddress(hDllInst,"Imp_Init");
	typedef  void(WINAPI  Imp_Disconnect)(short *CardHandle);
	Imp_Disconnect *pImp_Disconnect=(Imp_Disconnect* )GetProcAddress(hDllInst,"Imp_Disconnect");

	unsigned short CardAddr = 0xC800;	     /** Set Base Address **/
	while (!LpExit) /** Wait for Loop Exit **/
	{
		pImp_Connect(&CardAddr,&CardHandle); /** Connect to Adaptor Memory **/
		if (CardHandle >= 0)      /** Valid Card Handle ? **/
		{
			pImp_Init(&Poll_Tab[0],&InitStat,&CardHandle); /** Initialise IMP Adaptor **/
			if (!InitStat)      /** Initialise OK ? **/
			{
				LpExit = 1;      /** Set Loop Exit Flag **/
				AdapFound = 1;      /** Adaptor Card Found **/
			}
			else /** Initialise Error **/
			{
				pImp_Disconnect(&CardHandle); /** Disconnect from Adaptor Memory **/
				CardAddr += 0x100;      /** Increment Card Address **/
				if (CardAddr > 0xDFE0)  /** Past Last Address ? **/
					LpExit = 2;      /** Set Exit Flag **/
			}
		}
		else      /** Invalid Card Handle **/
			LpExit = 3; /** Set Exit Flag **/
	}
	return (AdapFound); /** Return Status **/
}
/******************** END LOCATECARD **************************************/ 

void CMainFrame::OnSetmodel()
{
	// TODO: 在此添加命令处理程序代码
	if (sensors_num>0)
	{
		MessageBox(_T("模型已经建立！"));
		return;
	}
	CModelSheet mymodelsheet(_T(""));
	mymodelsheet.SetWizardMode();
	mymodelsheet.DoModal();
	if (mymodelsheet.mysensornum.finishflag)
	{
		CString filepathname=mymodelsheet.mymodelset.pathname+mymodelsheet.mymodelset.myfilename;
		filepathname+=_T(".txt");
		WritetoFile(filepathname);
	}
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message==MSG_CLIENT)
	{
		ReceiveData();
		return TRUE;
	}else
	return CFrameWnd::PreTranslateMessage(pMsg);
}


void CMainFrame::ReceiveData(void)
{
	char buf[1024*1024];
	int num=recv(m_Client,buf,1024*1024,0);
	buf[num]=0;//接受完毕

	//解析

	
}


void CMainFrame::SaveData(void)
{

}

void CMainFrame::OnShowLine()
{
	
	
}

void CMainFrame::OnSetstaff()
{
	
	CSetStaff dlg;
	SetStaff=FALSE;
	if (dlg.DoModal()==IDOK)
	{
		mymax=dlg.m_value4;
		mymin=dlg.m_value1;
		temvalue1=dlg.m_value1;
		temvalue2=dlg.m_value2;
		temvalue3=dlg.m_value3;
		temvalue4=dlg.m_value55;
		temvalue5=dlg.m_value4;
		
	}
	pActiveView->SendMessage(WM_PAINT);
	// TODO: 在此添加命令处理程序代码
}


void CMainFrame::OnSetstaff1()
{
	// TODO: 在此添加命令处理程序代码
	SetStaff=TRUE;
	pActiveView->SendMessage(WM_PAINT);
}

bool CMainFrame::ReadfromFile(CString& filename)
{
	CStdioFile myfile;
	if(!myfile.Open(filename,CFile::typeText|CFile::modeRead))
		return false;
	CString tempstring;
	int index=0;
	while(myfile.ReadString(tempstring))
	{
		//开头
		if (tempstring==_T("[IMP_CARD]"))
		{
			continue;
		}
		//结尾
		if (tempstring==_T("[END]"))
		{
			continue;
		}
		int boundary=tempstring.Find('=');
		if (boundary!=-1)
		{
			CString name=tempstring.Left(boundary);
			CString value=tempstring.Right(tempstring.GetLength()-boundary-1);
			char pvalue[100]={0};
			//长度方向传感器数量
			if (name==_T("numX"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Length_Num=atof(pvalue);
			}
			//宽度方向传感器数量
			if (name==_T("numY"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Width_Num=atof(pvalue);
			}
			//高度方向层数
			if (name==_T("numZ"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Height_Num=atof(pvalue);
			}
			//采集卡数量
			if (name==_T("cardnum"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				cjbNum=atof(pvalue);
			}
			//传感器总量
			if (name==_T("Totalnum"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				sensors_num=atof(pvalue);
			}
		}
		else//采集点位置与传感器序号
		{
			CString leftstr;
			int boundary=tempstring.Find(',');
	        leftstr=tempstring.Left(boundary);
			int tempnum=_wtoi(leftstr);
			sensornumber[index]=tempnum;
			tempstring.Delete(0,leftstr.GetLength()+1);
			boundary=tempstring.Find(',');
            leftstr=tempstring.Left(boundary);
            OldDataNumbers[index]=_wtof(leftstr);
			tempstring.Delete(0,leftstr.GetLength()+1);
			boundary=tempstring.Find(',');
			leftstr=tempstring.Left(boundary);
            Sensors_Array[3*index]=_wtof(leftstr);
			tempstring.Delete(0,leftstr.GetLength()+1);
			boundary=tempstring.Find(',');
			leftstr=tempstring.Left(boundary);
			Sensors_Array[3*index+1]=_wtof(leftstr);
			tempstring.Delete(0,leftstr.GetLength()+1);
			Sensors_Array[3*index+2]=_wtof(tempstring);
			index++;
		}
	}
	myfile.Close();
	if (sensors_num==0)
	{
		return false;
	}
	return true;
}
bool CMainFrame::WritetoFile(CString& filename)
{
	CStdioFile myfile;
	if(!myfile.Open(filename,CFile::typeText|CFile::modeWrite|CFile::modeCreate))
		return false;
	CString tempstring;
	//文件头
	tempstring=_T("[IMP_CARD]\n");
	myfile.WriteString(tempstring);	
	//X向个数
	tempstring.Format(_T("numX=%d\n"),Xnum);
	myfile.WriteString(tempstring);
	//y向个数
	tempstring.Format(_T("numY=%d\n"),Ynum);
	myfile.WriteString(tempstring);	
	//z层数
	tempstring.Format(_T("numZ=%d\n"),Znum);
	myfile.WriteString(tempstring);
	//板卡数
	tempstring.Format(_T("cardnum=%d\n"),cardnum);
	myfile.WriteString(tempstring);
	//采集点总数
	tempstring.Format(_T("Totalnum=%d\n"),totalnum);
	myfile.WriteString(tempstring);
	//坐标点
	for (int z=0;z<Znum;z++)
	{
		for (int y=0;y<Ynum;y++)
		{
			for (int x=0;x<Xnum;x++)
			{
				double xspace=0;
				for (int xx=0;xx<=x;xx++)
				{
                      xspace+=Xdistances[xx];
				}
				double yspace=0;
				for (int yy=0;yy<=y;yy++)
				{
					yspace+=Ydistances[yy];
				}
				double zspace=0;
				for (int zz=0;zz<=z;zz++)
				{
					zspace+=Zdistances[zz];
				}
				tempstring.Format(_T("%d,0,%.3f,%.3f,%.3f\n"),sensor_pos[x][y][z],xspace,yspace,zspace);
				myfile.WriteString(tempstring);
			}
		}
	}
	//文件尾
	tempstring=_T("[END]");
	myfile.WriteString(tempstring);
	myfile.Close();
	return true;
}


void CMainFrame::OnMsarttest()
{
	// TODO: 在此添加命令处理程序代码
	if (!sensors_num)
	{
		AfxMessageBox(_T("请先载入配置文件"));
		return;
	}
	if (startwriteflag)
	{
		MessageBox(_T("试验已经开始！"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("正在进行历史记录回放！"));
		return;
	}
	if (sensors_num<=0)
	{
		MessageBox(_T("模型未建立，请先建立模型！"));
		return;
	}
	//关闭串口
	if (m_com_TConctral!=NULL)
	{
		m_com_TConctral->CloseCom();
		delete m_com_TConctral;
		m_com_TConctral=NULL;
	}
	if (m_com_Tsensor!=NULL)
	{
		m_com_Tsensor->CloseCom();
		delete m_com_Tsensor;
		m_com_Tsensor=NULL;
	}
	if (m_com_Pre!=NULL)
	{
		m_com_Pre->CloseCom();
		delete m_com_Pre;
		m_com_Pre=NULL;
	}
	//数据库已经建立
	for (int i=0;i<10;i++)
	{
		if (Pmyrecordset[i]!=NULL)
		{
			adoconn.ADO_CloseRecord(Pmyrecordset[i]);
			Pmyrecordset[i]=NULL;
		}
	}
	adoconn.ADO_ExitConnect();
	Cnewtest mynew;
	if (mynew.DoModal()!=IDOK)
		return;	
	//建立数据库文件	
	CTime time=CTime::GetCurrentTime();
	CString dbname=mynew.mydatapath+mynew.mytestname;
	dbname+=_T(".accdb");
	//建立数据库文件
	if (!adoconn.CreateAccess(dbname))
	{
		MessageBox(_T("数据库文件建立失败！"));
		return;
	}
	//连接数据库
	if (!adoconn.ADO_Connection(dbname))
	{
		MessageBox(_T("数据库连接失败！"));
		return;
	}
	if (mynew.mytestname==_T("温度场"))
	{
		Tempflag=true;
		m_com_TConctral=new CCom(mynew.m_ConCtral_com,9600,'N',8,1);
		if (!m_com_TConctral->InitCom())
		{
			MessageBox(_T("温度控制器串口打开失败！"));
			delete m_com_TConctral;
			m_com_TConctral=NULL;
			return;
		}
		m_com_Tsensor=new CCom(mynew.m_TSensorCom,9600,'N',8,1);
		if (!m_com_Tsensor->InitCom())
		{
			MessageBox(_T("温度传感器串口打开失败！"));
			delete m_com_Tsensor;
			m_com_TConctral->CloseCom();
			delete m_com_TConctral;
			m_com_TConctral=NULL;
			return;
		}
	}
	else
	{
		Tempflag=false;
		m_com_Pre=new CCom(mynew.m_pcom,9600,'N',8,1);
		if (!m_com_Pre->InitCom())
		{
			MessageBox(_T("压力串口打开失败！"));
			delete m_com_Pre;
			m_com_Pre=NULL;
			return;
		}
	}
	//建立数据表
	int numofrecords=sensors_num/250;
	if (sensors_num%250>0)
	{
		numofrecords+=1;
	}
	for (int i=0;i<numofrecords;i++)
	{
		CString tablename;
		tablename.Format(_T("传感器数据%d"),i+1);
		CString prama;
		if (i==0)
		{
			prama=_T("(序号 REAL PRIMARY KEY,时间 TEXT,");
		}
		else
		{
			prama=_T("(序号 REAL PRIMARY KEY,");
		}
		for (int j=0;j<250;j++)
		{
			CString tempstr;
			tempstr.Format(_T("采集点%d REAL,"),i*250+j+1);
			prama+=tempstr;
		}
		prama.TrimRight(',');
		prama+=_T(")");
		Pmyrecordset[i]=adoconn.ADO_Createtable(tablename,prama);
		if (Pmyrecordset[i]==NULL)
		{
			MessageBox(_T("数据表建立失败！"));
			return;
		}
	}
	//图片视频保存路径
	mypicsavepath=mynew.mypictpath;
	//采集时间间隔
	sjbc_socond=(int)mynew.myinetnaltime;
	//删除视图类定时回放功能
	pActiveView->KillTimer(5);
	//开始试验
	startwriteflag=true;
	ThreadReadCom=AfxBeginThread(AFX_THREADPROC(ReadCom),(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSARTTEST,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSTOPTEST,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_SETSTAFF,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_SETSTAFF1,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_START,MF_BYCOMMAND|MF_ENABLED);
}


void CMainFrame::OnMstoptest()
{
	// TODO: 在此添加命令处理程序代码
	if (recordflag)
	{
		MessageBox(_T("请先关闭记录！"));
		return;
	}
	startwriteflag=false;
	//开启视图类定时回放功能
	pActiveView->SetTimer(5,1000,NULL);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSARTTEST,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSTOPTEST,MF_BYCOMMAND|MF_GRAYED);
}


void CMainFrame::OnMstartavi()
{
	// TODO: 在此添加命令处理程序代码
	if (!ThreadReadDataflag)
	{
		return;
	}
	CAVIPrama mypaema;
	if (mypaema.DoModal()==IDOK)
	{
		//初始化视频参数
		Xwidth=mypaema.mywidth;
		Yheigth=mypaema.myheiht;
		FPS=mypaema.myfps;
		Avipathname=mypaema.myfilepath+mypaema.myname+_T(".avi");
		if (!InitAVI())
		{
			MessageBox(_T("视频初始化失败！"));
			return;
		}
		ThreadReadDataflag0=true;
		bAviFlag=true;
		GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_GRAYED);
		GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_GRAYED);	
		GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
		GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_ENABLED);
	}
}


void CMainFrame::OnMstopavi()
{
	// TODO: 在此添加命令处理程序代码
	if (!ThreadReadDataflag)
	{
		return;
	}
	ThreadReadDataflag0=false;
	bAviFlag=false;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_ENABLED);
}


void CMainFrame::OnMstophistry()
{
	// TODO: 在此添加命令处理程序代码
	if (ThreadReadDataflag0)
	{
		MessageBox(_T("请先关闭自动播放！"));
		return;
	}
	if (bAviFlag)
	{
		MessageBox(_T("请先关闭视频！"));
		return;
	}
	//开启视图类定时回放功能
	pActiveView->KillTimer(5);
    ThreadReadDataflag=false;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_HISTORY,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPHISTRY,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_GRAYED);


}
//OPenCV视频AVI文件保存
bool CMainFrame::SaveBMPToAVI()
{
	IplImage *pimage=cvLoadImage("C:\\temppict.bmp");
	//视频尺寸变换
	CvSize mysize;
	mysize.width=Xwidth;
	mysize.height=Yheigth;
	IplImage *tempimage=cvCreateImage(mysize,IPL_DEPTH_8U,3);
	cvResize(pimage,tempimage,CV_INTER_LINEAR);
	if (pimage==NULL)
	{
		return false;
	}
	cvWriteFrame(Pvideowrite,tempimage);
	cvReleaseImage(&pimage);
	cvReleaseImage(&tempimage);
	return true;
}
bool CMainFrame::InitAVI()
{
	if (Pvideowrite!=NULL)
	{
		return false;
	}
	char tempchars[1024]={0};
	//获取长度
	int wLen = WideCharToMultiByte(CP_ACP,0,Avipathname,-1,NULL,0,NULL,NULL);
	//将CString转换成char*  
	WideCharToMultiByte(CP_ACP, 0,Avipathname, -1,tempchars, wLen, NULL, NULL);    
	Pvideowrite=cvCreateVideoWriter(tempchars,-1,FPS,cvSize(Xwidth,Yheigth));
	if (Pvideowrite==NULL)
	{
		return false;
	}
	return true;
}
bool CMainFrame::CloseAvi()
{
	if (Pvideowrite==NULL)
	{
		return false;
	}
	cvReleaseVideoWriter(&Pvideowrite);
	Pvideowrite=NULL;
	return true;
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CFrameWnd::OnSysCommand(nID, lParam);
}


void CMainFrame::OnMsvpicttest()
{
	// TODO: 在此添加命令处理程序代码
	bpicture=true;
}


void CMainFrame::OnMsvpicthistory()
{
	// TODO: 在此添加命令处理程序代码
	bpicture=true;
}

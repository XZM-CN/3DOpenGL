// MainFrm.cpp : CMainFrame ���ʵ��
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

//��ǰʵ������˶�����
int nowtime[10000];
double DatatNum[1000][10000];


CADOConn adoconn;
//��ȡ�����߳�
CWinThread *ThreadBToA=NULL;
//�߳�ͬ�������¼�
CEvent DataEvent3;
CEvent DataSend;

int sjbc_socond=5;
//�ɼ�������
int IMP_Num=0;
int cjbNum;
HINSTANCE hDllInst=NULL;		//����ǿdllʵ�����
long data_index=0;

//ģ����Ϣ��ʾ
BOOL m_bModleInfo=FALSE;
//�ѿ�ʼ��¼��־λ
bool startwriteflag=false;//ʵ�鿪ʼ����λ
bool recordflag=false;//��ʼ��¼����λ
//ѹ���¶���ʾѡ���׼��falseѹ����ʾ��true�¶���ʾ
bool Tempflag=false;
//��������
extern int well_num;
extern CMyToolBar* pMyToolBar;
bool bSliderflag=false;
bool threadstarrtflag=false;
extern GLint viewport[4];
CEvent g_eventKill;
bool threadoverflag=false;
bool opengliniflag=false;	//opengl��Ҫ��ʼ����־λ
BOOL FolderExist(CString strPath);
bool DeleteDirectory(char* sDirName);
extern CLena* pLena;//���ָ��
extern void ImageToAVI( LPCTSTR pAviName, LPCTSTR pDir, int videoWidth, int videoHeight, int bpp );
int nAviFrameRate=20;
//��ȡ��ʷ���ݱ�־
bool bAviFlag=false;//����AVI�ļ���־
bool ThreadReadDataflag=false; //��ȡ��ʷ��¼��־
bool ThreadReadDataflag0=false; //�Զ����ű�־
bool bSuspendThreadflag=false;	//�̼߳��������־
bool m_playmoveflag=false; //���Ż��������ƶ���־��
bool bpicture=false;//����ͼƬ��־
int m_PlayPos=1;
//����ͨѶ�߳�
CWinThread *ThreadReadCom;
//��ȡ�����̺߳���
static UINT ReadCom(LPVOID *pPrama);
//��ȡ�����߳�
CWinThread *ThreadReadData;
//��ȡ�����̺߳���
static UINT ReadData(LPVOID *pPrama);

//�߳�ͬ�������¼�
CEvent DataEvent;
CEvent DataEvent2;
//�ط�ģʽ��־λ
bool Historyflag(false);
//���ݲɼ���ʱ���
CString timestring;

CString mypicsavepath;//��ͼ��Ƶ����·��

double OldDataNumbers[1000];//ģ������
extern int nPointNum;				//������Ŀ
extern int nPoints[100];			//�����������

//ģ�Ͳ���
float Model_Length;
float Model_Width;
float Model_Height;
UINT Length_Num,Width_Num,Height_Num;
float DataNumbers[1000];
float Sensors_Array[3000];
//����������
int sensors_num;
//�������ֵ
float z_down,z_up;//�ϡ���
float y_front,y_back;//ǰ����
float x_left,x_right;//����
/////////////���ڼ�¼�����ݱ�
_RecordsetPtr Pmyrecordset[10]={NULL};

//���ݵ�����
int order_number_total=1000;

// �¶���Сֵ
extern int mymin;
// �¶����ֵ
extern int mymax;

extern Vector3D Pt3d;
extern bool show_coordinateflag;
//����ͨ����
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
	ID_SEPARATOR,           // ״̬��ָʾ��
	IDS_PROGRESS,			// ������
	IDS_PRONUM,
	IDS_COORDINATE,
	IDS_TIMER,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// CMainFrame ����/����

CMainFrame::CMainFrame()
{
	// TODO: �ڴ���ӳ�Ա��ʼ������
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
	//������չ���߿�
	if (!m_wndExtendBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndExtendBar.LoadToolBar(IDR_EXTEND_BAR))
	{
		TRACE(_T("δ�ܴ���������\n"));
		return -1;
	}
	m_wndExtendBar.SetWindowText(_T("��չ������"));
	CRect rect;
	//������Ͽ���
	GetWindowRect(&rect);
	//������Ļ���
	int   nWidth   =   GetSystemMetrics(SM_CXSCREEN);
	m_wndExtendBar.SetButtonInfo(1,IDC_SRICROLLPLAY,TBBS_SEPARATOR,nWidth-40);
	//�õ���Ͽ�λ��
	m_wndExtendBar.GetItemRect(1,&rect);
	rect.top-=6;
	rect.bottom-=6;
	//����������
	if (!m_wndExtendBar.m_MyScroll.Create(TBS_HORZ|WS_VISIBLE|TBS_TOP|TBS_BOTTOM,rect,&m_wndExtendBar,IDC_SRICROLLPLAY))
		return -1;
	//order_number_total=1000;
	m_wndExtendBar.m_MyScroll.SetScrollRange(0,order_number_total,TRUE);//���ÿؼ���С���ֵ
	m_wndExtendBar.m_MyScroll.SetScrollPos(1,TRUE);//���ó�ʼλ��
	m_wndExtendBar.m_MyScroll.ShowScrollBar(TRUE);


	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("δ�ܴ���״̬��\n");
		return -1;      // δ�ܴ���
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

	////������ͣ��
	////����CToolBar�еĳ�Ա�������ù�������ͣ����λ�� 
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	////���ÿ�����еĳ�Ա��������������Ա�ͣ��  
	//EnableDocking(CBRS_ALIGN_ANY);  
	//m_wndToolBar.GetWindowRect(&rect);
	////ָ��ͣ���Ĺ�����.  
	//DockControlBar(&m_wndToolBar,AFX_IDW_DOCKBAR_TOP);

	//ͣ���¹�����
	m_wndExtendBar.EnableDocking(CBRS_ALIGN_ANY);

	//DockControlBarLeftOf(&m_wndExtendBar,&m_wndToolBar);

	//CSplashWnd::ShowSplashScreen(this);
	//�趨��ʱ��
	SetTimer(1, 1000, NULL); 
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	//cs.style &= ~WS_MAXIMIZEBOX; //��ֹ�������
	//cs.style &= ~WS_MINIMIZEBOX; //��ֹ������С��

	return TRUE;
}


// CMainFrame ���

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


// CMainFrame ��Ϣ�������




BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	 //TODO: �ڴ����ר�ô����/����û���
	CRect rect;
	GetWindowRect(&rect);
	int   nWidth   =   GetSystemMetrics(SM_CXSCREEN);    
	int   nHeight=   GetSystemMetrics(SM_CYSCREEN);

	//��������һ��Ϊ��
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
		MessageBox(_T("ģ���Ѿ�������"));
		return;
    }
	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CString filepathname;
	CFileDialog hFileDlg(FALSE,NULL,NULL,OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST|OFN_READONLY,TEXT("TXT(*.txt)|*.txt||"),NULL);

	hFileDlg.m_ofn.nFilterIndex=1; //
	hFileDlg.m_ofn.hwndOwner=m_hWnd;  //
	hFileDlg.m_ofn.lStructSize=sizeof(OPENFILENAME);   //
	hFileDlg.m_ofn.lpstrTitle=TEXT("�������ļ�");  //CFileDialog �Ի������ 
	hFileDlg.m_ofn.nMaxFile=MAX_PATH;		 // �����ļ������壬�Ա��ѡ

	if(hFileDlg.DoModal() !=IDOK)
	{
		::MessageBox(NULL, hFileDlg.GetPathName(),NULL,MB_OK ); // ��ʾ��ѡ�ļ�����ϸ·��
		return;
	} 

    CString myfilename=hFileDlg.GetPathName();
	if (!ReadfromFile(myfilename))
	{
		MessageBox(_T("�����ļ�����ʧ�ܣ�"));
		return;
	}
	for (int i=0;i<(sizeof(Sensors_Array)/sizeof(float));i++)
	{
		Sensors_Array[i]/=100;
	}
	//�������ֵ
	x_left=Sensors_Array[0];
	y_front=Sensors_Array[1];
	z_down=Sensors_Array[2];
	x_right=Sensors_Array[3*Length_Num*Width_Num*Height_Num-3];
	y_back=Sensors_Array[3*Length_Num*Width_Num*Height_Num-2];
	z_up=Sensors_Array[3*Length_Num*Width_Num*Height_Num-1];
	Model_Length=x_right-x_left;
	Model_Width=y_back-y_front;
	Model_Height=z_up-z_down;
	//����������
	pLenaView->m_ymin=y_front*100;
	pLenaView->m_xmin=x_left*100;
	pLenaView->m_zmin=z_down*100;
	pLenaView->UpdateData(FALSE);
	//���㴦��
	SetBadPoints();
	pActiveView->Visual_depth*=Model_Length/2;
	pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
}

void CMainFrame::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	CFrameWnd::PostNcDestroy();
}

void CMainFrame::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if (startwriteflag)
	{
		MessageBox(_T("���ȹر�ʵ�飡"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("���ȹر���ʷ��¼��"));
		return;
	}
// 	if (MessageBox(_T("�Ƿ�رճ���?"),_T("��ʾ"),MB_OKCANCEL|MB_ICONINFORMATION)==IDCANCEL)
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

	::WritePrivateProfileString(_T("�¶�"),_T("���ֵ"),TcMax,FilePath);
	::WritePrivateProfileString(_T("�¶�"),_T("��Сֵ"),TcMin,/*TcMin.GetBuffer(MAX_PATH),MAX_PATH,*/FilePath);

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

//��ʷ��¼
void CMainFrame::OnHistory()
{
	if (!sensors_num)
	{
		AfxMessageBox(_T("�������������ļ�"));
		return;
	}
	if (startwriteflag)
	{
		MessageBox(_T("ʵ������У�"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("�Ѿ�����ʷ���ݣ�"));
		return;
	}
	CHistoty myhistoty;
	if (myhistoty.DoModal()!=IDOK)
	{
		return;
	}
	//������ݿ�����
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
	//����ʷ���ݿ�
	if (!adoconn.ADO_Connection(myhistoty.mypathfilename))
	{
		MessageBox(_T("���ݿ�����ʧ�ܣ�"));
		return;
	}
	//�����ݱ�
	//int numofrecords=sensors_num/250;
	  int numofrecords=sensors_num/230;
         	if (sensors_num%230>0)
	{
		numofrecords+=1;
	}


	for (int i=0;i<numofrecords;i++)
	{
		CString tablename;
		//tablename.Format(_T("����������%d"),i+1);
		
	     if(i == 2)
		 {
			 tablename.Format(_T("����%d"),i+4);
		 }
		 else
		 {
			 tablename.Format(_T("����%d"),i+1);
		 }
	     	
	   CString str=tablename;
	

	  
		Pmyrecordset[i]=adoconn.ADO_GetRecordSet2(tablename);
		if (Pmyrecordset[i]==NULL)
		{
			MessageBox(_T("���ݱ��ʧ�ܣ�"));
			for(int j=0;j<i;j++)
			{
				adoconn.ADO_CloseRecord(Pmyrecordset[j]);
			}
			adoconn.ADO_ExitConnect();
			return;
		}
	}
	
	m_PlayPos=1;
	//ɾ����ͼ�ඨʱ�طŹ���
	pActiveView->SetTimer(5,1000,NULL);
	//�������ݶ�ȡ��־
	ThreadReadDataflag=true;
    //�������ݶ�ȡ�߳�
	ThreadReadData=AfxBeginThread(AFX_THREADPROC(ReadData),(LPVOID)this,THREAD_PRIORITY_NORMAL,0,0,NULL);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_HISTORY,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPHISTRY,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_ENABLED);
}

//��ʼ��¼
void CMainFrame::OnStart()
{
	// TODO: �ڴ���������������
	if (!startwriteflag)
	{
		return;
	}
	recordflag=true;
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_START,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(IDM_PAUSE,MF_BYCOMMAND|MF_ENABLED);
}


//�������
void CMainFrame::OnSetpoint()
{
	// TODO: �ڴ���������������
	CSetPoint SPdlg;
	SPdlg.DoModal();
}
void CMainFrame::DockControlBarLeftOf(CToolBar *Bar,CToolBar *LeftOf)
{
	// �� �� �� �� �� �� �� ͣ �� �� ͬ һ �� �� ��
	CRect rect; // �� �� �� �� �� ��
	DWORD dw;
	UINT n=0;
	RecalcLayout(); // �� �� �� ʾ
	LeftOf ->GetWindowRect(&rect);
	rect.OffsetRect(1,0); // �� �� ƫ �� ֲ �� ͣ �� �� ͬ һ �� ��
	dw=LeftOf ->GetBarStyle();
	n=(dw &CBRS_ALIGN_TOP)?AFX_IDW_DOCKBAR_TOP:n;
	n=(dw &CBRS_ALIGN_BOTTOM &&n==0)?AFX_IDW_DOCKBAR_BOTTOM:n;
	n=(dw &CBRS_ALIGN_LEFT &&n==0)?AFX_IDW_DOCKBAR_LEFT:n;
	n=(dw &CBRS_ALIGN_RIGHT &&n==0)?AFX_IDW_DOCKBAR_RIGHT:n;
	DockControlBar(Bar,n, &rect);
}
BOOL CMainFrame::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	return CFrameWnd::DestroyWindow();
}
void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
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
		m_progress.SetPos(number); //�趨������Ϣ
		number+=1;
		str.Format(_T("%d"),number);
		index=m_wndStatusBar.CommandToIndex(IDS_PRONUM);
		
		m_wndStatusBar.GetItemRect(index,&rect);
		if(m_progress.m_hWnd)
			m_progress.MoveWindow(rect);//���������Ƶ���λ�� 
		m_wndStatusBar.SetPaneText(index,str); //��ʾ������Ϣ
		if(number>=99)
			number=98;
		break;
	case 4:
		number=0;
		m_progress.SetPos(number); //�趨������Ϣ
		str.Format(_T("%d"),number);
		index=m_wndStatusBar.CommandToIndex(IDS_PRONUM);
		m_wndStatusBar.GetItemRect(index,&rect);
		if(m_progress.m_hWnd)
			m_progress.MoveWindow(rect);//���������Ƶ���λ��
		m_wndStatusBar.SetPaneText(index,str); //��ʾ������Ϣ
		break;
	default:
		break;
	}
	CFrameWnd::OnTimer(nIDEvent);
}

//�����ֵ
void CMainFrame::SetBadPoints()
{
	int tempsensornum[1024]={-1};
	//����
	for (int q=0;q<sensors_num;q++)
	{
		DataNumbers[q]=OldDataNumbers[q];
		tempsensornum[q]=sensornumber[q];
	}
	//����ͬһ���X���Y������ڲ�ֵ
	for (int j=0;j<Height_Num;j++)
	{
		//��־����
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
		//��ֵ
		for (int i=0;i<(int)(Length_Num*Width_Num);i++)
		{	
			if (tempsensornum[i+j*(Length_Num*Width_Num)]<0)
			{	
				//��ȡ�ĵ���X��Y����
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
				//����
				if (addnum==0)//һ�㶼û�д�����
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
//ɾ���ļ���Ŀ¼(�ǿ�)

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

//��ȡ�����߳�
UINT ReadData(LPVOID *pPrama)
{
	int nphotoframe=0;	//pic֡��
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
	CToolBar* pToolBar = (CToolBar*)AfxGetMainWnd()->GetDescendantWindow(IDR_EXTEND_BAR);//������ָ��
	//���ݶ�ȡ��λ��
	long nCounttotal=Pmyrecordset[0]->GetRecordCount();
	long nCount=0;
	//�ƶ����ļ�ͷ
	for (int i=0;Pmyrecordset[i]!=NULL;i++)
	{
		Pmyrecordset[i]->MoveFirst();
	}
	//���ݶ�ȡ����
	while(ThreadReadDataflag)
	{
//���ݻ�����λ�ö�λ
		if (!ThreadReadDataflag0)//�ֶ��ƶ�����
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
			//�Զ�������ʱ
			Sleep(500);
			//������һ�ζ�ȡλ��
			nCount+=20;
			if (nCount>nCounttotal)//�Ѿ���ͷ
			{
				nCount=nCounttotal;
				//�����Զ����ź�AVI��¼
				ThreadReadDataflag0=false;
				bAviFlag=false;
				//����״̬��
				pMainFram->pActiveView->SendMessage(WM_MY_MESSAGE,5,0);
			}
			m_PlayPos=nCount*order_number_total/(nCounttotal);
			//��λ����
			pMyToolBar->Setpos();
			//���ݿ����
			for (int i=0;Pmyrecordset[i]!=NULL;i++)
			{
				Pmyrecordset[i]->Move(20);
			}
		}	
/////////��ȡ����
		//���ݳ���ΧԼ��
		if (Pmyrecordset[0]->adoEOF)
		{
			for (int i=0;Pmyrecordset[i]!=NULL;i++)
			{
				Pmyrecordset[i]->MoveLast();
			}
		}
		//��ȡ����
		int tablenum=0;
		for (;Pmyrecordset[tablenum+1]!=NULL;tablenum++)
		{
			if (tablenum==0)
			{
				//���ݲɼ���ʱ���
				timestring=Pmyrecordset[0]->GetCollect(_T("ʱ��"));
			}
			for(int i=0;i<250;i++)
			{
				tempstr.Format(_T("�ɼ���%d"),tablenum*250+i+1);
				datatempstr=Pmyrecordset[tablenum]->GetCollect((_variant_t)tempstr);
				OldDataNumbers[tablenum*250+i]=_wtof(datatempstr);
			}
		}
		for (int i=tablenum*250;i<sensors_num;i++)
		{
			tempstr.Format(_T("�ɼ���%d"),i+1);
			datatempstr=Pmyrecordset[tablenum]->GetCollect((_variant_t)tempstr);
			OldDataNumbers[i]=_wtof(datatempstr);
		}
		//׼������
		pMainFram->SetBadPoints();
		//��ʾ
		pMainFram->pActiveView->SendMessage(WM_MY_MESSAGE,2,NULL);
		//�Զ�����ʱ��¼��Ƶ����
		if (bAviFlag)
		{
			//�����汣�浽��Ƶ
			pMainFram->SaveBMPToAVI();
		}
		else
		{
		   	pMainFram->CloseAvi();
		}
	}
	//�رճ���
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
	// TODO: �ڴ����ר�ô����/����û���
	DDX_Control(pDX, IDC_SLIDERCTRL, m_SliderCtrl_x);
	DDX_Slider(pDX, IDC_SLIDERCTRL, m_nSlider_x);
	DDV_MinMaxInt(pDX, m_nSlider_x, 1, order_number_total);
	CFrameWnd::DoDataExchange(pDX);
}

void CMainFrame::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFrameWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CMainFrame::OnGoon()
{
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	ThreadReadDataflag0=false;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
}

void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();

	g_eventKill.SetEvent();
	// TODO: �ڴ˴������Ϣ����������
}


afx_msg LRESULT CMainFrame::OnProgress(WPARAM wParam,LPARAM lParam)
{
	CRect rect;
	int index=m_wndStatusBar.CommandToIndex(IDS_PROGRESS);
	m_wndStatusBar.GetItemRect(index,&rect);
	m_progress.Create(WS_CHILD|WS_VISIBLE|PBS_SMOOTH,rect,&m_wndStatusBar,150);//����������
	if(m_progress.m_hWnd)
		m_progress.MoveWindow(rect);//���������Ƶ���λ�� 
	return 1;
}

void CMainFrame::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CFrameWnd::OnPaint()
	CRect rect;
	int index=m_wndStatusBar.CommandToIndex(IDS_PROGRESS);
	m_wndStatusBar.GetItemRect(index,&rect);
	if(m_progress.m_hWnd)
		m_progress.MoveWindow(rect);//���������Ƶ���λ�� 
}
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CFrameWnd::WindowProc(message, wParam, lParam);
}

void CMainFrame::OnClearlast()
{
	// TODO: �ڴ���������������
	well_num--;
	if (well_num<0)
	{
		well_num=0;
	}
	pActiveView->SendMessage(WM_PAINT);
}

void CMainFrame::OnPause()
{
	// TODO: �ڴ���������������
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
        ////////ʵ�����ݲɼ�
		if (Tempflag)
		{
		}
		////���3038�ȵ�ż�ɼ�����
		
	    /////��ȡ�Ǳ�ɼ�ʵ������
		//ʵ�����ݵ��������ɼ����ݵ���Ϊ�����ñ�˳�����е�����
		for (int iii=0;iii<sensors_num;iii++)
		{
			OldDataNumbers[iii]=tempdatavalue[sensornumber[iii]-1];
		}
		//���ݼ�¼
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
		//���㴦��
		pMainFram->SetBadPoints();
		pMainFram->pActiveView->PostMessageW(WM_MY_MESSAGE,2,NULL);
	}
	for (int iii=0;iii<sensors_num;iii++)
	{
		OldDataNumbers[iii]=tempdatavalue[sensornumber[iii]-1];
	}
	//������ݿ���Դ
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

	// UNICODE��Ϣ
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	//TCHAR szFullText[512];
	CString strTipText;
	UINT nID = pNMHDR->idFrom;

	if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
		pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
	{
		// idFromΪ��������HWND 
		nID = ::GetDlgCtrlID((HWND)nID);
	}

	if (nID != 0) //��Ϊ�ָ���
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

		// ʹ��������ʾ������������
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
		AfxMessageBox(_T("DLL����ʧ��"));
		return -1;
	}
	
	//����IMP�⺯��
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
		if (CardHandle>=0)	//���ӳɹ�
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
//	//����ɣͣп��Ŀ⺯��
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

	//����IMP�⺯��
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
	// TODO: �ڴ���������������
	if (sensors_num>0)
	{
		MessageBox(_T("ģ���Ѿ�������"));
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
	// TODO: �ڴ����ר�ô����/����û���
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
	buf[num]=0;//�������

	//����

	
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
	// TODO: �ڴ���������������
}


void CMainFrame::OnSetstaff1()
{
	// TODO: �ڴ���������������
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
		//��ͷ
		if (tempstring==_T("[IMP_CARD]"))
		{
			continue;
		}
		//��β
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
			//���ȷ��򴫸�������
			if (name==_T("numX"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Length_Num=atof(pvalue);
			}
			//��ȷ��򴫸�������
			if (name==_T("numY"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Width_Num=atof(pvalue);
			}
			//�߶ȷ������
			if (name==_T("numZ"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				Height_Num=atof(pvalue);
			}
			//�ɼ�������
			if (name==_T("cardnum"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				cjbNum=atof(pvalue);
			}
			//����������
			if (name==_T("Totalnum"))
			{
				int lenth=value.GetLength();
				pvalue[lenth+1]=0;
				WideCharToMultiByte(CP_OEMCP, 0, value, -1, pvalue, lenth, NULL, NULL);
				sensors_num=atof(pvalue);
			}
		}
		else//�ɼ���λ���봫�������
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
	//�ļ�ͷ
	tempstring=_T("[IMP_CARD]\n");
	myfile.WriteString(tempstring);	
	//X�����
	tempstring.Format(_T("numX=%d\n"),Xnum);
	myfile.WriteString(tempstring);
	//y�����
	tempstring.Format(_T("numY=%d\n"),Ynum);
	myfile.WriteString(tempstring);	
	//z����
	tempstring.Format(_T("numZ=%d\n"),Znum);
	myfile.WriteString(tempstring);
	//�忨��
	tempstring.Format(_T("cardnum=%d\n"),cardnum);
	myfile.WriteString(tempstring);
	//�ɼ�������
	tempstring.Format(_T("Totalnum=%d\n"),totalnum);
	myfile.WriteString(tempstring);
	//�����
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
	//�ļ�β
	tempstring=_T("[END]");
	myfile.WriteString(tempstring);
	myfile.Close();
	return true;
}


void CMainFrame::OnMsarttest()
{
	// TODO: �ڴ���������������
	if (!sensors_num)
	{
		AfxMessageBox(_T("�������������ļ�"));
		return;
	}
	if (startwriteflag)
	{
		MessageBox(_T("�����Ѿ���ʼ��"));
		return;
	}
	if (ThreadReadDataflag)
	{
		MessageBox(_T("���ڽ�����ʷ��¼�طţ�"));
		return;
	}
	if (sensors_num<=0)
	{
		MessageBox(_T("ģ��δ���������Ƚ���ģ�ͣ�"));
		return;
	}
	//�رմ���
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
	//���ݿ��Ѿ�����
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
	//�������ݿ��ļ�	
	CTime time=CTime::GetCurrentTime();
	CString dbname=mynew.mydatapath+mynew.mytestname;
	dbname+=_T(".accdb");
	//�������ݿ��ļ�
	if (!adoconn.CreateAccess(dbname))
	{
		MessageBox(_T("���ݿ��ļ�����ʧ�ܣ�"));
		return;
	}
	//�������ݿ�
	if (!adoconn.ADO_Connection(dbname))
	{
		MessageBox(_T("���ݿ�����ʧ�ܣ�"));
		return;
	}
	if (mynew.mytestname==_T("�¶ȳ�"))
	{
		Tempflag=true;
		m_com_TConctral=new CCom(mynew.m_ConCtral_com,9600,'N',8,1);
		if (!m_com_TConctral->InitCom())
		{
			MessageBox(_T("�¶ȿ��������ڴ�ʧ�ܣ�"));
			delete m_com_TConctral;
			m_com_TConctral=NULL;
			return;
		}
		m_com_Tsensor=new CCom(mynew.m_TSensorCom,9600,'N',8,1);
		if (!m_com_Tsensor->InitCom())
		{
			MessageBox(_T("�¶ȴ��������ڴ�ʧ�ܣ�"));
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
			MessageBox(_T("ѹ�����ڴ�ʧ�ܣ�"));
			delete m_com_Pre;
			m_com_Pre=NULL;
			return;
		}
	}
	//�������ݱ�
	int numofrecords=sensors_num/250;
	if (sensors_num%250>0)
	{
		numofrecords+=1;
	}
	for (int i=0;i<numofrecords;i++)
	{
		CString tablename;
		tablename.Format(_T("����������%d"),i+1);
		CString prama;
		if (i==0)
		{
			prama=_T("(��� REAL PRIMARY KEY,ʱ�� TEXT,");
		}
		else
		{
			prama=_T("(��� REAL PRIMARY KEY,");
		}
		for (int j=0;j<250;j++)
		{
			CString tempstr;
			tempstr.Format(_T("�ɼ���%d REAL,"),i*250+j+1);
			prama+=tempstr;
		}
		prama.TrimRight(',');
		prama+=_T(")");
		Pmyrecordset[i]=adoconn.ADO_Createtable(tablename,prama);
		if (Pmyrecordset[i]==NULL)
		{
			MessageBox(_T("���ݱ���ʧ�ܣ�"));
			return;
		}
	}
	//ͼƬ��Ƶ����·��
	mypicsavepath=mynew.mypictpath;
	//�ɼ�ʱ����
	sjbc_socond=(int)mynew.myinetnaltime;
	//ɾ����ͼ�ඨʱ�طŹ���
	pActiveView->KillTimer(5);
	//��ʼ����
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
	// TODO: �ڴ���������������
	if (recordflag)
	{
		MessageBox(_T("���ȹرռ�¼��"));
		return;
	}
	startwriteflag=false;
	//������ͼ�ඨʱ�طŹ���
	pActiveView->SetTimer(5,1000,NULL);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSARTTEST,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(1)->EnableMenuItem(ID_MSTOPTEST,MF_BYCOMMAND|MF_GRAYED);
}


void CMainFrame::OnMstartavi()
{
	// TODO: �ڴ���������������
	if (!ThreadReadDataflag)
	{
		return;
	}
	CAVIPrama mypaema;
	if (mypaema.DoModal()==IDOK)
	{
		//��ʼ����Ƶ����
		Xwidth=mypaema.mywidth;
		Yheigth=mypaema.myheiht;
		FPS=mypaema.myfps;
		Avipathname=mypaema.myfilepath+mypaema.myname+_T(".avi");
		if (!InitAVI())
		{
			MessageBox(_T("��Ƶ��ʼ��ʧ�ܣ�"));
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	if (ThreadReadDataflag0)
	{
		MessageBox(_T("���ȹر��Զ����ţ�"));
		return;
	}
	if (bAviFlag)
	{
		MessageBox(_T("���ȹر���Ƶ��"));
		return;
	}
	//������ͼ�ඨʱ�طŹ���
	pActiveView->KillTimer(5);
    ThreadReadDataflag=false;
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_HISTORY,MF_BYCOMMAND|MF_ENABLED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPHISTRY,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_GRAYED);
	GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_GRAYED);


}
//OPenCV��ƵAVI�ļ�����
bool CMainFrame::SaveBMPToAVI()
{
	IplImage *pimage=cvLoadImage("C:\\temppict.bmp");
	//��Ƶ�ߴ�任
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
	//��ȡ����
	int wLen = WideCharToMultiByte(CP_ACP,0,Avipathname,-1,NULL,0,NULL,NULL);
	//��CStringת����char*  
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CFrameWnd::OnSysCommand(nID, lParam);
}


void CMainFrame::OnMsvpicttest()
{
	// TODO: �ڴ���������������
	bpicture=true;
}


void CMainFrame::OnMsvpicthistory()
{
	// TODO: �ڴ���������������
	bpicture=true;
}

// lengnanView.cpp : ClengnanView ���ʵ��
//

#include "stdafx.h"
#include "lengnan.h"

//#include "lengnanDoc.h"
#include "MainFrm.h"
#include "lengnanView.h"
#include "IA.h"
#include "Lena.h"
#include "math.h"
#include "GLFont.h"
#include "code_change.h"
#include "SetWellDlg.h"
#include <ctime>
#include <string>
#include "afxmt.h"
#include "PointNow.h"
#include "myheader.h"
#include "ShowPoint.h"
using namespace std;
extern BOOL SetStaff;
extern bool bAviFlag;//����AVI�ļ���־
extern bool bpicture;//����ͼƬ��־
extern CString mypicsavepath;	//��ͼ��Ƶ����·��
int temvalue1=0;
int temvalue2=0;
int temvalue3=0;
int temvalue4=0;
int temvalue5=0;


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern int IMP_Num;
bool value_colorflag=true;
//////ģ�����񻮷ֵ�Ŀռ�����ֲ�
float verdex_array[200*200*100];

//ģ����Ϣ��ʾ
extern BOOL m_bModleInfo;
extern CLena* pLena;
extern bool numshowflag;//��ʾ���ݣ�true��,��ʾ��ţ�false��
extern bool cutxflag;
extern bool cutyflag;
extern bool cutzflag;

extern bool opengliniflag;	//opengl��Ҫ��ʼ����־λ
//��ȡ�����߳�
extern CWinThread *ThreadReadData;
extern bool bSuspendThreadflag;	//�̼߳��������־
extern bool ThreadReadDataflag; //�̹߳��𣬻ָ���־λ��trueΪ�ָ���falseΪ����
extern bool threadstarrtflag;//�߳̿�����־
extern bool threadoverflag;
GLint viewport[4];
extern CEvent DataEvent2;
extern int m_PlayPos;
//���ݲɼ���ʱ���
extern CString timestring;
//�ط�ģʽ��־λ
extern bool Historyflag;

int glvolume_total,glvolume;
extern int nPointNum;				//������Ŀ
extern int nPoints[100];			//�����������

float isosurface_data[10]={0};	//��ֵ����ֵ
int isosurface_num=0;

extern int error_num;
//��ֵ��������Ƭ32M��
TRIANGLE Triangle[4][2*1024*1024];


//����ü�ģ���������ϵ�ֵ
extern float x_cutout,y_cutout,z_cutout;

GLdouble eqn[4];
GLdouble eqnx[4];
GLdouble eqny[4];
GLdouble eqnz[4];

extern float Alpha_Mixing;		//Alpha���ϵ��
//���ߵ���ʼ��ֹ����
Vector3D PtWellS[20],PtWellE[20];
//ע��������
bool ymy[20];
//��������
int well_num=0;
extern BOOL bperspective;//�Ƿ�͸��
extern bool diswellflag;//��ʾ����־
//�������ֵ
extern float z_down,z_up;//�ϡ���
extern float y_front,y_back;//ǰ����
extern float x_left,x_right;//����
Vector3D Pt3d,Pt3d0,Pt3d1;
GLdouble World_x[2], World_y[2], World_z[2]; 
bool show_coordinateflag=false;
//��Ϣ���Ʊ�־λ��
extern UINT CtrlFlag;
//////////////////////////////////////////////////////////////////////////
//global variable
//�洢3άģ��ģ���¶ȳ����ݣ����Ϊ8M�����׾���
static float DataMatrices_3D[1*200*200*200];
//�洢2άƽ���¶ȳ����ݣ����Ϊ1000*1000�Ķ��׾���
static float DataMatrices_2D[10*200*200];
static float DataMatrices_2D_x[100*200*200];
static float DataMatrices_2D_y[10*200*200];

static float DataMatrices_2D_zz[2*200*200];
static float DataMatrices_2D_xx[20*200*200];
static float DataMatrices_2D_yy[2*200*200];

//static float DataArray[8*1024*1024]={0};

//����������
extern int sensors_num;
//ģ�Ͳ���,ģ�ʹ�С
extern float Model_Length;
extern float Model_Width;
extern float Model_Height;
//�����ߡ���
extern UINT Length_Num,Width_Num,Height_Num;
//����������
extern float DataNumbers[1000];
float range[1000];
//////////////////////////////////////////////////////////////////////////
//��������Ź���
//����ǰΪ����������ң���ǰ������������
//ǰ��Ϊ��һ�ţ���Ϊ��һ��
//////////////////////////////////////////////////////////////////////////
//������λ������
extern float Sensors_Array[3000];
//typedef struct SENSORS{
//	int num;//���������
//	float x;//���λ��
//	float y;//���λ��
//	float z;//���λ��
//	float data;//��������ֵ����3άͼ���ж�Ӧcolorֵ��ʾ
//}Sensors;

int nSlider;
// �¶���Сֵ
int mymin;
// �¶����ֵ
int mymax;
extern int nradio;//ѡ�еĲ�
extern BOOL bshow;//�Ƿ�������ʾ
extern int n3showmode;//��ʾģʽ�����㡢��㡢ʵ��
extern int MulFloorFlag;
extern bool initflag;
bool resflag=false;

extern int IsoSurface_num;
//////////////////////////////////////////////////////////////////////////
// ClengnanView

IMPLEMENT_DYNCREATE(ClengnanView, CView)

BEGIN_MESSAGE_MAP(ClengnanView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETCURSOR()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MY_MESSAGE,&ClengnanView::OnMyMessage)
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDM_SHOW_FRONT, &ClengnanView::OnShowFront)
	ON_COMMAND(IDM_SHOW_SIDE, &ClengnanView::OnShowSide)
	ON_COMMAND(IDM_SHOW_PLANFORM, &ClengnanView::OnShowPlanform)
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(IDM_SETWELL, &ClengnanView::OnSetwell)
	ON_COMMAND(ID_MDisSingle, &ClengnanView::OnMdissingle)
END_MESSAGE_MAP()

// ClengnanView ����/����

ClengnanView::ClengnanView()
{
	// TODO: �ڴ˴���ӹ������
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	ZFar=0.0f;
	zNear=0.0f;
	m_x=1.0f;
	m_y=1.0f;
	m_z=1.0f;
	mouseflag=FALSE;
	Visual_depth=-3.5f;
	camera_z=0.0;
	//mymin=0;
	//mymax=80;
	mouse_locationflag=0;
	dandan=0;
}

ClengnanView::~ClengnanView()
{
}

BOOL ClengnanView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	/*WS_CLIPCHILDREN��ʽ��Ҫ�����ڸ����ڣ�Ҳ����˵���ڸ����ڻ��Ƶ�ʱ��
	�������ϻ���һ���Ӵ��ڣ���ô�����������ʽ�Ļ����Ӵ������򸸴��ھͲ�������ơ�
	
	WS_CLIPSIBLINGS�Ӵ��ڼ��໥�ü���Ҳ����˵�����������໥�ص�ʱ��
	������WS_CLIPSIBLINGS��ʽ���Ӵ����ػ�ʱ���ܻ��Ʊ��ص��Ĳ��֡�*/

	cs.style	|=	WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}
//////////////////////////////////////////////////////////////////////////
// ClengnanView ����
//�ڻ��Ƴ���ʱ��һ��������²��裺
//1����ջ��档
//2�����Ƴ�����
//3��Flush����Ⱦ��ˮ�ߡ�
//4����������˫���壬�򽻻�ǰ��̨��������
//////////////////////////////////////////////////////////////////////////
void ClengnanView::OnDraw(CDC* /*pDC*/)
{
	ClengnanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//��ջ���
    //���Ƴ�������ͼ������������
	RenderScene();
	glFinish();//GL��������е�����͸��Կ������������У��Կ������Щ���Ҳ���ǻ����ˣ��󷵻ء�
	SwapBuffers(m_pDC->GetSafeHdc());//����������
	//������ʾ

	//����AVI
	if (bAviFlag)
	{
		CString tempstr=_T("C:\\temppict.bmp");
		SaveBMPFromOpenGL(tempstr);
	}
	//����ͼƬ
	if (bpicture)
	{
		CTime t=CTime::GetCurrentTime();
		CString tempstr=t.Format(_T("%Y%m%d%H%M%S"));
		tempstr+=_T(".bmp");
		tempstr=mypicsavepath+tempstr;
		SaveBMPFromOpenGL(tempstr);
		//���ͼƬ�����־
		bpicture=false;
	}
}  

// ClengnanView ��ӡ

BOOL ClengnanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void ClengnanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void ClengnanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// ClengnanView ���

#ifdef _DEBUG
void ClengnanView::AssertValid() const
{
	CView::AssertValid();
}

void ClengnanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ClengnanDoc* ClengnanView::GetDocument()// �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ClengnanDoc)));
	return (ClengnanDoc*)m_pDocument;
}
#endif //_DEBUG


// ClengnanView ��Ϣ�������

int ClengnanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �ڴ������ר�õĴ�������

	//��ʼ��opengl��ͼ
	if (!InitializeOpenGL())
		return -1;

	//glGetIntegerv(GL_VIEWPORT, viewport); 

	SetTimer(1,1000,NULL);
	return 0;
}

//Ϊ�˱����ڴ�й¶������Ҫ��������ʹ�õ�new�������������ڴ��ڳ���ر�ʱdelete����
void ClengnanView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(1);
	// TODO: �ڴ˴������Ϣ����������
	if (::wglMakeCurrent(NULL,NULL)==FALSE)
	{
		MessageBox(_T("wglMakeCurrent failed."));
	}
	if (::wglDeleteContext(m_hRC)==FALSE)
	{
		//MessageBox(_T("wglCreateContext failed."));
	}
	if (m_pDC)
	{
		ReleaseDC(m_pDC);
		delete m_pDC;
		m_pDC=NULL;
	}
}

void ClengnanView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	//nWidth   =   GetSystemMetrics(SM_CXSCREEN);    
	//nHeight=   GetSystemMetrics(SM_CYSCREEN);
	CRect rect;
	GetClientRect(&rect);
	nWidth = rect.Width();
	nHeight= rect.Height();
	
	//if (cx<=0||cy<=0)
	//{
	//	MessageBox(_T("Size error!"));
	//	return;
	//}
	glViewport(0,0,cx,cy);//�ӿ��趨���ӿڱ�����Ϊռ�ݴ򿪴��ڵ��������ؾ���

	aSpect=(GLdouble)cx/(GLdouble)cy;//ָ������Ӿ���Ŀ�߱�
	//////////////////////////////////////////////////////////////////////////
	//glMatrixMode���õ�ǰ����ģʽ:
	//GL_MODELVIEW,��ģ���Ӿ������ջӦ�����ľ������.
	//GL_PROJECTION,��ͶӰ����Ӧ�����ľ������.
	//GL_TEXTURE,����������ջӦ�����ľ������.
	//////////////////////////////////////////////////////////////////////////

	glMatrixMode(GL_PROJECTION);//ͶӰ����
	glLoadIdentity();//��λ����ǰ����
	//����͸��ͶӰ����
	gluPerspective(45,//�Ƕ�,ָ���Ӿ������Ұ�ĽǶȣ��Զ���Ϊ��λ��y������·��� 
		aSpect,//�Ӿ���Ŀ�߱�,ָ������Ӿ���Ŀ�߱�
		zNear+0.01f,//��z�᷽���������֮��ľ���Ľ���,ָ���۲��ߵ��Ӿ��������Ĳü���ľ���
		ZFar+100.0f//��z�᷽���������֮��ľ����Զ��,ָ���۲��ߵ��Ӿ������Զ�Ĳü���ľ���
		);

	glMatrixMode(GL_MODELVIEW);//ģ���Ӿ�����
	glLoadIdentity();

	// TODO: �ڴ˴������Ϣ����������
}

//Ϊ��ʹ�ı䴰�ڴ�Сʱ���ص���˸����OnEraseBkgnd����һЩ����������windows�Լ��Ĵ���ˢ����˸��
BOOL ClengnanView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CView::OnEraseBkgnd(pDC);
}

//��ʼ��opengl����
BOOL ClengnanView::InitializeOpenGL()
{
	m_pDC=new CClientDC(this);
	//m_pDC=new CWindowDC(this);

	if (m_pDC==NULL)
	{
		AfxMessageBox(_T("��ȡDC����"));
		return FALSE;
	}
	if (SetupPixelFormat()==FALSE)
	{
		AfxMessageBox(_T("������ɫ��ʽ����"));
		return FALSE;
	}
	//Create RC
	m_hRC=wglCreateContext(m_pDC->GetSafeHdc());

	//Failure to Create Rendering Context

	if(!m_hRC)

	{

		MessageBox(_T("����RC����"));
		return FALSE;

	}

	//Make the RC Current

	if(!wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC))

	{

		MessageBox(_T("Error making RC Current"));
		return FALSE;

	}
	
	//��ʼ������
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0);//ָ����Ȼ����������ֵ

	glEnable(GL_TEXTURE_2D);						// ��������ӳ��
	glShadeModel(GL_SMOOTH);						// ������Ӱƽ��
	//////////////////////////////////////////////////////////////////////////
	//ָ��������Ȼ���Ƚ�ֵ��
	//����˵����funcָ����ȱȽϺ�����
	//GL_NEVER,��ͨ������������ֵ��ȡ���ο�ֵ��
	//GL_LESS,�����������ֵС�ڲο�ֵ����ͨ��
	//GL_EQUAL,�����������ֵ���ڲο�ֵ����ͨ��
	//GL_LEQUAL,�����������ֵС�ڻ���ڲο�ֵ����ͨ��
	//GL_GREATER,�����������ֵ���ڲο�ֵ����ͨ��
	//GL_NOTE_QUAL,�����������ֵ�����ڲο�ֵ����ͨ��
	//GL_GEQUAL,�����������ֵ���ڻ���ڲο�ֵ����ͨ��
	//GL_ALWAYS,����ͨ������������ֵȡ���ο�ֵ��
	//////////////////////////////////////////////////////////////////////////
	glDepthFunc(GL_LESS);

	//////////////////////////////////////////////////////////////////////////
	//��Ȳ���,���漰������������������ڵ�������Ҫ������Ȳ���
	//glEnable(GL_DEPTH_TEST),Ӳ���ϴ�����Ȼ������������µ�ͬ��XY�����Ƭ�ϵ���ʱ��
	//�Ƚ����ߵ���ȣ������ڳ�ʼ��ʱ����ȣ�����ÿһ֡ǰҪgl_clear(gl-depth-bit)
	//�����clear_buffer_bit���ƣ�����ͬ��Ҫ����clear_deppth_bitd ֵ��
	//��glClearDepth(GLclampd depth)��һ����Ϊ1���⽫������Ϊ�������Ĭ�ϵģ�ͨ������д��
	//////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);//������ɫģʽ,GL_SMOOTHʹ�ù⻬��ɫ��GL_FALTʹ�ú㶨��ɫ

	//���
	//glColor4f(1.0f,1.0f,1.0f,alpha_mixing);			// ȫ���ȣ� 50% Alpha ���
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
	//if (m_pDC)
	//{
	//	m_pDC=NULL;
	//	ReleaseDC(m_pDC);
	//}
	//glPushMatrix();
	return TRUE;
}

//�������ظ�ʽ
BOOL ClengnanView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),//Structure size.  
		1,        // Structure version number.Property flags�����Ա�־����  
		PFD_DRAW_TO_WINDOW |  // support window  
		PFD_SUPPORT_OPENGL | // support OpenGL  
		PFD_DOUBLEBUFFER,                      
		PFD_TYPE_RGBA,   // RGBA type  
		24,      // 32��bit color.  
		0, 0, 0, 0, 0, 0,   // Not concerned with these:���漰������  
		0,      // No alpha ����alpha����  
		0,      // Shift bit ignored:����ת��λ  
		0, 0, 0, 0, 0,// No accum buffer��û���ۻ�����  
		32,  // 32��bit depth buffer.  
		0,      // No stencil����ģ�建��  
		0,      // No auxliliary buffers���޸�������  
		PFD_MAIN_PLANE, // Main layer type.����������  
		0,      // Reserved.�������ṹ��  
		0, 0, 0    // Unsupported.����֧�ֽṹ��  
	};

	int nPixelFormat	=	ChoosePixelFormat(m_pDC->GetSafeHdc(),&pfd);
	if (!nPixelFormat)
	{
		return FALSE;
	}
	if (!SetPixelFormat(m_pDC->GetSafeHdc(),nPixelFormat,&pfd))
	{
		return FALSE;
	}

	return TRUE;
}

float fff[]={	3.0,3.0,3.0,	1.0,0.0,0.0,
				0.0,0.0,0.0,	0.0,1.0,0.0,
				1.0,0.0,0.0,	0.0,0.0,1.0};

//��Ҫ��ͼ����
void ClengnanView::RenderScene()
{
	//ÿһ��Ĵ�С
	float size_rect[2]={Model_Length,Model_Width};
	//ģ����X,Y,Z�ϵĻ��֣�nSliderΪ��������
	int size_datanum[3]={(Length_Num-1)*(nSlider+1)+1,(Width_Num-1)*(nSlider+1)+1,(Height_Num-1)*(nSlider+1)+1};
	//ÿ�����ֵļ��
	float dx,dy,dz;
	//�������ֵ������Ŀռ�����
	float x,y,z;
	int tempindex=0;
	dx=(x_right-x_left)/(size_datanum[0]-1);
	dy=(y_back-y_front)/(size_datanum[1]-1);
	dz=(z_up-z_down)/(size_datanum[2]-1);
	//��ȡģ�͸������񻮷ֵ�Ŀռ�����ֲ�
	for (int i=0;i<(int)Height_Num;i++)
	{
		z=Sensors_Array[3*i*Length_Num*Width_Num+2];
		for (int j=0;j<size_datanum[1];j++)
		{
			y=y_front+dy*j;
			for (int k=0;k<size_datanum[0];k++)
			{
				x=x_left+dx*k;
				verdex_array[tempindex]=x;
				tempindex++;
				verdex_array[tempindex]=y;
				tempindex++;
				verdex_array[tempindex]=z;
				tempindex++;
			}
		}
	}
	//���// ����Դ����alphaͨ��ֵ�İ�͸����Ϻ���
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
	//////////////////////////////////////////////////////////////////////////
	//ģ�ͱ任����ת
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// ���õ�ǰ����Ϊ��λ����
	glLoadIdentity();					
	//�������λ��:����������������λ�ò������仯
	gluLookAt(0,0,camera_z,	0,0,Visual_depth,	0,1,0);;//����cameraλ��
	glTranslatef(m_xPos, m_yPos, Visual_depth);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glTranslatef(-Model_Length/2,-Model_Width/2,-Model_Height/2);
	//������ɫ���
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	GLfloat colortem[3];
	//�ж���ɫ���
	if (!bperspective)
	{
		glDisable(GL_BLEND);
		glEnable(GL_DEPTH_TEST);
	}
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);

	if (!cutxflag)
	{
		glClipPlane(GL_CLIP_PLANE1,eqnx);
		glEnable(GL_CLIP_PLANE1);
	}if (!cutyflag)
	{	
		glClipPlane(GL_CLIP_PLANE2,eqny);
		glEnable(GL_CLIP_PLANE2);
	}
	if (!cutzflag)
	{
		glClipPlane(GL_CLIP_PLANE3,eqnz);
		glEnable(GL_CLIP_PLANE3);
	}
	if (diswellflag)
	{
		//������
		DrawWell();
	}
	switch(n3showmode)
	{
	case 0:
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE2);
		glDisable(GL_CLIP_PLANE3);
		//���������
		if (bshow)
		{
			Draw_SigleFloor(DataMatrices_2D+(int)nradio*(size_datanum[0])*(size_datanum[1]),size_rect,size_datanum,GL_LINE_LOOP);
		}else{
			Draw_SigleFloor(DataMatrices_2D+(int)nradio*(size_datanum[0])*(size_datanum[1]),size_rect,size_datanum,GL_POLYGON);
		}
		//���ƴ�������
		if ((CtrlFlag&0x01)!=0)
		{	
			DrawPoint(Point_Radius,numshowflag);
		}
		break;
	case 1:	
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE2);
		glDisable(GL_CLIP_PLANE3);
		//���и��ƽ��
		if (z_cutout<z_up&&z_cutout>z_down)
		{
			if (bshow)
			{
				DrawCutFace_Z(DataMatrices_2D_zz,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_Z(DataMatrices_2D_zz,verdex_array,size_datanum,GL_POLYGON);
			}		
		}
		if (y_cutout<y_back&&y_cutout>y_front)
		{
			if (bshow)
			{
				DrawCutFace_Y(DataMatrices_2D_yy,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_Y(DataMatrices_2D_yy,verdex_array,size_datanum,GL_POLYGON);
			}		
		}
		if (x_cutout<x_right&&x_cutout>x_left)
		{
			if (bshow)
			{
				DrawCutFace_X(DataMatrices_2D_xx,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_X(DataMatrices_2D_xx,verdex_array,size_datanum,GL_POLYGON);
			}		
		}

		if (!bshow)
		{
			Draw_MulFloor(DataMatrices_2D,verdex_array,size_datanum,Height_Num,GL_QUADS);
		}else{
			Draw_MulFloor(DataMatrices_2D,verdex_array,size_datanum,Height_Num,GL_LINE_LOOP);
		}

		//���ƴ�������
		if ((CtrlFlag&0x01)!=0)
		{	
			DrawPoint(Point_Radius,numshowflag);
		}
		break;
	case 2:
		if (!bshow)
		{
			Draw_OutsideSurface(DataMatrices_2D,verdex_array,size_datanum,Length_Num,Width_Num,Height_Num,GL_QUADS);
		}else{
			Draw_OutsideSurface(DataMatrices_2D,verdex_array,size_datanum,Length_Num,Width_Num,Height_Num,GL_LINE_LOOP);
		}
		//���и��ƽ��
		if (z_cutout<z_up&&z_cutout>z_down)
		{
			if (bshow)
			{
				DrawCutFace_Z(DataMatrices_2D_zz,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_Z(DataMatrices_2D_zz,verdex_array,size_datanum,GL_POLYGON);
			}		
		}
		if (y_cutout<y_back&&y_cutout>y_front)
		{
			if (bshow)
			{
				DrawCutFace_Y(DataMatrices_2D_yy,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_Y(DataMatrices_2D_yy,verdex_array,size_datanum,GL_POLYGON);
			}		
		}
		if (x_cutout<x_right&&x_cutout>x_left)
		{
			if (bshow)
			{
				DrawCutFace_X(DataMatrices_2D_xx,verdex_array,size_datanum,GL_LINE_LOOP);
			}else{
				DrawCutFace_X(DataMatrices_2D_xx,verdex_array,size_datanum,GL_POLYGON);
			}		
		}
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE2);
		glDisable(GL_CLIP_PLANE3);
		//���ƴ�������
		if ((CtrlFlag&0x01)!=0)
		{	
			DrawPoint(Point_Radius,numshowflag);
		}
		break;
	default:
		break;
	}
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	DrawStaff(mymax,mymin);
	Draw_CoordinateSystem();
	if (show_coordinateflag)
	{
		PrintCoordinate(Pt3d);
	}
	//����ʱ���
	PrintTime();
	glDepthMask(GL_TRUE);
	//delete[] verdex_array;
}
void ClengnanView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ	
	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	//��¼��갴�µ���Ϣ
	m_MouseDownPoint=point;
	mouseflag=FALSE;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void ClengnanView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MouseDownPoint=CPoint(0,0);
	ReleaseCapture();
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}	
	CView::OnLButtonUp(nFlags, point);
}

BOOL ClengnanView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	if (zDelta<0)
	{
		//m_x/=1.1f;
		//m_y/=1.1f;
		//m_z/=1.1f;
		Visual_depth/=1.1f;
		//camera_z=(camera_z-Visual_depth)*1.1+Visual_depth;
	}
	else{
		//m_x*=1.1f;
		//m_y*=1.1f;
		//m_z*=1.1f;
		Visual_depth*=1.1f;
		//camera_z=(camera_z-Visual_depth)/1.1+Visual_depth;
	}
	InvalidateRect(NULL,FALSE);
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}	
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

BOOL ClengnanView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void ClengnanView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MouseDownPoint=point;
	mouseflag=TRUE;
	SetCapture();	
	CView::OnRButtonDown(nFlags, point);
}

void ClengnanView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_MouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

void ClengnanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	InvalidateRect(NULL,FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void ClengnanView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CRect rect;
	GetClientRect(&rect);
	if (GetCapture()==this)
	{
		if (mouseflag)
		{	
			m_xPos-=(point.x-m_MouseDownPoint.x)*Visual_depth/(0.85*nWidth);
			m_yPos+=(point.y-m_MouseDownPoint.y)*Visual_depth/nHeight;
			InvalidateRect(NULL,FALSE);
			m_MouseDownPoint=point;			
		}
		else{
			//Increment the object rotation angles
			m_xAngle+=(point.y-m_MouseDownPoint.y)/3.6;
			m_yAngle+=(point.x-m_MouseDownPoint.x)/3.6;
			//Redraw the view
			InvalidateRect(NULL,FALSE);
			//Set the mouse point
			m_MouseDownPoint=point;
		}
	}
	else{
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����	

		point.y=rect.Height()-point.y;
		GetSelectionRay(point.x,point.y);
	}
	SendMessage(WM_PAINT);
	
	CView::OnMouseMove(nFlags, point);
}

void ClengnanView::Draw_OutsideSurface(	float *datanum,		//������ɫֵ���ݱ�������
										float *verdex_array,	//��Ӧ������������
										int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����3ά
										int length_num,		//x���򴫸�������
										int width_num,			//y���򴫸�������
										int height_num,		//z���򴫸�������
										GLenum mode			//����ģʽ�����|�߿�
										)
{
	//��������
	//����,��
	int k,index;
	k=0;
	index=k*size_datanum[0]*size_datanum[1];
	DrawFlatSurface(datanum+index,verdex_array+3*index,size_datanum,mode);

	if(height_num!=1)
	{
		//���棬��
		k=height_num-1;
		index=k*size_datanum[0]*size_datanum[1];
		DrawFlatSurface(datanum+index,verdex_array+3*index,size_datanum,mode);

		FACECELL cell;
		float cell_z=0;
		float IsoSurface_data=0;
		int Iso_num=0;

		//��������
		float x,y,z,data;
		float dx,dy,dz;
		dx=(x_right-x_left)/(length_num-1);
		dy=(y_back-y_front)/(size_datanum[1]-1);
		dz=(z_up-z_down)/(size_datanum[2]-1);
		int index1;
		float color[3]={0};
		for	(k=0;k<length_num;k+=length_num-1)
		{
			index=k*size_datanum[1]*size_datanum[2];
			for (int j=0;j<size_datanum[2]-1;j++)
			{
				for (int i=0;i<size_datanum[1]-1;i++)
				{
					glBegin(mode);
					index1=i+j*size_datanum[1]+index;
					x=x_left+dx*k;
					y=y_front+dy*i;
					z=z_down+dz*j;				
					data=DataMatrices_2D_x[index1];

					cell.Pt00.x=y;
					cell.Pt00.y=z;
					cell_z=x;
					cell.Pt00.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1++;
					y+=dy;
					data=DataMatrices_2D_x[index1];

					cell.Pt10.x=y;
					cell.Pt10.y=z;
					cell_z=x;
					cell.Pt10.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1+=size_datanum[1];
					z+=dz;
					data=DataMatrices_2D_x[index1];

					cell.Pt11.x=y;
					cell.Pt11.y=z;
					cell_z=x;
					cell.Pt11.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1--;
					y-=dy;				
					data=DataMatrices_2D_x[index1];

					cell.Pt01.x=y;
					cell.Pt01.y=z;
					cell_z=x;
					cell.Pt01.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);
					glEnd();

					//���Ƶ�ֵ��
					if (CtrlFlag&0x04)
					{
						for (int k=0;k<IsoSurface_num+1;k++)
						{
							IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
							float ver[10]={0};
							Iso_num=MarchingFace(cell,IsoSurface_data,ver);
							for (int nu=0;nu<Iso_num;nu++)
							{	
								glBegin(GL_LINES);
								glColor4f(0.5,0.5,0.5,Alpha_Mixing);
								glVertex3f(0.01+cell_z,ver[4*nu],ver[4*nu+1]);
								glVertex3f(0.01+cell_z,ver[4*nu+2],ver[4*nu+3]);
								glVertex3f(cell_z-0.01,ver[4*nu],ver[4*nu+1]);
								glVertex3f(cell_z-0.01,ver[4*nu+2],ver[4*nu+3]);
								glEnd();
							}
						}
					}
				}
			}
		}
		//ǰ������
		dx=(x_right-x_left)/(size_datanum[0]-1);
		dy=(y_back-y_front)/(width_num-1);
		dz=(z_up-z_down)/(size_datanum[2]-1);
		for	(k=0;k<width_num;k+=width_num-1)
		{
			index=k*size_datanum[0]*size_datanum[2];
			for (int j=0;j<size_datanum[2]-1;j++)
			{
				for (int i=0;i<size_datanum[0]-1;i++)
				{
					glBegin(mode);
					index1=i+j*size_datanum[0]+index;
					x=x_left+dx*i;
					y=y_front+dy*k;
					z=z_down+dz*j;				
					data=DataMatrices_2D_y[index1];

					cell.Pt00.x=x;
					cell.Pt00.y=z;
					cell_z=y;
					cell.Pt00.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1++;
					x+=dx;
					data=DataMatrices_2D_y[index1];

					cell.Pt10.x=x;
					cell.Pt10.y=z;
					cell_z=y;
					cell.Pt10.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1+=size_datanum[0];
					z+=dz;
					data=DataMatrices_2D_y[index1];

					cell.Pt11.x=x;
					cell.Pt11.y=z;
					cell_z=y;
					cell.Pt11.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);

					index1--;
					x-=dx;				
					data=DataMatrices_2D_y[index1];

					cell.Pt01.x=x;
					cell.Pt01.y=z;
					cell_z=y;
					cell.Pt01.data=data;

					ChangeValue2Color(data,color);
					glColor4f(color[0],color[1],color[2],Alpha_Mixing);
					glVertex3f(x,y,z);
					glEnd();

					//���Ƶ�ֵ��
					if (CtrlFlag&0x04)
					{
						for (int k=0;k<IsoSurface_num+1;k++)
						{
							IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
							float ver[10]={0};
							Iso_num=MarchingFace(cell,IsoSurface_data,ver);
							for (int nu=0;nu<Iso_num;nu++)
							{	
								glBegin(GL_LINES);
								glColor4f(0.5,0.5,0.5,Alpha_Mixing);
								glVertex3f(ver[4*nu],0.01+cell_z,ver[4*nu+1]);
								glVertex3f(ver[4*nu+2],0.01+cell_z,ver[4*nu+3]);
								glVertex3f(ver[4*nu],cell_z-0.01,ver[4*nu+1]);
								glVertex3f(ver[4*nu+2],cell_z-0.01,ver[4*nu+3]);
								glEnd();
							}
						}
					}
				}
			}
		}
	}
}

void ClengnanView::Draw_MulFloor(float *datanum,		//������ɫֵ���ݱ�������
								 float *verdex_array,	//��Ӧ������������
								 int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����3ά
								 //int length_num,		//x���򴫸�������
								 //int width_num,			//y���򴫸�������
								 int height_num,		//z���򴫸�������
								 GLenum mode			//����ģʽ�����|�߿�
								 )
{
	//����,��
	int k,index;
	for (k=0;k<height_num;k++)
	{
		if (k==0)
		{
			if (!(MulFloorFlag&0x01))
			{
				continue;
			}			
		}else if (k==1)
		{
			if (!(MulFloorFlag&0x02))
			{
				continue;
			}			
		}else if (k==2)
		{
			if (!(MulFloorFlag&0x04))
			{
				continue;
			}			
		}else if (k==3)
		{
			if (!(MulFloorFlag&0x08))
			{
				continue;
			}			
		}else if (k==4)
		{
			if (!(MulFloorFlag&0x10))
			{
				continue;
			}			
		}else if (k==5)
		{
			if (!(MulFloorFlag&0x20))
			{
				continue;
			}			
		}else if (k==6)
		{
			if (!(MulFloorFlag&0x40))
			{
				continue;
			}			
		}
		else if (k==7)
		{
			if (!(MulFloorFlag&0x80))
			{
				continue;
			}			
		}else if (k==8)
		{
			if (!(MulFloorFlag&0x100))
			{
				continue;
			}			
		}else if (k==9)
		{
			if (!(MulFloorFlag&0x200))
			{
				continue;
			}			
		}
		index=k*size_datanum[0]*size_datanum[1];
		DrawFlatSurface(datanum+index,verdex_array+3*index,size_datanum,mode);

		//for (i=0;i<width_num-1;i++)
		//{
		//	for(j=0;j<length_num-1;j++)
		//	{
		//		glBegin(mode);
		//		index=(k*length_num*width_num+i*length_num+j);
		//		ChangeValue2Color(DataNumbers[index],color);
		//		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		//		index=index*3;
		//		glVertex3f(Vertex_Array[index],Vertex_Array[index+1],Vertex_Array[index+2]);
		//		index=(k*length_num*width_num+i*length_num+j+1);
		//		ChangeValue2Color(DataNumbers[index],color);
		//		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		//		index=index*3;
		//		glVertex3f(Vertex_Array[index],Vertex_Array[index+1],Vertex_Array[index+2]);
		//		index=(k*length_num*width_num+(i+1)*length_num+j+1);
		//		ChangeValue2Color(DataNumbers[index],color);
		//		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		//		index=index*3;
		//		glVertex3f(Vertex_Array[index],Vertex_Array[index+1],Vertex_Array[index+2]);
		//		index=(k*length_num*width_num+(i+1)*length_num+j);
		//		ChangeValue2Color(DataNumbers[index],color);
		//		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		//		index=index*3;
		//		glVertex3f(Vertex_Array[index],Vertex_Array[index+1],Vertex_Array[index+2]);
		//		glEnd();
		//	}
		//}
	}
}

//����NURBS��������
void ClengnanView::Draw_Bezier_SigleFloor(float *vertex_array,float *datanum)
{
	//GLfloat ver[][][];

	//GLUnurbsObj *nobj;//����NURBS����֮ǰ�����봴��һ��NURBS�����趨������
	//nobj = gluNewNurbsRenderer(); 
	////gluNurbsProperty(nobj , GLU_SAMPLING_TOLERANCE, 20);
	//gluNurbsProperty(nobj , GLU_DISPLAY_MODE, GLU_FILL);
	//gluBeginSurface(nobj);
	//gluNurbsSurface(nobj,20,sKnot,20,tKnot,15*3,3,&ver[0][0][0],5,5,GL_MAP2_VERTEX_3);
	//gluEndSurface(nobj);
}

//���Ƶ���
void ClengnanView::Draw_SigleFloor(float *datanum,		//������ɫֵ���ݱ�������
								   float *size_rect,	//ƽ����δ�С��2ά
								   int *size_datanum,	//�����ϵ����ݵ�����2ά
								   GLenum mode			//����ģʽ���߿�|��䣩														
								   )			
{
	FACECELL cell;
	float IsoSurface_data=0;
	int Iso_num=0;

	int i,j,index;
	float x,y,z,data;
	float dx,dy;
	float color[3]={0};
	float ma=mymax-mymin;
	ma*=2;
	dx=(size_rect[0])/(size_datanum[0]-1);
	dy=(size_rect[1])/(size_datanum[1]-1);
	for(j=0;j<size_datanum[1]-1;j++)
	{
		for(i=0;i<size_datanum[0]-1;i++)
		{
			glBegin(mode);
			x=i*dx;
			y=j*dy;
			index=j*size_datanum[0]+i;
			data=*(datanum+index);
			z=data/ma;
			//z=(z_down+z_up)/2;
			cell.Pt00.x=x;
			cell.Pt00.y=y;
			cell.Pt00.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			
			glVertex3f(x,y,z);

			x+=dx;
			index++;
			data=*(datanum+index);
			z=data/ma;
			cell.Pt10.x=x;
			cell.Pt10.y=y;
			cell.Pt10.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			
			glVertex3f(x,y,z);

			y+=dy;
			index+=size_datanum[0];
			data=*(datanum+index);
			z=data/ma;
			cell.Pt11.x=x;
			cell.Pt11.y=y;
			cell.Pt11.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			
			glVertex3f(x,y,z);

			x-=dx;
			index--;
			data=*(datanum+index);
			z=data/ma;
			cell.Pt01.x=x;
			cell.Pt01.y=y;
			cell.Pt01.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			
			glVertex3f(x,y,z);
			glEnd();

			//���Ƶ�ֵ��
			if (CtrlFlag&0x04)
			{
				for (int k=0;k<IsoSurface_num+1;k++)
				{
					IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
					float ver[10]={0};
					Iso_num=MarchingFace(cell,IsoSurface_data,ver);
					for (int nu=0;nu<Iso_num;nu++)
					{	
						glBegin(GL_LINES);
						glColor4f(0.5,0.5,0.5,Alpha_Mixing);
						glVertex3f(ver[4*nu],ver[4*nu+1],IsoSurface_data/ma);
						glVertex3f(ver[4*nu+2],ver[4*nu+3],IsoSurface_data/ma);
						glEnd();
					}
				}

			}
		}
	}
}

BOOL ClengnanView::ChangeValue2Color(/*float Max,float Min,*/float currvalue,GLfloat* color)
{
	if (mymax<mymin)
	{
		return FALSE;
	}
	float curr=currvalue-mymin;
	float range=mymax-mymin;
	if (SetStaff)
	{
		if (!value_colorflag)
		{
			if (currvalue>=mymax)
			{
				color[0]=1.0f;
				color[1]=0.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (currvalue<=mymin)
			{
				color[0]=0.0f;
				color[1]=0.0f;
				color[2]=1.0f;
				return TRUE;
			}
			if (curr>range*9/10)
			{
				color[0]=1.0f;
				color[1]=0.3f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*9/10&&curr>range*4/5)
			{
				color[0]=1.0f;
				color[1]=0.6f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*4/5&&curr>range*7/10)
			{
				color[0]=1.0f;
				color[1]=0.9f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*7/10&&curr>range*3/5)
			{
				color[0]=0.8f;
				color[1]=1.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*3/5&&curr>range*1/2)
			{
				color[0]=0.4f;
				color[1]=1.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*1/2&&curr>range*2/5)
			{
				color[0]=0.0f;
				color[1]=1.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*2/5&&curr>range*3/10)
			{
				color[0]=0.0f;
				color[1]=1.0f;
				color[2]=0.5f;
				return TRUE;
			}
			if (curr<=range*3/10&&curr>range*1/5)
			{
				color[0]=0.0f;
				color[1]=1.0f;
				color[2]=1.0f;
				return TRUE;
			}
			if (curr<=range*1/5&&curr>range*1/10)
			{
				color[0]=0.0f;
				color[1]=0.5f;
				color[2]=1.0f;
				return TRUE;
			}
		}else{//�ǽ���ģʽ
			if (currvalue>=mymax)
			{
				color[0]=1.0f;
				color[1]=0.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (currvalue<=mymin)
			{
				color[0]=0.0f;
				color[1]=0.0f;
				color[2]=1.0f;
				return TRUE;
			}
			if (curr>range*3/4)
			{
				color[0]=1.0f;
				color[1]=(range-curr)*4/range;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range*3/4&&curr>range/2)
			{
				color[0]=((curr-range/2)*4/range);
				color[1]=1.0f;
				color[2]=0.0f;
				return TRUE;
			}
			if (curr<=range/2&&curr>range/4)
			{
				color[0]=0.0f;
				color[1]=1.0f;
				color[2]=((range/2-curr)*4/range);
				return TRUE;
			}
			if (curr<=range/4)
			{
				color[0]=0.0f;
				color[1]=(curr*4/range);
				color[2]=1.0f;
				return TRUE;
			}
		}
	}else
		{
			if (!value_colorflag)
			{
				if (currvalue>=mymax)
				{
					color[0]=1.0f;
					color[1]=0.0f;
					color[2]=0.0f;
					return TRUE;
				}
				if (currvalue<=mymin)
				{
					color[0]=0.0f;
					color[1]=0.0f;
					color[2]=1.0f;
					return TRUE;
				}
				if (curr>range*9/10)
				{
					color[0]=1.0f;
					color[1]=0.3f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*9/10&&curr>range*4/5)
				{
					color[0]=1.0f;
					color[1]=0.6f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*4/5&&curr>range*7/10)
				{
					color[0]=1.0f;
					color[1]=0.9f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*7/10&&curr>range*3/5)
				{
					color[0]=0.8f;
					color[1]=1.0f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*3/5&&curr>range*1/2)
				{
					color[0]=0.4f;
					color[1]=1.0f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*1/2&&curr>range*2/5)
				{
					color[0]=0.0f;
					color[1]=1.0f;
					color[2]=0.0f;
					return TRUE;
				}
				if (curr<=range*2/5&&curr>range*3/10)
				{
					color[0]=0.0f;
					color[1]=1.0f;
					color[2]=0.5f;
					return TRUE;
				}
				if (curr<=range*3/10&&curr>range*1/5)
				{
					color[0]=0.0f;
					color[1]=1.0f;
					color[2]=1.0f;
					return TRUE;
				}
				if (curr<=range*1/5&&curr>range*1/10)
				{
					color[0]=0.0f;
					color[1]=0.5f;
					color[2]=1.0f;
					return TRUE;
				}
			}else{//�ǽ���ģʽ
				if (currvalue>=mymax)
				{
					color[0]=1.0f;
					color[1]=0.0f;
					color[2]=0.0f;
					
					return TRUE;
				}
				if (currvalue<=mymin)
				{
					color[0]=0.0f;
 					color[1]=0.0f;
 					color[2]=1.0f;
					return TRUE;
				}
				if (currvalue>=temvalue4)
				{
					color[0]=1.0f;
					color[1]=(range-curr)*4/range;
					color[2]=0.0f;
					return TRUE;
				}
				if (currvalue>temvalue3&&currvalue<temvalue4)
				{
					color[0]=((curr-range/2)*4/range);
					color[1]=1.0f;
					color[2]=0.0f;
					return TRUE;
				}
				if (currvalue<=temvalue3&&currvalue>temvalue2)
				{
					color[0]=0.0f;
					color[1]=1.0f;
					color[2]=((range/2-curr)*4/range);
					return TRUE;
				}
				if (currvalue<=temvalue2&&currvalue>temvalue1)
				{
					color[0]=0.0f;
					color[1]=(curr*4/range);
					color[2]=1.0f;
					return TRUE;
				}
				
			}
	}
	
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
//��ֵ�㷨 Interpolation algorithm 
//����Ȩ�ط� Distance weighted interpolation algorithm
//Vertex_Array[]Ϊ��֪������λ������
//datanumbers[3]Ϊ���������λ��
//datanumbers[]Ϊ��֪��������Ϣ
//nΪ���뷴�ȵ��ݴΣ�Խ���������Ȩ��Խ��Զ�����Ȩ��ԽС
//����ֵΪ�����������Ϣ
//���� 2013\01\23
//////////////////////////////////////////////////////////////////////////
float ClengnanView::DW_IA(float *Vertex_Array,int Vertex_num,float *datanumbers,float *Vertex,int n)
{
	float data=0.0f;
//	float *range=(float*)malloc(sizeof(float));
	memset(range,0,sizeof(range));
	//malloc(sizeof(float)*Vertex_num);
	float sum=0.0f;
	for (int i=0;i<Vertex_num;i++)
	{
		range[i]=(Vertex_Array[3*i]-Vertex[0])*(Vertex_Array[3*i]-Vertex[0])+(Vertex_Array[3*i+1]-Vertex[1])*(Vertex_Array[3*i+1]-Vertex[1])+(Vertex_Array[3*i+2]-Vertex[2])*(Vertex_Array[3*i+2]-Vertex[2]);
		if (range[i]==0.0)
		{
			//free(range);
			data=datanumbers[i];
			return data;
		}
		range[i]=sqrtf(range[i]);
		range[i]=1/range[i];
		range[i]=powf(range[i],n);
		sum+=range[i];
	}
	for(int i=0;i<Vertex_num;i++)
	{
		data+=(range[i]/sum)*datanumbers[i];
	}
//	free(range);
	return data;
}
// �����
void ClengnanView::DrawStaff(float dmax, float dmin)
{
	//CGLFont *glFont;
	HFONT hFont;
	//glFont=new CGLFont();
	hFont  =CreateFont(-16,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("����"));
	float color[3]={0};
	float currvalue[21]={0};
	CString tempstr;
	float fa=(float)2/IsoSurface_num;
	for (int i=0;i<21;i++)
	{
		currvalue[i]=(float)(dmin+i*(float)((dmax-dmin)/20));
	}
	float coefficientx=(float)nWidth/(float)1366;
	float coefficienty=(float)nHeight/(float)768;


	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	glTranslatef(-4.4*coefficientx+0.15,0.2f*coefficienty-0.05,-6.0f);
	//glTranslatef(m_xPos, m_yPos, Visual_depth);
	//glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(24, 0.0f,1.0f,0.0f);
	//glScalef(m_x,m_y,m_z);
	GLuint base = glGenLists(96); // ����96����ʾ�б�
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 
	for (int i=0;i<11;i++)
	{
		tempstr.Format(_T("%.1f"),currvalue[2*i]);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		int iSize;
		char* pszMultiByte;
		//pszMultiByte=unicode_to_ansi(pszMultiByte,tempstr);
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		//pszMultiByte = (char*)malloc((iSize+1));
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		ChangeValue2Color(currvalue[2*i],color);
		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		glRasterPos3f(0.0f,i*0.2f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;
		//free(pszMultiByte);
		//glFont->drawtext3d(0.0f,i*0.2f,0.0f,tempstr,hFont,color[0],color[1],color[2],0.2f);
	}
	if (m_bModleInfo)
	{
		glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
		//дģ����Ϣ
		//////////////////////////////////////////////////////////////////////////
		//Modle Info��
		//Length			XXX
		//Width				XXX
		//Height			XXX
		//Sensors Number	XXX
		//IMP Number		XXX
		//////////////////////////////////////////////////////////////////////////
		glTranslatef(3.0*coefficientx,1.5*coefficienty,-6.0f);
		glRotatef(24, 0.0f,1.0f,0.0f);
		glRasterPos3f(0.0f,0.0f,0.0f); 
		glColor3f(0.0f,1.0f,0.0f);
		tempstr=_T("Modle Info:");
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		int iSize;
		char* pszMultiByte;
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		////////////////////////////////////////////////////////////////////////// 
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;
 
		tempstr.Format(_T("Length                    %.2f"),Model_Length);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		glRasterPos3f(0.0f,-0.2f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;

		tempstr.Format(_T("Width                     %.2f"),Model_Width);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		glRasterPos3f(0.0f,-0.4f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;

		tempstr.Format(_T("Height                    %.2f"),Model_Height);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		glRasterPos3f(0.0f,-0.6f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;

		tempstr.Format(_T("Sensors Number   %d"),sensors_num);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		glRasterPos3f(0.0f,-0.8f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;

		tempstr.Format(_T("IMP Number          %d"),IMP_Num);
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��
		iSize = WideCharToMultiByte(CP_ACP, 0, tempstr, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
		WideCharToMultiByte(CP_ACP, 0, tempstr, -1, pszMultiByte, iSize, NULL, NULL);
		//////////////////////////////////////////////////////////////////////////
		glRasterPos3f(0.0f,-1.0f,0.0f);  
		glCallLists(iSize-1,GL_UNSIGNED_BYTE,pszMultiByte);
		delete [] pszMultiByte;
	}

	glDeleteLists(base, 96);
	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	glTranslatef(-4.4*coefficientx,0.2*coefficienty,-6.0f);
	//glTranslatef(-3,-3,-6.0);
	glBegin(GL_LINES);
	//���Ƶ�ֵ��
	for(int i=0;i<11;i++)
	{
		ChangeValue2Color(currvalue[2*i],color);
		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		glVertex3f(0.1f,i*0.2f,0.0f);
		glVertex3f(0.15f,i*0.2f,0.0f);
	}
	if ((CtrlFlag&0x04)!=0)
	{
		for(int i=0;i<IsoSurface_num+1;i++)
		{	
			glColor4f(0.5f,0.5f,0.5f,Alpha_Mixing);
			glVertex3f(0.0f,(float)i*fa,0.0f);
			glVertex3f(0.1f,(float)i*fa,0.0f);
		}		
	}
	glEnd();
	if (!value_colorflag)
	{
		glShadeModel(GL_FLAT);
	}	
	glBegin(GL_QUADS);
	for (int i=0;i<20;i++)
	{
		ChangeValue2Color(currvalue[i],color);
		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		glVertex3f(0.0f,i*0.1f,0.0f);
		glVertex3f(0.1f,i*0.1f,0.0f);
		ChangeValue2Color(currvalue[i+1],color);
		glColor4f(color[0],color[1],color[2],Alpha_Mixing);
		glVertex3f(0.1f,(i+1)*0.1f,0.0f);
		glVertex3f(0.0f,(i+1)*0.1f,0.0f);
	}
	glEnd();
	glShadeModel(GL_SMOOTH);
	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	DeleteObject(hFont);
	//delete glFont;
}
afx_msg LRESULT ClengnanView::OnMyMessage(WPARAM wParam,LPARAM lParam)
{
	int ncount[3]={nSlider,nSlider,nSlider};
	int mcount[2]={nSlider,nSlider};
	int num_cube[3]={(Length_Num-1)*(nSlider+1)+1,(Width_Num-1)*(nSlider+1)+1,(Height_Num-1)*(nSlider+1)+1};
	float *x=new float[/*Length_Num*/30];
	float *y=new float[/*Width_Num*/30];
	float *z=new float[/*Height_Num*/30];
	float *d=new float[/*Length_Num*Width_Num*/900];
	CUBECELL cubecell;
	float zz=0,dx,dy,dz;
	int index=0;
	int nvolume=0;
	float statistics=0;
	CString str;
	CMainFrame *pMain=(CMainFrame *)AfxGetApp()->m_pMainWnd;
	//��һ��
	for (int i=0;i<(int)Length_Num;i++)
	{
		x[i]=Sensors_Array[3*i];
	}
	for (int i=0;i<(int)Width_Num;i++)
	{
		y[i]=Sensors_Array[3*Length_Num*i+1];
	}
	for (int i=0;i<(int)Height_Num;i++)
	{
		z[i]=Sensors_Array[3*Width_Num*Length_Num*i+2];
	}
	switch(wParam)
	{
	case 0:			//��ʾ�汾���ط�ģʽ
		
		break;
	case 1:			//ʵʱģʽ
		MessageBox(_T("::"));
		break;
	case 2:			//����(��ʷ��¼)ģʽ	˫���������ڲ�
		//˫�����ڲ�
		//BI_IA(Sensors_Array,DataNumbers,mcount,DataMatrices_2D);
		//ˮƽ����ڲ�
		for (int k=0;k<(int)Height_Num;k++)
		{
			for (int i=0;i<(int)Length_Num*(int)Width_Num;i++)
			{
				d[i]=DataNumbers[i+k*Length_Num*Width_Num];
			}
			int size_floor=((nSlider+1)*(Length_Num-1)+1)*((nSlider+1)*(Width_Num-1)+1);
			//˫����������ֵ
			DCSIA(nSlider,nSlider,Length_Num-1,Width_Num-1,x,y,d,DataMatrices_2D+k*size_floor);
		}
		//��������ڲ�
		for (int k=0;k<(int)Length_Num;k++)
		{
			for (int i=0;i<(int)Height_Num*(int)Width_Num;i++)
			{
				d[i]=DataNumbers[i*Length_Num+k];
			}
			int size_floor=((nSlider+1)*(Height_Num-1)+1)*((nSlider+1)*(Width_Num-1)+1);
			//˫����������ֵ
			DCSIA(nSlider,nSlider,Width_Num-1,Height_Num-1,y,z,d,DataMatrices_2D_x+k*size_floor);
		}
		//ǰ������ڲ�
		for (int k=0;k<(int)Width_Num;k++)
		{
			for (int i=0;i<(int)Height_Num;i++)
			{
				for	(int j=0;j<(int)Length_Num;j++)
				{
					d[j+i*Length_Num]=DataNumbers[i*Length_Num*Width_Num+j+k*Length_Num];
				}
			}
			int size_floor=((nSlider+1)*(Height_Num-1)+1)*((nSlider+1)*(Length_Num-1)+1);
			//˫����������ֵ
			DCSIA(nSlider,nSlider,Length_Num-1,Height_Num-1,x,z,d,DataMatrices_2D_y+k*size_floor);
		}
		//�ü�����ڲ壨x��
		if (x_cutout>x_left	&&	x_cutout<x_right)
		{
			int size_floor=((nSlider+1)*(Width_Num-1)+1)*((nSlider+1)*(Height_Num-1)+1);
			for (int i=0;i<size_floor;i++)
			{
				for (int k=0;k<(int)Length_Num;k++)
				{
					d[k]=DataMatrices_2D_x[i+k*size_floor];
				}
				DataMatrices_2D_xx[i]=CSIA_EX(Length_Num-1,x,d,x_cutout);
			}
		}
		//�ü�����ڲ壨y��
		if (y_cutout>y_front&&	y_cutout<y_back)
		{
			int size_floor=((nSlider+1)*(Length_Num-1)+1)*((nSlider+1)*(Height_Num-1)+1);
			for (int i=0;i<size_floor;i++)
			{
				for (int k=0;k<(int)Width_Num;k++)
				{
					d[k]=DataMatrices_2D_y[i+k*size_floor];
				}
				DataMatrices_2D_yy[i]=CSIA_EX(Width_Num-1,y,d,y_cutout);
			}
		}
		//�ü�����ڲ壨z��
		if (z_cutout>z_down	&&	z_cutout<z_up)
		{
			int size_floor=((nSlider+1)*(Length_Num-1)+1)*((nSlider+1)*(Width_Num-1)+1);
			for (int i=0;i<size_floor;i++)
			{
				for (int k=0;k<(int)Height_Num;k++)
				{
					d[k]=DataMatrices_2D[i+k*size_floor];
				}
				DataMatrices_2D_zz[i]=CSIA_EX(Height_Num-1,z,d,z_cutout);
			}			
		} 
		//m_wndExtendBar.m_MySlider.SetPos(m_PlayPos);//���ò�����λ��	
		break;
		case 5://���Զ���ʷ�ط��лطŵ�ͷ�Զ�ֹͣ
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_GRAYED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_ENABLED);
			break;
	default:
		break;
	}
	//��Ⱦ����,ǿ���ػ棬����WM_PAINT
	SendMessage(WM_PAINT);
	delete[] x;
	delete[] y;
	delete[] z;
	delete[] d;
	return 0;
}
void ClengnanView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(nIDEvent)
	{
	case 1:
		break;
	case 2:
		break;
	case 5:
		PostMessageW(WM_MY_MESSAGE,2,NULL);
		break;
	default:
		break;
	}
	CView::OnTimer(nIDEvent);
}

void ClengnanView::DrawPoint(float radius,bool flag)
{
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	char *pszMultiByte=new char[30];
	int iSize;
	HFONT hFont;
	float color[3]={0};
	float ma=mymax-mymin;
	ma*=2;
	hFont  =CreateFont(36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("����"));
	GLuint base = glGenLists(96); // ����96����ʾ�б�
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 

	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����

	glTranslatef(m_xPos, m_yPos, Visual_depth);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glScalef(m_x,m_y,m_z);
	//��С100������mΪ��λ
	glTranslatef(-Model_Length/2,-Model_Width/2,-Model_Height/2);
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	if (n3showmode)
	{
		for(int i=0;i<sensors_num;i++)
		{
			if (sensornumber[i]==-1)//û�д������ĵ�
			{
				continue;
			}
			glTranslatef(Sensors_Array[3*i],Sensors_Array[3*i+1],Sensors_Array[3*i+2]);//�ƶ�ԭ��
			//ChangeValue2Color(DataNumbers[i],color);
			//glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glColor3f(1.0f,1.0f,1.0f/*,Alpha_Mixing*/);
			glRasterPos3f(0.0f,0.05f,0.05f); 
			if (flag)
			{
				if (BelongTo(sensornumber[i],nPoints,nPointNum))		//�������
				{			
					sprintf_s(pszMultiByte,30,"break");
					glColor4f(1.0f,0.0f,0.0f,Alpha_Mixing);
				}else{
					sprintf_s(pszMultiByte,30,"%.2f",DataNumbers[i]);
					glColor4f(0.0f,1.0f,1.0f,Alpha_Mixing);
				}
			}else
			{
				  sprintf_s(pszMultiByte,20,"%d",sensornumber[i]);
				  glColor4f(0.0f,1.0f,1.0f,Alpha_Mixing);
			}
			if ((CtrlFlag&0x02)!=0)
			{	
				if (sensornumber[i]>0)
				{
					iSize=sizeof(pszMultiByte);
					glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
				}
			}
			glutSolidSphere((x_right-x_left)/100,9,9);
			glTranslatef(-Sensors_Array[3*i],-Sensors_Array[3*i+1],-Sensors_Array[3*i+2]);//ȡ���ƶ�
		}
	}else{
		for(int i=nradio*Length_Num*Width_Num;i<(nradio+1)*Length_Num*Width_Num;i++)
		{	
			if (sensornumber[i]==-1)
			{
				continue;
			}
			glTranslatef(Sensors_Array[3*i],Sensors_Array[3*i+1],DataNumbers[i]/ma);//�ƶ�ԭ��
			//ChangeValue2Color(DataNumbers[i],color);
			//glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glColor3f(0.9f,0.9f,0.9f/*,Alpha_Mixing*/);
			glRasterPos3f(0.0f,0.05f,0.05f); 
			if (flag)
			{
				if (BelongTo(sensornumber[i],nPoints,nPointNum))		//�������
				{			
					sprintf_s(pszMultiByte,30,"break");
					glColor4f(1.0f,0.0f,0.0f,Alpha_Mixing);
				}else{
					sprintf_s(pszMultiByte,30,"%.2f",DataNumbers[i]);
					glColor4f(0.0f,1.0f,1.0f,Alpha_Mixing);
				}
			}else{
				sprintf_s(pszMultiByte,20,"%d",sensornumber[i]);
				glColor4f(0.0f,1.0f,1.0f,Alpha_Mixing);
			}
			if ((CtrlFlag&0x02)!=0)
			{	
				iSize=sizeof(pszMultiByte);
				glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
			}
			glutSolidSphere((x_right-x_left)/100,9,9);
			glTranslatef(-Sensors_Array[3*i],-Sensors_Array[3*i+1],-DataNumbers[i]/ma);//ȡ���ƶ�
		}
	}
	delete [] pszMultiByte;
	glDeleteLists(base, 96);
	DeleteObject(hFont);
}

BOOL ClengnanView::TLI_IA(float *Vertex_Array,float *src_data,int *ncount,float *desk_data)
{
	int i,j,k;
	int ii,jj,kk;
	//��Ԫ�ķָ�Ĳ���
	float Length_Cell,Width_Cell,Height_Cell;
	POINT3D pt;
	float Vertex_Cell[24]={0};
	float Data_Cell[8]={0};
	int index=0;
	int num=0;
	for (k=0;k<(int)Height_Num-1;k++)
	{
		for (j=0;j<(int)Width_Num-1;j++)
		{
			for (i=0;i<(int)Length_Num-1;i++)
			{
				index=k*Length_Num*Width_Num+j*Length_Num+i;
				Length_Cell=(Vertex_Array[3*index+3]-Vertex_Array[3*index])/(ncount[0]+1);
				Width_Cell=(Vertex_Array[3*index+3*Length_Num+1]-Vertex_Array[3*index+1])/(ncount[1]+1);
				Height_Cell=(Vertex_Array[3*index+3*Width_Num*Length_Num+2]-Vertex_Array[3*index+2])/(ncount[2]+1);

				//��Ԫ������������
				Data_Cell[0]=src_data[index];
				Data_Cell[1]=src_data[index+1];
				Data_Cell[2]=src_data[index+Length_Num];
				Data_Cell[3]=src_data[index+Length_Num+1];
				Data_Cell[4]=src_data[index+Width_Num*Length_Num];
				Data_Cell[5]=src_data[index+Width_Num*Length_Num+1];
				Data_Cell[6]=src_data[index+Width_Num*Length_Num+Length_Num];
				Data_Cell[7]=src_data[index+Width_Num*Length_Num+Length_Num+1];
				//��Ԫ������������
				Vertex_Cell[0]=Vertex_Array[3*index];
				Vertex_Cell[1]=Vertex_Array[3*index+1];
				Vertex_Cell[2]=Vertex_Array[3*index+2];
				Vertex_Cell[3]=Vertex_Array[3*index+3];
				Vertex_Cell[4]=Vertex_Array[3*index+4];
				Vertex_Cell[5]=Vertex_Array[3*index+5];
				Vertex_Cell[6]=Vertex_Array[3*index+3*Length_Num];
				Vertex_Cell[7]=Vertex_Array[3*index+3*Length_Num+1];
				Vertex_Cell[8]=Vertex_Array[3*index+3*Length_Num+2];
				Vertex_Cell[9]=Vertex_Array[3*index+3*Length_Num+3];
				Vertex_Cell[10]=Vertex_Array[3*index+3*Length_Num+4];
				Vertex_Cell[11]=Vertex_Array[3*index+3*Length_Num+5];
				Vertex_Cell[12]=Vertex_Array[3*index+3*Width_Num*Length_Num];
				Vertex_Cell[13]=Vertex_Array[3*index+3*Width_Num*Length_Num+1];
				Vertex_Cell[14]=Vertex_Array[3*index+3*Width_Num*Length_Num+2];
				Vertex_Cell[15]=Vertex_Array[3*index+3*Width_Num*Length_Num+3];
				Vertex_Cell[16]=Vertex_Array[3*index+3*Width_Num*Length_Num+4];
				Vertex_Cell[17]=Vertex_Array[3*index+3*Width_Num*Length_Num+5];
				Vertex_Cell[18]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num];
				Vertex_Cell[19]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num+1];
				Vertex_Cell[20]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num+2];
				Vertex_Cell[21]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num+3];
				Vertex_Cell[22]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num+4];
				Vertex_Cell[23]=Vertex_Array[3*index+3*Width_Num*Length_Num+3*Length_Num+5];
				
				//������Ԫ�ڵ��ֵ
				for (kk=0;kk<ncount[2]+2;kk++)
				{
					for (jj=0;jj<ncount[1]+2;jj++)
					{
						for (ii=0;ii<ncount[0]+2;ii++)
						{
							pt.x=Vertex_Cell[0]+ii*Length_Cell;
							pt.y=Vertex_Cell[1]+jj*Width_Cell;
							pt.z=Vertex_Cell[2]+kk*Height_Cell;
							pt.data=TLIA(Vertex_Cell,Data_Cell,pt.x,pt.y,pt.z);
							desk_data[num]=pt.data;
							num++;
						}
					}
				}
			}
		}
	}
	return TRUE;
}

BOOL ClengnanView::BI_IA(float *Vertex_Array,float *src_data,int *ncount,float *desk_data)
{
	int i,j,k;
	int ii,jj;
	int index=0;
	//��Ԫ�ķָ�Ĳ���
	float Length_Cell,Width_Cell;
	POINT3D pt;
	float Vertex_Cell[8]={0};
	float Data_Cell[4]={0};
	int num=0;
	//ÿ��ĵ���
	int num_x=(Length_Num-1)*(ncount[0]+1)+1;
	int num_y=(Width_Num-1)*(ncount[1]+1)+1;
	for (k=0;k<(int)Height_Num;k++)
	{
		for (j=0;j<(int)Width_Num-1;j++)
		{
			for (i=0;i<(int)Length_Num-1;i++)
			{
				index=k*Length_Num*Width_Num+j*Length_Num+i;
				Length_Cell=(Vertex_Array[3*index+3]-Vertex_Array[3*index])/(ncount[0]+1);
				Width_Cell=(Vertex_Array[3*index+3*Length_Num+1]-Vertex_Array[3*index+1])/(ncount[1]+1);
				//��Ԫ�����������ݡ�λ��(2ά��)
				Data_Cell[0]=src_data[index];
				Data_Cell[1]=src_data[index+1];
				Data_Cell[2]=src_data[index+Length_Num];
				Data_Cell[3]=src_data[index+Length_Num+1];

				Vertex_Cell[0]=Vertex_Array[3*index];
				Vertex_Cell[1]=Vertex_Array[3*index+1];

				Vertex_Cell[2]=Vertex_Array[3*index+3];
				Vertex_Cell[3]=Vertex_Array[3*index+4];

				Vertex_Cell[4]=Vertex_Array[3*index+3*Length_Num];
				Vertex_Cell[5]=Vertex_Array[3*index+3*Length_Num+1];

				Vertex_Cell[6]=Vertex_Array[3*index+3*Length_Num+3];
				Vertex_Cell[7]=Vertex_Array[3*index+3*Length_Num+4];

				for (jj=0;jj<ncount[1]+2;jj++)
				{
					for (ii=0;ii<ncount[0]+2;ii++)
					{
						pt.x=Vertex_Cell[0]+ii*Length_Cell;
						pt.y=Vertex_Cell[1]+jj*Width_Cell;
						pt.data=BLIA(Vertex_Cell,Data_Cell,pt.x,pt.y);
						num=k*num_x*num_y+j*num_x*(ncount[1]+1)+jj*num_x+i*(ncount[0]+1)+ii;
						DataMatrices_2D[num]=pt.data;
						//num++;
					}
				}
			}
		}
	}
	return TRUE;
}

//��С����ϵ
void ClengnanView::Draw_CoordinateSystem()
{
	HFONT hFont;
	float coefficientx=(float)nWidth/(float)1366;
	float coefficienty=(float)nHeight/(float)768;
	coefficientx=(coefficientx+1)/2;
	coefficienty=(coefficienty+1)/2;
	float coefficient=coefficientx>coefficienty?coefficientx:coefficienty;
	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("����"));
	GLuint base = glGenLists(96); // ����96����ʾ�б�
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 
	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	glTranslatef(5.3/coefficient, -3.0/coefficient, -10.0f/*Visual_depth*/);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glBegin(GL_LINES);
	glColor3f(1.0f,0.0f,1.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(1.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,1.0f,0.0f);
	glVertex3f(0.0f,0.0f,0.0f);
	glVertex3f(0.0f,0.0f,1.0f);
	glEnd();
	glColor3f(0.0f,1.0f,0.0f);
	glTranslatef(1.1f,0.0f,0.0f);//�ƶ�ԭ��
	glRasterPos3f(0.0f,0.0f,0.0f); 
	glCallLists(2,GL_UNSIGNED_BYTE,"x");
	glTranslatef(-1.1f,0.0f,0.0f);//ȡ���ƶ�
	glTranslatef(0.0f,1.1f,0.0f);//�ƶ�ԭ��
	glRasterPos3f(0.0f,0.0f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,"y");
	glTranslatef(0.0f,-1.1f,0.0f);//ȡ���ƶ�
	glTranslatef(0.0f,0.0f,1.1f);//�ƶ�ԭ��
	glRasterPos3f(0.0f,0.0f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,"z");
	glTranslatef(0.0f,0.0f,-1.1f);//ȡ���ƶ�
	glDeleteLists(base, 96);
	//z��
	glColor3f(1.0f,0.0f,1.0f);
	//glRotatef(-90,0.0f,0.0f,1.0f);
	glTranslatef(0.0f,0.0f, 0.7f);
	glutSolidCone(0.05,0.3f,20.0,1);
	//x��
	glTranslatef(0.7f,0.0f, -0.7f);
	glRotatef(90,0.0f,1.0f,0.0f);
	glutSolidCone(0.05,0.3f,20.0,1);
	//y��
	glTranslatef(0.0f,0.7f, -0.7f);
	glRotatef(-90,1.0f,0.0f,0.0f);
	glutSolidCone(0.05,0.3f,20.0,1);
	DeleteObject(hFont);
}
//˫���������ת���ƶ�״̬����ԭ
void ClengnanView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}

	resflag=true;
	float tempf_x,tempf_y,tempf_xp,tempf_yp;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	tempf_xp=m_xPos;
	tempf_yp=m_yPos;
	for(int i=0;i<5;i++){
		m_xPos-=tempf_xp/5;
		m_yPos-=tempf_yp/5;
		m_xAngle-=tempf_x/5;
		m_yAngle-=tempf_y/5;
		SendMessage(WM_PAINT);
	}
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}	
	CView::OnLButtonDblClk(nFlags, point);
}

//����ֵ��
void ClengnanView::Draw_Isogram(float *Vertex_Array,float *datanum,int Isogram_num)
{
	//һ����һ�������
}

// ������߼���
Vector3D ClengnanView::GetSelectionRay(int mouse_x, int mouse_y)
{
	GLdouble    modelview[16];//Ϊ�Ӿ���
	GLdouble    projection[16];//ΪͶӰ����
	GLint       viewport[4];//Ϊ�ӿ�
	CString str;
	unsigned char roflag=0x00;

	glGetDoublev (GL_MODELVIEW_MATRIX, modelview);
	glGetDoublev (GL_PROJECTION_MATRIX, projection); 
	glGetIntegerv (GL_VIEWPORT, viewport);
	float mouse_z = 0;	
	glReadPixels((int)mouse_x, (int)mouse_y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &mouse_z); 
	gluUnProject(mouse_x,mouse_y,mouse_z,modelview,projection,viewport,&World_x[1],&World_y[1],&World_z[1]);	
	str.Format(_T("%.2lf,%.2lf,%.2lf"),World_x[1],World_y[1],World_z[1]);
	//AfxMessageBox(str);
	gluUnProject(mouse_x,mouse_y,0.0,modelview,projection,viewport,&World_x[0],&World_y[0],&World_z[0]);
	str.Format(_T("%.2lf,%.2lf,%.2lf"),World_x[0],World_y[0],World_z[0]);
	//AfxMessageBox(str);
	//���ߵķ���(0<=t<=1)������������ϵ��
	//t_x=(World_x[1]-World_x[0])*t;
	//t_y=(World_y[1]-World_y[0])*t;
	//t_z=(World_z[1]-World_z[0])*t;
	//camera����ϵ��ģ������ϵ�ı任����
	Vector4D V0,V1;
	V0=MakePoint(World_x[0],World_y[0],World_z[0],1);
	V1=MakePoint(World_x[1],World_y[1],World_z[1],1);
	////ת��Ϊģ������ϵ�ϵ�����
	////����������任
	Pt3d0=MakePoint(V0.x,V0.y,V0.z);
	Pt3d1=MakePoint(V1.x,V1.y,V1.z);
	//ƽ��
	Pt3d0=TranslationPoint(Pt3d0,m_xPos, m_yPos, Visual_depth);
	Pt3d1=TranslationPoint(Pt3d1,m_xPos, m_yPos, Visual_depth);
	//��ת	
	Pt3d0=Rotate_X(Pt3d0,-m_xAngle);
	Pt3d1=Rotate_X(Pt3d1,-m_xAngle);
	Pt3d0=Rotate_Y(Pt3d0,-m_yAngle);
	Pt3d1=Rotate_Y(Pt3d1,-m_yAngle);
	while (m_yAngle<0)
	{
		m_yAngle+=360;
	}
	while(m_yAngle>360)
	{
		m_yAngle-=360;
	}
	while (m_xAngle<0)
	{
		m_xAngle+=360;
	}
	while(m_xAngle>360)
	{
		m_xAngle-=360;
	}

	if (m_yAngle<90)
	{
		roflag|=0x01;
	}else if(m_yAngle<180){
		roflag|=0x02;
	}else if(m_yAngle<270){
		roflag|=0x04;
	}else{
		roflag|=0x08;
	}

	if (m_xAngle<90)
	{
		roflag|=0x10;
	}else if(m_xAngle<180){
		roflag|=0x20;
	}else if(m_xAngle<270){
		roflag|=0x40;
	}else{
		roflag|=0x80;
	}
	switch(roflag)
	{
	case 0x11:
		mouse_locationflag=2;//�����
		break;
	case 0x12:
		mouse_locationflag=6;//�����
		break;
	case 0x14:
		mouse_locationflag=7;//�Һ���
		break;
	case 0x18:
		mouse_locationflag=3;//�Һ���
		break;
	case 0x21:
		mouse_locationflag=7;//�Һ���
		break;
	case 0x22:
		mouse_locationflag=3;//�Һ���
		break;
	case 0x24:
		mouse_locationflag=2;//�����
		break;
	case 0x28:
		mouse_locationflag=6;//�����
		break;
	case 0x41:
		mouse_locationflag=8;//ǰ����
		break;
	case 0x42:
		mouse_locationflag=4;//ǰ����
		break;
	case 0x44:
		mouse_locationflag=1;//ǰ����
		break;
	case 0x48:
		mouse_locationflag=5;//ǰ����
		break;
	case 0x81:
		mouse_locationflag=1;//��ǰ��
		break;
	case 0x82:
		mouse_locationflag=5;//��ǰ��
		break;
	case 0x84:
		mouse_locationflag=8;//��ǰ��
		break;
	case 0x88:
		mouse_locationflag=4;//��ǰ��
		break;
	}
	show_coordinateflag=false;
	if (mouse_locationflag<5)
	{
		//�����潻�㣨z=z_up��
		Pt3d.z=z_up-(z_up+z_down)/2;
		Pt3d.x=(Pt3d.z-Pt3d0.z)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.z-Pt3d0.z)+Pt3d0.x;
		Pt3d.y=(Pt3d.z-Pt3d0.z)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.z-Pt3d0.z)+Pt3d0.y;
		Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
		if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.y<=y_back&&Pt3d.y>=y_front)
		{
			show_coordinateflag=true;
		}else if (mouse_locationflag==1)
		{
			if (Pt3d.x<x_left)
			{	
				//�����潻�㣨x=x_left��
				Pt3d.x=x_left-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y<y_front)
			{	
				//��ǰ�潻�㣨y=y_front��
				Pt3d.y=y_front-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}else if (mouse_locationflag==2)
		{
			if (Pt3d.x<x_left)
			{	
				//�����潻�㣨x=x_left��
				Pt3d.x=x_left-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y>y_back)
			{	
				//����潻�㣨y=y_back��
				Pt3d.y=y_back-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
		else if (mouse_locationflag==3)
		{
			if (Pt3d.x>x_right)
			{	
				//�����潻�㣨x>x_right��
				Pt3d.x=x_right-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y>y_back)
			{	
				//����潻�㣨y=y_back��
				Pt3d.y=y_back-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
		else if (mouse_locationflag==4)
		{
			if (Pt3d.x>x_right)
			{	
				//�����潻�㣨x>x_right��
				Pt3d.x=x_right-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y<y_front)
			{	
				//��ǰ�潻�㣨y=y_front��
				Pt3d.y=y_front-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
	}else{
		//�����潻�㣨z=z_down��
		Pt3d.z=z_down-(z_up+z_down)/2;
		Pt3d.x=(Pt3d.z-Pt3d0.z)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.z-Pt3d0.z)+Pt3d0.x;
		Pt3d.y=(Pt3d.z-Pt3d0.z)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.z-Pt3d0.z)+Pt3d0.y;
		Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
		if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.y<=y_back&&Pt3d.y>=y_front)
		{
			show_coordinateflag=true;
		}else if (mouse_locationflag==5)
		{
			if (Pt3d.x<x_left)
			{	
				//�����潻�㣨x=x_left��
				Pt3d.x=x_left-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y<y_front)
			{	
				//��ǰ�潻�㣨y=y_front��
				Pt3d.y=y_front-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}else if (mouse_locationflag==6)
		{
			if (Pt3d.x<x_left)
			{	
				//�����潻�㣨x=x_left��
				Pt3d.x=x_left-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y>y_back)
			{	
				//����潻�㣨y=y_back��
				Pt3d.y=y_back-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
		else if (mouse_locationflag==7)
		{
			if (Pt3d.x>x_right)
			{	
				//�����潻�㣨x>x_right��
				Pt3d.x=x_right-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y>y_back)
			{	
				//����潻�㣨y=y_back��
				Pt3d.y=y_back-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
		else if (mouse_locationflag==8)
		{
			if (Pt3d.x>x_right)
			{	
				//�����潻�㣨x>x_right��
				Pt3d.x=x_right-(x_right+x_left)/2;
				Pt3d.z=(Pt3d.x-Pt3d0.x)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.x-Pt3d0.x)+Pt3d0.z;
				Pt3d.y=(Pt3d.x-Pt3d0.x)*(Pt3d1.y-Pt3d0.y)/(Pt3d1.x-Pt3d0.x)+Pt3d0.y;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.y<=y_back&&Pt3d.y>=y_front&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
			if (Pt3d.y<y_front)
			{	
				//��ǰ�潻�㣨y=y_front��
				Pt3d.y=y_front-(y_back+y_front)/2;
				Pt3d.z=(Pt3d.y-Pt3d0.y)*(Pt3d1.z-Pt3d0.z)/(Pt3d1.y-Pt3d0.y)+Pt3d0.z;
				Pt3d.x=(Pt3d.y-Pt3d0.y)*(Pt3d1.x-Pt3d0.x)/(Pt3d1.y-Pt3d0.y)+Pt3d0.x;
				Pt3d=TranslationPoint(Pt3d,-Model_Length/2,-Model_Width/2,-Model_Height/2);
				if (Pt3d.x<=x_right&&Pt3d.x>=x_left&&Pt3d.z<=z_up&&Pt3d.z>=z_down)
				{
					show_coordinateflag=true;
				}
			}
		}
	}
	return Pt3d;
}

void ClengnanView::PrintCoordinate(Vector3D pt)
{
	CString str;
	HFONT hFont;
	float coefficientx=(float)nWidth/(float)1366;
	float coefficienty=(float)nHeight/(float)768;
	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("����"));
	GLuint base = glGenLists(96); // ����96����ʾ�б�
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32);
	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	glTranslatef(2.5*coefficientx,2.3*coefficienty,-6.0f);
	glRotatef(24, 0.0f,1.0f,0.0f);
	glRasterPos3f(0.0f,0.0f,0.0f); 
	glColor3f(0.0f,1.0f,0.0f);
	str.Format(_T("X=%.3lf  Y=%.3lf  Z=%.3lf"),Pt3d.x*100,Pt3d.y*100,Pt3d.z*100);
	if (Pt3d.x-x_left<0.00001&&Pt3d.x-x_left>-0.00001)
	{
		str+=_T("  left");
	}else if (Pt3d.x-x_right<0.00001&&Pt3d.x-x_right>-0.00001)
	{
		str+=_T("  right");
	}
	else if (Pt3d.y-y_front<0.00001&&Pt3d.y-y_front>-0.00001)
	{
		str+=_T("  front");
	}
	else if (Pt3d.y-y_back<0.00001&&Pt3d.y-y_back>-0.00001)
	{
		str+=_T("  back");
	}
	else if (Pt3d.z-z_down<0.00001&&Pt3d.z-z_down>-0.00001)
	{
		str+=_T("  underside");
	}
	else if (Pt3d.z-z_up<0.00001&&Pt3d.z-z_up>-0.00001)
	{
		str+=_T("  above");
	}
	//////////////////////////////////////////////////////////////////////////
	//Unicode��ת��ΪAscII��
	int iSize;
	char* pszMultiByte;
	//pszMultiByte=unicode_to_ansi(pszMultiByte,tempstr);
	iSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
	//pszMultiByte = (char*)malloc((iSize+1));
	pszMultiByte = new char[iSize + 1];
	memset( ( void* )pszMultiByte, 0, sizeof( char ) * ( iSize + 1 ) );
	WideCharToMultiByte(CP_ACP, 0, str, -1, pszMultiByte, iSize, NULL, NULL);
	//////////////////////////////////////////////////////////////////////////
	glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
	glDeleteLists(base, 96);
	delete[] pszMultiByte;

	CString strr;
	strr.Format(_T("x=%.3lf,y=%.3lf,z=%.3lf"),Pt3d.x,Pt3d.y,Pt3d.z);
	//GetParent()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(str);

	CClientDC dc((CMainFrame*)GetParent());
	CSize sz=dc.GetTextExtent(strr);
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//��������ʼ��һ�������ָ�룬ʹ��ָ����ʾ�Ĵ���
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;//��������ʼ��һ��״̬��ָ�룬ʹ��ָ�򴰿��е�״̬��
	int index=pStatus->CommandToIndex(IDS_COORDINATE);
	pStatus->SetPaneInfo(index,IDS_COORDINATE,SBPS_NORMAL,sz.cx);
	pStatus->SetPaneText(index,strr);	
	DeleteObject(hFont);
}
//������
void ClengnanView::DrawWell()
{
	//���������������
	GLUquadricObj *objCylinder = gluNewQuadric();
	// ���õ�ǰ����Ϊ��λ����
	glLoadIdentity();
	//���°ڷ��������̬
	glTranslatef(m_xPos, m_yPos, Visual_depth);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glTranslatef(-Model_Length/2,-Model_Width/2,-Model_Height/2);

	for (int i=0;i<well_num;i++)
	{
		if (ymy[i])
		{
			glColor3f(1.0f,0.0f,0.0f);
		}else{
			glColor3f(0.0f,1.0f,0.0f);
		}
		//��ջ���ı仯������ѹջ
		glPushMatrix();
		//X��
		if (PtWellS[i].x!=PtWellE[i].x)
		{
			//������ԭ���ƶ������ƾ������
			glTranslatef(m_start[i],PtWellE[i].y,PtWellE[i].z);
			//���û�����ϵ����Y����ת�����û�����X��Z�����������л���
			glRotatef(90,0.0,1.0,0.0);
			gluCylinder(objCylinder,(x_right-x_left)/100,(x_right-x_left)/100,m_end[i]-m_start[i],10,18);
		}else if (PtWellS[i].y!=PtWellE[i].y)
		{
			glTranslatef(PtWellE[i].x,m_start[i],PtWellE[i].z);
			glRotatef(-90,1.0,0.0,0.0);
			gluCylinder(objCylinder,0.01,0.01,m_end[i]-m_start[i],10,18);
		}else{
			glTranslatef(PtWellE[i].x,PtWellE[i].y,m_start[i]);
			gluCylinder(objCylinder,0.01,0.01,m_end[i]-m_start[i],10,18);
		}
		glPopMatrix();
	}
}

Vector3D ClengnanView::ThreePointToSurface()
{
	Vector3D resu;
	float m[3]={0};
	float mat[12]={0};
	mat[0]=point_section[0].x;
	mat[1]=point_section[1].x;
	mat[2]=point_section[2].x;
	mat[3]=point_section[0].y;
	mat[4]=point_section[1].y;
	mat[5]=point_section[2].y;
	mat[6]=point_section[0].z;
	mat[7]=point_section[1].z;
	mat[8]=point_section[2].z;
	mat[9]=-1;
	mat[10]=-1;
	mat[11]=-1;
	GaussIA(mat,3,m);
	resu.x=m[0];
	resu.y=m[1];
	resu.z=m[2];
	return resu;
}

//�������ཻ����
void ClengnanView::FaceAndFaceCut(Vector3D coefficient)
{
	//�����Ľ���
	Vector3D pt;
	Line3D lin[10]={0};
	int index=0;
	//float x,y,z;
	for (int i=0;i<(int)Height_Num;i++)
	{
		index=0;
		pt.z=z_down+i*(z_up-z_down)/(Height_Num-1);
		//���߷���
		//coefficient.x*x+coefficient.y*y+coefficient.z*z=-1
		pt.x=x_left;
		pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
		if (pt.y>=y_front&&pt.y<=y_back)
		{
			//��ȡ�߶�һ���˵�
			lin[i].pt[index]=pt;
			index++;
		}
		pt.x=x_right;
		pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
		if (pt.y>=y_front&&pt.y<=y_back)
		{
			//��ȡ�߶�һ���˵�
			lin[i].pt[index]=pt;
			index++;
		}
		pt.y=y_front;
		pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
		if (pt.x>=x_left&&pt.x<=x_right)
		{
			//��ȡ�߶�һ���˵�
			lin[i].pt[index]=pt;
			index++;
		}
		pt.y=y_back;
		pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
		if (pt.x>=x_left&&pt.x<=x_right)
		{
			//��ȡ�߶�һ���˵�
			lin[i].pt[index]=pt;
			index++;
		}
	}
	index=0;
	//��ǰ���潻��
	//ǰ
	pt.y=y_front;

	pt.x=x_left;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.x=x_right;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	//��
	index=0;
	pt.y=y_back;

	pt.x=x_left;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.x=x_right;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	//�������潻��
	//��
	index=0;
	pt.x=x_left;
	pt.y=y_front;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.y=y_back;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	//��
	index=0;
	pt.x=x_right;
	pt.y=y_front;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.y=y_back;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//��ȡ�߶�һ���˵�
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
}
void ClengnanView::OnShowFront()
{
	// TODO: �ڴ���������������
	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	float tempf_x,tempf_y;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	for(int i=0;i<5;i++){
		m_xAngle-=(90+tempf_x)/5;
		m_yAngle-=tempf_y/5;
		SendMessage(WM_PAINT);
	}	
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}
}

void ClengnanView::OnShowSide()
{
	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	// TODO: �ڴ���������������
	float tempf_x,tempf_y;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	for(int i=0;i<5;i++){
		m_xAngle-=tempf_x/5;
		m_yAngle+=(90-tempf_y)/5;
		SendMessage(WM_PAINT);
	}
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}
}

void ClengnanView::OnShowPlanform()
{
	//�̹߳���
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	// TODO: �ڴ���������������
	float tempf_x,tempf_y;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	for(int i=0;i<5;i++){
		m_xAngle-=tempf_x/5;
		m_yAngle-=tempf_y/5;
		SendMessage(WM_PAINT);
	}
	//�ָ̻߳�
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}
}

void ClengnanView::PrintTime()
{
	CString str;
	HFONT hFont;

	float coefficientx=(float)nWidth/(float)1161;
	float coefficienty=(float)nHeight/(float)637;

	coefficientx=(coefficientx+1)/2;
	coefficienty=(coefficienty+1)/2;
	float coefficient=coefficientx>coefficienty?coefficientx:coefficienty;

	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("����"));
	GLuint base = glGenLists(96); // ����96����ʾ�б�
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32);
	glLoadIdentity();					// ���õ�ǰ����Ϊ��λ����
	glTranslatef(2.5*coefficientx,2.3*coefficienty,-6.0f);
	glRotatef(24, 0.0f,1.0f,0.0f);
	glRasterPos3f(0.0f,0.05f,0.0f); 
	glColor3f(0.0f,1.0f,0.0f);
	char* pszMultiByte;
	int iSize;
	if (ThreadReadDataflag)//��ʷ����
	{		
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��	
		//pszMultiByte=unicode_to_ansi(pszMultiByte,tempstr);
		iSize = WideCharToMultiByte(CP_ACP, 0, timestring, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		WideCharToMultiByte(CP_ACP, 0, timestring, -1, pszMultiByte, iSize, NULL, NULL);
		glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
		glDeleteLists(base, 96);
		delete[] pszMultiByte;
	}
	else//ʵ����
	{
		CTime t=CTime::GetCurrentTime();
		str=t.Format(_T("%Y-%m-%d %H:%M:%S"));
		//////////////////////////////////////////////////////////////////////////
		//Unicode��ת��ΪAscII��	
		iSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		WideCharToMultiByte(CP_ACP, 0, str, -1, pszMultiByte, iSize, NULL, NULL);
		glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
		glDeleteLists(base, 96);
		delete[] pszMultiByte;
	}
	DeleteObject(hFont);
}


// ����ƽ��
void ClengnanView::DrawFlatSurface(float *datanum,		//������ɫֵ���ݱ�������
								   float *verdex_array,	//��Ӧ������������
								   int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����2ά
								   GLenum mode			//����ģʽ���߿�|��䣩
								   )
{
	FACECELL cell;
	float cell_z=0;
	float IsoSurface_data=0;
	int Iso_num=0;

	float x,y,z,data;
	int index;
	float color[3]={0};
	for (int j=0;j<size_datanum[1]-1;j++)
	{
		for (int i=0;i<size_datanum[0]-1;i++)
		{
			glBegin(mode);
			index=j*size_datanum[0]+i;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			z=verdex_array[3*index+2];				
			data=*(datanum+index);

			cell.Pt00.x=x;
			cell.Pt00.y=y;
			cell_z=z;
			cell.Pt00.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index++;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			z=verdex_array[3*index+2];				
			data=*(datanum+index);

			cell.Pt10.x=x;
			cell.Pt10.y=y;
			cell_z=z;
			cell.Pt10.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index+=size_datanum[0];
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			z=verdex_array[3*index+2];				
			data=*(datanum+index);

			cell.Pt11.x=x;
			cell.Pt11.y=y;
			cell_z=z;
			cell.Pt11.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index--;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			z=verdex_array[3*index+2];				
			data=*(datanum+index);

			cell.Pt01.x=x;
			cell.Pt01.y=y;
			cell_z=z;
			cell.Pt01.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);
			glEnd();
			//���Ƶ�ֵ��
			if (CtrlFlag&0x04)
			{
				for (int k=0;k<IsoSurface_num+1;k++)
				{
					IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
					float ver[10]={0};
					Iso_num=MarchingFace(cell,IsoSurface_data,ver);
					for (int nu=0;nu<Iso_num;nu++)
					{	
						glBegin(GL_LINES);
						glColor4f(0.5,0.5,0.5,Alpha_Mixing);
						glVertex3f(ver[4*nu],ver[4*nu+1],0.01+cell_z);
						glVertex3f(ver[4*nu+2],ver[4*nu+3],0.01+cell_z);
						glVertex3f(ver[4*nu],ver[4*nu+1],cell_z-0.01);
						glVertex3f(ver[4*nu+2],ver[4*nu+3],cell_z-0.01);
						glEnd();
					}
				}
			}
		}
	}
}

// ���ƴ�ֱ��z���ƽ��
void ClengnanView::DrawCutFace_Z(float *datanum,		//������ɫֵ���ݱ�������
							   float *verdex_array,	//��Ӧ������������
							   int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����2ά
							   GLenum mode			//����ģʽ���߿�|��䣩
							   )
{
	FACECELL cell;
	float cell_z=0;
	float IsoSurface_data=0;
	int Iso_num=0;

	float x,y,z,data;
	int index;
	float color[3]={0};
	//���ƴ�ֱ��z���ƽ��
	for (int j=0;j<size_datanum[1]-1;j++)
	{
		for (int i=0;i<size_datanum[0]-1;i++)
		{
			glBegin(mode);
			index=j*size_datanum[0]+i;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			//z=verdex_array[3*index+2];
			z=z_cutout;
			data=*(datanum+index);

			cell.Pt00.x=x;
			cell.Pt00.y=y;
			cell_z=z;
			cell.Pt00.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index++;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			//z=verdex_array[3*index+2];
			z=z_cutout;
			data=*(datanum+index);

			cell.Pt10.x=x;
			cell.Pt10.y=y;
			cell_z=z;
			cell.Pt10.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index+=size_datanum[0];
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			//z=verdex_array[3*index+2];
			z=z_cutout;
			data=*(datanum+index);

			cell.Pt11.x=x;
			cell.Pt11.y=y;
			cell_z=z;
			cell.Pt11.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index--;
			x=verdex_array[3*index];
			y=verdex_array[3*index+1];
			//z=verdex_array[3*index+2];
			z=z_cutout;
			data=*(datanum+index);

			cell.Pt01.x=x;
			cell.Pt01.y=y;
			cell_z=z;
			cell.Pt01.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);
			glEnd();
			//���Ƶ�ֵ��
			if (CtrlFlag&0x04)
			{
				for (int k=0;k<IsoSurface_num+1;k++)
				{
					IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
					float ver[10]={0};
					Iso_num=MarchingFace(cell,IsoSurface_data,ver);
					for (int nu=0;nu<Iso_num;nu++)
					{	
						glBegin(GL_LINES);
						glColor4f(0.5,0.5,0.5,Alpha_Mixing);
						glVertex3f(ver[4*nu],ver[4*nu+1],0.01+cell_z);
						glVertex3f(ver[4*nu+2],ver[4*nu+3],0.01+cell_z);
						glVertex3f(ver[4*nu],ver[4*nu+1],cell_z-0.01);
						glVertex3f(ver[4*nu+2],ver[4*nu+3],cell_z-0.01);
						glEnd();
					}
				}
			}
		}
	}
}

// ���ƴ�ֱ��y���ƽ��
void ClengnanView::DrawCutFace_Y(float *datanum,		//������ɫֵ���ݱ�������
							   float *verdex_array,	//��Ӧ������������
							   int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����2ά
							   GLenum mode			//����ģʽ���߿�|��䣩
							   )
{
	FACECELL cell;
	float cell_z=0;
	float IsoSurface_data=0;
	int Iso_num=0;

	float x,y,z,dx,dy,dz,data;
	int index;
	float color[3]={0};
	dx=(x_right-x_left)/(size_datanum[0]-1);
	dy=(y_back-y_front)/(size_datanum[1]-1);
	dz=(z_up-z_down)/(size_datanum[2]-1);
	//���ƴ�ֱ��y���ƽ��
	for (int j=0;j<size_datanum[2]-1;j++)
	{
		for (int i=0;i<size_datanum[0]-1;i++)
		{
			glBegin(mode);
			index=j*size_datanum[0]+i;
			x=x_left+dx*i;
			y=y_cutout;
			z=z_down+dz*j;				
			data=*(datanum+index);

			cell.Pt00.x=x;
			cell.Pt00.y=z;
			cell_z=y;
			cell.Pt00.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index++;
			x+=dx;
			data=*(datanum+index);

			cell.Pt10.x=x;
			cell.Pt10.y=z;
			cell_z=y;
			cell.Pt10.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index+=size_datanum[0];
			z+=dz;				
			data=*(datanum+index);

			cell.Pt11.x=x;
			cell.Pt11.y=z;
			cell_z=y;
			cell.Pt11.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index--;
			x-=dx;
			data=*(datanum+index);

			cell.Pt01.x=x;
			cell.Pt01.y=z;
			cell_z=y;
			cell.Pt01.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);
			glEnd();
			//���Ƶ�ֵ��
			if (CtrlFlag&0x04)
			{
				for (int k=0;k<IsoSurface_num+1;k++)
				{
					IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
					float ver[10]={0};
					Iso_num=MarchingFace(cell,IsoSurface_data,ver);
					for (int nu=0;nu<Iso_num;nu++)
					{	
						glBegin(GL_LINES);
						glColor4f(0.5,0.5,0.5,Alpha_Mixing);
						glVertex3f(ver[4*nu],0.01+cell_z,ver[4*nu+1]);
						glVertex3f(ver[4*nu+2],0.01+cell_z,ver[4*nu+3]);
						glVertex3f(ver[4*nu],cell_z-0.01,ver[4*nu+1]);
						glVertex3f(ver[4*nu+2],cell_z-0.01,ver[4*nu+3]);
						glEnd();
					}
				}
			}
		}
	}
}

// ���ƴ�ֱ��x���ƽ��
void ClengnanView::DrawCutFace_X(float *datanum,		//������ɫֵ���ݱ�������
							   float *verdex_array,	//��Ӧ������������
							   int *size_datanum,	//һ�����ϸ����ϵ����ݵ�����2ά
							   GLenum mode			//����ģʽ���߿�|��䣩
							   )
{
	FACECELL cell;
	float cell_z=0;
	float IsoSurface_data=0;
	int Iso_num=0;

	float x,y,z,dx,dy,dz,data;
	int index;
	float color[3]={0};
	dx=(x_right-x_left)/(size_datanum[0]-1);
	dy=(y_back-y_front)/(size_datanum[1]-1);
	dz=(z_up-z_down)/(size_datanum[2]-1);
	//���ƴ�ֱ��x���ƽ��
	for (int j=0;j<size_datanum[2]-1;j++)
	{
		for (int i=0;i<size_datanum[1]-1;i++)
		{
			glBegin(mode);
			index=j*size_datanum[1]+i;
			x=x_cutout;
			y=y_front+dy*i;
			z=z_down+dz*j;
			data=*(datanum+index);

			cell.Pt00.x=y;
			cell.Pt00.y=z;
			cell_z=x;
			cell.Pt00.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index++;
			y+=dy;
			data=*(datanum+index);

			cell.Pt10.x=y;
			cell.Pt10.y=z;
			cell_z=x;
			cell.Pt10.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index+=size_datanum[1];
			z+=dz;
			data=*(datanum+index);

			cell.Pt11.x=y;
			cell.Pt11.y=z;
			cell_z=x;
			cell.Pt11.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);

			index--;
			y-=dy;
			data=*(datanum+index);

			cell.Pt01.x=y;
			cell.Pt01.y=z;
			cell_z=x;
			cell.Pt01.data=data;

			ChangeValue2Color(data,color);
			glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glVertex3f(x,y,z);
			glEnd();
			//���Ƶ�ֵ��
			if (CtrlFlag&0x04)
			{
				for (int k=0;k<IsoSurface_num+1;k++)
				{
					IsoSurface_data=mymin+(float)(k*(mymax-mymin)/IsoSurface_num);
					float ver[10]={0};
					Iso_num=MarchingFace(cell,IsoSurface_data,ver);
					for (int nu=0;nu<Iso_num;nu++)
					{	
						glBegin(GL_LINES);
						glColor4f(0.5,0.5,0.5,Alpha_Mixing);
						glVertex3f(0.01+cell_z,ver[4*nu],ver[4*nu+1]);
						glVertex3f(0.01+cell_z,ver[4*nu+2],ver[4*nu+3]);
						glVertex3f(cell_z-0.01,ver[4*nu],ver[4*nu+1]);
						glVertex3f(cell_z-0.01,ver[4*nu+2],ver[4*nu+3]);
						glEnd();
					}
				}
			}
		}
	}
}


void ClengnanView::Draw_Isosurface(TRIANGLE triangle[],GLfloat colortem[3],float isosurface_da,GLenum mode)
{
	glBegin(mode);
	for (int i=0;i<sum;i++)
	{	
		glColor4f(colortem[0],colortem[1],colortem[2],3*Alpha_Mixing);
		glVertex3f(triangle[i].Pt[0].x,triangle[i].Pt[0].y,triangle[i].Pt[0].z);
		glVertex3f(triangle[i].Pt[1].x,triangle[i].Pt[1].y,triangle[i].Pt[1].z);
		glVertex3f(triangle[i].Pt[2].x,triangle[i].Pt[2].y,triangle[i].Pt[2].z);
	}
	glEnd();
}

//ȡ����һ������
void ClengnanView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CView::OnRButtonDblClk(nFlags, point);
}
//OPenGL����ͼƬ����
bool ClengnanView::SaveBMPFromOpenGL(CString lpFileName)  
{  
	GLint viewport[4];  
	glGetIntegerv(GL_VIEWPORT, viewport);  
	int width = viewport[2];  
	int height = viewport[3];  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   
	int nAlignWidth = (width*24+31)/32;  
	unsigned char* pdata = new unsigned char[nAlignWidth * height * 4];  
	memset(pdata, 0, nAlignWidth * height * 4);  
	glReadPixels(0, 0, width, height, GL_BGR_EXT,GL_UNSIGNED_BYTE, pdata);   
	//����
	BITMAPFILEHEADER Header;  
	BITMAPINFOHEADER HeaderInfo;  
	Header.bfType = 0x4D42;  
	Header.bfReserved1 = 0;  
	Header.bfReserved2 = 0;  
	Header.bfOffBits = (DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)) ;  
	Header.bfSize =(DWORD)(sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + nAlignWidth* height * 4);  
	HeaderInfo.biSize = sizeof(BITMAPINFOHEADER);  
	HeaderInfo.biWidth = width;  
	HeaderInfo.biHeight = height;  
	HeaderInfo.biPlanes = 1;  
	HeaderInfo.biBitCount = 24;  
	HeaderInfo.biCompression = 0;  
	HeaderInfo.biSizeImage = 4 * nAlignWidth * height;  
	HeaderInfo.biXPelsPerMeter = 0;  
	HeaderInfo.biYPelsPerMeter = 0;  
	HeaderInfo.biClrUsed = 0;  
	HeaderInfo.biClrImportant = 0;   
	FILE *pfile;  
	char temppathname[1024]={0};
	int wLen = WideCharToMultiByte(CP_ACP,0,lpFileName,-1,NULL,0,NULL,NULL);//�õ�Char�ĳ���                                             //��������ĵ�ַ��С 
	WideCharToMultiByte(CP_ACP, 0,lpFileName, -1,temppathname, wLen, NULL, NULL);       //��CStringת����char*  
	if(!(pfile = fopen(temppathname,"wb+")))  
	{  
		return FALSE;  
	}  
	fwrite(&Header, 1, sizeof(BITMAPFILEHEADER), pfile);  
	fwrite(&HeaderInfo, 1, sizeof(BITMAPINFOHEADER), pfile);  
	fwrite(pdata, 1, HeaderInfo.biSizeImage, pfile);  
	fclose(pfile);  
	delete[] pdata;  
	return TRUE;  
}  


void ClengnanView::OnSetwell()
{
	// TODO: �ڴ���������������
	//���þ���(��þ��ߵ��ģ������)
	if (sensors_num<=0)
	{
		MessageBox(_T("���Ƚ���ģ�ͣ�"));
		return;
	}
	CSetWellDlg dlg;
	if (dlg.DoModal()==IDOK)
	{
		// ��ʼ����
		dlg.m_end/=100.0;
		dlg.m_start/=100.0;
		dlg.x_start/=100.0;
		dlg.y_start/=100.0;
		dlg.Z_start/=100.0;
		m_start[well_num]=dlg.m_start;
		// ��ֹ����
		m_end[well_num]=dlg.m_end;
		int m_wellflag=dlg.m_wellflag;
		well_num++;
		switch(m_wellflag)
		{
		case 0:
			//x����ע����
			PtWellS[well_num-1].x=m_start[well_num-1];
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=m_end[well_num-1];
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=true;
			break;
		case 1:
			//y����ע����
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=m_start[well_num-1];
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=m_end[well_num-1];
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=true;
			break;
		case 2:
			//x���������
			PtWellS[well_num-1].x=m_start[well_num-1];
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=m_end[well_num-1];
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=false;
			break;
		case 3:
			//y���������
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=m_start[well_num-1];
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=m_end[well_num-1];
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=false;
			break;
		case 4:
			//z����ע����
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=m_start[well_num-1];
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=m_end[well_num-1];
			ymy[well_num-1]=true;
			break;
		case 5:
			//z���������
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=m_start[well_num-1];
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=m_end[well_num-1];
			ymy[well_num-1]=false;
			break;
		default:
			break;
		}
	}
}


void ClengnanView::OnMdissingle()
{
	// TODO: �ڴ���������������
	CShowPoint dlg;
	if (dlg.DoModal()==IDOK)
	{
		;
	}
}


void ClengnanView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	SetTimer(5,1000,NULL);
	// TODO: �ڴ����ר�ô����/����û���
}

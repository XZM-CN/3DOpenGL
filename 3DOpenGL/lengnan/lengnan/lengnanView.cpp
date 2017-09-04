// lengnanView.cpp : ClengnanView 类的实现
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
extern bool bAviFlag;//导出AVI文件标志
extern bool bpicture;//导出图片标志
extern CString mypicsavepath;	//截图视频保存路径
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
//////模型网格划分点的空间坐标分布
float verdex_array[200*200*100];

//模型信息显示
extern BOOL m_bModleInfo;
extern CLena* pLena;
extern bool numshowflag;//显示数据（true）,显示序号（false）
extern bool cutxflag;
extern bool cutyflag;
extern bool cutzflag;

extern bool opengliniflag;	//opengl需要初始化标志位
//读取数据线程
extern CWinThread *ThreadReadData;
extern bool bSuspendThreadflag;	//线程即将挂起标志
extern bool ThreadReadDataflag; //线程挂起，恢复标志位（true为恢复，false为挂起）
extern bool threadstarrtflag;//线程开启标志
extern bool threadoverflag;
GLint viewport[4];
extern CEvent DataEvent2;
extern int m_PlayPos;
//数据采集的时间点
extern CString timestring;
//回放模式标志位
extern bool Historyflag;

int glvolume_total,glvolume;
extern int nPointNum;				//坏点数目
extern int nPoints[100];			//坏点序号数组

float isosurface_data[10]={0};	//等值面数值
int isosurface_num=0;

extern int error_num;
//等值面三角面片32M个
TRIANGLE Triangle[4][2*1024*1024];


//滑块裁剪模型在三轴上的值
extern float x_cutout,y_cutout,z_cutout;

GLdouble eqn[4];
GLdouble eqnx[4];
GLdouble eqny[4];
GLdouble eqnz[4];

extern float Alpha_Mixing;		//Alpha混合系数
//井线的起始终止坐标
Vector3D PtWellS[20],PtWellE[20];
//注气、出气
bool ymy[20];
//井线条数
int well_num=0;
extern BOOL bperspective;//是否透视
extern bool diswellflag;//显示井标志
//各个面的值
extern float z_down,z_up;//上、下
extern float y_front,y_back;//前、后
extern float x_left,x_right;//左、右
Vector3D Pt3d,Pt3d0,Pt3d1;
GLdouble World_x[2], World_y[2], World_z[2]; 
bool show_coordinateflag=false;
//消息控制标志位：
extern UINT CtrlFlag;
//////////////////////////////////////////////////////////////////////////
//global variable
//存储3维模型模拟温度场数据，最大为8M的三阶矩阵
static float DataMatrices_3D[1*200*200*200];
//存储2维平面温度场数据，最大为1000*1000的二阶矩阵
static float DataMatrices_2D[10*200*200];
static float DataMatrices_2D_x[100*200*200];
static float DataMatrices_2D_y[10*200*200];

static float DataMatrices_2D_zz[2*200*200];
static float DataMatrices_2D_xx[20*200*200];
static float DataMatrices_2D_yy[2*200*200];

//static float DataArray[8*1024*1024]={0};

//传感器数量
extern int sensors_num;
//模型参数,模型大小
extern float Model_Length;
extern float Model_Width;
extern float Model_Height;
//长、高、宽
extern UINT Length_Num,Width_Num,Height_Num;
//传感器读数
extern float DataNumbers[1000];
float range[1000];
//////////////////////////////////////////////////////////////////////////
//传感器编号规则
//左下前为起点由左至右，由前至后，由下至上
//前下为第一排，下为第一层
//////////////////////////////////////////////////////////////////////////
//传感器位置数组
extern float Sensors_Array[3000];
//typedef struct SENSORS{
//	int num;//传感器序号
//	float x;//相对位置
//	float y;//相对位置
//	float z;//相对位置
//	float data;//传感器数值，在3维图形中对应color值表示
//}Sensors;

int nSlider;
// 温度最小值
int mymin;
// 温度最大值
int mymax;
extern int nradio;//选中的层
extern BOOL bshow;//是否网格显示
extern int n3showmode;//显示模式，单层、多层、实体
extern int MulFloorFlag;
extern bool initflag;
bool resflag=false;

extern int IsoSurface_num;
//////////////////////////////////////////////////////////////////////////
// ClengnanView

IMPLEMENT_DYNCREATE(ClengnanView, CView)

BEGIN_MESSAGE_MAP(ClengnanView, CView)
	// 标准打印命令
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

// ClengnanView 构造/析构

ClengnanView::ClengnanView()
{
	// TODO: 在此处添加构造代码
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
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	/*WS_CLIPCHILDREN样式主要是用于父窗口，也就是说当在父窗口绘制的时候，
	父窗口上还有一个子窗口，那么设置了这个样式的话，子窗口区域父窗口就不负责绘制。
	
	WS_CLIPSIBLINGS子窗口间相互裁减。也就是说当两个窗口相互重叠时，
	设置了WS_CLIPSIBLINGS样式的子窗口重绘时不能绘制被重叠的部分。*/

	cs.style	|=	WS_CLIPSIBLINGS|WS_CLIPCHILDREN;

	return CView::PreCreateWindow(cs);
}
//////////////////////////////////////////////////////////////////////////
// ClengnanView 绘制
//在绘制场景时，一般包括如下步骤：
//1）清空缓存。
//2）绘制场景。
//3）Flush掉渲染流水线。
//4）若设置了双缓冲，则交换前后台缓冲区。
//////////////////////////////////////////////////////////////////////////
void ClengnanView::OnDraw(CDC* /*pDC*/)
{
	ClengnanDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//清空缓存
    //绘制场景，绘图函数都放这里
	RenderScene();
	glFinish();//GL命令队列中的命令发送给显卡并清空命令队列，显卡完成这些命令（也就是画完了）后返回。
	SwapBuffers(m_pDC->GetSafeHdc());//交互缓冲区
	//单点显示

	//保存AVI
	if (bAviFlag)
	{
		CString tempstr=_T("C:\\temppict.bmp");
		SaveBMPFromOpenGL(tempstr);
	}
	//保存图片
	if (bpicture)
	{
		CTime t=CTime::GetCurrentTime();
		CString tempstr=t.Format(_T("%Y%m%d%H%M%S"));
		tempstr+=_T(".bmp");
		tempstr=mypicsavepath+tempstr;
		SaveBMPFromOpenGL(tempstr);
		//清除图片保存标志
		bpicture=false;
	}
}  

// ClengnanView 打印

BOOL ClengnanView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void ClengnanView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void ClengnanView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// ClengnanView 诊断

#ifdef _DEBUG
void ClengnanView::AssertValid() const
{
	CView::AssertValid();
}

void ClengnanView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ClengnanDoc* ClengnanView::GetDocument()// 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ClengnanDoc)));
	return (ClengnanDoc*)m_pDocument;
}
#endif //_DEBUG


// ClengnanView 消息处理程序

int ClengnanView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	//初始化opengl绘图
	if (!InitializeOpenGL())
		return -1;

	//glGetIntegerv(GL_VIEWPORT, viewport); 

	SetTimer(1,1000,NULL);
	return 0;
}

//为了避免内存泄露，我们要将程序中使用的new运算符来分配的内存在程序关闭时delete掉。
void ClengnanView::OnDestroy()
{
	CView::OnDestroy();

	KillTimer(1);
	// TODO: 在此处添加消息处理程序代码
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
	glViewport(0,0,cx,cy);//视口设定，视口被设置为占据打开窗口的整个像素矩形

	aSpect=(GLdouble)cx/(GLdouble)cy;//指定你的视景体的宽高比
	//////////////////////////////////////////////////////////////////////////
	//glMatrixMode设置当前矩阵模式:
	//GL_MODELVIEW,对模型视景矩阵堆栈应用随后的矩阵操作.
	//GL_PROJECTION,对投影矩阵应用随后的矩阵操作.
	//GL_TEXTURE,对纹理矩阵堆栈应用随后的矩阵操作.
	//////////////////////////////////////////////////////////////////////////

	glMatrixMode(GL_PROJECTION);//投影矩阵
	glLoadIdentity();//单位化当前矩阵
	//设置透视投影矩阵
	gluPerspective(45,//角度,指定视景体的视野的角度，以度数为单位，y轴的上下方向 
		aSpect,//视景体的宽高比,指定你的视景体的宽高比
		zNear+0.01f,//沿z轴方向的两裁面之间的距离的近处,指定观察者到视景体的最近的裁剪面的距离
		ZFar+100.0f//沿z轴方向的两裁面之间的距离的远处,指定观察者到视景体的最远的裁剪面的距离
		);

	glMatrixMode(GL_MODELVIEW);//模型视镜矩阵
	glLoadIdentity();

	// TODO: 在此处添加消息处理程序代码
}

//为了使改变窗口大小时严重的闪烁，在OnEraseBkgnd里做一些操作，避免windows自己的窗口刷新闪烁。
BOOL ClengnanView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CView::OnEraseBkgnd(pDC);
}

//初始化opengl绘制
BOOL ClengnanView::InitializeOpenGL()
{
	m_pDC=new CClientDC(this);
	//m_pDC=new CWindowDC(this);

	if (m_pDC==NULL)
	{
		AfxMessageBox(_T("获取DC错误"));
		return FALSE;
	}
	if (SetupPixelFormat()==FALSE)
	{
		AfxMessageBox(_T("设置颜色格式错误"));
		return FALSE;
	}
	//Create RC
	m_hRC=wglCreateContext(m_pDC->GetSafeHdc());

	//Failure to Create Rendering Context

	if(!m_hRC)

	{

		MessageBox(_T("设置RC错误"));
		return FALSE;

	}

	//Make the RC Current

	if(!wglMakeCurrent (m_pDC->GetSafeHdc (), m_hRC))

	{

		MessageBox(_T("Error making RC Current"));
		return FALSE;

	}
	
	//初始化背景
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0);//指定深度缓冲区的清除值

	glEnable(GL_TEXTURE_2D);						// 启用纹理映射
	glShadeModel(GL_SMOOTH);						// 启用阴影平滑
	//////////////////////////////////////////////////////////////////////////
	//指定用于深度缓冲比较值。
	//参数说明：func指定深度比较函数，
	//GL_NEVER,不通过（输入的深度值不取代参考值）
	//GL_LESS,如果输入的深度值小于参考值，则通过
	//GL_EQUAL,如果输入的深度值等于参考值，则通过
	//GL_LEQUAL,如果输入的深度值小于或等于参考值，则通过
	//GL_GREATER,如果输入的深度值大于参考值，则通过
	//GL_NOTE_QUAL,如果输入的深度值不等于参考值，则通过
	//GL_GEQUAL,如果输入的深度值大于或等于参考值，则通过
	//GL_ALWAYS,总是通过（输入的深度值取代参考值）
	//////////////////////////////////////////////////////////////////////////
	glDepthFunc(GL_LESS);

	//////////////////////////////////////////////////////////////////////////
	//深度测试,在涉及到消隐等情况（可能遮挡），都要开启深度测试
	//glEnable(GL_DEPTH_TEST),硬件上打开了深度缓存区，当有新的同样XY坐标的片断到来时，
	//比较两者的深度，并且在初始化时打开深度，绘制每一帧前要gl_clear(gl-depth-bit)
	//（这根clear_buffer_bit类似，而且同样要设置clear_deppth_bitd 值，
	//用glClearDepth(GLclampd depth)，一般设为1，这将背景设为最深，这是默认的，通常不用写）
	//////////////////////////////////////////////////////////////////////////
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);//设置着色模式,GL_SMOOTH使用光滑着色，GL_FALT使用恒定着色

	//混合
	//glColor4f(1.0f,1.0f,1.0f,alpha_mixing);			// 全亮度， 50% Alpha 混合
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc(GL_SRC_ALPHA,GL_ONE);		// 基于源象素alpha通道值的半透明混合函数
	//if (m_pDC)
	//{
	//	m_pDC=NULL;
	//	ReleaseDC(m_pDC);
	//}
	//glPushMatrix();
	return TRUE;
}

//设置像素格式
BOOL ClengnanView::SetupPixelFormat()
{
	static PIXELFORMATDESCRIPTOR pfd=
	{
		sizeof(PIXELFORMATDESCRIPTOR),//Structure size.  
		1,        // Structure version number.Property flags（特性标志）：  
		PFD_DRAW_TO_WINDOW |  // support window  
		PFD_SUPPORT_OPENGL | // support OpenGL  
		PFD_DOUBLEBUFFER,                      
		PFD_TYPE_RGBA,   // RGBA type  
		24,      // 32－bit color.  
		0, 0, 0, 0, 0, 0,   // Not concerned with these:不涉及的属性  
		0,      // No alpha ：无alpha缓存  
		0,      // Shift bit ignored:忽略转换位  
		0, 0, 0, 0, 0,// No accum buffer：没有累积缓存  
		32,  // 32－bit depth buffer.  
		0,      // No stencil：无模板缓存  
		0,      // No auxliliary buffers：无辅助缓存  
		PFD_MAIN_PLANE, // Main layer type.：主层类型  
		0,      // Reserved.：保留结构数  
		0, 0, 0    // Unsupported.：不支持结构数  
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

//主要绘图函数
void ClengnanView::RenderScene()
{
	//每一层的大小
	float size_rect[2]={Model_Length,Model_Width};
	//模型在X,Y,Z上的划分，nSlider为划分粒度
	int size_datanum[3]={(Length_Num-1)*(nSlider+1)+1,(Width_Num-1)*(nSlider+1)+1,(Height_Num-1)*(nSlider+1)+1};
	//每个划分的间距
	float dx,dy,dz;
	//各个划分的网格点的空间坐标
	float x,y,z;
	int tempindex=0;
	dx=(x_right-x_left)/(size_datanum[0]-1);
	dy=(y_back-y_front)/(size_datanum[1]-1);
	dz=(z_up-z_down)/(size_datanum[2]-1);
	//获取模型各个网格划分点的空间坐标分布
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
	//混合// 基于源象素alpha通道值的半透明混合函数
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);		
	//////////////////////////////////////////////////////////////////////////
	//模型变换、旋转
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	// 重置当前矩阵为单位矩阵
	glLoadIdentity();					
	//设置相机位置:在整个软件中相机的位置不发生变化
	gluLookAt(0,0,camera_z,	0,0,Visual_depth,	0,1,0);;//设置camera位置
	glTranslatef(m_xPos, m_yPos, Visual_depth);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glTranslatef(-Model_Length/2,-Model_Width/2,-Model_Height/2);
	//开启颜色混合
	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	GLfloat colortem[3];
	//判断颜色混合
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
		//画井线
		DrawWell();
	}
	switch(n3showmode)
	{
	case 0:
		glDisable(GL_CLIP_PLANE0);
		glDisable(GL_CLIP_PLANE1);
		glDisable(GL_CLIP_PLANE2);
		glDisable(GL_CLIP_PLANE3);
		//网格还是填充
		if (bshow)
		{
			Draw_SigleFloor(DataMatrices_2D+(int)nradio*(size_datanum[0])*(size_datanum[1]),size_rect,size_datanum,GL_LINE_LOOP);
		}else{
			Draw_SigleFloor(DataMatrices_2D+(int)nradio*(size_datanum[0])*(size_datanum[1]),size_rect,size_datanum,GL_POLYGON);
		}
		//绘制传感器点
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
		//画切割的平面
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

		//绘制传感器点
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
		//画切割的平面
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
		//绘制传感器点
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
	//绘制时间点
	PrintTime();
	glDepthMask(GL_TRUE);
	//delete[] verdex_array;
}
void ClengnanView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值	
	//线程挂起
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	//记录鼠标按下的消息
	m_MouseDownPoint=point;
	mouseflag=FALSE;
	SetCapture();
	CView::OnLButtonDown(nFlags, point);
}

void ClengnanView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_MouseDownPoint=CPoint(0,0);
	ReleaseCapture();
	//线程恢复
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//线程挂起
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
	//线程恢复
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
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CView::OnSetCursor(pWnd, nHitTest, message);
}

void ClengnanView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_MouseDownPoint=point;
	mouseflag=TRUE;
	SetCapture();	
	CView::OnRButtonDown(nFlags, point);
}

void ClengnanView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_MouseDownPoint=CPoint(0,0);
	ReleaseCapture();

	CView::OnRButtonUp(nFlags, point);
}

void ClengnanView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	InvalidateRect(NULL,FALSE);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
void ClengnanView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

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
		glLoadIdentity();					// 重置当前矩阵为单位矩阵	

		point.y=rect.Height()-point.y;
		GetSelectionRay(point.x,point.y);
	}
	SendMessage(WM_PAINT);
	
	CView::OnMouseMove(nFlags, point);
}

void ClengnanView::Draw_OutsideSurface(	float *datanum,		//顶点颜色值数据表征数组
										float *verdex_array,	//对应顶点坐标数组
										int *size_datanum,	//一个面上各轴上的数据点数，3维
										int length_num,		//x轴向传感器点数
										int width_num,			//y轴向传感器点数
										int height_num,		//z轴向传感器点数
										GLenum mode			//绘制模式（填充|线框）
										)
{
	//上下两面
	//画面,下
	int k,index;
	k=0;
	index=k*size_datanum[0]*size_datanum[1];
	DrawFlatSurface(datanum+index,verdex_array+3*index,size_datanum,mode);

	if(height_num!=1)
	{
		//画面，上
		k=height_num-1;
		index=k*size_datanum[0]*size_datanum[1];
		DrawFlatSurface(datanum+index,verdex_array+3*index,size_datanum,mode);

		FACECELL cell;
		float cell_z=0;
		float IsoSurface_data=0;
		int Iso_num=0;

		//左右两面
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

					//绘制等值线
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
		//前后两面
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

					//绘制等值线
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

void ClengnanView::Draw_MulFloor(float *datanum,		//顶点颜色值数据表征数组
								 float *verdex_array,	//对应顶点坐标数组
								 int *size_datanum,	//一个面上各轴上的数据点数，3维
								 //int length_num,		//x轴向传感器点数
								 //int width_num,			//y轴向传感器点数
								 int height_num,		//z轴向传感器点数
								 GLenum mode			//绘制模式（填充|线框）
								 )
{
	//画面,下
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

//绘制NURBS单层曲面
void ClengnanView::Draw_Bezier_SigleFloor(float *vertex_array,float *datanum)
{
	//GLfloat ver[][][];

	//GLUnurbsObj *nobj;//绘制NURBS曲面之前，必须创建一个NURBS对象并设定其属性
	//nobj = gluNewNurbsRenderer(); 
	////gluNurbsProperty(nobj , GLU_SAMPLING_TOLERANCE, 20);
	//gluNurbsProperty(nobj , GLU_DISPLAY_MODE, GLU_FILL);
	//gluBeginSurface(nobj);
	//gluNurbsSurface(nobj,20,sKnot,20,tKnot,15*3,3,&ver[0][0][0],5,5,GL_MAP2_VERTEX_3);
	//gluEndSurface(nobj);
}

//绘制单层
void ClengnanView::Draw_SigleFloor(float *datanum,		//顶点颜色值数据表征数组
								   float *size_rect,	//平面矩形大小，2维
								   int *size_datanum,	//各轴上的数据点数，2维
								   GLenum mode			//绘制模式（线框|填充）														
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

			//绘制等值线
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
		}else{//非渐变模式
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
			}else{//非渐变模式
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
//插值算法 Interpolation algorithm 
//距离权重法 Distance weighted interpolation algorithm
//Vertex_Array[]为已知点坐标位置数组
//datanumbers[3]为待求点坐标位置
//datanumbers[]为已知点数据信息
//n为距离反比的幂次，越大近距离点的权重越大，远距离点权重越小
//返回值为待求点数据信息
//敖建 2013\01\23
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
// 画标尺
void ClengnanView::DrawStaff(float dmax, float dmin)
{
	//CGLFont *glFont;
	HFONT hFont;
	//glFont=new CGLFont();
	hFont  =CreateFont(-16,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("宋体"));
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


	glLoadIdentity();					// 重置当前矩阵为单位矩阵
	glTranslatef(-4.4*coefficientx+0.15,0.2f*coefficienty-0.05,-6.0f);
	//glTranslatef(m_xPos, m_yPos, Visual_depth);
	//glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(24, 0.0f,1.0f,0.0f);
	//glScalef(m_x,m_y,m_z);
	GLuint base = glGenLists(96); // 创建96个显示列表
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 
	for (int i=0;i<11;i++)
	{
		tempstr.Format(_T("%.1f"),currvalue[2*i]);
		//////////////////////////////////////////////////////////////////////////
		//Unicode码转化为AscII码
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
		glLoadIdentity();					// 重置当前矩阵为单位矩阵
		//写模型信息
		//////////////////////////////////////////////////////////////////////////
		//Modle Info：
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
		//Unicode码转化为AscII码
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
		//Unicode码转化为AscII码
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
		//Unicode码转化为AscII码
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
		//Unicode码转化为AscII码
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
		//Unicode码转化为AscII码
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
		//Unicode码转化为AscII码
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
	glLoadIdentity();					// 重置当前矩阵为单位矩阵
	glTranslatef(-4.4*coefficientx,0.2*coefficienty,-6.0f);
	//glTranslatef(-3,-3,-6.0);
	glBegin(GL_LINES);
	//绘制等值线
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
	glLoadIdentity();					// 重置当前矩阵为单位矩阵
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
	//第一层
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
	case 0:			//演示版本，回放模式
		
		break;
	case 1:			//实时模式
		MessageBox(_T("::"));
		break;
	case 2:			//后处理(历史记录)模式	双三次样条内插
		//双线性内插
		//BI_IA(Sensors_Array,DataNumbers,mcount,DataMatrices_2D);
		//水平面的内插
		for (int k=0;k<(int)Height_Num;k++)
		{
			for (int i=0;i<(int)Length_Num*(int)Width_Num;i++)
			{
				d[i]=DataNumbers[i+k*Length_Num*Width_Num];
			}
			int size_floor=((nSlider+1)*(Length_Num-1)+1)*((nSlider+1)*(Width_Num-1)+1);
			//双三次样条插值
			DCSIA(nSlider,nSlider,Length_Num-1,Width_Num-1,x,y,d,DataMatrices_2D+k*size_floor);
		}
		//左右面的内插
		for (int k=0;k<(int)Length_Num;k++)
		{
			for (int i=0;i<(int)Height_Num*(int)Width_Num;i++)
			{
				d[i]=DataNumbers[i*Length_Num+k];
			}
			int size_floor=((nSlider+1)*(Height_Num-1)+1)*((nSlider+1)*(Width_Num-1)+1);
			//双三次样条插值
			DCSIA(nSlider,nSlider,Width_Num-1,Height_Num-1,y,z,d,DataMatrices_2D_x+k*size_floor);
		}
		//前后面的内插
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
			//双三次样条插值
			DCSIA(nSlider,nSlider,Length_Num-1,Height_Num-1,x,z,d,DataMatrices_2D_y+k*size_floor);
		}
		//裁剪面的内插（x）
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
		//裁剪面的内插（y）
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
		//裁剪面的内插（z）
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
		//m_wndExtendBar.m_MySlider.SetPos(m_PlayPos);//设置播放条位置	
		break;
		case 5://在自动历史回放中回放到头自动停止
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_GOON,MF_BYCOMMAND|MF_ENABLED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(IDM_SUSPEND,MF_BYCOMMAND|MF_GRAYED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTOPAVI,MF_BYCOMMAND|MF_GRAYED);
			GetParentFrame()->GetMenu()->GetSubMenu(2)->EnableMenuItem(ID_MSTARTAVI,MF_BYCOMMAND|MF_ENABLED);
			break;
	default:
		break;
	}
	//渲染场景,强制重绘，调用WM_PAINT
	SendMessage(WM_PAINT);
	delete[] x;
	delete[] y;
	delete[] z;
	delete[] d;
	return 0;
}
void ClengnanView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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
	hFont  =CreateFont(36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("宋体"));
	GLuint base = glGenLists(96); // 创建96个显示列表
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 

	glLoadIdentity();					// 重置当前矩阵为单位矩阵

	glTranslatef(m_xPos, m_yPos, Visual_depth);
	glRotatef(m_xAngle, 1.0f,0.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,1.0f,0.0f);
	glRotatef(m_yAngle, 0.0f,0.0f,1.0f);
	glScalef(m_x,m_y,m_z);
	//缩小100倍，以m为单位
	glTranslatef(-Model_Length/2,-Model_Width/2,-Model_Height/2);
	//glEnable(GL_BLEND);
	//glDisable(GL_DEPTH_TEST);
	if (n3showmode)
	{
		for(int i=0;i<sensors_num;i++)
		{
			if (sensornumber[i]==-1)//没有传感器的点
			{
				continue;
			}
			glTranslatef(Sensors_Array[3*i],Sensors_Array[3*i+1],Sensors_Array[3*i+2]);//移动原点
			//ChangeValue2Color(DataNumbers[i],color);
			//glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glColor3f(1.0f,1.0f,1.0f/*,Alpha_Mixing*/);
			glRasterPos3f(0.0f,0.05f,0.05f); 
			if (flag)
			{
				if (BelongTo(sensornumber[i],nPoints,nPointNum))		//坏点查找
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
			glTranslatef(-Sensors_Array[3*i],-Sensors_Array[3*i+1],-Sensors_Array[3*i+2]);//取消移动
		}
	}else{
		for(int i=nradio*Length_Num*Width_Num;i<(nradio+1)*Length_Num*Width_Num;i++)
		{	
			if (sensornumber[i]==-1)
			{
				continue;
			}
			glTranslatef(Sensors_Array[3*i],Sensors_Array[3*i+1],DataNumbers[i]/ma);//移动原点
			//ChangeValue2Color(DataNumbers[i],color);
			//glColor4f(color[0],color[1],color[2],Alpha_Mixing);
			glColor3f(0.9f,0.9f,0.9f/*,Alpha_Mixing*/);
			glRasterPos3f(0.0f,0.05f,0.05f); 
			if (flag)
			{
				if (BelongTo(sensornumber[i],nPoints,nPointNum))		//坏点查找
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
			glTranslatef(-Sensors_Array[3*i],-Sensors_Array[3*i+1],-DataNumbers[i]/ma);//取消移动
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
	//体元的分割的步长
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

				//体元各个顶点数据
				Data_Cell[0]=src_data[index];
				Data_Cell[1]=src_data[index+1];
				Data_Cell[2]=src_data[index+Length_Num];
				Data_Cell[3]=src_data[index+Length_Num+1];
				Data_Cell[4]=src_data[index+Width_Num*Length_Num];
				Data_Cell[5]=src_data[index+Width_Num*Length_Num+1];
				Data_Cell[6]=src_data[index+Width_Num*Length_Num+Length_Num];
				Data_Cell[7]=src_data[index+Width_Num*Length_Num+Length_Num+1];
				//体元各个顶点坐标
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
				
				//立方体元内点插值
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
	//面元的分割的步长
	float Length_Cell,Width_Cell;
	POINT3D pt;
	float Vertex_Cell[8]={0};
	float Data_Cell[4]={0};
	int num=0;
	//每层的点数
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
				//面元各个顶点数据、位置(2维度)
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

//画小坐标系
void ClengnanView::Draw_CoordinateSystem()
{
	HFONT hFont;
	float coefficientx=(float)nWidth/(float)1366;
	float coefficienty=(float)nHeight/(float)768;
	coefficientx=(coefficientx+1)/2;
	coefficienty=(coefficienty+1)/2;
	float coefficient=coefficientx>coefficienty?coefficientx:coefficienty;
	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("宋体"));
	GLuint base = glGenLists(96); // 创建96个显示列表
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32); 
	glLoadIdentity();					// 重置当前矩阵为单位矩阵
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
	glTranslatef(1.1f,0.0f,0.0f);//移动原点
	glRasterPos3f(0.0f,0.0f,0.0f); 
	glCallLists(2,GL_UNSIGNED_BYTE,"x");
	glTranslatef(-1.1f,0.0f,0.0f);//取消移动
	glTranslatef(0.0f,1.1f,0.0f);//移动原点
	glRasterPos3f(0.0f,0.0f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,"y");
	glTranslatef(0.0f,-1.1f,0.0f);//取消移动
	glTranslatef(0.0f,0.0f,1.1f);//移动原点
	glRasterPos3f(0.0f,0.0f,0.0f);
	glCallLists(2,GL_UNSIGNED_BYTE,"z");
	glTranslatef(0.0f,0.0f,-1.1f);//取消移动
	glDeleteLists(base, 96);
	//z轴
	glColor3f(1.0f,0.0f,1.0f);
	//glRotatef(-90,0.0f,0.0f,1.0f);
	glTranslatef(0.0f,0.0f, 0.7f);
	glutSolidCone(0.05,0.3f,20.0,1);
	//x轴
	glTranslatef(0.7f,0.0f, -0.7f);
	glRotatef(90,0.0f,1.0f,0.0f);
	glutSolidCone(0.05,0.3f,20.0,1);
	//y轴
	glTranslatef(0.0f,0.7f, -0.7f);
	glRotatef(-90,1.0f,0.0f,0.0f);
	glutSolidCone(0.05,0.3f,20.0,1);
	DeleteObject(hFont);
}
//双击左键，旋转、移动状态都还原
void ClengnanView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//线程挂起
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
	//线程恢复
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}	
	CView::OnLButtonDblClk(nFlags, point);
}

//画等值线
void ClengnanView::Draw_Isogram(float *Vertex_Array,float *datanum,int Isogram_num)
{
	//一个面一个面绘制
}

// 鼠标射线捡练
Vector3D ClengnanView::GetSelectionRay(int mouse_x, int mouse_y)
{
	GLdouble    modelview[16];//为视矩阵
	GLdouble    projection[16];//为投影矩阵
	GLint       viewport[4];//为视口
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
	//射线的方程(0<=t<=1)，在世界坐标系上
	//t_x=(World_x[1]-World_x[0])*t;
	//t_y=(World_y[1]-World_y[0])*t;
	//t_z=(World_z[1]-World_z[0])*t;
	//camera坐标系到模型坐标系的变换矩阵
	Vector4D V0,V1;
	V0=MakePoint(World_x[0],World_y[0],World_z[0],1);
	V1=MakePoint(World_x[1],World_y[1],World_z[1],1);
	////转化为模型坐标系上的坐标
	////经过了坐标变换
	Pt3d0=MakePoint(V0.x,V0.y,V0.z);
	Pt3d1=MakePoint(V1.x,V1.y,V1.z);
	//平移
	Pt3d0=TranslationPoint(Pt3d0,m_xPos, m_yPos, Visual_depth);
	Pt3d1=TranslationPoint(Pt3d1,m_xPos, m_yPos, Visual_depth);
	//旋转	
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
		mouse_locationflag=2;//左后上
		break;
	case 0x12:
		mouse_locationflag=6;//左后下
		break;
	case 0x14:
		mouse_locationflag=7;//右后下
		break;
	case 0x18:
		mouse_locationflag=3;//右后上
		break;
	case 0x21:
		mouse_locationflag=7;//右后下
		break;
	case 0x22:
		mouse_locationflag=3;//右后上
		break;
	case 0x24:
		mouse_locationflag=2;//左后上
		break;
	case 0x28:
		mouse_locationflag=6;//左后下
		break;
	case 0x41:
		mouse_locationflag=8;//前右下
		break;
	case 0x42:
		mouse_locationflag=4;//前右上
		break;
	case 0x44:
		mouse_locationflag=1;//前左上
		break;
	case 0x48:
		mouse_locationflag=5;//前左下
		break;
	case 0x81:
		mouse_locationflag=1;//左前上
		break;
	case 0x82:
		mouse_locationflag=5;//左前下
		break;
	case 0x84:
		mouse_locationflag=8;//右前下
		break;
	case 0x88:
		mouse_locationflag=4;//右前上
		break;
	}
	show_coordinateflag=false;
	if (mouse_locationflag<5)
	{
		//与上面交点（z=z_up）
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
				//与左面交点（x=x_left）
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
				//与前面交点（y=y_front）
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
				//与左面交点（x=x_left）
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
				//与后面交点（y=y_back）
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
				//与右面交点（x>x_right）
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
				//与后面交点（y=y_back）
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
				//与右面交点（x>x_right）
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
				//与前面交点（y=y_front）
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
		//与下面交点（z=z_down）
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
				//与左面交点（x=x_left）
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
				//与前面交点（y=y_front）
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
				//与左面交点（x=x_left）
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
				//与后面交点（y=y_back）
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
				//与右面交点（x>x_right）
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
				//与后面交点（y=y_back）
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
				//与右面交点（x>x_right）
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
				//与前面交点（y=y_front）
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
	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("宋体"));
	GLuint base = glGenLists(96); // 创建96个显示列表
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32);
	glLoadIdentity();					// 重置当前矩阵为单位矩阵
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
	//Unicode码转化为AscII码
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
	CMainFrame* pFrame=(CMainFrame*)AfxGetApp()->m_pMainWnd;//声明并初始化一个框架类指针，使其指向显示的窗口
	CStatusBar* pStatus=&pFrame->m_wndStatusBar;//声明并初始化一个状态栏指针，使其指向窗口中的状态栏
	int index=pStatus->CommandToIndex(IDS_COORDINATE);
	pStatus->SetPaneInfo(index,IDS_COORDINATE,SBPS_NORMAL,sz.cx);
	pStatus->SetPaneText(index,strr);	
	DeleteObject(hFont);
}
//画井线
void ClengnanView::DrawWell()
{
	//创建二次曲面对象
	GLUquadricObj *objCylinder = gluNewQuadric();
	// 重置当前矩阵为单位矩阵
	glLoadIdentity();
	//重新摆放物体的姿态
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
		//将栈顶的变化矩阵复制压栈
		glPushMatrix();
		//X向井
		if (PtWellS[i].x!=PtWellE[i].x)
		{
			//将坐标原点移动到绘制井的起点
			glTranslatef(m_start[i],PtWellE[i].y,PtWellE[i].z);
			//将用户坐标系绕着Y向旋转，将用户坐标X和Z在世界坐标中互换
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

//求面面相交交线
void ClengnanView::FaceAndFaceCut(Vector3D coefficient)
{
	//与各层的交线
	Vector3D pt;
	Line3D lin[10]={0};
	int index=0;
	//float x,y,z;
	for (int i=0;i<(int)Height_Num;i++)
	{
		index=0;
		pt.z=z_down+i*(z_up-z_down)/(Height_Num-1);
		//交线方程
		//coefficient.x*x+coefficient.y*y+coefficient.z*z=-1
		pt.x=x_left;
		pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
		if (pt.y>=y_front&&pt.y<=y_back)
		{
			//获取线段一个端点
			lin[i].pt[index]=pt;
			index++;
		}
		pt.x=x_right;
		pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
		if (pt.y>=y_front&&pt.y<=y_back)
		{
			//获取线段一个端点
			lin[i].pt[index]=pt;
			index++;
		}
		pt.y=y_front;
		pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
		if (pt.x>=x_left&&pt.x<=x_right)
		{
			//获取线段一个端点
			lin[i].pt[index]=pt;
			index++;
		}
		pt.y=y_back;
		pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
		if (pt.x>=x_left&&pt.x<=x_right)
		{
			//获取线段一个端点
			lin[i].pt[index]=pt;
			index++;
		}
	}
	index=0;
	//与前后面交线
	//前
	pt.y=y_front;

	pt.x=x_left;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.x=x_right;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//获取线段一个端点
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//获取线段一个端点
		lin[Height_Num].pt[index]=pt;
		index++;
	}
	//后
	index=0;
	pt.y=y_back;

	pt.x=x_left;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.x=x_right;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//获取线段一个端点
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.x=(-1-pt.z*coefficient.z-pt.y*coefficient.y)/coefficient.x;
	if (pt.x>=x_left&&pt.x<=x_right)
	{
		//获取线段一个端点
		lin[Height_Num+1].pt[index]=pt;
		index++;
	}
	//与左右面交线
	//左
	index=0;
	pt.x=x_left;
	pt.y=y_front;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.y=y_back;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//获取线段一个端点
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//获取线段一个端点
		lin[Height_Num+2].pt[index]=pt;
		index++;
	}
	//右
	index=0;
	pt.x=x_right;
	pt.y=y_front;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.y=y_back;
	pt.z=(-1-pt.x*coefficient.x-pt.y*coefficient.y)/coefficient.z;
	if (pt.z>=z_down&&pt.z<=z_up)
	{
		//获取线段一个端点
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.z=z_down;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//获取线段一个端点
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
	pt.z=z_up;
	pt.y=(-1-pt.z*coefficient.z-pt.x*coefficient.x)/coefficient.y;
	if (pt.y>=y_front&&pt.y<=y_back)
	{
		//获取线段一个端点
		lin[Height_Num+3].pt[index]=pt;
		index++;
	}
}
void ClengnanView::OnShowFront()
{
	// TODO: 在此添加命令处理程序代码
	//线程挂起
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
	//线程恢复
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}
}

void ClengnanView::OnShowSide()
{
	//线程挂起
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	// TODO: 在此添加命令处理程序代码
	float tempf_x,tempf_y;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	for(int i=0;i<5;i++){
		m_xAngle-=tempf_x/5;
		m_yAngle+=(90-tempf_y)/5;
		SendMessage(WM_PAINT);
	}
	//线程恢复
	if (!ThreadReadDataflag&&threadstarrtflag)
	{
		ThreadReadData->ResumeThread();
		opengliniflag=true;
		ThreadReadDataflag=true;
	}
}

void ClengnanView::OnShowPlanform()
{
	//线程挂起
	if (ThreadReadDataflag&&threadstarrtflag)
	{
		bSuspendThreadflag=true;
		//ThreadReadData->SuspendThread();
		ThreadReadDataflag=false;
	}
	// TODO: 在此添加命令处理程序代码
	float tempf_x,tempf_y;
	tempf_x=m_xAngle;
	tempf_y=m_yAngle;
	for(int i=0;i<5;i++){
		m_xAngle-=tempf_x/5;
		m_yAngle-=tempf_y/5;
		SendMessage(WM_PAINT);
	}
	//线程恢复
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

	hFont  =CreateFont(-36,0,0,0,FW_NORMAL,0,0,0,GB2312_CHARSET,0,0,0,FF_MODERN,TEXT("宋体"));
	GLuint base = glGenLists(96); // 创建96个显示列表
	wglUseFontBitmaps(wglGetCurrentDC(),32, 96, base);  
	glListBase(base-32);
	glLoadIdentity();					// 重置当前矩阵为单位矩阵
	glTranslatef(2.5*coefficientx,2.3*coefficienty,-6.0f);
	glRotatef(24, 0.0f,1.0f,0.0f);
	glRasterPos3f(0.0f,0.05f,0.0f); 
	glColor3f(0.0f,1.0f,0.0f);
	char* pszMultiByte;
	int iSize;
	if (ThreadReadDataflag)//历史数据
	{		
		//////////////////////////////////////////////////////////////////////////
		//Unicode码转化为AscII码	
		//pszMultiByte=unicode_to_ansi(pszMultiByte,tempstr);
		iSize = WideCharToMultiByte(CP_ACP, 0, timestring, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		WideCharToMultiByte(CP_ACP, 0, timestring, -1, pszMultiByte, iSize, NULL, NULL);
		glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
		glDeleteLists(base, 96);
		delete[] pszMultiByte;
	}
	else//实验中
	{
		CTime t=CTime::GetCurrentTime();
		str=t.Format(_T("%Y-%m-%d %H:%M:%S"));
		//////////////////////////////////////////////////////////////////////////
		//Unicode码转化为AscII码	
		iSize = WideCharToMultiByte(CP_ACP, 0, str, -1, NULL, 0, NULL, NULL);
		pszMultiByte = new char[iSize + 1];
		WideCharToMultiByte(CP_ACP, 0, str, -1, pszMultiByte, iSize, NULL, NULL);
		glCallLists(iSize,GL_UNSIGNED_BYTE,pszMultiByte);
		glDeleteLists(base, 96);
		delete[] pszMultiByte;
	}
	DeleteObject(hFont);
}


// 绘制平面
void ClengnanView::DrawFlatSurface(float *datanum,		//顶点颜色值数据表征数组
								   float *verdex_array,	//对应顶点坐标数组
								   int *size_datanum,	//一个面上各轴上的数据点数，2维
								   GLenum mode			//绘制模式（线框|填充）
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
			//绘制等值线
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

// 绘制垂直于z面的平面
void ClengnanView::DrawCutFace_Z(float *datanum,		//顶点颜色值数据表征数组
							   float *verdex_array,	//对应顶点坐标数组
							   int *size_datanum,	//一个面上各轴上的数据点数，2维
							   GLenum mode			//绘制模式（线框|填充）
							   )
{
	FACECELL cell;
	float cell_z=0;
	float IsoSurface_data=0;
	int Iso_num=0;

	float x,y,z,data;
	int index;
	float color[3]={0};
	//绘制垂直于z面的平面
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
			//绘制等值线
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

// 绘制垂直于y面的平面
void ClengnanView::DrawCutFace_Y(float *datanum,		//顶点颜色值数据表征数组
							   float *verdex_array,	//对应顶点坐标数组
							   int *size_datanum,	//一个面上各轴上的数据点数，2维
							   GLenum mode			//绘制模式（线框|填充）
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
	//绘制垂直于y面的平面
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
			//绘制等值线
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

// 绘制垂直于x面的平面
void ClengnanView::DrawCutFace_X(float *datanum,		//顶点颜色值数据表征数组
							   float *verdex_array,	//对应顶点坐标数组
							   int *size_datanum,	//一个面上各轴上的数据点数，2维
							   GLenum mode			//绘制模式（线框|填充）
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
	//绘制垂直于x面的平面
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
			//绘制等值线
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

//取消上一项命令
void ClengnanView::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CView::OnRButtonDblClk(nFlags, point);
}
//OPenGL保存图片函数
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
	//保存
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
	int wLen = WideCharToMultiByte(CP_ACP,0,lpFileName,-1,NULL,0,NULL,NULL);//得到Char的长度                                             //分配变量的地址大小 
	WideCharToMultiByte(CP_ACP, 0,lpFileName, -1,temppathname, wLen, NULL, NULL);       //将CString转换成char*  
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
	// TODO: 在此添加命令处理程序代码
	//放置井线(获得井线点的模型坐标)
	if (sensors_num<=0)
	{
		MessageBox(_T("请先建立模型！"));
		return;
	}
	CSetWellDlg dlg;
	if (dlg.DoModal()==IDOK)
	{
		// 起始坐标
		dlg.m_end/=100.0;
		dlg.m_start/=100.0;
		dlg.x_start/=100.0;
		dlg.y_start/=100.0;
		dlg.Z_start/=100.0;
		m_start[well_num]=dlg.m_start;
		// 终止坐标
		m_end[well_num]=dlg.m_end;
		int m_wellflag=dlg.m_wellflag;
		well_num++;
		switch(m_wellflag)
		{
		case 0:
			//x方向注气井
			PtWellS[well_num-1].x=m_start[well_num-1];
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=m_end[well_num-1];
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=true;
			break;
		case 1:
			//y方向注气井
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=m_start[well_num-1];
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=m_end[well_num-1];
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=true;
			break;
		case 2:
			//x方向出气井
			PtWellS[well_num-1].x=m_start[well_num-1];
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=m_end[well_num-1];
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=false;
			break;
		case 3:
			//y方向出气井
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=m_start[well_num-1];
			PtWellS[well_num-1].z=dlg.Z_start;
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=m_end[well_num-1];
			PtWellE[well_num-1].z=dlg.Z_start;
			ymy[well_num-1]=false;
			break;
		case 4:
			//z方向注气井
			PtWellS[well_num-1].x=dlg.x_start;
			PtWellS[well_num-1].y=dlg.y_start;
			PtWellS[well_num-1].z=m_start[well_num-1];
			PtWellE[well_num-1].x=dlg.x_start;
			PtWellE[well_num-1].y=dlg.y_start;
			PtWellE[well_num-1].z=m_end[well_num-1];
			ymy[well_num-1]=true;
			break;
		case 5:
			//z方向出气井
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
	// TODO: 在此添加命令处理程序代码
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
	// TODO: 在此添加专用代码和/或调用基类
}

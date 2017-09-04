// lengnanView.h : ClengnanView 类的接口
//


#pragma once
#include "lengnanDoc.h"
#include "IA.h"
#define WM_MY_MESSAGE	WM_USER+0x0001


class ClengnanView : public CView
{
protected: // 仅从序列化创建
	ClengnanView();
	DECLARE_DYNCREATE(ClengnanView)

// 属性
public:
	ClengnanDoc* GetDocument();

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~ClengnanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	    /************************************************************************/

    /* 设置的变量是Rendering Context(着色描述表)。每一个OpenGL都被连接到一个着

    色描述表上。着色描述表将所有的OpenGL调用命令连接到Device Context(设备描述表)上。

    我将OpenGL的着色描述表定义为hRC 。要让您的程序能够绘制窗口的话，还需要创建一个

    设备描述表，也就是第二行的内容。Windows的设备描述表被定义为hDC 。DC将窗口连接到

    GDI(Graphics Device Interface图形设备接口)。而RC将OpenGL连接到DC                                                                     */

    /************************************************************************/

    HGLRC m_hRC;    //Rendering Context着色描述表

    CDC* m_pDC;        //Device Context设备描述表

    BOOL InitializeOpenGL();    //Initialize OpenGL

    BOOL SetupPixelFormat();    //设置像素格式

    void RenderScene();            //渲染场景
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//保存图片函数
	bool SaveBMPFromOpenGL(CString lpFileName);

	GLdouble aSpect;//视景体的宽高比

	//井线参数
	// 起始坐标	// 终止坐标
	float m_start[30];
	float m_end[30];
	////////模型角度旋转坐标
	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	GLdouble zNear;
	GLdouble ZFar;
	BOOL m_bIsMove; //判断当前选中的是选择按钮还是放缩按钮
	//true为旋转,false为选取
	GLfloat m_x,m_y,m_z;
	BOOL mouseflag;
	//camera位置
	float camera_z;
	//视景深度
	float Visual_depth;
	int nWidth;
	int nHeight;
	//鼠标落于那个面上
	unsigned char mouse_locationflag;

	int sum;				//三角面片的数量

	//截面时，依次选取的3个点的坐标
	Vector3D point_section[3];
	int dandan;//确定了第几个点
	//实验过程中的单点曲线显示数组
	double SingleDatas[3][5000];//最大支持3个点显示
	double SingleTime[5000];//单点显示的时间数组
	//绘制等值面(体上)
	void Draw_Isosurface(TRIANGLE triangle[],GLfloat color[3],float isosurface_da,GLenum mode);

	//绘制等值线(面上)
	void Draw_Isogram(float *Vertex_Array,float *datanum,int Isogram_num);

	// 绘制实心体外轮廓
	void Draw_OutsideSurface(float *datanum,float *verdex_array,int *size_datanum,int length_num,int width_num,int height_num,GLenum mode);

	//分层绘制每层平面
	void Draw_MulFloor(float *datanum,float *verdex_array,int *size_datanum,/*int length_num,int width_num,*/int height_num,GLenum mode);

	//绘制单层
	void Draw_SigleFloor(float *datanum,float *size_rect,int *size_datanum,GLenum mode);

	//绘制NURBS单层曲面
	void Draw_Bezier_SigleFloor(float *vertex_array,float *datanum);

	//数值颜色对应关系
	BOOL ChangeValue2Color(/*float Max,float Min,*/float currvalue,float* color);

	void DrawStaff(float dmax, float dmin);//画标尺
	void Draw_CoordinateSystem();//画小坐标系
	void DrawPoint(float radius,bool flag);//画采集点,采集点数值显示
public:
	//////////////////////////////////////////////////////////////////////////
	//插值算法 Interpolation algorithm 
	//距离权重法 Distance weighted interpolation algorithm
	float DW_IA(float *Vertex_Array,int Vertex_num,float *datanumbers,float *Vertex,int n);
	//////////////////////////////////////////////////////////////////////////
	//双线性内插值算法 The Tri-linear interpolation algorithm 
	BOOL BI_IA(float *Vertex_Array,float *src_data,int *count,float *desk_data);
	//三线性内插值算法 The Tri-linear interpolation algorithm 
	BOOL TLI_IA(float *Vertex_Array,float *src_data,int *count,float *desk_data);
	//////////////////////////////////////////////////////////////////////////
public:
	afx_msg LRESULT OnMyMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// 鼠标射线捡练
	Vector3D GetSelectionRay(int mouse_x, int mouse_y);
	// 写坐标
	void PrintCoordinate(Vector3D pt);
	void DrawWell();
	Vector3D ThreePointToSurface();
	void FaceAndFaceCut(Vector3D coefficient);
	afx_msg void OnShowFront();
	afx_msg void OnShowSide();
	afx_msg void OnShowPlanform();
	void PrintTime();
	// 绘制平面
	void DrawFlatSurface(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// 绘制垂直于z面的平面
	void DrawCutFace_Z(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// 绘制垂直于y面的平面
	void DrawCutFace_Y(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// 绘制垂直于x面的平面
	void DrawCutFace_X(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);

	
	afx_msg void OnSetwell();
	afx_msg void OnMdissingle();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // lengnanView.cpp 中的调试版本
inline ClengnanDoc* ClengnanView::GetDocument()
   { return reinterpret_cast<ClengnanDoc*>(m_pDocument); }
#endif


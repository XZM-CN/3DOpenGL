// lengnanView.h : ClengnanView ��Ľӿ�
//


#pragma once
#include "lengnanDoc.h"
#include "IA.h"
#define WM_MY_MESSAGE	WM_USER+0x0001


class ClengnanView : public CView
{
protected: // �������л�����
	ClengnanView();
	DECLARE_DYNCREATE(ClengnanView)

// ����
public:
	ClengnanDoc* GetDocument();

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~ClengnanView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	    /************************************************************************/

    /* ���õı�����Rendering Context(��ɫ������)��ÿһ��OpenGL�������ӵ�һ����

    ɫ�������ϡ���ɫ���������е�OpenGL�����������ӵ�Device Context(�豸������)�ϡ�

    �ҽ�OpenGL����ɫ��������ΪhRC ��Ҫ�����ĳ����ܹ����ƴ��ڵĻ�������Ҫ����һ��

    �豸������Ҳ���ǵڶ��е����ݡ�Windows���豸����������ΪhDC ��DC���������ӵ�

    GDI(Graphics Device Interfaceͼ���豸�ӿ�)����RC��OpenGL���ӵ�DC                                                                     */

    /************************************************************************/

    HGLRC m_hRC;    //Rendering Context��ɫ������

    CDC* m_pDC;        //Device Context�豸������

    BOOL InitializeOpenGL();    //Initialize OpenGL

    BOOL SetupPixelFormat();    //�������ظ�ʽ

    void RenderScene();            //��Ⱦ����
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	//����ͼƬ����
	bool SaveBMPFromOpenGL(CString lpFileName);

	GLdouble aSpect;//�Ӿ���Ŀ�߱�

	//���߲���
	// ��ʼ����	// ��ֹ����
	float m_start[30];
	float m_end[30];
	////////ģ�ͽǶ���ת����
	GLfloat m_xAngle;
	GLfloat m_yAngle;
	GLfloat m_xPos;
	GLfloat m_yPos;
	CPoint m_MouseDownPoint;
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	GLdouble zNear;
	GLdouble ZFar;
	BOOL m_bIsMove; //�жϵ�ǰѡ�е���ѡ��ť���Ƿ�����ť
	//trueΪ��ת,falseΪѡȡ
	GLfloat m_x,m_y,m_z;
	BOOL mouseflag;
	//cameraλ��
	float camera_z;
	//�Ӿ����
	float Visual_depth;
	int nWidth;
	int nHeight;
	//��������Ǹ�����
	unsigned char mouse_locationflag;

	int sum;				//������Ƭ������

	//����ʱ������ѡȡ��3���������
	Vector3D point_section[3];
	int dandan;//ȷ���˵ڼ�����
	//ʵ������еĵ���������ʾ����
	double SingleDatas[3][5000];//���֧��3������ʾ
	double SingleTime[5000];//������ʾ��ʱ������
	//���Ƶ�ֵ��(����)
	void Draw_Isosurface(TRIANGLE triangle[],GLfloat color[3],float isosurface_da,GLenum mode);

	//���Ƶ�ֵ��(����)
	void Draw_Isogram(float *Vertex_Array,float *datanum,int Isogram_num);

	// ����ʵ����������
	void Draw_OutsideSurface(float *datanum,float *verdex_array,int *size_datanum,int length_num,int width_num,int height_num,GLenum mode);

	//�ֲ����ÿ��ƽ��
	void Draw_MulFloor(float *datanum,float *verdex_array,int *size_datanum,/*int length_num,int width_num,*/int height_num,GLenum mode);

	//���Ƶ���
	void Draw_SigleFloor(float *datanum,float *size_rect,int *size_datanum,GLenum mode);

	//����NURBS��������
	void Draw_Bezier_SigleFloor(float *vertex_array,float *datanum);

	//��ֵ��ɫ��Ӧ��ϵ
	BOOL ChangeValue2Color(/*float Max,float Min,*/float currvalue,float* color);

	void DrawStaff(float dmax, float dmin);//�����
	void Draw_CoordinateSystem();//��С����ϵ
	void DrawPoint(float radius,bool flag);//���ɼ���,�ɼ�����ֵ��ʾ
public:
	//////////////////////////////////////////////////////////////////////////
	//��ֵ�㷨 Interpolation algorithm 
	//����Ȩ�ط� Distance weighted interpolation algorithm
	float DW_IA(float *Vertex_Array,int Vertex_num,float *datanumbers,float *Vertex,int n);
	//////////////////////////////////////////////////////////////////////////
	//˫�����ڲ�ֵ�㷨 The Tri-linear interpolation algorithm 
	BOOL BI_IA(float *Vertex_Array,float *src_data,int *count,float *desk_data);
	//�������ڲ�ֵ�㷨 The Tri-linear interpolation algorithm 
	BOOL TLI_IA(float *Vertex_Array,float *src_data,int *count,float *desk_data);
	//////////////////////////////////////////////////////////////////////////
public:
	afx_msg LRESULT OnMyMessage(WPARAM wParam,LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	// ������߼���
	Vector3D GetSelectionRay(int mouse_x, int mouse_y);
	// д����
	void PrintCoordinate(Vector3D pt);
	void DrawWell();
	Vector3D ThreePointToSurface();
	void FaceAndFaceCut(Vector3D coefficient);
	afx_msg void OnShowFront();
	afx_msg void OnShowSide();
	afx_msg void OnShowPlanform();
	void PrintTime();
	// ����ƽ��
	void DrawFlatSurface(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// ���ƴ�ֱ��z���ƽ��
	void DrawCutFace_Z(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// ���ƴ�ֱ��y���ƽ��
	void DrawCutFace_Y(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	// ���ƴ�ֱ��x���ƽ��
	void DrawCutFace_X(float *datanum,float *verdex_array,int *size_datanum,GLenum mode);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);

	
	afx_msg void OnSetwell();
	afx_msg void OnMdissingle();
	virtual void OnInitialUpdate();
};

#ifndef _DEBUG  // lengnanView.cpp �еĵ��԰汾
inline ClengnanDoc* ClengnanView::GetDocument()
   { return reinterpret_cast<ClengnanDoc*>(m_pDocument); }
#endif


//////////////////////////////////////////////////////////////////////////
//创建人：	敖建
//email:	sejarce@yahoo.com.cn
//QQ:		421974083
//////////////////////////////////////////////////////////////////////////
//#pragma once
#ifndef _IA_H
#define _IA_H

#include <math.h>							// 数学库


#define	PI	3.14159	
//二维平面点模型
typedef struct tagPOINT2D
{
	float  x;
	float  y;
	float  data;
}POINT2D;

//三维空间点模型
typedef struct tagPOINT3D
{
	float  x;
	float  y;
	float  z;
	float  data;
}POINT3D;


//三维空间点坐标
typedef struct tagVector3D
{
	double  x;
	double  y;
	double  z;
}Vector3D;
//4维向量
typedef struct tagVector4D
{
	double  x;
	double  y;
	double  z;
	double	al;
}Vector4D;
//3维空间线段
typedef struct tagLine3D
{
	Vector3D pt[2];
}Line3D;

//4*4矩阵
typedef struct tagMatrix4D
{
	double  m[16];
}Matrix4D;
//面元模型
typedef struct tagFACECELL
{
	POINT2D Pt00,Pt10,Pt01,Pt11;
}FACECELL;

//三角面片
typedef struct tagTRIANGULE
{
	POINT3D Pt[3];
}TRIANGLE;

//体元模型
typedef struct tagCUBECELL
{
	POINT3D Pt[8];
}CUBECELL;

typedef struct bpatch {						// 贝塞尔面片结构
	POINT3D	anchors[10][10];					// 由4x4点组成
	GLuint		dlBPatch;					// 绘制面片的显示列表名称
	//GLuint		texture;					// 面片的纹理
} BEZIER_PATCH;



//////////////////////////////////////////////////////////////////////////
//向量的一些基本运算
//参考NEHE的OPENGL教程第28课
//////////////////////////////////////////////////////////////////////////
// 两个向量相加，p=p+q
inline POINT3D pointAdd(POINT3D p, POINT3D q) {
	p.x += q.x;		p.y += q.y;		p.z += q.z;
	return p;
}

// 向量和标量相乘p=c*p
inline POINT3D pointTimes(double c, POINT3D p) {
	p.x *= c;	p.y *= c;	p.z *= c;
	return p;
}

// 创建一个3D向量
inline POINT3D makePoint(double a, double b, double c) {
	POINT3D p;
	p.x = a;	p.y = b;	p.z = c;
	return p;
}

//阶乘
inline int factorial(int n){
	int m=1;
	if (n=0)
	{
		m=1;
	}else{
		while (n>0)
		{
			m*=n;
			n--;
		}
	}
	return m;
}


//////////////////////////////////////////////////////////////////////////
// 创建一个3D点
Vector3D MakePoint(double a, double b, double c);
//坐标变换
//点的一些坐标变换（平移，旋转，放缩）
//平移
Vector3D TranslationPoint(Vector3D pt,double x,double y,double z);
//放缩
Vector3D ScalingCoordinateSystem(Vector3D pt,double x,double y,double z);
//旋转，分成3个
//绕x轴旋转(逆时针)
Vector3D Rotate_X(Vector3D pt,double angle);
//绕y轴旋转
Vector3D Rotate_Y(Vector3D pt,double angle);
//绕z轴旋转
Vector3D Rotate_Z(Vector3D pt,double angle);
// 创建一个4维向量
Vector4D MakePoint(double a, double b, double c,double d);
//创建矩阵
Matrix4D MakeMatrix(double m[16]);
//平移矩阵
Matrix4D MakeTranslationMatrix(double x,double y,double z);
//旋转矩阵(x轴)
Matrix4D MakeRotateMatrix_X(double angle);
//旋转矩阵(y轴)
Matrix4D MakeRotateMatrix_Y(double angle);
//旋转矩阵(z轴)
Matrix4D MakeRotateMatrix_Z(double angle);
//矩阵乘法	4*4矩阵左乘4*1
Vector4D MultMatrix(Matrix4D mat,Vector4D v);
//矩阵求逆(4阶)
Matrix4D MatrixInversion(float mat[16]);
//----------------------------------------------
//功能: 求矩阵(n*n)的行列式
//入口参数: 矩阵的首地址，矩阵的行数
//返回值: 矩阵的行列式值
//----------------------------------------------
float MatDet(float *p, int n);
//----------------------------------------------
//功能: 求k*k矩阵中元素A(m, n)的代数余之式
//入口参数: k*k矩阵的首地址，矩阵元素A的下标m,n,矩阵行数k
//返回值: k*k矩阵中元素A(m, n)的代数余之式
//----------------------------------------------
float Creat_M(float *p, int m, int n, int k);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//求值器基于Bernstein多项式定义曲线，定义p(u ')为：
//p(u')=∑Bni(u')Ri
//
//这里Ri为控制点
//Bni(u')=[ni]u'i(1-u')n-i
//
//	且00=1,[n0]=1
//
//	u'=(u-u1)/(u2-u1)
//
//当为贝塞尔曲线时，控制点为4，相应的4个Bernstein多项式为:
//1、B30 =（1-u）3
//2、B31 =3u(1-u)2
//3、B32 =3u2(1-u)
//4、B33 =u3
//////////////////////////////////////////////////////////////////////////
// 计算贝塞尔方程的值
// 变量u的范围在0-1之间
POINT3D Bernstein(float u, POINT3D *p); 
// 生成贝塞尔曲面的显示列表
GLuint genBezier(BEZIER_PATCH patch, int divs);
// 设置贝塞尔曲面的控制点
void initBezier(BEZIER_PATCH mybezier,float *Vertex_Array,float *datanum);

//class CIA
//{
//public:
//	CIA(void);
//	~CIA(void);
//public:
	//空间插值算法 Interpolation algorithm 
	//////////////////////////////////////////////////////////////////////////
	//线性内插 linear interpolation
	inline float LIA(float pt1,float pt2,float data1,float data2,float Point_data)
	{
		float pt;
		pt=(pt2-pt1)*(Point_data-data1)/(data2-data1)+pt1;
		return pt;
	}
	//双线性内插 Bilinear Interpolation 
	float BLIA(float *vertex_array,float *src_datanum,float ptx,float pty);
	//三线性内插 Tri-linear Interpolation 
	float TLIA(float *vertex_array,float *src_datanum,float ptx,float pty,float ptz);
	//线性内插效果不理想，形成的三位图不平滑，棱角分明，不美观
	//////////////////////////////////////////////////////////////////////////
	//三次样条插值 cubic spline interpolation
	//获取参数数组
	void CSIA(int n,float x[],float y[],float *M);
	//获取内插点的值
	float CS2IA(int n,float x[],float y[],float xx,float *M);
	//直接获取样条插值内插点的值
	float CSIA_EX(int n,float x[],float y[],float xx);
	//双三次样条插值 double cubic spline interpolation
	void DCSIA(int nnx,int nny,int nx,int ny,float x[],float y[],float d[],float* fxy);
	//////////////////////////////////////////////////////////////////////////
	//三次多项式插值	Three times spline interpolation
	//(一次导数连续)
	//1.自然边界（边界点的导数为0），
	//2.夹持边界（边界点导数给定），
	//3.非扭结边界（使两端点的三阶导与这两端点的邻近点的三阶导相等）
	//可采用高斯消元法解n元1次方程组
	float* TTSplineIA(POINT2D pt[],int n,float *Coefficient_Matrix);
	//高斯消元法解n元1次方程组
	void GaussIA(float Ma[],int n,float M[]);
	//双三次多项式插值

	//////////////////////////////////////////////////////////////////////////
	//谢别德法 Shepard's Method
	//float ShepardIA(float *vertex_array,float *src_datanum,int n,float x,float y,float z);

	//Inverse Distance to a Power（反距离加权插值法）
	float IDTPIA(float *vertex_array,float *src_datanum,int sensors_n,float x,float y,float z,float min,float max);
	//八叉树分型算法
	int oc_tree(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,int init_n, float *size,float* datanum,int n); 
	//平面网格化
	int meshing2d(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,float *size_rect,float *datanum,int *size_num);
	//空间网格化
	int meshing3d(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,float *size_cube,float *datanum,int *size_num);

	//////////////////////////////////////////////////////////////////////////
	//等值面绘制算法
	//Marching Cubes algorithm 
	//①每次读入两张切片，形成一层（Layer）；
	//②两张切片上下相对应的四个点构成一个立方体（Cube），也叫Cell、Voxel等；
	//③从左到右，从下到上顺序处理一层中的立方体（抽取每个立方体中的等值面），然后从下向上顺序处理（n-1）层，算法结束
	//基于“分治（divide-and-conquer）”思想将整个数据场的等值面抽取分解到每一个体素中去完成
	int MarchingCubes(CUBECELL cubecell,float threshold,TRIANGLE triangle[]);
	//体元表
	int MarchingCubes_Table(CUBECELL cubecell,float threshold,unsigned char flag,TRIANGLE triangle[]);
	//求2个点的线性插值
	POINT3D Linear_Interpolation(POINT3D a,POINT3D b,float data);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//等值线绘制算法
	//一个面元上存在4个点,比较4各点与等值线值的大小，可存在几种情况：
	//1.4个点都大于或都小于等值线值，则面元与等值线无交线
	//2.4个点中有一个点或者3个点大于等值线值，则面元与等值线1条交线
	//3.4个点中有2个点大于等值线值，非对角，则面元与等值线1条交线
	//4.4个点中有2个点大于等值线值，对角，则面元与等值线2条交线（需判定二义性）
	//
	//面元划分足够小，则等值线会趋于平滑
	int MarchingFace(FACECELL cell,float Isogram_data,float *ver);
	//////////////////////////////////////////////////////////////////////////
//};

	bool BelongTo(int a,int b[],int size_b);

#endif
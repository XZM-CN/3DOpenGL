//////////////////////////////////////////////////////////////////////////
//�����ˣ�	����
//email:	sejarce@yahoo.com.cn
//QQ:		421974083
//////////////////////////////////////////////////////////////////////////
//#pragma once
#ifndef _IA_H
#define _IA_H

#include <math.h>							// ��ѧ��


#define	PI	3.14159	
//��άƽ���ģ��
typedef struct tagPOINT2D
{
	float  x;
	float  y;
	float  data;
}POINT2D;

//��ά�ռ��ģ��
typedef struct tagPOINT3D
{
	float  x;
	float  y;
	float  z;
	float  data;
}POINT3D;


//��ά�ռ������
typedef struct tagVector3D
{
	double  x;
	double  y;
	double  z;
}Vector3D;
//4ά����
typedef struct tagVector4D
{
	double  x;
	double  y;
	double  z;
	double	al;
}Vector4D;
//3ά�ռ��߶�
typedef struct tagLine3D
{
	Vector3D pt[2];
}Line3D;

//4*4����
typedef struct tagMatrix4D
{
	double  m[16];
}Matrix4D;
//��Ԫģ��
typedef struct tagFACECELL
{
	POINT2D Pt00,Pt10,Pt01,Pt11;
}FACECELL;

//������Ƭ
typedef struct tagTRIANGULE
{
	POINT3D Pt[3];
}TRIANGLE;

//��Ԫģ��
typedef struct tagCUBECELL
{
	POINT3D Pt[8];
}CUBECELL;

typedef struct bpatch {						// ��������Ƭ�ṹ
	POINT3D	anchors[10][10];					// ��4x4�����
	GLuint		dlBPatch;					// ������Ƭ����ʾ�б�����
	//GLuint		texture;					// ��Ƭ������
} BEZIER_PATCH;



//////////////////////////////////////////////////////////////////////////
//������һЩ��������
//�ο�NEHE��OPENGL�̵̳�28��
//////////////////////////////////////////////////////////////////////////
// ����������ӣ�p=p+q
inline POINT3D pointAdd(POINT3D p, POINT3D q) {
	p.x += q.x;		p.y += q.y;		p.z += q.z;
	return p;
}

// �����ͱ������p=c*p
inline POINT3D pointTimes(double c, POINT3D p) {
	p.x *= c;	p.y *= c;	p.z *= c;
	return p;
}

// ����һ��3D����
inline POINT3D makePoint(double a, double b, double c) {
	POINT3D p;
	p.x = a;	p.y = b;	p.z = c;
	return p;
}

//�׳�
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
// ����һ��3D��
Vector3D MakePoint(double a, double b, double c);
//����任
//���һЩ����任��ƽ�ƣ���ת��������
//ƽ��
Vector3D TranslationPoint(Vector3D pt,double x,double y,double z);
//����
Vector3D ScalingCoordinateSystem(Vector3D pt,double x,double y,double z);
//��ת���ֳ�3��
//��x����ת(��ʱ��)
Vector3D Rotate_X(Vector3D pt,double angle);
//��y����ת
Vector3D Rotate_Y(Vector3D pt,double angle);
//��z����ת
Vector3D Rotate_Z(Vector3D pt,double angle);
// ����һ��4ά����
Vector4D MakePoint(double a, double b, double c,double d);
//��������
Matrix4D MakeMatrix(double m[16]);
//ƽ�ƾ���
Matrix4D MakeTranslationMatrix(double x,double y,double z);
//��ת����(x��)
Matrix4D MakeRotateMatrix_X(double angle);
//��ת����(y��)
Matrix4D MakeRotateMatrix_Y(double angle);
//��ת����(z��)
Matrix4D MakeRotateMatrix_Z(double angle);
//����˷�	4*4�������4*1
Vector4D MultMatrix(Matrix4D mat,Vector4D v);
//��������(4��)
Matrix4D MatrixInversion(float mat[16]);
//----------------------------------------------
//����: �����(n*n)������ʽ
//��ڲ���: ������׵�ַ�����������
//����ֵ: ���������ʽֵ
//----------------------------------------------
float MatDet(float *p, int n);
//----------------------------------------------
//����: ��k*k������Ԫ��A(m, n)�Ĵ�����֮ʽ
//��ڲ���: k*k������׵�ַ������Ԫ��A���±�m,n,��������k
//����ֵ: k*k������Ԫ��A(m, n)�Ĵ�����֮ʽ
//----------------------------------------------
float Creat_M(float *p, int m, int n, int k);
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//��ֵ������Bernstein����ʽ�������ߣ�����p(u ')Ϊ��
//p(u')=��Bni(u')Ri
//
//����RiΪ���Ƶ�
//Bni(u')=[ni]u'i(1-u')n-i
//
//	��00=1,[n0]=1
//
//	u'=(u-u1)/(u2-u1)
//
//��Ϊ����������ʱ�����Ƶ�Ϊ4����Ӧ��4��Bernstein����ʽΪ:
//1��B30 =��1-u��3
//2��B31 =3u(1-u)2
//3��B32 =3u2(1-u)
//4��B33 =u3
//////////////////////////////////////////////////////////////////////////
// ���㱴�������̵�ֵ
// ����u�ķ�Χ��0-1֮��
POINT3D Bernstein(float u, POINT3D *p); 
// ���ɱ������������ʾ�б�
GLuint genBezier(BEZIER_PATCH patch, int divs);
// ���ñ���������Ŀ��Ƶ�
void initBezier(BEZIER_PATCH mybezier,float *Vertex_Array,float *datanum);

//class CIA
//{
//public:
//	CIA(void);
//	~CIA(void);
//public:
	//�ռ��ֵ�㷨 Interpolation algorithm 
	//////////////////////////////////////////////////////////////////////////
	//�����ڲ� linear interpolation
	inline float LIA(float pt1,float pt2,float data1,float data2,float Point_data)
	{
		float pt;
		pt=(pt2-pt1)*(Point_data-data1)/(data2-data1)+pt1;
		return pt;
	}
	//˫�����ڲ� Bilinear Interpolation 
	float BLIA(float *vertex_array,float *src_datanum,float ptx,float pty);
	//�������ڲ� Tri-linear Interpolation 
	float TLIA(float *vertex_array,float *src_datanum,float ptx,float pty,float ptz);
	//�����ڲ�Ч�������룬�γɵ���λͼ��ƽ������Ƿ�����������
	//////////////////////////////////////////////////////////////////////////
	//����������ֵ cubic spline interpolation
	//��ȡ��������
	void CSIA(int n,float x[],float y[],float *M);
	//��ȡ�ڲ���ֵ
	float CS2IA(int n,float x[],float y[],float xx,float *M);
	//ֱ�ӻ�ȡ������ֵ�ڲ���ֵ
	float CSIA_EX(int n,float x[],float y[],float xx);
	//˫����������ֵ double cubic spline interpolation
	void DCSIA(int nnx,int nny,int nx,int ny,float x[],float y[],float d[],float* fxy);
	//////////////////////////////////////////////////////////////////////////
	//���ζ���ʽ��ֵ	Three times spline interpolation
	//(һ�ε�������)
	//1.��Ȼ�߽磨�߽��ĵ���Ϊ0����
	//2.�гֱ߽磨�߽�㵼����������
	//3.��Ť��߽磨ʹ���˵�����׵��������˵���ڽ�������׵���ȣ�
	//�ɲ��ø�˹��Ԫ����nԪ1�η�����
	float* TTSplineIA(POINT2D pt[],int n,float *Coefficient_Matrix);
	//��˹��Ԫ����nԪ1�η�����
	void GaussIA(float Ma[],int n,float M[]);
	//˫���ζ���ʽ��ֵ

	//////////////////////////////////////////////////////////////////////////
	//л��·� Shepard's Method
	//float ShepardIA(float *vertex_array,float *src_datanum,int n,float x,float y,float z);

	//Inverse Distance to a Power���������Ȩ��ֵ����
	float IDTPIA(float *vertex_array,float *src_datanum,int sensors_n,float x,float y,float z,float min,float max);
	//�˲��������㷨
	int oc_tree(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,int init_n, float *size,float* datanum,int n); 
	//ƽ������
	int meshing2d(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,float *size_rect,float *datanum,int *size_num);
	//�ռ�����
	int meshing3d(float *vertex_array,float *src_datanum,int sensors_n,float *ver_arr,float *size_cube,float *datanum,int *size_num);

	//////////////////////////////////////////////////////////////////////////
	//��ֵ������㷨
	//Marching Cubes algorithm 
	//��ÿ�ζ���������Ƭ���γ�һ�㣨Layer����
	//��������Ƭ�������Ӧ���ĸ��㹹��һ�������壨Cube����Ҳ��Cell��Voxel�ȣ�
	//�۴����ң����µ���˳����һ���е������壨��ȡÿ���������еĵ�ֵ�棩��Ȼ���������˳����n-1���㣬�㷨����
	//���ڡ����Σ�divide-and-conquer����˼�뽫�������ݳ��ĵ�ֵ���ȡ�ֽ⵽ÿһ��������ȥ���
	int MarchingCubes(CUBECELL cubecell,float threshold,TRIANGLE triangle[]);
	//��Ԫ��
	int MarchingCubes_Table(CUBECELL cubecell,float threshold,unsigned char flag,TRIANGLE triangle[]);
	//��2��������Բ�ֵ
	POINT3D Linear_Interpolation(POINT3D a,POINT3D b,float data);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	//��ֵ�߻����㷨
	//һ����Ԫ�ϴ���4����,�Ƚ�4�������ֵ��ֵ�Ĵ�С���ɴ��ڼ��������
	//1.4���㶼���ڻ�С�ڵ�ֵ��ֵ������Ԫ���ֵ���޽���
	//2.4��������һ�������3������ڵ�ֵ��ֵ������Ԫ���ֵ��1������
	//3.4��������2������ڵ�ֵ��ֵ���ǶԽǣ�����Ԫ���ֵ��1������
	//4.4��������2������ڵ�ֵ��ֵ���Խǣ�����Ԫ���ֵ��2�����ߣ����ж������ԣ�
	//
	//��Ԫ�����㹻С�����ֵ�߻�����ƽ��
	int MarchingFace(FACECELL cell,float Isogram_data,float *ver);
	//////////////////////////////////////////////////////////////////////////
//};

	bool BelongTo(int a,int b[],int size_b);

#endif
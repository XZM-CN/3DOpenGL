#include "stdafx.h"
#include "IA.h"
#include "math.h"

static float fxx[27*1000]={0};
static float ddd[30]={0};
static float MMM[30]={0};
static float csh[30]={0};
static float csu[30]={0};
static float csd[30]={0};
static float csMa[1000]={0};

int error_num;
//CIA::CIA(void)
//{
//}
//
//CIA::~CIA(void)
//{
//}
//////////////////////////////////////////////////////////////////////////
//谢别德法 Shepard's Method
//利用已知点插未知点,使用距离倒数加权的最小二乘方的方法
//min为圆滑阈值,max为加权最大阈值
float IDTPIA(float *vertex_array,float *src_datanum,int sensors_n,float x,float y,float z,float min,float max)
{
	float data=0;
	double range[1000]={0};
	float sum=0;
	for (int i=0;i<sensors_n;i++)
	{
		range[i]=(vertex_array[3*i]-x)*(vertex_array[3*i]-x)+(vertex_array[3*i+1]-y)*(vertex_array[3*i+1]-y)+(vertex_array[3*i+1]-z)*(vertex_array[3*i+1]-z);
		if (range[i]<min)
		{
			range[i]=1/sqrt(min);
			data=range[i];
			return data;
		}else if (range[i]>max)
		{
			range[i]=0;
		}else{
			range[i]=1/sqrt(range[i]);
		}
		sum+=range[i];
	}	
	for (int i=0;i<sensors_n;i++)
	{
		data+=(range[i]/sum)*src_datanum[i];
	}

	return data;
}
//八叉树递归算法,网格化
//左下前 0，右下前 1，左下后 2，右下后 3，
//左上前 4，右上前 5，左上后 6，右上后 7
//n为递归的次数，尽量不要大于8!取6或7为佳
//datanum[]存储数据量


int oc_tree(float *vertex_array,
				 float *src_datanum,
				 int sensors_n,
				 float *ver_arr,		//ver_arr[3]左下前点坐标
				 int init_n,			//左下前点数据在数据数组中的下标
				 float *size_cube,			//要分型的立方体大小
				 float* datanum,		//储存各点数据
				 int n)					//分块次数			 
{
	int ver_num;
	if (n==0)
	{
		ver_num=8;
		return ver_num;
	}
	float x,y,z;
	int index=0;
	int m=1;
	for (int i=0;i<n-1;i++)
	{
		m*=2;
	}
	int m1,m2;
	m1=m*(2*m+1);
	m2=m*(2*m+1)*(2*m+1);
	for (int i=0;i<27;i++)
	{
		index=(i/9)*m2+((i%9)/3)*m1+(i%3)*m+init_n;
		x=ver_arr[0]+(size_cube[0]/2)*(i%3);
		y=ver_arr[1]+(size_cube[1]/2)*((i%9)/3);
		z=ver_arr[2]+(size_cube[2]/2)*(i/9);
		*(datanum+index)=IDTPIA(vertex_array,src_datanum,sensors_n,x,y,z,0.0004,0.25);
	}
	//八叉树分块
	float ver_arr1[3];
	float init_n1;
	float size_cube1[3];

	//左下前块 0
	ver_arr1[0]=ver_arr[0];
	ver_arr1[1]=ver_arr[1];
	ver_arr1[2]=ver_arr[2];
	init_n1=init_n;
	size_cube1[0]=size_cube[0]/2;
	size_cube1[1]=size_cube[1]/2;
	size_cube1[2]=size_cube[2]/2;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//右下前块 1
	ver_arr1[0]=ver_arr[0]+size_cube[0]/2;
	init_n1=m+init_n;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//左下后块 2
	ver_arr1[0]=ver_arr[0];
	ver_arr1[1]=ver_arr[1]+size_cube[1]/2;
	init_n1=m1+init_n;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//右下后块 3
	ver_arr1[0]=ver_arr[0]+size_cube[0]/2;
	init_n1=init_n+m1+m;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//左上前块 4
	ver_arr1[0]=ver_arr[0];
	ver_arr1[1]=ver_arr[1];
	ver_arr1[2]=ver_arr[2]+size_cube[2]/2;
	init_n1=init_n+m2;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//右上前块 5
	ver_arr1[0]=ver_arr[0]+size_cube[0]/2;
	init_n1=init_n+m2+m;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//左上后块 6
	ver_arr1[0]=ver_arr[0];
	ver_arr1[1]=ver_arr[1]+size_cube[1]/2;
	init_n1=init_n+m2+m1;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	//左上前块 7
	ver_arr1[0]=ver_arr[0]+size_cube[0]/2;
	init_n1=init_n+m2+m1+m;
	oc_tree(vertex_array,src_datanum,sensors_n,ver_arr1,init_n1,size_cube1,datanum,n-1);

	ver_num=(2*m+1)*(2*m+1)*(2*m+1);
	return ver_num;
}

//空间网格化
int meshing3d(float *vertex_array,
				 float *src_datanum,
				 int sensors_n,
				 float *ver_arr,		//左下前点坐标
				 float *size_cube,		//要分型的矩形大小
				 float *datanum,		//储存各点数据
				 int *size_num)		//各个轴向上网格的数量
{
	int i,j,k,index=0;
	float x,y,z;
	float dx,dy,dz;
	float data;
	dx=size_cube[0]/size_num[0];
	dy=size_cube[1]/size_num[1];
	dz=size_cube[2]/size_num[2];
	for(k=0;k<size_num[2]+1;k++)
	{
		for(j=0;j<size_num[1]+1;j++)
		{
			for(i=0;i<size_num[0]+1;i++)
			{
				x=ver_arr[0]+dx*i;
				y=ver_arr[1]+dy*j;
				z=ver_arr[2]+dz*k;
				*(datanum+index)=IDTPIA(vertex_array,src_datanum,sensors_n,x,y,z,0.0004,0.09);
				data=*(datanum+index);
				//datanum[k][j][i]=IDTPIA(vertex_array,src_datanum,sensors_n,x,y,z,0.0004,0.25);
				index++;
			}
		}
	}
	return index;
}

//平面网格化
int meshing2d(float *vertex_array,
				   float *src_datanum,
				   int sensors_n,
				   float *ver_arr,		//左下点坐标
				   float *size_rect,		//要分型的矩形大小
				   float *datanum,		//储存各点数据
				   int *size_num)		//各个轴向上网格的数量
{
	int i,j,index=0;
	float x,y,z;
	float dx,dy;
	dx=size_rect[0]/size_num[0];
	dy=size_rect[1]/size_num[1];
	z=ver_arr[2];
	for(j=0;j<size_num[1]+1;j++)
	{
		for(i=0;i<size_num[0]+1;i++)
		{
			x=ver_arr[0]+dx*i;
			y=ver_arr[1]+dy*j;
			*(datanum+index)=IDTPIA(vertex_array,src_datanum,sensors_n,x,y,z,0.0004,0.25);
			//datanum[j][i]=IDTPIA(vertex_array,src_datanum,sensors_n,x,y,z,0.0004,0.25);
			index++;
		}
	}
	return index;
}


float BLIA(float *vertex_array,float *src_datanum,float ptx,float pty)
{
	float data=0;
	float a=0,b=0;
	a=(ptx-vertex_array[0])*(src_datanum[1]-src_datanum[0])/(vertex_array[2]-vertex_array[0])+src_datanum[0];
	b=(ptx-vertex_array[4])*(src_datanum[3]-src_datanum[2])/(vertex_array[6]-vertex_array[4])+src_datanum[2];
	data=(pty-vertex_array[1])*(b-a)/(vertex_array[5]-vertex_array[1])+a;
	return data;
}

float TLIA(float *vertex_array,float *src_datanum,float ptx,float pty,float ptz)
{
	float data,data1,data2;
	float vertex_array1[16];
	for(int i=0;i<8;i++)
	{
		vertex_array1[2*i]=vertex_array[3*i];
		vertex_array1[2*i+1]=vertex_array[3*i+1];
	}
	data1=BLIA(vertex_array1,src_datanum,ptx,pty);
	data2=BLIA(vertex_array1+8,src_datanum+4,ptx,pty);
	data=(ptz-vertex_array1[2])*(data2-data1)/(vertex_array1[14]-vertex_array1[2])+data1;
	return data;
}

BEZIER_PATCH		mybezier;					// 创建一个贝塞尔曲面结构

// 计算贝塞尔方程的值
// 变量u的范围在0-1之间
// Bezier参数曲线上各点坐标的插值公式,n+1为控制点数
POINT3D Bernstein(int n,float u, POINT3D *p)
{
	int a,c;
	float b,d;
	POINT3D Pt;
	POINT3D	r;
	r.x=0.0;
	r.y=0.0;
	r.z=0.0;
	r.data=0.0;
	for(int i=0;i<n+1;i++)
	{
		a=factorial(n);
		if ((u==0&&i==0)||(u==1,i==n))
		{
			b=1;
		}else{
			b=pow(u,i)*pow(1-u,n-i);
		}
		c=factorial(i);
		c*=factorial(n-i);
		d=(float)a*b/(float)c;
		Pt = pointTimes(d, p[0]);
		r=pointAdd(r,Pt);
	}
	return r;
}

// 生成贝塞尔曲面的显示列表
GLuint genBezier(BEZIER_PATCH patch, int divs) 
{
	GLuint a=0;
	return a;
}
// 设置贝塞尔曲面的控制点(m行n列)
void initBezier(int m,int n,float *Vertex_Array,float *datanum)
{
	mybezier.anchors[0][0] = makePoint(-0.75,	-0.75,	-0.50);	// 设置贝塞尔曲面的控制点
	mybezier.anchors[0][1] = makePoint(-0.25,	-0.75,	 0.00);
	mybezier.anchors[0][2] = makePoint( 0.25,	-0.75,	 0.00);
	mybezier.anchors[0][3] = makePoint( 0.75,	-0.75,	-0.50);
	mybezier.anchors[1][0] = makePoint(-0.75,	-0.25,	-0.75);
	mybezier.anchors[1][1] = makePoint(-0.25,	-0.25,	 0.50);
	mybezier.anchors[1][2] = makePoint( 0.25,	-0.25,	 0.50);
	mybezier.anchors[1][3] = makePoint( 0.75,	-0.25,	-0.75);
	mybezier.anchors[2][0] = makePoint(-0.75,	 0.25,	 0.00);
	mybezier.anchors[2][1] = makePoint(-0.25,	 0.25,	-0.50);
	mybezier.anchors[2][2] = makePoint( 0.25,	 0.25,	-0.50);
	mybezier.anchors[2][3] = makePoint( 0.75,	 0.25,	 0.00);
	mybezier.anchors[3][0] = makePoint(-0.75,	 0.75,	-0.50);
	mybezier.anchors[3][1] = makePoint(-0.25,	 0.75,	-1.00);
	mybezier.anchors[3][2] = makePoint( 0.25,	 0.75,	-1.00);
	mybezier.anchors[3][3] = makePoint( 0.75,	 0.75,	-0.50);

}

//返回值为面元与等值线交线的条数
int MarchingFace(FACECELL cell,			//面元信息（坐标，属性值）
				 float Isogram_data,	//等值线值
				 float *ver)			//等值线与面元的交点坐标数组
{
	unsigned char flag=0x00;
	int line_num=0;
	if (Isogram_data>cell.Pt00.data)
	{
		flag|=0x01;
	}
	if (Isogram_data>cell.Pt10.data)
	{
		flag|=0x02;
	}
	if (Isogram_data>cell.Pt11.data)
	{
		flag|=0x04;
	}
	if (Isogram_data>cell.Pt01.data)
	{
		flag|=0x08;
	}
	//合并模式
	switch(flag)
	{
	case 0x00:
		ver=NULL;
		line_num=0;
		break;
	case 0x01:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//下边交点
		*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+3)=cell.Pt00.y;
		break;
	case 0x02:
		line_num=1;
		//右边交点
		*ver=cell.Pt11.x;
		*(ver+1)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		//下边交点
		*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+3)=cell.Pt00.y;
		break;
	case 0x03:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//右边交点
		*(ver+2)=cell.Pt11.x;
		*(ver+3)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		break;
	case 0x04:
		line_num=1;
		//右边交点
		*ver=cell.Pt11.x;
		*(ver+1)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x05:
		line_num=2;
		//二义性
		//中心点判别
		if ((cell.Pt00.data+cell.Pt01.data+cell.Pt10.data+cell.Pt11.data)>Isogram_data*4)
		{
			//左边交点
			*ver=cell.Pt00.x;
			*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
			//下边交点
			*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
			*(ver+3)=cell.Pt00.y;
			//右边交点
			*(ver+4)=cell.Pt11.x;
			*(ver+5)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
			//上边交点
			*(ver+6)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
			*(ver+7)=cell.Pt11.y;
		}else{
			//左边交点
			*ver=cell.Pt00.x;
			*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
			//上边交点
			*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
			*(ver+3)=cell.Pt11.y;
			//右边交点
			*(ver+4)=cell.Pt11.x;
			*(ver+5)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
			//下边交点
			*(ver+6)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
			*(ver+7)=cell.Pt00.y;
		}
		break;
	case 0x06:
		line_num=1;
		//下边交点
		*ver=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+1)=cell.Pt00.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x07:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x08:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x09:
		line_num=1;
		//下边交点
		*ver=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+1)=cell.Pt00.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x0a:
		line_num=2;
		//二义性
		if ((cell.Pt00.data+cell.Pt01.data+cell.Pt10.data+cell.Pt11.data)>Isogram_data*4)
		{
			//左边交点
			*ver=cell.Pt00.x;
			*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
			//上边交点
			*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
			*(ver+3)=cell.Pt11.y;
			//右边交点
			*(ver+4)=cell.Pt11.x;
			*(ver+5)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
			//下边交点
			*(ver+6)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
			*(ver+7)=cell.Pt00.y;
		}else{
			//左边交点
			*ver=cell.Pt00.x;
			*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
			//下边交点
			*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
			*(ver+3)=cell.Pt00.y;
			//右边交点
			*(ver+4)=cell.Pt11.x;
			*(ver+5)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
			//上边交点
			*(ver+6)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
			*(ver+7)=cell.Pt11.y;
		}
		break;
	case 0x0b:
		line_num=1;
		//右边交点
		*ver=cell.Pt11.x;
		*(ver+1)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		//上边交点
		*(ver+2)=(cell.Pt11.x-cell.Pt01.x)*(Isogram_data-cell.Pt01.data)/(cell.Pt11.data-cell.Pt01.data)+cell.Pt01.x;
		*(ver+3)=cell.Pt11.y;
		break;
	case 0x0c:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//右边交点
		*(ver+2)=cell.Pt11.x;
		*(ver+3)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		break;
	case 0x0d:
		line_num=1;
		//右边交点
		*ver=cell.Pt11.x;
		*(ver+1)=(cell.Pt11.y-cell.Pt10.y)*(Isogram_data-cell.Pt10.data)/(cell.Pt11.data-cell.Pt10.data)+cell.Pt10.y;
		//下边交点
		*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+3)=cell.Pt00.y;
		break;
	case 0x0e:
		line_num=1;
		//左边交点
		*ver=cell.Pt00.x;
		*(ver+1)=(cell.Pt01.y-cell.Pt00.y)*(Isogram_data-cell.Pt00.data)/(cell.Pt01.data-cell.Pt00.data)+cell.Pt00.y;
		//下边交点
		*(ver+2)=(cell.Pt10.x-cell.Pt00.x)*(Isogram_data-cell.Pt00.data)/(cell.Pt10.data-cell.Pt00.data)+cell.Pt00.x;
		*(ver+3)=cell.Pt00.y;
		break;
	case 0x0f:
		line_num=0;
		ver=NULL;
		break;
	default:
		break;
	}
	return line_num;
}

float* TTSplineIA(POINT2D pt[],int n,float *Coefficient_Matrix)
{
	return Coefficient_Matrix;
}



//双三次样条插值 double cubic spline interpolation
//nx+1为x的点数，ny+1为y的点数,至少为2;
//x[]由小到大次序排列，d[i]=fx(x[i]);y[]由小到大次序排列，d[i]=fy(y[i]);
//并且x[0]=<xx=<x[n];并且y[0]=<yy=<y[n]
//nnxx为每个方格内x方向，y方向内插的点数
//相当于x轴方向有nx*nnxx+1个点,相当于y轴方向有ny*nnyy+1个点
//函数返回：		点阵的值
//采用先y后x
void DCSIA(int nnxx,int nnyy,int nx,int ny,float x[],float y[],float d[],float* fxy)
{
	float xx,yy,dx,dy;
	int i,j,k;
	int index=0;
	int xlong,ylong;
	xlong=nx*(nnxx+1)+1;
	ylong=ny*(nnyy+1)+1;
	float temp=0.0;
	//float *fxx=new float[xlong*(ny+1)];	
	//float *MMM=new float[nx+1];
	//float *ddd=new float[nx+1];
	//每条y轴上,y方向上
	for (j=0;j<2000;j++)
	{
		fxx[j]=0;
	}
	for (i=0;i<nx+1;i++)		//i为列
	{
		for (j=0;j<ny+1;j++)
		{
			ddd[j]=d[j*(nx+1)+i];
		}
		//MMM清零
		for (j=0;j<30;j++)
		{
			MMM[j]=0;
		}
		CSIA(ny,y,ddd,MMM);
		//每2个控制点之间
		for (j=0;j<ny;j++)		//j为行
		{
			dy=y[j+1]-y[j];
			dy/=(nnyy+1);
			for (k=0;k<nnyy+1;k++)
			{
				yy=y[j]+dy*k;
				fxx[(j*(nnyy+1)+k)*(nx+1)+i]=CS2IA(ny,y,ddd,yy,MMM);
				//fxx[i*xlong+j*nnxx+k]=CS2IA(ny,y,ddd,yy,MMM);
			}			
			//fxx[(i+1)*xlong-1]=ddd[nx];
		}
		fxx[(ylong-1)*(nx+1)+i]=ddd[ny];
	}
	for (i=0;i<ylong;i++)
	{
		for (j=0;j<nx+1;j++)
		{
			ddd[j]=fxx[i*(nx+1)+j];	//???
		}
		//MMM清零
		for (j=0;j<30;j++)
		{
			MMM[j]=0;
		}
		CSIA(nx,x,ddd,MMM);	
		//每2个控制点之间
		for (j=0;j<nx;j++)
		{
			index=i*xlong+j*(nnxx+1);//(i,j)点
			dx=x[j+1]-x[j];
			dx/=(nnxx+1);
			for (k=0;k<nnxx+2;k++)
			{
				xx=x[j]+dx*k;
				temp=CS2IA(nx,x,ddd,xx,MMM);
				fxy[index]=temp;
				index++;
			}
		}
	}
	//delete[] ddd;
	//delete[] MMM;
	//delete[] fxx;
	//return fxy;
}

//此函数似乎有bug！
float CS2IA(int n,float x[],float y[],float xx,float *M)
{
	float fuu;
	//判断xx在那个区间内
	int i=0,k;
	float h[30]={0};
	if (xx>x[n]||xx<x[0])
	{
		return 0;
	}
	while(xx-x[i]>=0&&i<n)
	{
		i++;
	}
	k=i>n?n:i;

	for (i=1;i<n+1;i++)
	{
		h[i]=x[i]-x[i-1];
	}

	fuu=(M[k-1]*(pow((x[k]-xx),3))+M[k]*(pow((xx-x[k-1]),3)))/(6*h[k]);
	fuu+=y[k-1]*(x[k]-xx)/h[k]-M[k-1]*h[k]*(x[k]-xx)/6;
	fuu+=y[k]*(xx-x[k-1])/h[k]-M[k]*h[k]*(xx-x[k-1])/6;

	//delete[] h;
	return fuu;
}

//三次样条插值
float CSIA_EX(int n,float x[],float y[],float xx)
{
	int i=0,index=0;
	float *M=new float[30];


	for (i=1;i<n+1;i++)
	{
		csh[i]=x[i]-x[i-1];//h[i]=x[i]-x[i-1]
	}
	for (i=1;i<n;i++)
	{
		csu[i]=csh[i]/(csh[i]+csh[i+1]);
		csd[i]=6*((y[i+1]-y[i])/csh[i+1]-(y[i]-y[i-1])/csh[i])/(csh[i]+csh[i+1]);
	}
	//边界条件(自然边界)
	float f0=0,fn=0;
	csu[0]=0;
	csu[n]=1;
	csd[0]=6*((y[1]-y[0])/csh[1]-f0)/csh[1];
	csd[n]=6*(fn-(y[n]-y[n-1])/csh[n])/csh[n];

	for (i=0;i<1000;i++)
	{
		csMa[i]=0;
	}

	for (i=0;i<n+1;i++)
	{
		index=i*(n+2);
		csMa[index]=2;
		if (i!=0)
		{
			csMa[index-n-1]=csu[i];
		}
		if (i!=n)
		{
			csMa[index+n+1]=1-csu[i];
		}
		csMa[(n+1)*(n+1)+i]=csd[i];
	}
	GaussIA(csMa,n+1,M);

	float fuu;
	//判断xx在那个区间内
	int k;
	float h[30]={0};
	if (xx>x[n]||xx<x[0])
	{
		return 0;
	}
	i=0;
	while(xx-x[i]>0)
	{
		i++;
	}
	k=i>1?i:1;

	for (i=1;i<n+1;i++)
	{
		h[i]=x[i]-x[i-1];
	}

	fuu=(M[k-1]*(pow((x[k]-xx),3))+M[k]*(pow((xx-x[k-1]),3)))/(6*h[k]);
	fuu+=y[k-1]*(x[k]-xx)/h[k]-M[k-1]*h[k]*(x[k]-xx)/6;
	fuu+=y[k]*(xx-x[k-1])/h[k]-M[k]*h[k]*(xx-x[k-1])/6;

	delete [] M;

	return fuu;
}

//三次样条插值 cubic spline interpolation
//自然边界（边界导数为0）
//n+1为x的点数，至少为2;
//x[]由小到大次序排列，y[i]=f(x[i]);
//并且x[0]=<xx=<x[n];
//函数返回 参数数组M
void CSIA(int n,float x[],float y[],float *M)
{	
	int i=0,index=0;
	////float *M=new float[n+1]();
	//float *csh=new float[n];
	//float *csu=new float[n+1];
	//float *csd=new float[n+1];
	//float *csMa=new float[(n+2)*(n+1)];
	
	for (i=1;i<n+1;i++)
	{
		csh[i]=x[i]-x[i-1];//h[i]=x[i]-x[i-1]
	}
	for (i=1;i<n;i++)
	{
		csu[i]=csh[i]/(csh[i]+csh[i+1]);
		csd[i]=6*((y[i+1]-y[i])/csh[i+1]-(y[i]-y[i-1])/csh[i])/(csh[i]+csh[i+1]);
	}
	//边界条件(自然边界)
	float f0=0,fn=0;
	csu[0]=0;
	csu[n]=1;
	csd[0]=6*((y[1]-y[0])/csh[1]-f0)/csh[1];
	csd[n]=6*(fn-(y[n]-y[n-1])/csh[n])/csh[n];

	for (i=0;i<1000;i++)
	{
		csMa[i]=0;
	}

	for (i=0;i<n+1;i++)
	{
		index=i*(n+2);
		csMa[index]=2;
		if (i!=0)
		{
			csMa[index-n-1]=csu[i];
		}
		if (i!=n)
		{
			csMa[index+n+1]=1-csu[i];
		}
		csMa[(n+1)*(n+1)+i]=csd[i];
	}

	GaussIA(csMa,n+1,M);

	//delete[] csMa;
	//delete[] csd;
	//delete[] csu;
	//delete[] csh;
	//delete M;
	//return M;
}


//函数功能：	高斯消元法解n元1次方程组
//函数参数：	Ma为n*(n+1)矩阵，存储为先列后行，M为方程组唯一解集
//返回值：	无
void GaussIA(float Ma[],int n,float M[])
{
	int i=0,j=0,k=0;
	float temp=0;
	//这个大循环将数组的左下角转化为0
	for (i=0;i<n;i++)
	{
		//对角线上的数为0的话，变为不为0;
		while (Ma[i*(n+1)]==0)
		{
			for (j=i+1;j<n;j++)		//j为行
			{
				for(k=i;k<n+1;k++)	//k为列
				{
					//Ma[i*(n+1)+k]+=Ma[j*(n+1)+k];
					Ma[k*n+i]+=Ma[k*n+j];
				}
			}
		}
		//将第i列变为1
		for (j=i;j<n;j++)			//j为行
		{
			if (Ma[i*n+j]!=0)
			{
				temp=Ma[i*n+j];
				for (k=i;k<n+1;k++)	//k为列
				{
					//Ma[j*(n+1)+k]/=temp;
					Ma[k*n+j]/=temp;
				}
			}
		}
		//将下面的数列与上面的数列相减使其下面为0
		for(j=i+1;j<n;j++)			//j为行
		{
			if (Ma[i*n+j]!=0)
			{
				for(k=i;k<n+1;k++)	//k为列
				{
					//Ma[j*(n+1)+k]-=Ma[i*(n+1)+k];
					Ma[k*n+j]-=Ma[k*n+i];
				}
			}
		}
	}
	//这个大循环将数组的右上角转化为0
	for (i=n-1;i>=0;i--)		//i为列
	{
		for (j=0;j<i;j++)		//j为行
		{
			Ma[n*n+j]-=Ma[n*n+i]*Ma[i*n+j];
			Ma[i*n+j]=0;
		}
	}
	for (i=0;i<n;i++)
	{
		M[i]=Ma[n*n+i];
	}
}
//////////////////////////////////////////////////////////////////////////
// 创建一个3D点
Vector3D MakePoint(double a, double b, double c) {
	Vector3D p;
	p.x = a;	p.y = b;	p.z = c;
	return p;
}
//坐标变换
//点的一些坐标变换（平移，旋转，放缩）
//平移
Vector3D TranslationPoint(Vector3D pt,double x,double y,double z){
	pt.x-=x;	pt.y-=y;	pt.z-=z;
	return pt;
}
//放缩
Vector3D ScalingCoordinateSystem(Vector3D pt,double x,double y,double z){
	pt.x*=x;	pt.y*=y;	pt.z*=z;
	return pt;
}
//旋转，分成3个
//绕x轴旋转(逆时针)
Vector3D Rotate_X(Vector3D pt,double angle){
	Vector3D v;
	angle*=PI/180;
	v.x=pt.x;
	v.y=pt.y*cos(angle)-pt.z*sin(angle);
	v.z=pt.z*cos(angle)+pt.y*sin(angle);
	return v;
}
//绕y轴旋转
Vector3D Rotate_Y(Vector3D pt,double angle){
	Vector3D v;
	angle*=PI/180;
	v.x=pt.x*cos(angle)+pt.z*sin(angle);
	v.y=pt.y;
	v.z=pt.z*cos(angle)-pt.x*sin(angle);
	return v;
}
//绕z轴旋转
Vector3D Rotate_Z(Vector3D pt,double angle){
	angle*=PI/180;
	Vector3D v;
	v.x=pt.x*cos(angle)-pt.y*sin(angle);
	v.y=pt.y*cos(angle)+pt.x*sin(angle);
	v.z=pt.z;
	return v;
}
// 创建一个4维向量
Vector4D MakePoint(double a, double b, double c,double d) {
	Vector4D p;
	p.x = a;	p.y = b;	p.z = c;	p.al=d;
	return p;
}
//平移矩阵
Matrix4D MakeTranslationMatrix(double x,double y,double z){
	Matrix4D matrix4;
	matrix4.m[0]=1;	matrix4.m[1]=0;	matrix4.m[2]=0;	matrix4.m[3]=0;
	matrix4.m[4]=0;	matrix4.m[5]=1;	matrix4.m[6]=0;	matrix4.m[7]=0;
	matrix4.m[8]=0;	matrix4.m[9]=0;	matrix4.m[10]=1;	matrix4.m[11]=0;
	matrix4.m[12]=x;	matrix4.m[13]=y;	matrix4.m[14]=z;	matrix4.m[15]=1;
	return matrix4;
}
//旋转矩阵(x轴)
Matrix4D MakeRotateMatrix_X(double angle){
	Matrix4D matrix4;
	angle*=PI/180;
	matrix4.m[0]=1;		matrix4.m[1]=0;		matrix4.m[2]=0;		matrix4.m[3]=0;
	matrix4.m[4]=0;	matrix4.m[5]=cos(angle);	matrix4.m[6]=-sin(angle);	matrix4.m[7]=0;
	matrix4.m[8]=0;	matrix4.m[9]=sin(angle);	matrix4.m[10]=cos(angle);	matrix4.m[11]=0;
	matrix4.m[12]=0;	matrix4.m[13]=0;	matrix4.m[14]=0;	matrix4.m[15]=1;
	return matrix4;
}
//旋转矩阵(y轴)
Matrix4D MakeRotateMatrix_Y(double angle){
	Matrix4D matrix4;
	angle*=PI/180;
	matrix4.m[0]=cos(angle);	matrix4.m[1]=0;		matrix4.m[2]=sin(angle);	matrix4.m[3]=0;
	matrix4.m[4]=0;	matrix4.m[5]=1;	matrix4.m[6]=0;	matrix4.m[7]=0;
	matrix4.m[8]=-sin(angle);	matrix4.m[9]=0;	matrix4.m[10]=cos(angle);	matrix4.m[11]=0;
	matrix4.m[12]=0;	matrix4.m[13]=0;	matrix4.m[14]=0;	matrix4.m[15]=1;
	return matrix4;
}
//旋转矩阵(z轴)
Matrix4D MakeRotateMatrix_Z(double angle){
	Matrix4D matrix4;
	angle*=PI/180;
	matrix4.m[0]=cos(angle);	matrix4.m[1]=sin(angle);	matrix4.m[2]=0;	matrix4.m[3]=0;
	matrix4.m[4]=-sin(angle);	matrix4.m[5]=cos(angle);	matrix4.m[6]=0;	matrix4.m[7]=0;
	matrix4.m[8]=0;	matrix4.m[9]=0;	matrix4.m[10]=1;	matrix4.m[11]=0;
	matrix4.m[12]=0;	matrix4.m[13]=0;	matrix4.m[14]=0;	matrix4.m[15]=1;
	return matrix4;
}
//矩阵乘法	4*4矩阵左乘4*1
Vector4D MultMatrix(Matrix4D mat,Vector4D v){
	v.x=mat.m[0]*v.x+mat.m[4]*v.y+mat.m[8]*v.z+mat.m[12]*v.al;
	v.y=mat.m[1]*v.x+mat.m[5]*v.y+mat.m[9]*v.z+mat.m[13]*v.al;
	v.z=mat.m[2]*v.x+mat.m[6]*v.y+mat.m[10]*v.z+mat.m[14]*v.al;
	v.al=mat.m[3]*v.x+mat.m[7]*v.y+mat.m[11]*v.z+mat.m[15]*v.al;
	return v;
}
//创建矩阵
Matrix4D MakeMatrix(double m[16])
{
	Matrix4D matrix4;
	matrix4.m[0]=m[0];		matrix4.m[1]=m[1];		matrix4.m[2]=m[2];		matrix4.m[3]=m[3];
	matrix4.m[4]=m[4];		matrix4.m[5]=m[5];		matrix4.m[6]=m[6];		matrix4.m[7]=m[7];
	matrix4.m[8]=m[8];		matrix4.m[9]=m[9];		matrix4.m[10]=m[10];	matrix4.m[11]=m[11];
	matrix4.m[12]=m[12];	matrix4.m[13]=m[13];	matrix4.m[14]=m[14];	matrix4.m[15]=m[15];
	return matrix4;
}
//矩阵求逆(4阶)
Matrix4D MatrixInversion(float mat[16])
{
	Matrix4D m;
	float det_m=0;
	float A=0;
	det_m=MatDet(mat,4);
	for (int i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			A=Creat_M(mat,i,j,4);
			m.m[i*4+j]=A/det_m;
		}
	}
	return m;
}
//-----------------------------------------------
//功能: 求矩阵(n*n)的行列式
//入口参数: 矩阵的首地址，矩阵的行数
//返回值: 矩阵的行列式值
//----------------------------------------------
float MatDet(float *p, int n)
{
	float det=0;
	float mid_result = 0;
	float *p_creat;
	int sign = 1;
	int index=0;
	if (n==1)
	{
		det=p[0];
	}else{
		for (int i=0;i<n;i++)
		{
			p_creat = (float*)calloc((n-1)*(n-1), sizeof(float)); //分配内存单元
			index=0;
			for (int j = 0; j < n; j++)
			{
				for (int k = 0; k < n; k++)
				{
					if (j != 0 && k != i) //将除第j列和第k行外的所有元素存储到以p_mid为首地址的内存单元
					{
						p_creat[index] = *(p+j*n+k);
						index++;
					}
				}
			}
			sign = i%2 == 0 ? 1 : -1;    //代数余之式前面的正、负号
			mid_result = (float)sign*p[i]*MatDet(p_creat, n-1);
			free(p_creat);
			det+=mid_result;
		}
	}
	return det;
}
//----------------------------------------------------------------------------
//功能: 求k*k矩阵中元素A(m, n)的代数余之式
//入口参数: k*k矩阵的首地址，矩阵元素A的下标m,n,矩阵行数k
//返回值: k*k矩阵中元素A(m, n)的代数余之式
//----------------------------------------------------------------------------
float Creat_M(float *p, int m, int n, int k)
{
	int len;
	int i, j;
	float mid_result = 0;
	int sign = 1;
	float *p_creat, *p_mid;

	len = (k-1)*(k-1);            //k阶矩阵的代数余之式为k-1阶矩阵
	p_creat = (float*)calloc(len, sizeof(float)); //分配内存单元
	p_mid = p_creat;
	for (i = 0; i < k; i++)
	{
		for (j = 0; j < k; j++)
		{
			if (i != n && j != m) //将除第i列和第j行外的所有元素存储到以p_mid为首地址的内存单元
			{
				*p_mid++ = *(p+i*k+j);
			}
		}
	}
	sign = (m+n)%2 == 0 ? 1 : -1;    //代数余之式前面的正、负号
	mid_result = (float)sign*MatDet(p_creat, k-1);
	free(p_creat);
	return mid_result;
}
//////////////////////////////////////////////////////////////////////////
//这个函数写的很蠢笨，以后有时间可改进改进，优化下数据结构，及表链接形式！！！
//作者：		敖建
//Email：	sejarce@sina.com
//移动立方体算法 等值面三角面片255种内连接方式查找表
//小体元8个顶点排列为：
//前左下0,前右下1,后右下2,后左下3,前左上4,前右上5,后右上6,后左上7
//Marching Cubes algorithm
int MarchingCubes(CUBECELL cc,			//一个小体元的坐标、属性
				  float threshold,			//阈值
				  TRIANGLE triangle[])		//构造面顶点坐标（3个顶点为一组形成三角面）
{
	int triangle_num=0;
	unsigned char m_flag=0;
	//POINT3D point[12];
	CUBECELL cc_temp;

	//判定各个顶点与阈值大小
	if (cc.Pt[0].data>threshold)
	{
		m_flag|=0x01;
	}
	if (cc.Pt[1].data>threshold)
	{
		m_flag|=0x02;
	}
	if (cc.Pt[2].data>threshold)
	{
		m_flag|=0x04;
	}
	if (cc.Pt[3].data>threshold)
	{
		m_flag|=0x08;
	}
	if (cc.Pt[4].data>threshold)
	{
		m_flag|=0x10;
	}
	if (cc.Pt[5].data>threshold)
	{
		m_flag|=0x20;
	}
	if (cc.Pt[6].data>threshold)
	{
		m_flag|=0x40;
	}
	if (cc.Pt[7].data>threshold)
	{
		m_flag|=0x80;
	}
	switch(m_flag)
	{
	case 0:
		triangle_num=0;
		break;
	case 1://c1
		triangle_num=MarchingCubes_Table(cc,threshold,1,triangle);
		break;
	case 2://c1
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 3://c2
		triangle_num=MarchingCubes_Table(cc,threshold,2,triangle);
		break;
	case 4://c1
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 5://c3
		triangle_num=MarchingCubes_Table(cc,threshold,3,triangle);
		break;
	case 6://c2
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 7://c5
		triangle_num=MarchingCubes_Table(cc,threshold,5,triangle);
		break;
	case 8://c1
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 9://c2
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 10://c3
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 11://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 12://c2
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 13://c5
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 14://c5
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 15://c8
		triangle_num=MarchingCubes_Table(cc,threshold,8,triangle);
		break;
	case 16://c1
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 17://c2
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 18://c3
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 19://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 20://c4
		triangle_num=MarchingCubes_Table(cc,threshold,4,triangle);
		break;
	case 21://c6
		triangle_num=MarchingCubes_Table(cc,threshold,6,triangle);
		break;
	case 22://c6
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 23://c11
		triangle_num=MarchingCubes_Table(cc,threshold,11,triangle);
		break;
	case 24://c3
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 25://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 26://c7
		triangle_num=MarchingCubes_Table(cc,threshold,7,triangle);
		break;
	case 27://c9
		triangle_num=MarchingCubes_Table(cc,threshold,9,triangle);
		break;
	case 28://c6
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 29://c14
		triangle_num=MarchingCubes_Table(cc,threshold,14,triangle);
		break;
	case 30://c12
		triangle_num=MarchingCubes_Table(cc,threshold,12,triangle);
		break;
	case 31://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 32://c1
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 33://c3
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 34://c2
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 35://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 36://c3
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 37://c7
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;
	case 38://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 39://c9
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;
	case 40://c4
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,4,triangle);
		break;
	case 41://c6
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 42://c6
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 43://c14
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 44://c6
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 45://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 46://c11
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 47://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 48://c2
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 49://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 50://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 51://c8
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,8,triangle);
		break;
	case 52://c6
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 53://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 54://c14
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 55://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 56://c6
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 57://c11
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 58://c12
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 59://c5
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 60://c10
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,10,triangle);
		break;
	case 61://c16
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 62://c16
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 63://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 64://c1
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 65://c4
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,4,triangle);
		break;
	case 66://c3
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 67://c6
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 68://c2
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 69://c6
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 70://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 71://c14
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 72://c3
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 73://c6
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 74://c7
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;
	case 75://c12
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 76://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 77://c11
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 78://c9
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;
	case 79://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 80://c3
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 81://c6
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 82://c7
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;
	case 83://12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 84://c6
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 85://c10
		triangle_num=MarchingCubes_Table(cc,threshold,10,triangle);
		break;
	case 86://c12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 87://c16
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 88://c7
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;
	case 89://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 90://c13
		triangle_num=MarchingCubes_Table(cc,threshold,13,triangle);
		break;
	case 91://c12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 92://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 93://c16
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 94://c15
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;
	case 95://c19
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 96://c2
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 97://c6
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 98://c5
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 99://c11
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 100://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 101://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 102://c8
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,8,triangle);
		break;
	case 103://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 104://c6
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 105://c10
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,10,triangle);
		break;
	case 106://c12
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 107://c16
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 108://c14
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 109://c16
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 110://c5
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 111://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 112://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 113://c14
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 114://c9
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;
	case 115://c5
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 116://c11
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 117://c16
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 118://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 119://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 120://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 121://c16
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 122://c15
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;
	case 123://19
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 124://c16
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 125://c18
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,18,triangle);
		break;
	case 126://c19
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 127://c1
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 128://c1
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 129://c3
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 130://c4
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,4,triangle);
		break;
	case 131://c6
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 132://c3
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 133://c7
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;	
	case 134://c6
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 135://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 136://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 137://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 138://c6
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 139://c11
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 140://c5
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 141://c9
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;
	case 142://c14
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 143://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 144://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 145://c5
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 146://c6
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 147://c14
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 148://c6
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 149://c12
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 150://c10
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,10,triangle);
		break;

	case 151://c16
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 152://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 153://c8
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,8,triangle);
		break;
	case 154://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 155://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 156://c11
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;	
	case 157://c5
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 158://c16
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 159://c2
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 160://c3
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,3,triangle);
		break;
	case 161://c7
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,7,triangle);
		break;
	case 162://c6
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;	
	case 163://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 164://c12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 165://c13
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,13,triangle);
		break;
	case 166://c12
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;	
	case 167://c15
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;
	case 168://c6
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 169://c12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;	
	case 170://c10
		triangle_num=MarchingCubes_Table(cc,threshold,10,triangle);
		break;	
	case 171://c16
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 172://12
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 173://c15
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;	
	case 174://c16
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 175://c19
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 176://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 177://c9
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;	
	case 178://c11
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 179://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 180://c12
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;	
	case 181://c15
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;
	case 182://c16
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 183://c19
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;	
	case 184://c14
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;
	case 185://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 186://c16
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 187://c2
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 188://c16
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 189://c19
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 190://c18
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,18,triangle);
		break;
	case 191://c1
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;	
	case 192://c2
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 193://c6
		cc_temp.Pt[0]=cc.Pt[7];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[1];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 194://c6
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,6,triangle);
		break;
	case 195://c10
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,10,triangle);
		break;	
	case 196://c5
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[3];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[5];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 197://c12
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;
	case 198://c11
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;	
	case 199://c16
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 200://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 201://c14
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;	
	case 202://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;	
	case 203://c16
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 204://c8
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,8,triangle);
		break;	
	case 205://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 206://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[4];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[2];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 207://c2
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 208://c5
		cc_temp.Pt[0]=cc.Pt[6];		cc_temp.Pt[1]=cc.Pt[7];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[0];		cc_temp.Pt[7]=cc.Pt[1];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 209://c11
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,11,triangle);
		break;
	case 210://c12
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,12,triangle);
		break;	
	case 211://c16
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 212://c14
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,14,triangle);
		break;	
	case 213://c16
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;
	case 214://c16
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 215://c18
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,18,triangle);
		break;	
	case 216://c9
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,9,triangle);
		break;
	case 217://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 218://c15
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,15,triangle);
		break;	
	case 219://c19
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[2];		cc_temp.Pt[3]=cc.Pt[6];
		cc_temp.Pt[4]=cc.Pt[4];		cc_temp.Pt[5]=cc.Pt[0];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 220://c5
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 221://c2
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 222://c19
		cc_temp.Pt[0]=cc.Pt[0];		cc_temp.Pt[1]=cc.Pt[1];		cc_temp.Pt[2]=cc.Pt[5];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[3];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[6];		cc_temp.Pt[7]=cc.Pt[7];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 223://c1
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 224://c5
		cc_temp.Pt[0]=cc.Pt[5];		cc_temp.Pt[1]=cc.Pt[6];		cc_temp.Pt[2]=cc.Pt[7];		cc_temp.Pt[3]=cc.Pt[4];
		cc_temp.Pt[4]=cc.Pt[1];		cc_temp.Pt[5]=cc.Pt[2];		cc_temp.Pt[6]=cc.Pt[3];		cc_temp.Pt[7]=cc.Pt[0];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 225://c12
		triangle_num=MarchingCubes_Table(cc,threshold,12,triangle);
		break;	
	case 226://c14
		triangle_num=MarchingCubes_Table(cc,threshold,14,triangle);
		break;
	case 227://c16
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 228://c9
		triangle_num=MarchingCubes_Table(cc,threshold,9,triangle);
		break;
	case 229://c15
		triangle_num=MarchingCubes_Table(cc,threshold,15,triangle);
		break;
	case 230://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 231://c19
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;	
	case 232://c11
		triangle_num=MarchingCubes_Table(cc,threshold,11,triangle);
		break;
	case 233://c16
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[4];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[2];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,16,triangle);
		break;	
	case 234://c16
		triangle_num=MarchingCubes_Table(cc,threshold,16,triangle);
		break;	
	case 235://c18
		triangle_num=MarchingCubes_Table(cc,threshold,18,triangle);
		break;	
	case 236://c5
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 237://c19
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;	
	case 238://c2
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[5];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[3];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 239://c1
		cc_temp.Pt[0]=cc.Pt[4];		cc_temp.Pt[1]=cc.Pt[5];		cc_temp.Pt[2]=cc.Pt[6];		cc_temp.Pt[3]=cc.Pt[7];
		cc_temp.Pt[4]=cc.Pt[0];		cc_temp.Pt[5]=cc.Pt[1];		cc_temp.Pt[6]=cc.Pt[2];		cc_temp.Pt[7]=cc.Pt[3];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;	
	case 240://c8
		triangle_num=MarchingCubes_Table(cc,threshold,8,triangle);
		break;
	case 241://c5
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 242://c5
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;	
	case 243://c2
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 244://c5
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,5,triangle);
		break;
	case 245://c19
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,19,triangle);
		break;
	case 246://c2
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;
	case 247://c1
		cc_temp.Pt[0]=cc.Pt[3];		cc_temp.Pt[1]=cc.Pt[0];		cc_temp.Pt[2]=cc.Pt[1];		cc_temp.Pt[3]=cc.Pt[2];
		cc_temp.Pt[4]=cc.Pt[7];		cc_temp.Pt[5]=cc.Pt[4];		cc_temp.Pt[6]=cc.Pt[5];		cc_temp.Pt[7]=cc.Pt[6];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 248://c5
		triangle_num=MarchingCubes_Table(cc,threshold,5,triangle);
		break;	
	case 249://c2
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,2,triangle);
		break;	
	case 250://c19
		triangle_num=MarchingCubes_Table(cc,threshold,19,triangle);
		break;
	case 251://c1
		cc_temp.Pt[0]=cc.Pt[2];		cc_temp.Pt[1]=cc.Pt[3];		cc_temp.Pt[2]=cc.Pt[0];		cc_temp.Pt[3]=cc.Pt[1];
		cc_temp.Pt[4]=cc.Pt[6];		cc_temp.Pt[5]=cc.Pt[7];		cc_temp.Pt[6]=cc.Pt[4];		cc_temp.Pt[7]=cc.Pt[5];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 252://c2
		triangle_num=MarchingCubes_Table(cc,threshold,2,triangle);
		break;
	case 253://c1
		cc_temp.Pt[0]=cc.Pt[1];		cc_temp.Pt[1]=cc.Pt[2];		cc_temp.Pt[2]=cc.Pt[3];		cc_temp.Pt[3]=cc.Pt[0];
		cc_temp.Pt[4]=cc.Pt[5];		cc_temp.Pt[5]=cc.Pt[6];		cc_temp.Pt[6]=cc.Pt[7];		cc_temp.Pt[7]=cc.Pt[4];
		triangle_num=MarchingCubes_Table(cc_temp,threshold,1,triangle);
		break;
	case 254://c1
		triangle_num=MarchingCubes_Table(cc,threshold,1,triangle);
		break;
	case 255://c0
		triangle_num=0;
		break;
	default:
		break;
	}
	return triangle_num;
}

//////////////////////////////////////////////////////////////////////////
//作者：		敖建
//Email：	sejarce@sina.com
//移动立方体算法 体元与等值面连接方式查找表
//一共有19种，考虑二义性，由Nielson方法一共有93种
//此处略去一些内部较为复杂的连接方式
//////////////////////////////////////////////////////////////////////////

int MarchingCubes_Table(CUBECELL cc,			//一个小体元的坐标、属性
						float threshold,			//阈值
						unsigned char flag,			//标志位		//15种划分方式
						TRIANGLE triangle[])		//构造面顶点坐标（3个顶点为一组形成三角面）
{
	int triangle_num=0;
	POINT3D point[12];
	switch(flag)
	{
	case 0:
		triangle_num=0;
		break;
	case 1:
		triangle_num=1;	
		//0-1边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//0-3边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//0-4边
		point[2]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[1];
		triangle[0].Pt[2]=point[2];
		break;
	case 2:
		triangle_num=2;
		//0-3边
		point[0]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//0-4边
		point[1]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//1-2边
		point[2]=Linear_Interpolation(cc.Pt[2],cc.Pt[1],threshold);
		//1-5边
		point[3]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[1];
		triangle[0].Pt[2]=point[2];
		triangle[1].Pt[0]=point[1];
		triangle[1].Pt[1]=point[2];
		triangle[1].Pt[2]=point[3];
		break;
	case 3:
		//0-1边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//0-3边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//0-4边
		point[2]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//1-2边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//6-2边
		point[5]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//二义性,面元中点判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data>threshold*4)
		{
			triangle_num=4;
			////其实这里有4种组合方式，我们随即选一种
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[2];
			triangle[0].Pt[2]=point[3];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[3];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[1];
			triangle[2].Pt[1]=point[2];
			triangle[2].Pt[2]=point[5];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[4];
			triangle[3].Pt[2]=point[5];
		}else{
			triangle_num=2;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
		}
		break;
	case 4:
		//1-2边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//6-2边
		point[2]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//5-4边
		point[3]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//7-4边
		point[4]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//0-4边
		point[5]=Linear_Interpolation(cc.Pt[0],cc.Pt[4],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data>threshold*8)
		{
			triangle_num=6;
			triangle[0].Pt[0]=point[3];
			triangle[0].Pt[1]=point[5];
			triangle[0].Pt[2]=point[0];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[2];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[1];
			triangle[3].Pt[0]=point[0];
			triangle[3].Pt[1]=point[1];
			triangle[3].Pt[2]=point[5];
			triangle[4].Pt[0]=point[0];
			triangle[4].Pt[1]=point[2];
			triangle[4].Pt[2]=point[3];
			triangle[5].Pt[0]=point[1];
			triangle[5].Pt[1]=point[2];
			triangle[5].Pt[2]=point[4];
		}else{
			triangle_num=2;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
		}
		break;
	case 5:
		//3-0边
		point[0]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//4-0边
		point[1]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//5-1边
		point[2]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//6-2边
		point[3]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//3-2边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		triangle_num=3;
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[3];
		triangle[0].Pt[2]=point[4];
		triangle[1].Pt[0]=point[0];
		triangle[1].Pt[1]=point[1];
		triangle[1].Pt[2]=point[3];
		triangle[2].Pt[0]=point[1];
		triangle[2].Pt[1]=point[2];
		triangle[2].Pt[2]=point[3];
		break;
	case 6:
		//此处略掉了一种相交方式
		//6-2边
		point[0]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//1-2边
		point[1]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[2]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//1-0边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//3-0边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//5-4边
		point[5]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//6-4边
		point[6]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data>threshold*8)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[3];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[3];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[0];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[6];
			triangle[3].Pt[0]=point[0];
			triangle[3].Pt[1]=point[6];
			triangle[3].Pt[2]=point[4];
			triangle[4].Pt[0]=point[0];
			triangle[4].Pt[1]=point[4];
			triangle[4].Pt[2]=point[2];
		}else{
			triangle_num=3;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[6];
		}
		break;
	case 7://3个二义性面，本来有8种连接情况，此处略去4种
		//0-4边
		point[0]=Linear_Interpolation(cc.Pt[0],cc.Pt[4],threshold);
		//5-4边
		point[1]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//7-4边
		point[2]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//0-1边
		point[3]=Linear_Interpolation(cc.Pt[0],cc.Pt[1],threshold);
		//5-1边
		point[4]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//2-1边
		point[5]=Linear_Interpolation(cc.Pt[2],cc.Pt[1],threshold);
		//0-3边
		point[6]=Linear_Interpolation(cc.Pt[0],cc.Pt[3],threshold);
		//7-3边
		point[7]=Linear_Interpolation(cc.Pt[7],cc.Pt[3],threshold);
		//2-3边
		point[8]=Linear_Interpolation(cc.Pt[2],cc.Pt[3],threshold);
		//面元判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data>threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[6];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[6];
			triangle[2].Pt[2]=point[7];
			triangle[3].Pt[0]=point[5];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[6];
			triangle[4].Pt[0]=point[5];
			triangle[4].Pt[1]=point[7];
			triangle[4].Pt[2]=point[8];
		}else if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[4].data+cc.Pt[5].data>threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[6];
			triangle[0].Pt[1]=point[7];
			triangle[0].Pt[2]=point[8];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[2];
			triangle[1].Pt[2]=point[3];
			triangle[2].Pt[0]=point[2];
			triangle[2].Pt[1]=point[3];
			triangle[2].Pt[2]=point[5];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[5];
			triangle[3].Pt[2]=point[2];
			triangle[4].Pt[0]=point[1];
			triangle[4].Pt[1]=point[5];
			triangle[4].Pt[2]=point[4];
		}else if (cc.Pt[0].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[7].data>threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[3];
			triangle[0].Pt[1]=point[4];
			triangle[0].Pt[2]=point[5];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[8];
			triangle[1].Pt[2]=point[1];
			triangle[2].Pt[0]=point[0];
			triangle[2].Pt[1]=point[8];
			triangle[2].Pt[2]=point[6];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[2];
			triangle[4].Pt[0]=point[1];
			triangle[4].Pt[1]=point[7];
			triangle[4].Pt[2]=point[8];
		}else{
			triangle_num=3;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[6];
			triangle[2].Pt[1]=point[7];
			triangle[2].Pt[2]=point[8];
		}
		break;
	case 8:
		//0-4边
		point[0]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//1-5边
		point[1]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//2-6边
		point[2]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//3-7边
		point[3]=Linear_Interpolation(cc.Pt[7],cc.Pt[3],threshold);
		triangle_num=2;
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[1];
		triangle[0].Pt[2]=point[2];
		triangle[1].Pt[0]=point[0];
		triangle[1].Pt[1]=point[2];
		triangle[1].Pt[2]=point[3];
		break;
	case 9:
		//1-2边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//3-7边
		point[2]=Linear_Interpolation(cc.Pt[3],cc.Pt[7],threshold);
		//7-4边
		point[3]=Linear_Interpolation(cc.Pt[4],cc.Pt[7],threshold);
		//4-5边
		point[4]=Linear_Interpolation(cc.Pt[4],cc.Pt[5],threshold);
		//5-1边
		point[5]=Linear_Interpolation(cc.Pt[1],cc.Pt[5],threshold);
		triangle_num=4;
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[1];
		triangle[0].Pt[2]=point[5];
		triangle[1].Pt[0]=point[2];
		triangle[1].Pt[1]=point[3];
		triangle[1].Pt[2]=point[4];
		triangle[2].Pt[0]=point[1];
		triangle[2].Pt[1]=point[2];
		triangle[2].Pt[2]=point[5];
		triangle[3].Pt[0]=point[4];
		triangle[3].Pt[1]=point[2];
		triangle[3].Pt[2]=point[5];
		break;
	case 10://2个二义性面，本来有4种连接情况，此处略去2种
		//0-1边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//0-3边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//4-5边
		point[2]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//4-7边
		point[3]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//1-2边
		point[4]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[5]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//5-6边
		point[6]=Linear_Interpolation(cc.Pt[5],cc.Pt[6],threshold);
		//7-6边
		point[7]=Linear_Interpolation(cc.Pt[7],cc.Pt[6],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data>threshold*8)
		{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[2];
			triangle[0].Pt[2]=point[4];
			triangle[1].Pt[0]=point[2];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[6];
			triangle[2].Pt[0]=point[1];
			triangle[2].Pt[1]=point[7];
			triangle[2].Pt[2]=point[2];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[5];
		}else{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[1];
			triangle[1].Pt[1]=point[2];
			triangle[1].Pt[2]=point[3];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[6];
			triangle[3].Pt[0]=point[4];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[6];
		}
		break;
	case 11:
		//7-4边
		point[0]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//5-4边
		point[1]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//3-0边
		point[2]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//5-1边
		point[3]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//6-2边
		point[4]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//3-2边
		point[5]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		triangle_num=4;
		triangle[0].Pt[0]=point[1];
		triangle[0].Pt[1]=point[4];
		triangle[0].Pt[2]=point[3];
		triangle[1].Pt[0]=point[1];
		triangle[1].Pt[1]=point[4];
		triangle[1].Pt[2]=point[0];
		triangle[2].Pt[0]=point[0];
		triangle[2].Pt[1]=point[4];
		triangle[2].Pt[2]=point[5];
		triangle[3].Pt[0]=point[2];
		triangle[3].Pt[1]=point[5];
		triangle[3].Pt[2]=point[0];
		break;
	case 12://2个二义性面，本来有6种连接情况，此处略去4种
		//0-4边
		point[0]=Linear_Interpolation(cc.Pt[0],cc.Pt[4],threshold);
		//5-4边
		point[1]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//7-4边
		point[2]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//1-0边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//3-0边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//5-1边
		point[5]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//6-2边
		point[6]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//7-3边
		point[7]=Linear_Interpolation(cc.Pt[7],cc.Pt[3],threshold);
		//面元判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[4].data+cc.Pt[5].data>threshold*4)
		{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[3];
			triangle[0].Pt[2]=point[4];
			triangle[1].Pt[0]=point[6];
			triangle[1].Pt[1]=point[1];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[6];
			triangle[2].Pt[1]=point[1];
			triangle[2].Pt[2]=point[2];
			triangle[3].Pt[0]=point[6];
			triangle[3].Pt[1]=point[2];
			triangle[3].Pt[2]=point[7];		
		}else{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[6];
			triangle[1].Pt[1]=point[3];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[6];
			triangle[2].Pt[1]=point[3];
			triangle[2].Pt[2]=point[4];
			triangle[3].Pt[0]=point[6];
			triangle[3].Pt[1]=point[4];
			triangle[3].Pt[2]=point[7];	
		}
		break;
	case 13://6个二义性面，本来有64种连接情况，此处简便起见，只选取最简单的2种，只要方格划分够细，影响很微小
		//1-0边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//3-0边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//4-0边
		point[2]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//1-2边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//6-2边
		point[5]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//1-5边
		point[6]=Linear_Interpolation(cc.Pt[1],cc.Pt[5],threshold);
		//4-5边
		point[7]=Linear_Interpolation(cc.Pt[4],cc.Pt[5],threshold);
		//6-5边
		point[8]=Linear_Interpolation(cc.Pt[6],cc.Pt[5],threshold);
		//3-7边
		point[9]=Linear_Interpolation(cc.Pt[3],cc.Pt[7],threshold);
		//4-7边
		point[10]=Linear_Interpolation(cc.Pt[4],cc.Pt[7],threshold);
		//6-7边
		point[11]=Linear_Interpolation(cc.Pt[6],cc.Pt[7],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data>threshold*8)
		{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[6];
			triangle[2].Pt[1]=point[7];
			triangle[2].Pt[2]=point[8];
			triangle[3].Pt[0]=point[9];
			triangle[3].Pt[1]=point[10];
			triangle[3].Pt[2]=point[11];
		}else{
			triangle_num=4;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[3];
			triangle[0].Pt[2]=point[6];
			triangle[1].Pt[0]=point[1];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[9];
			triangle[2].Pt[0]=point[2];
			triangle[2].Pt[1]=point[7];
			triangle[2].Pt[2]=point[10];
			triangle[3].Pt[0]=point[5];
			triangle[3].Pt[1]=point[8];
			triangle[3].Pt[2]=point[11];
		}
		break;
	case 14:
		//1-2边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//6-2边
		point[1]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//3-7边
		point[2]=Linear_Interpolation(cc.Pt[3],cc.Pt[7],threshold);
		//4-7边
		point[3]=Linear_Interpolation(cc.Pt[4],cc.Pt[7],threshold);
		//4-5边
		point[4]=Linear_Interpolation(cc.Pt[4],cc.Pt[5],threshold);
		//0-1边
		point[5]=Linear_Interpolation(cc.Pt[0],cc.Pt[1],threshold);
		triangle_num=4;
		triangle[0].Pt[0]=point[0];
		triangle[0].Pt[1]=point[1];
		triangle[0].Pt[2]=point[2];
		triangle[1].Pt[0]=point[0];
		triangle[1].Pt[1]=point[2];
		triangle[1].Pt[2]=point[4];
		triangle[2].Pt[0]=point[2];
		triangle[2].Pt[1]=point[3];
		triangle[2].Pt[2]=point[4];
		triangle[3].Pt[0]=point[0];
		triangle[3].Pt[1]=point[4];
		triangle[3].Pt[2]=point[5];
		break;
	case 15:
		//0-4边
		point[0]=Linear_Interpolation(cc.Pt[0],cc.Pt[4],threshold);
		//5-4边
		point[1]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//7-4边
		point[2]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//0-1边
		point[3]=Linear_Interpolation(cc.Pt[0],cc.Pt[1],threshold);
		//5-1边
		point[4]=Linear_Interpolation(cc.Pt[5],cc.Pt[1],threshold);
		//2-1边
		point[5]=Linear_Interpolation(cc.Pt[2],cc.Pt[1],threshold);
		//0-3边
		point[6]=Linear_Interpolation(cc.Pt[0],cc.Pt[3],threshold);
		//7-3边
		point[7]=Linear_Interpolation(cc.Pt[7],cc.Pt[3],threshold);
		//2-3边
		point[8]=Linear_Interpolation(cc.Pt[2],cc.Pt[3],threshold);
		//面元判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data<threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[6];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[6];
			triangle[2].Pt[2]=point[7];
			triangle[3].Pt[0]=point[5];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[6];
			triangle[4].Pt[0]=point[5];
			triangle[4].Pt[1]=point[7];
			triangle[4].Pt[2]=point[8];
		}else if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[4].data+cc.Pt[5].data<threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[6];
			triangle[0].Pt[1]=point[7];
			triangle[0].Pt[2]=point[8];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[2];
			triangle[1].Pt[2]=point[3];
			triangle[2].Pt[0]=point[2];
			triangle[2].Pt[1]=point[3];
			triangle[2].Pt[2]=point[5];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[5];
			triangle[3].Pt[2]=point[2];
			triangle[4].Pt[0]=point[1];
			triangle[4].Pt[1]=point[5];
			triangle[4].Pt[2]=point[4];
		}else if (cc.Pt[0].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[7].data<threshold*4)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[3];
			triangle[0].Pt[1]=point[4];
			triangle[0].Pt[2]=point[5];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[8];
			triangle[1].Pt[2]=point[1];
			triangle[2].Pt[0]=point[0];
			triangle[2].Pt[1]=point[8];
			triangle[2].Pt[2]=point[6];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[7];
			triangle[3].Pt[2]=point[2];
			triangle[4].Pt[0]=point[1];
			triangle[4].Pt[1]=point[7];
			triangle[4].Pt[2]=point[8];
		}else{
			triangle_num=3;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[6];
			triangle[2].Pt[1]=point[7];
			triangle[2].Pt[2]=point[8];
		}
		break;
	case 16:
		//此处略掉了一种相交方式
		//6-2边
		point[0]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//1-2边
		point[1]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[2]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//1-0边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//3-0边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//5-4边
		point[5]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//6-4边
		point[6]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data<threshold*8)
		{
			triangle_num=5;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[3];
			triangle[1].Pt[0]=point[0];
			triangle[1].Pt[1]=point[3];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[0];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[6];
			triangle[3].Pt[0]=point[0];
			triangle[3].Pt[1]=point[6];
			triangle[3].Pt[2]=point[4];
			triangle[4].Pt[0]=point[0];
			triangle[4].Pt[1]=point[4];
			triangle[4].Pt[2]=point[2];
		}else{
			triangle_num=3;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[6];
		}
		break;
	case 18:
		//1-2边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//6-2边
		point[2]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//5-4边
		point[3]=Linear_Interpolation(cc.Pt[5],cc.Pt[4],threshold);
		//7-4边
		point[4]=Linear_Interpolation(cc.Pt[7],cc.Pt[4],threshold);
		//0-4边
		point[5]=Linear_Interpolation(cc.Pt[0],cc.Pt[4],threshold);
		//二义性,体元中心判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data+cc.Pt[4].data+cc.Pt[5].data+cc.Pt[6].data+cc.Pt[7].data<threshold*8)
		{
			triangle_num=6;
			triangle[0].Pt[0]=point[3];
			triangle[0].Pt[1]=point[5];
			triangle[0].Pt[2]=point[0];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[2];
			triangle[2].Pt[0]=point[4];
			triangle[2].Pt[1]=point[5];
			triangle[2].Pt[2]=point[1];
			triangle[3].Pt[0]=point[0];
			triangle[3].Pt[1]=point[1];
			triangle[3].Pt[2]=point[5];
			triangle[4].Pt[0]=point[0];
			triangle[4].Pt[1]=point[2];
			triangle[4].Pt[2]=point[3];
			triangle[5].Pt[0]=point[1];
			triangle[5].Pt[1]=point[2];
			triangle[5].Pt[2]=point[4];
		}else{
			triangle_num=2;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
		}
		break;
	case 19:
		//0-1边
		point[0]=Linear_Interpolation(cc.Pt[1],cc.Pt[0],threshold);
		//0-3边
		point[1]=Linear_Interpolation(cc.Pt[3],cc.Pt[0],threshold);
		//0-4边
		point[2]=Linear_Interpolation(cc.Pt[4],cc.Pt[0],threshold);
		//2-1边
		point[3]=Linear_Interpolation(cc.Pt[1],cc.Pt[2],threshold);
		//3-2边
		point[4]=Linear_Interpolation(cc.Pt[3],cc.Pt[2],threshold);
		//6-2边
		point[5]=Linear_Interpolation(cc.Pt[6],cc.Pt[2],threshold);
		//二义性,面元中点判别法
		if (cc.Pt[0].data+cc.Pt[1].data+cc.Pt[2].data+cc.Pt[3].data<threshold*4)
		{
			triangle_num=4;
			////其实这里有4种组合方式，我们随即选一种
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[2];
			triangle[0].Pt[2]=point[3];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[3];
			triangle[1].Pt[2]=point[5];
			triangle[2].Pt[0]=point[1];
			triangle[2].Pt[1]=point[2];
			triangle[2].Pt[2]=point[5];
			triangle[3].Pt[0]=point[1];
			triangle[3].Pt[1]=point[4];
			triangle[3].Pt[2]=point[5];
		}else{
			triangle_num=2;
			triangle[0].Pt[0]=point[0];
			triangle[0].Pt[1]=point[1];
			triangle[0].Pt[2]=point[2];
			triangle[1].Pt[0]=point[3];
			triangle[1].Pt[1]=point[4];
			triangle[1].Pt[2]=point[5];
		}
		break;
	default:
		break;
	}
	return triangle_num;
}

//求2个点的线性插值(3维空间点)
POINT3D Linear_Interpolation(POINT3D a,POINT3D b,float data)
{
	//debug用
	//if ((data>a.data&&data>b.data)||(data<a.data&&data<b.data))
	//{
	//	error_num++;
	//}
	POINT3D c;
	c.x=(a.x-b.x)*(data-b.data)/(a.data-b.data)+b.x;
	c.y=(a.y-b.y)*(data-b.data)/(a.data-b.data)+b.y;
	c.z=(a.z-b.z)*(data-b.data)/(a.data-b.data)+b.z;
	return c;
}

bool BelongTo(int a,int b[],int size_b)
{
	for (int i=0;i<size_b;i++)
	{
		if (a==b[i])
		{
			return true;
		}
	}
	return false;
}
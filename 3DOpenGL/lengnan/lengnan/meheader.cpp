#include "StdAfx.h"
#include "myheader.h"
//模型尺寸
double modelL=0;
double modelW=0;
double modelH=0;
//采集卡数量
unsigned int cardnum=0;
//采集时间间隔
unsigned int gathertime=0;
//X向传感器数量
unsigned int Xnum=0;
//Y向传感器数量
unsigned int Ynum=0;
//层数
unsigned int Znum=0;
//传感器总数
unsigned int TotalNum=0;
////传感器的总量
unsigned int totalnum=0;
//////////////间距数组
//X向间距
double Xdistances[SENSORLAYNUM]={0};
//Y向间距
double Ydistances[SENSORLAYNUM]={0};
//Z向间距
double Zdistances[SENSORLAYNUM]={0};
///////////////////////////传感器方位坐标数组
int sensor_pos[SENSORLAYNUM][SENSORLAYNUM][SENSORLAYNUM]={0};
////////////////////////传感器编号一维数组
//空间点的传感器编号
int sensornumber[SENSORNUM]={0};




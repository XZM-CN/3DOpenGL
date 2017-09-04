//模型尺寸
#define  SENSORLAYNUM 128
#define SENSORNUM 1024
extern double modelL;
extern double modelW;
extern double modelH;
//采集卡数量
extern unsigned int cardnum;
//采集时间间隔
extern unsigned int gathertime;
//X向传感器数量
extern unsigned int Xnum;
//Y向传感器数量
extern unsigned int Ynum;
//传感器总数
extern unsigned int TotalNum;
//层数
extern unsigned int Znum;
////传感器的总量
extern unsigned int totalnum;
//////////////间距数组
//X向间距
extern double Xdistances[SENSORLAYNUM];
//Y向间距
extern double Ydistances[SENSORLAYNUM];
//Z向间距
extern double Zdistances[SENSORLAYNUM];
////////////////传感器方位坐标数组
extern int sensor_pos[SENSORLAYNUM][SENSORLAYNUM][SENSORLAYNUM];
//空间点的传感器编号
extern int sensornumber[SENSORNUM];


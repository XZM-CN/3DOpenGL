#pragma once
#include "afxmt.h"
class CCom
{
public:
	CCom(CString comname,DWORD dwBuad,char  cParity,BYTE  bDataBit,BYTE bStopBit);
	~CCom(void);
private:
	//串口句柄
	HANDLE h_com;
	//串口名称
	CString m_comname;
	//波特率：CBR_300，
    /*
	#define CBR_110             110
	#define CBR_300             300
	#define CBR_600             600
	#define CBR_1200            1200
	#define CBR_2400            2400
	#define CBR_4800            4800
	#define CBR_9600            9600
	#define CBR_14400           14400
	#define CBR_19200           19200
	#define CBR_38400           38400
	#define CBR_56000           56000
	#define CBR_57600           57600
	#define CBR_115200          115200
	#define CBR_128000          128000
	#define CBR_256000          256000
	*/
	DWORD m_dwBuad;
	//奇偶校验：'N'无校验;'O'奇校验;'E'偶校验
	char m_cParity;
	//数据位：4-8
	char m_bDataBit;
	//停止位：10-1位停止位；15-1.5位停止位;20-2个停止位
	BYTE m_bStopBit;
	//资源锁
	CCriticalSection mycritical;
public:
	BOOL InitCom();
	bool WriteCom(unsigned char* writebuffer,int size,bool clearflag);
	bool ReadCom(unsigned char* readbuffer,int size,bool clearflag);
	void CloseCom();
	//CRC校验码函数
	unsigned short GetCRC( BYTE * dataPacket,WORD dataLenth );
};




#pragma once
#include "afxmt.h"
class CCom
{
public:
	CCom(CString comname,DWORD dwBuad,char  cParity,BYTE  bDataBit,BYTE bStopBit);
	~CCom(void);
private:
	//���ھ��
	HANDLE h_com;
	//��������
	CString m_comname;
	//�����ʣ�CBR_300��
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
	//��żУ�飺'N'��У��;'O'��У��;'E'żУ��
	char m_cParity;
	//����λ��4-8
	char m_bDataBit;
	//ֹͣλ��10-1λֹͣλ��15-1.5λֹͣλ;20-2��ֹͣλ
	BYTE m_bStopBit;
	//��Դ��
	CCriticalSection mycritical;
public:
	BOOL InitCom();
	bool WriteCom(unsigned char* writebuffer,int size,bool clearflag);
	bool ReadCom(unsigned char* readbuffer,int size,bool clearflag);
	void CloseCom();
	//CRCУ���뺯��
	unsigned short GetCRC( BYTE * dataPacket,WORD dataLenth );
};




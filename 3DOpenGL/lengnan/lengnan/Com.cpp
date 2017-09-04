#include "StdAfx.h"
#include "Com.h"



CCom::CCom(CString comname,DWORD dwBuad,char  cParity,BYTE  bDataBit,BYTE bStopBit)
{
   h_com=NULL;
   m_comname=comname;
   m_bDataBit=bDataBit;
   m_bStopBit=bStopBit;
   m_cParity=cParity;
   m_dwBuad=dwBuad;
}
CCom::~CCom(void)
{
}

BOOL CCom::InitCom( ) 
{
	h_com=CreateFile(m_comname,GENERIC_WRITE|GENERIC_READ,0,NULL,OPEN_EXISTING,0/*FILE_FLAG_OVERLAPPED*/,NULL);
	if (INVALID_HANDLE_VALUE==h_com)
	{
		h_com=NULL;
		return FALSE;
	}
	//初始化串口通信
	DCB    tempdcb;
	GetCommState(h_com,&tempdcb);
	switch(m_dwBuad)
	{		
	case 300:
		tempdcb.BaudRate=CBR_300;
		break;
	case 600:
		tempdcb.BaudRate=CBR_600;
		break;
	case 1200:
		tempdcb.BaudRate=CBR_1200;
		break;
	case 2400:
		tempdcb.BaudRate=CBR_2400;
		break;
	case 4800:
		tempdcb.BaudRate=CBR_4800;
		break;
	case 9600:
		tempdcb.BaudRate=CBR_9600;
		break;
	case 19200:
		tempdcb.BaudRate=CBR_19200;
		break;
	case 38400:
		tempdcb.BaudRate=CBR_38400;
		break;
	case 57600:
		tempdcb.BaudRate=CBR_57600;
		break;
	case 115200:
		tempdcb.BaudRate=CBR_115200;
		break;
	default:
		break;
	}
	tempdcb.ByteSize=m_bDataBit;
	switch(m_cParity)
	{
	case 'N':
		tempdcb.Parity=NOPARITY;
		break;
	case 'O':
		tempdcb.Parity=ODDPARITY;
		break;
	case 'E':
		tempdcb.Parity=EVENPARITY;
		break;
	default:
		break;
	}
	switch(m_bStopBit)
	{
	case 10:
		tempdcb.StopBits=ONESTOPBIT;
		break;
	case 15:
		tempdcb.StopBits=ONE5STOPBITS;
		break;
	case 20:
		tempdcb.StopBits=TWOSTOPBITS;
		break;
	default:
		break;
	}
	//配置串口
	SetupComm(h_com,1024,1024);
	SetCommState(h_com,&tempdcb);
	//设定读超时  
	COMMTIMEOUTS TimeOuts;
	//设定读超时
	TimeOuts.ReadIntervalTimeout=0;
	TimeOuts.ReadTotalTimeoutMultiplier=0;
	TimeOuts.ReadTotalTimeoutConstant=200;
	//设定写超时
	TimeOuts.WriteTotalTimeoutMultiplier=0;
	TimeOuts.WriteTotalTimeoutConstant=0;
	SetCommTimeouts(h_com,&TimeOuts);//设置超时 
	PurgeComm(h_com,PURGE_TXCLEAR|PURGE_RXCLEAR);
	SetCommMask(h_com,EV_RXCHAR|EV_TXEMPTY);
	return TRUE;
}
bool CCom::WriteCom(unsigned char* writebuffer,int size,bool clearflag)
{
	DWORD numofwrite=0;
	mycritical.Lock();
	if (clearflag)
	{
	   PurgeComm(h_com,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
	BOOL result=WriteFile(h_com,writebuffer,size,&numofwrite,NULL);
	mycritical.Unlock();
	if(!result||size!=numofwrite)//串口写入出错，从新打开串口
	{
		CloseCom();
		InitCom();
		return false;
	}
	return true;
}
bool CCom::ReadCom(unsigned char* readbuffer,int size,bool clearflag)
{
	DWORD numofread=0;
	//清空串口
	mycritical.Lock();
	if (clearflag)
	{
	   PurgeComm(h_com,PURGE_TXABORT|PURGE_RXABORT|PURGE_TXCLEAR|PURGE_RXCLEAR);
	}
	bool result=ReadFile(h_com,readbuffer,size,&numofread,NULL);
	mycritical.Unlock();
	if(!result||size!=numofread)//串口读取出错，从新打开串口
	{
        CloseCom();
		InitCom();
		return false;
	}
	return true;
}
void CCom::CloseCom()
{
	try
	{
		if (h_com!=NULL)
		{
			CloseHandle(h_com);
			h_com=NULL;
		}
	}
	catch (...)
	{
	}
}
unsigned short CCom::GetCRC( BYTE * pchMsg,WORD wDataLen  )
{
	const BYTE chCRCHTalbe[] =                                 // CRC 高位字节值表
	{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
		0x00, 0xC1, 0x81, 0x40
	};
	const BYTE chCRCLTalbe[] =                                 // CRC 低位字节值表
	{
		0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
		0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
		0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
		0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
		0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
		0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
		0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
		0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38,
		0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
		0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
		0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
		0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
		0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB,
		0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
		0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
		0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
		0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
		0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
		0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
		0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
		0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
		0x41, 0x81, 0x80, 0x40
	};
	BYTE chCRCHi = 0xFF; // 高CRC字节初始化
	BYTE chCRCLo = 0xFF; // 低CRC字节初始化
	WORD wIndex;            // CRC循环中的索引
	while (wDataLen--)
	{
		// 计算CRC
		wIndex = chCRCLo ^ *pchMsg++ ;
		chCRCLo = chCRCHi ^ chCRCHTalbe[wIndex]; 
		chCRCHi = chCRCLTalbe[wIndex] ;
	} 
	return ((chCRCHi << 8) | chCRCLo) ;
}


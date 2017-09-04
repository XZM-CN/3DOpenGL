// MyToolBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "MyToolBar.h"


// CMyToolBar
CMyToolBar* pMyToolBar;
extern int m_PlayPos;
extern bool ThreadReadDataflag0; //�̹߳��𣬻ָ���־λ��trueΪ�ָ���falseΪ����
extern bool bSuspendThreadflag;	//�̼߳��������־
extern bool threadstarrtflag;
extern bool m_playmoveflag; //���Ż��������ƶ���־��
extern bool opengliniflag;	//opengl��Ҫ��ʼ����־λ
//��ȡ�����߳�
extern CWinThread *ThreadReadData;


IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

CMyToolBar::CMyToolBar()
{
	pMyToolBar=this;
}

CMyToolBar::~CMyToolBar()
{
}


BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()



// CMyToolBar ��Ϣ�������


void CMyToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int pos=0;
	pos = m_MyScroll.GetScrollPos();    // ��ȡˮƽ��������ǰλ��
	switch (nSBCode)    
	{    
		// ����������һ�У���pos��1   
	case SB_LINEUP:    
		pos -= 1;    
		break;    
		// ������ҹ���һ�У���pos��1   
	case SB_LINEDOWN:    
		pos  += 1;    
		break;    
		// ����������һҳ����pos��10   
	case SB_PAGEUP:    
		pos -= 50;    
		break;    
		// ������ҹ���һҳ����pos��10   
	case SB_PAGEDOWN:    
		pos  += 50;    
		break;    
		// �������������ˣ���posΪ1   
	case SB_TOP:    
		pos = 0;    
		break;    
		// ������������Ҷˣ���posΪ100   
	case SB_BOTTOM:    
		pos = 1000;    
		break;      
		// ����϶������������ָ��λ�ã���pos��ֵΪnPos��ֵ   
	case SB_THUMBPOSITION:   //�϶�����ʱ��Ѱ�Ҽ�¼λ�ã�
		pos = nPos; 
		break;    
	default:    
		break;  
	}
	m_playmoveflag=true; 
	opengliniflag=true;
	m_PlayPos=pos;
	//ȷ��������λ��
	m_MyScroll.SetScrollPos(pos); 
	CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}
bool CMyToolBar::Setpos()
{
	m_MyScroll.SetScrollPos(m_PlayPos);
	return true;
}

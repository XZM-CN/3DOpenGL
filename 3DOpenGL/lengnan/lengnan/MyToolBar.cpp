// MyToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "lengnan.h"
#include "MyToolBar.h"


// CMyToolBar
CMyToolBar* pMyToolBar;
extern int m_PlayPos;
extern bool ThreadReadDataflag0; //线程挂起，恢复标志位（true为恢复，false为挂起）
extern bool bSuspendThreadflag;	//线程即将挂起标志
extern bool threadstarrtflag;
extern bool m_playmoveflag; //播放滑动条有移动标志量
extern bool opengliniflag;	//opengl需要初始化标志位
//读取数据线程
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



// CMyToolBar 消息处理程序


void CMyToolBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int pos=0;
	pos = m_MyScroll.GetScrollPos();    // 获取水平滚动条当前位置
	switch (nSBCode)    
	{    
		// 如果向左滚动一列，则pos减1   
	case SB_LINEUP:    
		pos -= 1;    
		break;    
		// 如果向右滚动一列，则pos加1   
	case SB_LINEDOWN:    
		pos  += 1;    
		break;    
		// 如果向左滚动一页，则pos减10   
	case SB_PAGEUP:    
		pos -= 50;    
		break;    
		// 如果向右滚动一页，则pos加10   
	case SB_PAGEDOWN:    
		pos  += 50;    
		break;    
		// 如果滚动到最左端，则pos为1   
	case SB_TOP:    
		pos = 0;    
		break;    
		// 如果滚动到最右端，则pos为100   
	case SB_BOTTOM:    
		pos = 1000;    
		break;      
		// 如果拖动滚动块滚动到指定位置，则pos赋值为nPos的值   
	case SB_THUMBPOSITION:   //拖动滑块时，寻找记录位置，
		pos = nPos; 
		break;    
	default:    
		break;  
	}
	m_playmoveflag=true; 
	opengliniflag=true;
	m_PlayPos=pos;
	//确定滚动块位置
	m_MyScroll.SetScrollPos(pos); 
	CToolBar::OnHScroll(nSBCode, nPos, pScrollBar);
}
bool CMyToolBar::Setpos()
{
	m_MyScroll.SetScrollPos(m_PlayPos);
	return true;
}

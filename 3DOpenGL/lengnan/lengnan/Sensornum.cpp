// Sensornum.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "lengnan.h"
#include "Sensornum.h"
#include "afxdialogex.h"
#include "Cselectnum.h"

// CSensornum �Ի���

IMPLEMENT_DYNAMIC(CSensornum, CPropertyPage)

CSensornum::CSensornum()
	: CPropertyPage(CSensornum::IDD)
	, myxnum(0)
	,myynum(0)
	,myznum(0)
	,redrawflag(TRUE)
	,finishflag(FALSE)
{

}

CSensornum::~CSensornum()
{
}

void CSensornum::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LSENSORNUM, mylist);
}


BEGIN_MESSAGE_MAP(CSensornum, CPropertyPage)
	ON_WM_PAINT()
	ON_NOTIFY(NM_DBLCLK, IDC_LSENSORNUM, &CSensornum::OnNMDblclkLsensornum)
END_MESSAGE_MAP()


// CSensornum ��Ϣ�������


BOOL CSensornum::OnSetActive()
{
	// TODO: �ڴ����ר�ô����/����û���

	// ��ø����ڣ������Ա�CPropertySheet��    
	CPropertySheet* psheet = (CPropertySheet*) GetParent();      
	// �������Ա�ֻ�С���һ������ť    
	psheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);   
	return CPropertyPage::OnSetActive();
}


void CSensornum::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if (!redrawflag)
	{
		return;
	}
	myxnum=Xnum;
	myynum=Ynum;
	myznum=Znum;
////////////����С��С���ʹ��������
	CString tempsting;
	int num=0;
	for (int z=0;z<Znum;z++)
	{
		for (int y=0;y<Ynum;y++)
		{
			for (int x=0;x<Xnum;x++)
			{
				//X
				tempsting.Format(_T("%d"),x+1);
				mylist.InsertItem(num,tempsting);
				//Y
				tempsting.Format(_T("%d"),y+1);
				mylist.SetItemText(num,1,tempsting);
				//Z
				tempsting.Format(_T("%d"),z+1);
				mylist.SetItemText(num,2,tempsting);
				//���������
				if (sensor_pos[x][y][z]!=-1)
				{
					tempsting.Format(_T("%d"),sensor_pos[x][y][z]+1);
					mylist.SetItemText(num,3,tempsting);
				}
				//X��ռ�λ��
				double xspace=0;
				for (int xx=0;xx<=x;xx++)
				{
					xspace+=Xdistances[xx];
				}
				tempsting.Format(_T("%.3f"),xspace);
				mylist.SetItemText(num,4,tempsting);
				//Y��ռ�λ��
				double yspace=0;
				for (int yy=0;yy<=y;yy++)
				{
					yspace+=Ydistances[yy];
				}
				tempsting.Format(_T("%.3f"),yspace);
				mylist.SetItemText(num,5,tempsting);
				//Z��ռ�λ��
				double zspace=0;
				for (int zz=0;zz<=z;zz++)
				{
					zspace+=Zdistances[zz];
				}
				tempsting.Format(_T("%.3f"),zspace);
				mylist.SetItemText(num,6,tempsting);
				num++;
			}
		}
	}
	//������±�־
	redrawflag=false;
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CPropertyPage::OnPaint()
}


BOOL CSensornum::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//��ʼ���б���
	mylist.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT|LVS_EX_ONECLICKACTIVATE|LVS_EX_TRACKSELECT|LVS_EX_BORDERSELECT);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString tempsting;
	//��ʼ������ؼ�
	mylist.InsertColumn(0,_T("X�����"),LVCFMT_LEFT,80);
	mylist.InsertColumn(1,_T("Y�����"),LVCFMT_LEFT,80);
	mylist.InsertColumn(2,_T("Z����"),LVCFMT_LEFT,80);
	mylist.InsertColumn(3,_T("���������"),LVCFMT_LEFT,80);
	mylist.InsertColumn(4,_T("X��λ��"),LVCFMT_LEFT,80);
	mylist.InsertColumn(5,_T("Y��λ��"),LVCFMT_LEFT,80);
	mylist.InsertColumn(6,_T("Z��λ��"),LVCFMT_LEFT,80);
	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CSensornum::OnNMDblclkLsensornum(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	UINT numofselect=mylist.GetSelectedCount();
	if (numofselect==0)
	{
		return;
	}
	POSITION posofselect=mylist.GetFirstSelectedItemPosition();
	if (posofselect==NULL)
	{
		return;
	}
	int index = mylist.GetNextSelectedItem(posofselect);
	//��ȡѡ���������
	CString tempx=mylist.GetItemText(index,0);
	CString tempy=mylist.GetItemText(index,1);
	CString tempz=mylist.GetItemText(index,2);
	unsigned int x=_wtoi(tempx);
	x-=1;
	unsigned int y=_wtoi(tempy);
	y-=1;
	unsigned int z=_wtoi(tempz);
	z-=1;
	//����������ѡ��Ի���
	Cselectnum myselectnum;
	myselectnum.Xposnum=x;
	myselectnum.Yposnum=y;
	myselectnum.Zposnum=z;
	if (IDOK==myselectnum.DoModal())
	{  
		CString tempnum;
		//���ԭ������
		if (myselectnum.SelectNum==-1)
		{
		    tempnum=mylist.GetItemText(index,3);	
			if (tempnum!=_T(""))
			{
				unsigned int numofsensor=_wtoi(tempnum);
				//�����������λ
				sensor_pos[x][y][z]=-1;
			}
			//�����ʾ
			tempnum.Format(_T(""));
			mylist.SetItemText(index,3,tempnum);
		}
		else//���ô������ռ䷽λ
		{
			myselectnum.SelectNum;
			//��鴫�����Ƿ��������
			//for (int xx=0;xx<Xnum;xx++)
			//{
			//	for (int yy=0;yy<Ynum;yy++)
			//	{
			//		for (int zz=0;zz<Znum;zz++)
			//		{
			//			if (sensor_pos[xx][yy][zz]==myselectnum.SelectNum)
			//			{
			//				tempnum.Format(_T("��%d�Ŵ������Ѿ����õ���\nX���%d�У�Y���%d�У�Z���%d��"),myselectnum.SelectNum+1,xx+1,yy+1,zz+1);
			//				MessageBox(tempnum);
			//				return;
			//			}
			//		}
			//	}
			//}
			//��¼��������λ
			sensor_pos[x][y][z]=myselectnum.SelectNum;
			//�����б�
			tempnum.Format(_T("%d"),sensor_pos[x][y][z]+1);
			mylist.SetItemText(index,3,tempnum);
		}
	}
}


LRESULT CSensornum::OnWizardBack()
{
	// TODO: �ڴ����ר�ô����/����û���
	//ɾ������
	mylist.DeleteAllItems();
	//���ø��±�־
	redrawflag=TRUE;
	return CPropertyPage::OnWizardBack();
}


BOOL CSensornum::OnWizardFinish()
{
	// TODO: �ڴ����ר�ô����/����û���
	//���㴫��������
	totalnum=Xnum*Ynum*Znum;
	finishflag=true;
	return CPropertyPage::OnWizardFinish();
}

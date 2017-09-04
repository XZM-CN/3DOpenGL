#include "StdAfx.h"
#include "ADOConn.h"
CADOConn::CADOConn(void)
{
	// ��ʼ���⻷��
	::CoInitialize(NULL);
	//���Ӷ���
	m_pConnection.CreateInstance(_T("ADODB.Connection"));
	//���ݿ��ļ�����
	m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
}

CADOConn::~CADOConn(void)
{
	//�ͷŻ���
	::CoUninitialize();
}
////////////////////////////////Access����/////////////////////////////////////////////////////////////////
//����Access���ݿ��ļ�
BOOL CADOConn::CreateAccess(CString filepathname)
{
	HRESULT hr = S_OK;
	//����Access2007���ļ�SQL�ַ���
	CString myConnection=_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=")+filepathname;
	try
	{
		m_pCatalog->Create(_bstr_t(myConnection)); //Create MDB
		return TRUE;
	}  
	catch(...)
	{
		return FALSE;
	}
}

//����Access���ݿ�
bool CADOConn::ADO_Connection(CString filepathname)
{
	try
	{
		if (m_pConnection->GetState() == adStateOpen)
		{
			m_pConnection->Close();
		}
		m_pConnection->ConnectionTimeout=5;
		//���������ַ���
		_bstr_t myConnection;
		//�����ַ���������Ҫ��������
		myConnection=_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=")+filepathname;
		m_pConnection->Open(myConnection,"","",adModeUnknown);	
		return true;
	}
	catch (...)
	{
		return false;
	}
}
//�Զ�̬��ʽ��Access���ݼ�
_RecordsetPtr CADOConn::ADO_GetRecordSet(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from ");
	sql+=tablename;
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//������¼������
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//ȡ�ñ��еļ�¼
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//�Ծ�̬����Access���ݼ�
_RecordsetPtr CADOConn::ADO_GetRecordSet2(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from ");
	sql+=tablename;
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//������¼������
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//ȡ�ñ��еļ�¼:���þ�̬���ʵʱ��ʾ�����ݿ���޸ģ��ֹ�������꣬�ı�����֧�Ŷ�SQL����ʹ��
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//////////////////////////////////////////////////////////Excel/////////////////////////////////////////////////
//�򿪲�������ݼ�
_RecordsetPtr CADOConn::ADO_GetRecordSetExcel(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from [");
	sql+=tablename;
	sql+=_T("$]");
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//������¼������
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//ȡ�ñ��еļ�¼
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//�����ʹ�Excel�ļ�
BOOL CADOConn::ADO_CreateOrOpenExcel(CString filepathname)
{
	//����Excel2007���ļ�SQL�ַ���
	CString myConnection=_T("Provider=Microsoft.Ace.OleDb.12.0;Data Source=");
	myConnection+=filepathname;
	myConnection+=_T(";Extended Properties=\"Excel 8.0;HDR=Yes;IMEX=0\""); 
	try
	{
		if (m_pConnection->GetState() == adStateOpen)
		{
			return false;
		}
		m_pConnection->ConnectionTimeout=5;
		//���������ַ���
		_bstr_t myconnection=(_bstr_t)myConnection;
		//�����ַ���������Ҫ��������
		m_pConnection->Open(myconnection,"","",adModeUnknown);	
		return true;
	}
	catch (...)
	{
		return false;
	}
}
/////////////////////////////////////////////////////////���ݿ�������ú���////////////////////////////////////////////////////////////
//�½����ݼ�
_RecordsetPtr CADOConn::ADO_Createtable(CString tablename,CString prama)
{
	//�������ݱ�
	CString sql=_T("CREATE TABLE ");
	sql+=tablename;
	sql+=prama;
	_bstr_t bstrsql=(_bstr_t)sql;
	if (ExecuteSQL(bstrsql)==NULL)
	{
		return NULL;
	}
	//�����ݱ�
	return ADO_GetRecordSet(tablename);
}
//ִ��SQL���
_RecordsetPtr CADOConn::ExecuteSQL(_bstr_t bstrSQL)
{
	try
	{
		return m_pConnection->Execute(bstrSQL,NULL,adCmdText);
	}
	catch(...)
	{
		return NULL;
	}
}
bool CADOConn::ADO_ExitConnect()
{
	//�رռ�¼��������
	try
	{
		if (m_pConnection->GetState() == adStateOpen)
		{
			m_pConnection->Close();
		}
		return true;
	}
	catch(...)
	{
       return false;
	}
}
bool CADOConn::ADO_Insertdata(_RecordsetPtr* PIntoreport,CString datastr,double* datas,int num)
{
	static int numofrecord=0;
	CString tempstr;
	CString dataname;
	try
	{
		//���
		tempstr.Format(_T("%d"),numofrecord+1);
		numofrecord++;
		int numofrecords=num/250;
		if (num%250!=0)
		{
			numofrecords+=1;
		}
		for (int i=0;i<numofrecords;i++)
		{
			PIntoreport[i]->AddNew();
			if (i==0)
			{
				PIntoreport[i]->PutCollect(_T("���"),(_bstr_t)tempstr);
				PIntoreport[i]->PutCollect(_T("ʱ��"),(_bstr_t)datastr);
			}
			else
			{
			    PIntoreport[i]->PutCollect(_T("���"),(_bstr_t)tempstr);
			}
		}


		for (int i=0;i<num;i++)
		{
			dataname.Format(_T("�ɼ���%d"),i+1);
			tempstr.Format(_T("%.3f"),datas[i]);
			//��ǰ�������ڵı��
			PIntoreport[i/250]->PutCollect((_bstr_t)dataname,(_bstr_t)tempstr);
		}
		for (int i=0;i<numofrecords;i++)
		{
			PIntoreport[i]->Update();
		}
		return true;
	}
	catch (...)
	{
		return false;
	}
}
bool CADOConn::ADO_CloseRecord(_RecordsetPtr closereport)
{
	//�رռ�¼��������
	try
	{
		if (closereport->GetState() == adStateOpen)
		{
			closereport->Close();
		}
		return true;
	}
	catch(...)
	{
		return false;
	}
}
////////////////////////////�����Ǵ����ݱ��ж�ȡ���ݣ���ͬ����Ŀ���в�ͬ�����ʺ����ڷ�װ/////////////////////////////////
/*
_variant_t   vCount=Rs1->GetCollect("geshu");
int num1=vCount.lVal;   //���������ļ�¼����

pRst->MoveFirst();  //ֻ��ȡ��һ��

_variant_t t = _variant_t(long(6));
result = (LPCSTR)_bstr_t(pRecordset->GetCollect(t));//������ŵķ�ʽ����ȡ�ֶ�����  0based

result = (LPCSTR)_bstr_t(pRecordset->GetCollect("�˿�"));//���ֶ����ķ�ʽ�����ֶ�����


*/

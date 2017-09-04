#include "StdAfx.h"
#include "ADOConn.h"
CADOConn::CADOConn(void)
{
	// 初始化库环境
	::CoInitialize(NULL);
	//连接对象
	m_pConnection.CreateInstance(_T("ADODB.Connection"));
	//数据库文件对象
	m_pCatalog.CreateInstance(__uuidof (ADOX::Catalog));
}

CADOConn::~CADOConn(void)
{
	//释放环境
	::CoUninitialize();
}
////////////////////////////////Access函数/////////////////////////////////////////////////////////////////
//创建Access数据库文件
BOOL CADOConn::CreateAccess(CString filepathname)
{
	HRESULT hr = S_OK;
	//创建Access2007的文件SQL字符串
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

//连接Access数据库
bool CADOConn::ADO_Connection(CString filepathname)
{
	try
	{
		if (m_pConnection->GetState() == adStateOpen)
		{
			m_pConnection->Close();
		}
		m_pConnection->ConnectionTimeout=5;
		//设置连接字符串
		_bstr_t myConnection;
		//链接字符串根据需要具体配置
		myConnection=_T("Provider=Microsoft.ACE.OLEDB.12.0;Data Source=")+filepathname;
		m_pConnection->Open(myConnection,"","",adModeUnknown);	
		return true;
	}
	catch (...)
	{
		return false;
	}
}
//以动态方式打开Access数据集
_RecordsetPtr CADOConn::ADO_GetRecordSet(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from ");
	sql+=tablename;
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//创建记录集对象
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//取得表中的记录
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//以静态光标打开Access数据集
_RecordsetPtr CADOConn::ADO_GetRecordSet2(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from ");
	sql+=tablename;
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//创建记录集对象
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//取得表中的记录:采用静态光标实时显示对数据库的修改，乐观锁定光标，文本命令支撑对SQL语句的使用
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenStatic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//////////////////////////////////////////////////////////Excel/////////////////////////////////////////////////
//打开并获得数据集
_RecordsetPtr CADOConn::ADO_GetRecordSetExcel(CString tablename)
{
	_RecordsetPtr myprecordset;
	CString sql=_T("select *from [");
	sql+=tablename;
	sql+=_T("$]");
	_bstr_t bstrsql=(_bstr_t)sql;
	try
	{
		//创建记录集对象
		myprecordset.CreateInstance(_T("ADODB.Recordset"));
		//取得表中的记录
		myprecordset->Open(bstrsql,m_pConnection.GetInterfacePtr(),adOpenDynamic,adLockOptimistic,adCmdText);
		return myprecordset;
	}
	catch(...)
	{
		return NULL;
	}
}
//创建和打开Excel文件
BOOL CADOConn::ADO_CreateOrOpenExcel(CString filepathname)
{
	//创建Excel2007的文件SQL字符串
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
		//设置连接字符串
		_bstr_t myconnection=(_bstr_t)myConnection;
		//链接字符串根据需要具体配置
		m_pConnection->Open(myconnection,"","",adModeUnknown);	
		return true;
	}
	catch (...)
	{
		return false;
	}
}
/////////////////////////////////////////////////////////数据库操作公用函数////////////////////////////////////////////////////////////
//新建数据集
_RecordsetPtr CADOConn::ADO_Createtable(CString tablename,CString prama)
{
	//建立数据表
	CString sql=_T("CREATE TABLE ");
	sql+=tablename;
	sql+=prama;
	_bstr_t bstrsql=(_bstr_t)sql;
	if (ExecuteSQL(bstrsql)==NULL)
	{
		return NULL;
	}
	//打开数据表
	return ADO_GetRecordSet(tablename);
}
//执行SQL语句
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
	//关闭记录集和链接
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
		//序号
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
				PIntoreport[i]->PutCollect(_T("序号"),(_bstr_t)tempstr);
				PIntoreport[i]->PutCollect(_T("时间"),(_bstr_t)datastr);
			}
			else
			{
			    PIntoreport[i]->PutCollect(_T("序号"),(_bstr_t)tempstr);
			}
		}


		for (int i=0;i<num;i++)
		{
			dataname.Format(_T("采集点%d"),i+1);
			tempstr.Format(_T("%.3f"),datas[i]);
			//当前数据所在的表格
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
	//关闭记录集和链接
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
////////////////////////////如下是从数据表中读取数据，不同的项目各有不同，不适合用于封装/////////////////////////////////
/*
_variant_t   vCount=Rs1->GetCollect("geshu");
int num1=vCount.lVal;   //符合条件的记录个数

pRst->MoveFirst();  //只读取第一行

_variant_t t = _variant_t(long(6));
result = (LPCSTR)_bstr_t(pRecordset->GetCollect(t));//以列序号的方式来读取字段内容  0based

result = (LPCSTR)_bstr_t(pRecordset->GetCollect("人口"));//以字段名的方式来读字段内容


*/

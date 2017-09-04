#pragma once
#include "afxwin.h"
//导入动态库
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")
//导入动态ADOX库
#import "C:/Program Files/Common Files/System/ADO/Msadox.dll"

class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);
public:
	_ConnectionPtr m_pConnection;//指向Connection对象指针
	ADOX::_CatalogPtr m_pCatalog;//指向数据库文件的指针 
//////////////Access2007操作函数////////////////////////////////
	//文件创建函数，filepathname数据库文件的绝对路径名称
	BOOL CreateAccess(CString filepathname);
	//以动态方式打开并获得记录集，tablename数据表名
	_RecordsetPtr ADO_GetRecordSet(CString tablename);
	//以静态方式打开并获取记录，tablename数据表名
	_RecordsetPtr ADO_GetRecordSet2(CString tablename);
	//数据库连接函数,filepathname数据库文件的绝对路径名称
	bool ADO_Connection(CString filepathname);
//////////Excel2007操作函数/////////////////////////////////////
       //打开并获得数据集，tablename数据表名
       _RecordsetPtr ADO_GetRecordSetExcel(CString tablename);
       //创建和打开Excel文件
       BOOL ADO_CreateOrOpenExcel(CString filepathname);
/////////////////////////数据库操作公用函数//////////////////////////////////////////////
       //关闭数据库连接函数
	bool ADO_ExitConnect();	
	//新建数据集,tablename数据表名,param指针参数字符串
	_RecordsetPtr ADO_Createtable(CString tablename,CString prama);
	//关闭数据集 closereport待关闭的数据集
	bool ADO_CloseRecord(_RecordsetPtr closereport);
    //实验数据记录函数
	bool ADO_Insertdata(_RecordsetPtr* PIntoreport,CString datastr,double* datas,int num);
private:
	//数据连接的SQL执行函数，bstrSQL执行的SQL语句，函数私有
	_RecordsetPtr ExecuteSQL(_bstr_t bstrSQL);


};

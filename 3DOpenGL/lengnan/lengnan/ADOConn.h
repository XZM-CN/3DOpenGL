#pragma once
#include "afxwin.h"
//���붯̬��
#import "C:\Program Files\Common Files\System\ado\msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF")
//���붯̬ADOX��
#import "C:/Program Files/Common Files/System/ADO/Msadox.dll"

class CADOConn
{
public:
	CADOConn(void);
	~CADOConn(void);
public:
	_ConnectionPtr m_pConnection;//ָ��Connection����ָ��
	ADOX::_CatalogPtr m_pCatalog;//ָ�����ݿ��ļ���ָ�� 
//////////////Access2007��������////////////////////////////////
	//�ļ�����������filepathname���ݿ��ļ��ľ���·������
	BOOL CreateAccess(CString filepathname);
	//�Զ�̬��ʽ�򿪲���ü�¼����tablename���ݱ���
	_RecordsetPtr ADO_GetRecordSet(CString tablename);
	//�Ծ�̬��ʽ�򿪲���ȡ��¼��tablename���ݱ���
	_RecordsetPtr ADO_GetRecordSet2(CString tablename);
	//���ݿ����Ӻ���,filepathname���ݿ��ļ��ľ���·������
	bool ADO_Connection(CString filepathname);
//////////Excel2007��������/////////////////////////////////////
       //�򿪲�������ݼ���tablename���ݱ���
       _RecordsetPtr ADO_GetRecordSetExcel(CString tablename);
       //�����ʹ�Excel�ļ�
       BOOL ADO_CreateOrOpenExcel(CString filepathname);
/////////////////////////���ݿ�������ú���//////////////////////////////////////////////
       //�ر����ݿ����Ӻ���
	bool ADO_ExitConnect();	
	//�½����ݼ�,tablename���ݱ���,paramָ������ַ���
	_RecordsetPtr ADO_Createtable(CString tablename,CString prama);
	//�ر����ݼ� closereport���رյ����ݼ�
	bool ADO_CloseRecord(_RecordsetPtr closereport);
    //ʵ�����ݼ�¼����
	bool ADO_Insertdata(_RecordsetPtr* PIntoreport,CString datastr,double* datas,int num);
private:
	//�������ӵ�SQLִ�к�����bstrSQLִ�е�SQL��䣬����˽��
	_RecordsetPtr ExecuteSQL(_bstr_t bstrSQL);


};

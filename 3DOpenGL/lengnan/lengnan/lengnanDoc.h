// lengnanDoc.h : ClengnanDoc ��Ľӿ�
//


#pragma once


class ClengnanDoc : public CDocument
{
protected: // �������л�����
	ClengnanDoc();
	DECLARE_DYNCREATE(ClengnanDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~ClengnanDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};



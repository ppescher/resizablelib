// DemoDoc.cpp : implementation of the CDemoDoc class
//

#include "stdafx.h"
#include "Demo.h"

#include "DemoDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc

IMPLEMENT_DYNCREATE(CDemoDoc, CDocument)

BEGIN_MESSAGE_MAP(CDemoDoc, CDocument)
	//{{AFX_MSG_MAP(CDemoDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc construction/destruction

CDemoDoc::CDemoDoc()
{
}

CDemoDoc::~CDemoDoc()
{
}

BOOL CDemoDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDemoDoc serialization

void CDemoDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc diagnostics

#ifdef _DEBUG
void CDemoDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDemoDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDemoDoc commands

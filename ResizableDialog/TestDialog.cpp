// TestDialog.cpp : implementation file
//

#include "stdafx.h"
#include "demo.h"
#include "TestDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDialog dialog


CTestDialog::CTestDialog(CWnd* pParent /*=NULL*/)
	: CResizableDialog(CTestDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDialog)
	m_nStaticType = -1;
	m_bCenterImage = FALSE;
	m_bEndEllipsis = FALSE;
	m_bPathEllipsis = FALSE;
	m_bRealSizeImage = FALSE;
	m_bRightJust = FALSE;
	m_sStaticText = _T("");
	//}}AFX_DATA_INIT
}


void CTestDialog::DoDataExchange(CDataExchange* pDX)
{
	CResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDialog)
	DDX_Control(pDX, IDC_STATIC_TEST, m_ctrlTest);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nStaticType);
	DDX_Check(pDX, IDC_CHECK_CENTERIMAGE, m_bCenterImage);
	DDX_Check(pDX, IDC_CHECK_ENDELLIPSIS, m_bEndEllipsis);
	DDX_Check(pDX, IDC_CHECK_PATHELLIPSIS, m_bPathEllipsis);
	DDX_Check(pDX, IDC_CHECK_REALSIZEIMAGE, m_bRealSizeImage);
	DDX_Check(pDX, IDC_CHECK_RIGHTJUST, m_bRightJust);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_sStaticText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDialog, CResizableDialog)
	//{{AFX_MSG_MAP(CTestDialog)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE, OnButtonUpdate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDialog message handlers

BOOL CTestDialog::OnInitDialog() 
{
	CResizableDialog::OnInitDialog();
	
	AddAnchor(IDC_GROUPBOX, TOP_RIGHT, BOTTOM_RIGHT);
	AddAnchor(IDC_COMBO_TYPE, TOP_RIGHT);
	AddAnchor(IDC_CHECK_CENTERIMAGE, BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK_RIGHTJUST, BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK_REALSIZEIMAGE, BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK_ENDELLIPSIS, BOTTOM_RIGHT);
	AddAnchor(IDC_CHECK_PATHELLIPSIS, BOTTOM_RIGHT);

	AddAnchor(IDC_EDIT_TEXT, BOTTOM_RIGHT);
	AddAnchor(IDC_BUTTON_UPDATE, BOTTOM_RIGHT);
	AddAnchor(IDCANCEL, BOTTOM_RIGHT);

	AddAnchor(m_ctrlTest, TOP_LEFT, BOTTOM_RIGHT);

	SetDefID(IDC_BUTTON_UPDATE);
	
	return FALSE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTestDialog::OnButtonUpdate() 
{
	UpdateData();
	if (m_nStaticType < 0)
		return;

	CRect rect;
	m_ctrlTest.GetWindowRect(rect);
	ScreenToClient(rect);

	RemoveAnchor(m_ctrlTest);
	m_ctrlTest.DestroyWindow();

	UINT nStyle = m_nStaticType & SS_TYPEMASK;
	if (m_bCenterImage)
		nStyle |= SS_CENTERIMAGE;
	if (m_bEndEllipsis)
		nStyle |= SS_ENDELLIPSIS;
	if (m_bPathEllipsis)
		nStyle |= SS_PATHELLIPSIS;
	if (m_bRealSizeImage)
		nStyle |= SS_REALSIZEIMAGE;
	if (m_bRightJust)
		nStyle |= SS_RIGHTJUST;

	m_ctrlTest.Create(m_sStaticText, nStyle|WS_CHILD|WS_VISIBLE, rect, this, IDC_STATIC_TEST);
	AddAnchor(m_ctrlTest, TOP_LEFT, BOTTOM_RIGHT);
}


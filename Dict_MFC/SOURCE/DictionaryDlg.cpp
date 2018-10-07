
// DictionaryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Dictionary.h"
#include "DictionaryDlg.h"
#include "afxdialogex.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDictionaryDlg dialog



CDictionaryDlg::CDictionaryDlg(CWnd* pParent /*=NULL*/)
: CDialogEx(CDictionaryDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);


}

void CDictionaryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, edit_keyword);
	DDX_Control(pDX, IDC_EDIT_RESULT, edit_result);
	DDX_Control(pDX, IDC_BUTTON_SEARCH, button_search);
	DDX_Control(pDX, IDC_EDIT_KEYWORD_CLUE, edit_keyword_clue);
	DDX_Control(pDX, IDC_EDIT_KEYWORD_CLUE_2, edit_keyword_clue_2);
}

BEGIN_MESSAGE_MAP(CDictionaryDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()

	ON_EN_CHANGE(IDC_EDIT_KEYWORD, &CDictionaryDlg::OnEnChangeEdit2)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, &CDictionaryDlg::OnBnClickedResult)
	ON_EN_CHANGE(IDC_EDIT_RESULT, &CDictionaryDlg::OnEnChangeEditKeyword2)
	ON_EN_CHANGE(IDC_EDIT_KEYWORD_CLUE, &CDictionaryDlg::OnEnChangeEditKeywordClue)
END_MESSAGE_MAP()


// CDictionaryDlg message handlers

BOOL CDictionaryDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	HuffmanExtraction tmp("data.txt");
	data = tmp;
	data.extraction(tree);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDictionaryDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDictionaryDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
	AfxMessageBox(_T("Loading completed!"), MB_ICONINFORMATION);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDictionaryDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDictionaryDlg::OnEnChangeEdit2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}


void CDictionaryDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}


void CDictionaryDlg::OnEnChangeEditKeyword2()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CDictionaryDlg::OnBnClickedResult()
{
	UpdateData(true);
	CString str;
	edit_keyword.GetWindowText(str);

	wstring word(str);
	wstring meaning;
	wstring keyword_clue = L"";
	wstring keyword_clue_2 = L"";
	meaning = tree.search(word);

	vector<wstring> res;
	tree.traverse2(res, word);

	int i;
	int count = 0;
	for (i = 0; i < res.size(); i++){
		if (count + res[i].length() < 65)
		{
			keyword_clue = keyword_clue + L"~" + res[i];
		}
		else  if (i < 20)
		{
			keyword_clue_2 = keyword_clue_2 + L"~" + res[i];
		}
		else break;
		count += res[i].length() + 1;
	}

	CString str_result(meaning.c_str());
	CString str_clue(keyword_clue.c_str());
	CString str_clue_2(keyword_clue_2.c_str());

	edit_result.SetWindowText(str_result);
	edit_keyword_clue.SetWindowText(str_clue);
	edit_keyword_clue_2.SetWindowText(str_clue_2);

	UpdateData(false);
	return;
}

void CDictionaryDlg::OnEnChangeEditKeywordClue()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

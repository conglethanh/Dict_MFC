
// DictionaryDlg.h : header file
//

#pragma once
#include "afxwin.h"

#include "BTree.h"
#include "HuffmanExtraction.h"


// CDictionaryDlg dialog
class CDictionaryDlg : public CDialogEx
{
// Construction
public:
	CDictionaryDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_DICTIONARY_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEditKeyword2();

	afx_msg void OnBnClickedResult();
protected:
	CEdit edit_result;
	CEdit edit_keyword;
	CButton button_search;
	BTree tree;
	HuffmanExtraction data;
public:
	afx_msg void OnEnChangeEditKeywordClue();
protected:
	CEdit edit_keyword_clue;
	CEdit edit_keyword_clue_2;
};

// HW4_Holland_HoDlg.h : header file
//

#pragma once


// CHW4HollandHoDlg dialog
class CHW4HollandHoDlg : public CDialogEx
{
// Construction
public:
	CHW4HollandHoDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HW4_HOLLAND_HO_DIALOG };
#endif

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
	afx_msg void OnBnClickedSend();
	afx_msg void OnEnChangeEditChat();
	afx_msg void OnEnChangeEditShow();
};

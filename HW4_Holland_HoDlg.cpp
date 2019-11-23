
// HW4_Holland_HoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HW4_Holland_Ho.h"
#include "HW4_Holland_HoDlg.h"
#include "afxdialogex.h"
// Include the window socket library
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>
#include <thread>
using namespace std;

bool msgEnd = false;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About
SOCKET MakeSocket(unsigned short Port) {
	SOCKET sock = INVALID_SOCKET;
	SOCKADDR_IN Addr = { 0 };

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		WSACleanup();

		return 1;
	}

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	//Addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	inet_pton(AF_INET, "127.0.0.1", &Addr.sin_addr.s_addr);

	if (bind(sock, (SOCKADDR *)&Addr, sizeof(Addr)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();

		return 1;
	}

	// if socket is successful
	return sock;
}

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHW4HollandHoDlg dialog



CHW4HollandHoDlg::CHW4HollandHoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HW4_HOLLAND_HO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}



void CHW4HollandHoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHW4HollandHoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CHW4HollandHoDlg::OnBnClickedSend)
	ON_EN_CHANGE(IDC_EDIT_CHAT, &CHW4HollandHoDlg::OnEnChangeEditChat)
	ON_EN_CHANGE(IDC_EDIT_SHOW, &CHW4HollandHoDlg::OnEnChangeEditShow)
END_MESSAGE_MAP()

// CHW4HollandHoDlg message handlers

BOOL CHW4HollandHoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	AfxBeginThread(listenfunc, this, THREAD_PRIORITY_NORMAL, 0, 0, NULL);
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

UINT __cdecl CHW4HollandHoDlg::listenfunc(LPVOID pParam) {
	ASSERT(pParam != NULL);
	CHW4HollandHoDlg* getMsg = reinterpret_cast<CHW4HollandHoDlg*>(pParam);
	return getMsg->disMessage();
}

UINT __cdecl CHW4HollandHoDlg::disMessage() {
	SOCKET sock = INVALID_SOCKET;
	SOCKADDR_IN RecvAddr = { 0 };
	int iRet;
	int iRecvSize;
	CString message;
	CString chatLog;
	char buf[1024];
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (sock == INVALID_SOCKET) {
		WSACleanup();

		return 1;
	}

	RecvAddr.sin_family = AF_INET;
	RecvAddr.sin_port = htons(3515);
	inet_pton(AF_INET, "127.0.0.1", &RecvAddr.sin_addr.s_addr);

	if (bind(sock, (SOCKADDR*)&RecvAddr, sizeof(RecvAddr)) == SOCKET_ERROR) {
		closesocket(sock);
		WSACleanup();

		return 1;
	}

	while (1) {
		iRecvSize = sizeof(RecvAddr);
		iRet = recvfrom(sock, buf, 1024, 0, (SOCKADDR*)&RecvAddr, &iRecvSize);

		if (iRet == SOCKET_ERROR) {
			continue;
		}

		buf[iRet] = '\0';

		message = CString(buf);
		GetDlgItemText(IDC_EDIT_SHOW, chatLog);
		SetDlgItemText(IDC_EDIT_SHOW, chatLog + _T("Oates:") + message + _T("\r\n"));


	}

	return 0;

}



void CHW4HollandHoDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CHW4HollandHoDlg::OnPaint()
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
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHW4HollandHoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CHW4HollandHoDlg::OnBnClickedSend()
{
	SOCKET sock = INVALID_SOCKET;
	SOCKADDR_IN Addr = { 0 };
	SOCKADDR_IN SendAddr = { 0 };
	char buf[1024];
	CString message;
	CString chatLog; 
	CString init = _T("");
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	SendAddr.sin_family = AF_INET;
	SendAddr.sin_port = htons(3514);
	inet_pton(AF_INET, "127.0.0.1", &SendAddr.sin_addr.s_addr);
	GetDlgItemText(IDC_EDIT_CHAT, message);
	GetDlgItemText(IDC_EDIT_SHOW, chatLog);
	SetDlgItemText(IDC_EDIT_SHOW, chatLog + _T("Me: ") + message + _T("\r\n"));
	SetDlgItemText(IDC_EDIT_CHAT, init);

	strcpy_s(buf, CStringA(message).GetString());

	sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&SendAddr, sizeof(SendAddr));
	
	closesocket(sock);

}


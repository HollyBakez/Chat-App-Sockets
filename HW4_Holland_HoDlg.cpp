
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

using namespace std;
// define our ip address as local host

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

SOCKET MakeSocket(unsigned short Port) {
	SOCKET sock = INVALID_SOCKET;
	SOCKADDR_IN Addr = { 0 };

	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock == INVALID_SOCKET) {
		cout << "socket function failed with error = %d\n" << WSAGetLastError();
		WSACleanup();

		return 1;
	}

	Addr.sin_family = AF_INET;
	Addr.sin_port = htons(Port);
	Addr.sin_addr.s_addr = inet_addr("127.0.0.1");


	if (bind(sock, (SOCKADDR *)&Addr, sizeof(Addr)) == SOCKET_ERROR) {

		cout << "bind failed with error %u\n " << WSAGetLastError();
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

CString SendData(SOCKET sock, unsigned short Port) {
	SOCKADDR_IN SendAddr = { 0 };
	char buf[1024];
	CString msgDisplay;

	SendAddr.sin_family = AF_INET;
	SendAddr.sin_port = htons(Port);
	SendAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

	fgets(buf, 1024, stdin);
	msgDisplay = buf;
	sendto(sock, buf, strlen(buf), 0, (SOCKADDR*)&SendAddr, sizeof(SendAddr));
	return msgDisplay;
}

void CHW4HollandHoDlg::OnBnClickedSend()
{
	CString msgDisplay  ;
	// we can set this var with sendthread to display our sent msgs
	CString msgRecv = "Hello World";

	WSADATA wsaData = {0};
	SOCKET sock; 
	SOCKADDR_IN RecvAddr;

	// assign var to ports
	unsigned short DesPort = 3514;

	unsigned short RecPort = 3515; 

	WSAStartup(MAKEWORD(2, 2), &wsaData); 

	sock = MakeSocket(RecPort);


	// this needs to send our msg to our chat box and Oates' Chat box
	// To read text from the chat message control 
	if (sock) { 

		//HANDLE hThread = CreateThread(NULL, 0, RecvThread, (PVOID)sock, 0, NULL);
		// 1. type our msg 
		// 2. click send
		// 3. send data as our buf 

			msgDisplay = SendData(sock, DesPort);
			GetDlgItemText(IDC_EDIT_CHAT, msgDisplay);
		
		// this read's the users text in and stores it in memory
		//GetDlgItemText(IDC_EDIT_CHAT, msgDisplay);

		// sets the a message into the empty send field
		SetDlgItemText(IDC_EDIT_CHAT, msgRecv);

		// set's the chatMessage in the read only text box
		SetDlgItemText(IDC_EDIT_SHOW, msgDisplay);
	}

}


void CHW4HollandHoDlg::OnEnChangeEditChat()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

}	


void CHW4HollandHoDlg::OnEnChangeEditShow()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here

	// recv the data from the other chat application and display
	// also to send the msg that we send needs to be displayed 
	// 2 things

}

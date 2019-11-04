
// HW4_Holland_Ho.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CHW4HollandHoApp:
// See HW4_Holland_Ho.cpp for the implementation of this class
//

class CHW4HollandHoApp : public CWinApp
{
public:
	CHW4HollandHoApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CHW4HollandHoApp theApp;

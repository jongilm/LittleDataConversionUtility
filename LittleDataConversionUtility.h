// LittleDataConversionUtility.h : main header file for the LITTLEDATACONVERSIONUTILITY application
//

#if !defined(AFX_LITTLEDATACONVERSIONUTILITY_H__E9D296CD_ED4D_4C08_953A_932805A83A8D__INCLUDED_)
#define AFX_LITTLEDATACONVERSIONUTILITY_H__E9D296CD_ED4D_4C08_953A_932805A83A8D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CLittleDataConversionUtilityApp:
// See LittleDataConversionUtility.cpp for the implementation of this class
//

class CLittleDataConversionUtilityApp : public CWinApp
{
public:
	CLittleDataConversionUtilityApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLittleDataConversionUtilityApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CLittleDataConversionUtilityApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LITTLEDATACONVERSIONUTILITY_H__E9D296CD_ED4D_4C08_953A_932805A83A8D__INCLUDED_)

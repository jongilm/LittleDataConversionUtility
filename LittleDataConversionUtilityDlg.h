// LittleDataConversionUtilityDlg.h : header file
//

#if !defined(AFX_LITTLEDATACONVERSIONUTILITYDLG_H__2993E26A_E3B7_4827_A78B_31B07E952ABA__INCLUDED_)
#define AFX_LITTLEDATACONVERSIONUTILITYDLG_H__2993E26A_E3B7_4827_A78B_31B07E952ABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CLittleDataConversionUtilityDlg dialog

class CLittleDataConversionUtilityDlg : public CDialog
{
// Construction
public:
	CLittleDataConversionUtilityDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CLittleDataConversionUtilityDlg)
	enum { IDD = IDD_LITTLEDATACONVERSIONUTILITY_DIALOG };
	CString	m_Edit1;
	CString	m_Edit2;
	int		m_Radio_Encode;
	DWORD	m_Edit1_Size;
	DWORD	m_Edit2_Size;
	int		m_Action;
	int		m_Format;
	CString	m_Key;
	CString	m_UserId;
	CString	m_RandomChallenge;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLittleDataConversionUtilityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLittleDataConversionUtilityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonConvert();
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnRadio1Base64();
	afx_msg void OnRadio1Hex();
	afx_msg void OnRadio1Md5();
	afx_msg void OnButtonClose();
	afx_msg void OnRadio1HmacMd5();
	afx_msg void OnRadio1CramMd5();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
  CFont m_Font_Title;
  CFont m_Font_MultilineEditFields;

  void UnEscapeData(CString &Str1);
  void EscapeData(CString &Str1);
  void UnEscapeData(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);
  void EscapeData(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LITTLEDATACONVERSIONUTILITYDLG_H__2993E26A_E3B7_4827_A78B_31B07E952ABA__INCLUDED_)

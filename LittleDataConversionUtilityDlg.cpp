// LittleDataConversionUtilityDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LittleDataConversionUtility.h"
#include "LittleDataConversionUtilityDlg.h"

#include "md5.h"
#include "hmac.h"
#include "base64.h"
#include "hexdump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//#include <stdlib.h>
//#include <stdio.h>
//#include <string.h>
//#include <ctype.h>
//#include <conio.h>
//#include <io.h>

//#define TRUE  -1
//#define FALSE 0

#define GETFIELDDATA() UpdateData(true)
#define SETFIELDDATA() UpdateData(false)

enum // m_Action
{
  ACTION_BASE64 = 0,
  ACTION_HEX,
  ACTION_MD5,
  ACTION_HMAC_MD5,
  ACTION_CRAM_MD5,
  ACTION_DEFAULT = ACTION_BASE64
};

enum // m_Format
{
  FORMAT_BASIC,
  FORMAT_NORMAL,
  FORMAT_VERBOSE,
  FORMAT_DEFAULT = FORMAT_BASIC
};

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLittleDataConversionUtilityDlg dialog

CLittleDataConversionUtilityDlg::CLittleDataConversionUtilityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLittleDataConversionUtilityDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLittleDataConversionUtilityDlg)
	m_Edit1 = _T("");
	m_Edit2 = _T("");
	m_Radio_Encode = -1;
	m_Edit1_Size = 0;
	m_Edit2_Size = 0;
	m_Action = -1;
	m_Format = -1;
	m_Key = _T("");
	m_UserId = _T("");
	m_RandomChallenge = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CLittleDataConversionUtilityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLittleDataConversionUtilityDlg)
	DDX_Text(pDX, IDC_EDIT1, m_Edit1);
	DDX_Text(pDX, IDC_EDIT2, m_Edit2);
	DDX_Radio(pDX, IDC_RADIO_ENCODE, m_Radio_Encode);
	DDX_Text(pDX, IDC_EDIT1_SIZE, m_Edit1_Size);
	DDX_Text(pDX, IDC_EDIT2_SIZE, m_Edit2_Size);
	DDX_Radio(pDX, IDC_RADIO1_BASE64, m_Action);
	DDX_Radio(pDX, IDC_RADIO_FORMAT_BASIC, m_Format);
	DDX_Text(pDX, IDC_EDIT_KEY, m_Key);
	DDX_Text(pDX, IDC_EDIT_USERID, m_UserId);
	DDX_Text(pDX, IDC_EDIT_RANDOMCHALLENGE, m_RandomChallenge);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CLittleDataConversionUtilityDlg, CDialog)
	//{{AFX_MSG_MAP(CLittleDataConversionUtilityDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONVERT, OnButtonConvert)
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_EN_CHANGE(IDC_EDIT2, OnChangeEdit2)
	ON_BN_CLICKED(IDC_RADIO1_BASE64, OnRadio1Base64)
	ON_BN_CLICKED(IDC_RADIO1_HEX, OnRadio1Hex)
	ON_BN_CLICKED(IDC_RADIO1_MD5, OnRadio1Md5)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnButtonClose)
	ON_BN_CLICKED(IDC_RADIO1_HMAC_MD5, OnRadio1HmacMd5)
	ON_BN_CLICKED(IDC_RADIO1_CRAM_MD5, OnRadio1CramMd5)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLittleDataConversionUtilityDlg message handlers

BOOL CLittleDataConversionUtilityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
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

  m_Font_Title.CreatePointFont(120, _T("Arial")); // Creates a 12-point-Arial-font
  GetDlgItem(IDC_STATIC_TITLE)->SetFont(&m_Font_Title); // Without a member variable

  
  m_Font_MultilineEditFields.CreatePointFont(80, _T("Courier")); // Creates a 8-point-Courier-font
  //m_Edit1.SetFont(&m_Font_MultilineEditFields); // With a member variable associated to the static control
  //m_Edit2.SetFont(&m_Font_MultilineEditFields); // With a member variable associated to the static control
  GetDlgItem(IDC_EDIT1)->SetFont(&m_Font_MultilineEditFields); // Without a member variable
  GetDlgItem(IDC_EDIT2)->SetFont(&m_Font_MultilineEditFields); // Without a member variable


  GETFIELDDATA();
  m_Action = ACTION_DEFAULT; // 0=Base64, 1=Hex, 2=MD5 (default=Base64) 
  m_Format = FORMAT_DEFAULT;
  OnRadio1Base64();
  m_Radio_Encode = 0; // default = Encode
  SETFIELDDATA();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CLittleDataConversionUtilityDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CLittleDataConversionUtilityDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

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
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CLittleDataConversionUtilityDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CLittleDataConversionUtilityDlg::UnEscapeData(CString &Str1)
{
  unsigned char *pBufferIn;
  unsigned int cbBufferIn;
  unsigned char *pBufferOut;
  unsigned int cbBufferOut;

  cbBufferIn = Str1.GetLength();
  pBufferIn = (unsigned char *)Str1.GetBuffer(cbBufferIn);
  cbBufferOut = cbBufferIn*2;
  pBufferOut = (unsigned char *)malloc(cbBufferOut);

  UnEscapeData(pBufferIn, cbBufferIn, pBufferOut, &cbBufferOut);
  Str1.ReleaseBuffer(cbBufferIn);

  cbBufferIn = cbBufferOut;
  pBufferIn = (unsigned char *)Str1.GetBuffer(cbBufferIn);
  memcpy(pBufferIn,pBufferOut,cbBufferOut);
  Str1.ReleaseBuffer(cbBufferOut);

  free(pBufferOut);
}

void CLittleDataConversionUtilityDlg::EscapeData(CString &Str1)
{
  unsigned char *pBufferIn;
  unsigned int cbBufferIn;
  unsigned char *pBufferOut;
  unsigned int cbBufferOut;

  cbBufferIn = Str1.GetLength();
  pBufferIn = (unsigned char *)Str1.GetBuffer(cbBufferIn);
  cbBufferOut = cbBufferIn*2;
  pBufferOut = (unsigned char *)malloc(cbBufferOut);

  EscapeData(pBufferIn, cbBufferIn, pBufferOut, &cbBufferOut);
  Str1.ReleaseBuffer(cbBufferIn);

  cbBufferIn = cbBufferOut;
  pBufferIn = (unsigned char *)Str1.GetBuffer(cbBufferIn);
  memcpy(pBufferIn,pBufferOut,cbBufferOut);
  Str1.ReleaseBuffer(cbBufferOut);

  free(pBufferOut);
}

void CLittleDataConversionUtilityDlg::UnEscapeData(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
{
  unsigned int i;
  unsigned int j;

  *pcbBufferOut = 0;

  for(i=0,j=0;i<cbBufferIn;i++)
  {
    if (pBufferIn[i] == '\\')
    {
      switch(pBufferIn[i+1])
      {
        case '0': pBufferOut[j++] = '\0'; break;
        case 'n': pBufferOut[j++] = '\n'; break;
        case 'r': pBufferOut[j++] = '\r'; break;
        case 't': pBufferOut[j++] = '\t'; break;
        case 'f': pBufferOut[j++] = '\f'; break;
        case '\\': pBufferOut[j++] = '\\'; break;
      }
      i++;
    }
    else
      pBufferOut[j++] = pBufferIn[i];
  }
  *pcbBufferOut = j;
}

void CLittleDataConversionUtilityDlg::EscapeData(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
{
  unsigned int i;
  unsigned int j;

  *pcbBufferOut = 0;
  for(i=0,j=0;i<cbBufferIn;i++)
  {
    switch(pBufferIn[i])
    {
      case '\0': pBufferOut[j++] = '\\'; pBufferOut[j++] = '0'; break;
      case '\n': pBufferOut[j++] = '\\'; pBufferOut[j++] = 'n'; break;
      case '\r': pBufferOut[j++] = '\\'; pBufferOut[j++] = 'r'; break;
      case '\t': pBufferOut[j++] = '\\'; pBufferOut[j++] = 't'; break;
      case '\f': pBufferOut[j++] = '\\'; pBufferOut[j++] = 'f'; break;
      case '\\': pBufferOut[j++] = '\\'; pBufferOut[j++] = '\\'; break;
      default: pBufferOut[j++] = pBufferIn[i]; break;
    }
  }
  *pcbBufferOut = j;
}

void CLittleDataConversionUtilityDlg::OnButtonConvert() 
{
  unsigned char *pBuffer1;
  unsigned int cbBuffer1;
  unsigned char *pBuffer2;
  unsigned int cbBuffer2;

  GETFIELDDATA();

  hexdump_config(HEX_USE_CRLF                   ,TRUE);
  hexdump_config(HEX_ASCII_ONLY                 ,FALSE);
  hexdump_config(HEX_DISPLAY_HEADERS            ,m_Format>=FORMAT_VERBOSE);
  hexdump_config(HEX_DISPLAY_ADDRESSES          ,m_Format>=FORMAT_NORMAL);
  hexdump_config(HEX_DISPLAY_ASCII_CHARS        ,m_Format>=FORMAT_NORMAL);
  hexdump_config(HEX_DISPLAY_INTERMEDIATE_SPACES,m_Format>=FORMAT_NORMAL);
  base64_config (B64_USE_CRLF                   ,TRUE);

  UnEscapeData(m_Edit1);
  //UnEscapeData(m_Edit2);

  if (m_Action == ACTION_BASE64) // 0=Base64, 1=Hex, 2=MD5 (default=Base64) 
  {
    if (m_Radio_Encode==0) // Encode
    {
      cbBuffer1 = m_Edit1.GetLength();
      pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
      m_Edit2.Empty();
      pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
      cbBuffer2 = 0;

      encodeB64(pBuffer1, cbBuffer1, pBuffer2, &cbBuffer2);
      m_Edit2.ReleaseBuffer(cbBuffer2);
    }
    else if (m_Radio_Encode==1) // Decode
    {
      cbBuffer2 = m_Edit2.GetLength();
      pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(cbBuffer2);
      m_Edit1.Empty();
      pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
      cbBuffer1 = 0;

      decodeB64(pBuffer2, cbBuffer2, pBuffer1, &cbBuffer1);
      m_Edit1.ReleaseBuffer(cbBuffer1);
    }
  }
  else if (m_Action == ACTION_HEX) // 0=Base64, 1=Hex, 2=MD5 (default=Base64) 
  {
    if (m_Radio_Encode==0) // Encode
    {
      int rc;

      cbBuffer1 = m_Edit1.GetLength();
      pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
      m_Edit2.Empty();
      pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
      cbBuffer2 = 0;

      rc = hexdump(pBuffer1, cbBuffer1, pBuffer2, &cbBuffer2);
      m_Edit2.ReleaseBuffer(cbBuffer2);
    }
    else if (m_Radio_Encode==1) // Decode
    {
      int rc;

      cbBuffer2 = m_Edit2.GetLength();
      pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(cbBuffer2);
      m_Edit1.Empty();
      pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
      cbBuffer1 = 0;

      rc = hexundump(pBuffer2, cbBuffer2, pBuffer1, &cbBuffer1);
      m_Edit1.ReleaseBuffer(cbBuffer1);
    }
  }
  else if (m_Action == ACTION_MD5) // 0=Base64, 1=Hex, 2=MD5 (default=Base64) 
  {
    MD5_CTX context;
    unsigned char digest[16];
    int rc;

    cbBuffer1 = m_Edit1.GetLength();
    pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
    m_Edit2.Empty();
    pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
    cbBuffer2 = 0;

    MD5Init(&context);
    MD5Update(&context, pBuffer1, cbBuffer1); // context, unsigned char *input, unsigned int inputLen;
    MD5Final(digest, &context); // unsigned char digest[16], context

    rc = hexdump(digest, 16, pBuffer2, &cbBuffer2);
    m_Edit2.ReleaseBuffer(cbBuffer2);
  }
  else if (m_Action == ACTION_HMAC_MD5)
  {
    unsigned char digest[16];
    int rc;

    cbBuffer1 = m_Edit1.GetLength();
    pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
    m_Edit2.Empty();
    pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
    cbBuffer2 = 0;

    hmac_md5 ( pBuffer1, cbBuffer1, (unsigned char *)((LPCTSTR)m_Key), m_Key.GetLength(), digest);

    rc = hexdump(digest, 16, pBuffer2, &cbBuffer2);
    m_Edit2.ReleaseBuffer(cbBuffer2);
  }
  else if (m_Action == ACTION_CRAM_MD5)
  {
    //---------------------------------------------------------------
    // RFC2104 says...
    // The client makes note of the data and then responds with a string
    //   consisting of the user name, a space, and a 'digest'.  The latter is
    //   computed by applying the keyed MD5 algorithm from [KEYED-MD5] where
    //   the key is a shared secret and the digested text is the timestamp
    //   (including angle-brackets).
    //
    //     S: * OK IMAP4 Server
    //     C: A0001 AUTHENTICATE CRAM-MD5
    //     S: + PDE4OTYuNjk3MTcwOTUyQHBvc3RvZmZpY2UucmVzdG9uLm1jaS5uZXQ+
    //     C: dGltIGI5MTNhNjAyYzdlZGE3YTQ5NWI0ZTZlNzMzNGQzODkw
    //     S: A0001 OK CRAM authentication successful
    //
    // Random challenge received from server is...
    // PDE4OTYuNjk3MTcwOTUyQHBvc3RvZmZpY2UucmVzdG9uLm1jaS5uZXQ+
    // Decoded from base64 is...
    // <1896.697170952@postoffice.reston.mci.net>
    // Shared secret is the string 'tanstaaftanstaaf'.
    //
    // MD5((tanstaaftanstaaf XOR opad),
    //     MD5((tanstaaftanstaaf XOR ipad),
    //         <1896.697170952@postoffice.reston.mci.net>))
    //             
    // b913a602c7eda7a495b4e6e7334d3890
    // prepended with usename is...
    // tim b913a602c7eda7a495b4e6e7334d3890
    // encoded to base64 is...
    // dGltIGI5MTNhNjAyYzdlZGE3YTQ5NWI0ZTZlNzMzNGQzODkw
    // ---------------------------------------------------------------
    // My own example....
    // S:* OK Welcome to S-PC156W2K.ste.sharp-eu.com, with Code-Crafters Ability Mail Server 2.52.                                                                       
    // S:* CAPABILITY IMAP4rev1 IDLE LITERAL+ UIDPLUS CHILDREN AUTH=PLAIN AUTH=CRAM-MD5 AUTH=CRAM-SHA1                                                                   
    // C:1 CAPABILITY
    // S:1 OK CAPABILITY completed.                                                      
    // C:2 AUTHENTICATE CRAM-MD5
    // S:+ PDUzNi4yMDA3MTAyMzEzMDkzNUBTLVBDMTU2VzJLLnN0ZS5zaGFycC1ldS5jb20+              
    // C:amcgQzAyOUYyNkI4QTY0M0MxRkQ3RDAzMkE5NkRERDc3NkQ=
    // S:2 OK AUTHENTICATE completed.                                                    
    //
    // where, Random Challenge is...
    //   PDUzNi4yMDA3MTAyMzEzMDkzNUBTLVBDMTU2VzJLLnN0ZS5zaGFycC1ldS5jb20+
    // decodes to...
    //   <536.20071023130935@S-PC156W2K.ste.sharp-eu.com>
    // then, using password "password" with HMAC_MD5 produces...
    //   C029F26B8A643C1FD7D032A96DDD776D
    // then prefix with userid...
    //   jg C029F26B8A643C1FD7D032A96DDD776D
    // and encode to base64, produces...
    //   amcgQzAyOUYyNkI4QTY0M0MxRkQ3RDAzMkE5NkRERDc3NkQ=
    // which is sent as the challenge response.
    //---------------------------------------------------------------

    unsigned char digest[16];
    int rc;

    // Hexdump must produce a raw 01020304...EF result
    hexdump_config(HEX_USE_CRLF                   ,TRUE);
    hexdump_config(HEX_ASCII_ONLY                 ,FALSE);
    hexdump_config(HEX_DISPLAY_HEADERS            ,m_Format>=FORMAT_VERBOSE);
    hexdump_config(HEX_DISPLAY_ADDRESSES          ,m_Format>=FORMAT_NORMAL);
    hexdump_config(HEX_DISPLAY_ASCII_CHARS        ,m_Format>=FORMAT_NORMAL);
    hexdump_config(HEX_DISPLAY_INTERMEDIATE_SPACES,m_Format>=FORMAT_NORMAL);
    base64_config (B64_USE_CRLF                   ,TRUE);

    // Decode RandomChallenge from Base64 to Text
    cbBuffer2 = m_RandomChallenge.GetLength();
    pBuffer2 = (unsigned char *)m_RandomChallenge.GetBuffer(cbBuffer2);
    m_Edit1.Empty();
    pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
    cbBuffer1 = 0;

    decodeB64(pBuffer2, cbBuffer2, pBuffer1, &cbBuffer1);
    m_Edit1.ReleaseBuffer(cbBuffer1);

    // Perform HMAC_MD5 on RandomChallenge text using the shared secret (password) as the key, yielding the MAC
    cbBuffer1 = m_Edit1.GetLength();
    pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
    m_Edit2.Empty();
    pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
    cbBuffer2 = 0;

    hmac_md5 ( pBuffer1, cbBuffer1, (unsigned char *)((LPCTSTR)m_Key), m_Key.GetLength(), digest);

    // Convert MAC from Binary to Raw Hex
    rc = hexdump(digest, 16, pBuffer2, &cbBuffer2);
    m_Edit2.ReleaseBuffer(cbBuffer2);

    // Encode "<UserId> <MAC>" to base64
    m_Edit1 = m_UserId;
    m_Edit1 += " ";
    m_Edit1 += m_Edit2;

    cbBuffer1 = m_Edit1.GetLength();
    pBuffer1 = (unsigned char *)m_Edit1.GetBuffer(cbBuffer1);
    m_Edit2.Empty();
    pBuffer2 = (unsigned char *)m_Edit2.GetBuffer(ENCODE_BUFFER_SIZE_OUT);
    cbBuffer2 = 0;

    encodeB64(pBuffer1, cbBuffer1, pBuffer2, &cbBuffer2);
    m_Edit2.ReleaseBuffer(cbBuffer2);
  }

  EscapeData(m_Edit1);
  //EscapeData(m_Edit2);
  //GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  SETFIELDDATA();

  OnChangeEdit1();
  OnChangeEdit2();
  GetDlgItem(IDC_EDIT1)->SetFocus();
}


void CLittleDataConversionUtilityDlg::OnChangeEdit1() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
  GETFIELDDATA();
  m_Edit1_Size = m_Edit1.GetLength();
  SETFIELDDATA();
}

void CLittleDataConversionUtilityDlg::OnChangeEdit2() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
  GETFIELDDATA();
  m_Edit2_Size = m_Edit2.GetLength();
  SETFIELDDATA();
}


void CLittleDataConversionUtilityDlg::OnRadio1Base64() 
{
  GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_ENCODE_DECODE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_ENCODE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_DECODE)->EnableWindow(TRUE); // Without a member variable
  
  GetDlgItem(IDC_STATIC_FORMAT)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_VERBOSE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_NORMAL)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_BASIC)->EnableWindow(FALSE); // Without a member variable

  GetDlgItem(IDC_EDIT_RANDOMCHALLENGE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_USERID)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_KEY)->EnableWindow(FALSE); // Without a member variable
}

void CLittleDataConversionUtilityDlg::OnRadio1Hex() 
{
  GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_ENCODE_DECODE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_ENCODE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_DECODE)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_FORMAT)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_VERBOSE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_NORMAL)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_BASIC)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_EDIT_RANDOMCHALLENGE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_USERID)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_KEY)->EnableWindow(FALSE); // Without a member variable
}

void CLittleDataConversionUtilityDlg::OnRadio1Md5() 
{
  GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_ENCODE_DECODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_ENCODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_DECODE)->EnableWindow(FALSE); // Without a member variable

  GetDlgItem(IDC_STATIC_FORMAT)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_VERBOSE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_NORMAL)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_BASIC)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_EDIT_RANDOMCHALLENGE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_USERID)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_KEY)->EnableWindow(FALSE); // Without a member variable
}

void CLittleDataConversionUtilityDlg::OnRadio1HmacMd5() 
{
  GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_ENCODE_DECODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_ENCODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_DECODE)->EnableWindow(FALSE); // Without a member variable

  GetDlgItem(IDC_STATIC_FORMAT)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_VERBOSE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_NORMAL)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_BASIC)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_EDIT_RANDOMCHALLENGE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_USERID)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT_KEY)->EnableWindow(TRUE); // Without a member variable
}

void CLittleDataConversionUtilityDlg::OnRadio1CramMd5() 
{
  GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_STATIC_ENCODE_DECODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_ENCODE)->EnableWindow(FALSE); // Without a member variable
  GetDlgItem(IDC_RADIO_DECODE)->EnableWindow(FALSE); // Without a member variable

  GetDlgItem(IDC_STATIC_FORMAT)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_VERBOSE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_NORMAL)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_RADIO_FORMAT_BASIC)->EnableWindow(TRUE); // Without a member variable

  GetDlgItem(IDC_EDIT_RANDOMCHALLENGE)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT_USERID)->EnableWindow(TRUE); // Without a member variable
  GetDlgItem(IDC_EDIT_KEY)->EnableWindow(TRUE); // Without a member variable
}

void CLittleDataConversionUtilityDlg::OnButtonClose() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}





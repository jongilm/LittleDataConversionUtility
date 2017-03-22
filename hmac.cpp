/////////////////////////////////////////////////////////////////////////////
// RFC 2104                          HMAC                     February 1997
//
// Appendix -- Sample Code
//
//   For the sake of illustration we provide the following sample code for
//   the implementation of HMAC-MD5 as well as some corresponding test
//   vectors (the code is based on MD5 code as described in [MD5]).
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "md5.h"
#include "hmac.h"

// Function: hmac_md5

void bzero ( void *pData, unsigned int cbData)
{
  if (pData && cbData)
    memset(pData,0,cbData);
}

void bcopy ( void *pSrc, void *pDst, unsigned int cbData)
{
  if (pSrc && pDst && cbData)
    memcpy(pDst,pSrc,cbData);
}

void hmac_md5 ( unsigned char*  text,     // pointer to data stream
                int             text_len, // length of data stream
                unsigned char*  key,      // pointer to authentication key
                int             key_len,  // length of authentication key
                caddr_t         digest)   // caller digest to be filled in
{
  MD5_CTX context;
  unsigned char k_ipad[65];    // inner padding - key XORd with ipad
  unsigned char k_opad[65];    // outer padding - key XORd with opad

  unsigned char tk[16];
  int i;

  // if key is longer than 64 bytes reset it to key=MD5(key)
  if (key_len > 64) 
  {
    MD5_CTX      tctx;

    MD5Init(&tctx);
    MD5Update(&tctx, key, key_len);
    MD5Final(tk, &tctx);

    key = tk;
    key_len = 16;
  }

  // The HMAC_MD5 transform looks like:
  //
  //   MD5(K XOR opad, MD5(K XOR ipad, text))
  //
  // where K is an n byte key
  //       ipad is the byte 0x36 repeated 64 times
  //       opad is the byte 0x5c repeated 64 times
  //       and text is the data being protected


  // start out by storing key in pads
  bzero( k_ipad, sizeof k_ipad);
  bzero( k_opad, sizeof k_opad);
  bcopy( key, k_ipad, key_len);
  bcopy( key, k_opad, key_len);

  // XOR key with ipad and opad values
  for (i=0; i<64; i++)
  {
    k_ipad[i] ^= 0x36;
    k_opad[i] ^= 0x5c;
  }

  // Perform inner MD5
  MD5Init(&context);                   // init context for 1st pass
  MD5Update(&context, k_ipad, 64);     // start with inner pad
  MD5Update(&context, text, text_len); // then text of datagram
  MD5Final(digest, &context);          // finish up 1st pass
                                         
  // Perform outer MD5                   
  MD5Init(&context);                   // init context for 2nd pass
  MD5Update(&context, k_opad, 64);     // start with outer pad
  MD5Update(&context, digest, 16);     // then results of 1st hash
  MD5Final(digest, &context);          // finish up 2nd pass
}

// Test Vectors (Trailing '\0' of a character string not included in test):
//
//  key =         0x0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b0b
//  key_len =     16 bytes
//  data =        "Hi There"
//  data_len =    8  bytes
//  digest =      0x9294727a3638bb1c13f48ef8158bfc9d
//
//  key =         "Jefe"
//  data =        "what do ya want for nothing?"
//  data_len =    28 bytes
//  digest =      0x750c783e6ab0b503eaa86e310a5db738
//
//  key =         0xAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
//
//  key_len       16 bytes
//  data =        0xDDDDDDDDDDDDDDDDDDDD...
//                ..DDDDDDDDDDDDDDDDDDDD...
//                ..DDDDDDDDDDDDDDDDDDDD...
//                ..DDDDDDDDDDDDDDDDDDDD...
//                ..DDDDDDDDDDDDDDDDDDDD
//  data_len =    50 bytes
//  digest =      0x56be34521d144c88dbb8c733f0e8b3f6


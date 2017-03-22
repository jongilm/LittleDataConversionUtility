/* Modifications Copyright (c) J.S.A.Kapp 1994 - 1996.

	 MD5 Component Of RSAEURO using RSA Data Security, Inc.
	 MD5 Message Digest Algorithm.

	 All Trademarks Acknowledged.

	 Future versions may not use this modified RSADSI implementation
	 of MD5.
*/


/* Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
   rights reserved.

   License to copy and use this software is granted provided that it
   is identified as the "RSA Data Security, Inc. MD5 Message-Digest
   Algorithm" in all material mentioning or referencing this software
   or this function.

   License is also granted to make and use derivative works provided
   that such works are identified as "derived from the RSA Data
	 Security, Inc. MD5 Message-Digest Algorithm" in all material
   mentioning or referencing the derived work.

   RSA Data Security, Inc. makes no representations concerning either
   the merchantability of this software or the suitability of this
   software for any particular purpose. It is provided "as is"
   without express or implied warranty of any kind.

   These notices must be retained in any copies of any part of this
   documentation and/or software.
*/

#ifndef _MD5_H_
#define _MD5_H_

#ifdef __cplusplus
extern "C" {
#endif

#define PROTO_LIST(list) list
typedef unsigned char *POINTER;
typedef unsigned long int UINT4;

/* MD5 context. */
typedef struct {
  UINT4 state[4];                                   /* state (ABCD) */
  UINT4 count[2];        /* number of bits, modulo 2^64 (lsb first) */
  unsigned char buffer[64];                         /* input buffer */
} MD5_CTX;

//void MD5Init PROTO_LIST ((MD5_CTX *));
//void MD5Update PROTO_LIST ((MD5_CTX *, unsigned char *, unsigned int));
//void MD5Final PROTO_LIST ((unsigned char [16], MD5_CTX *));

extern void MD5Init ( MD5_CTX *context );             /* context */
extern void MD5Update(MD5_CTX *context,               /* context */
                      unsigned char *input,           /* input block */
                      unsigned int inputLen);         /* length of input block */
extern void MD5Final ( unsigned char digest[16],      /* message digest */
                       MD5_CTX *context );            /* context */

#ifdef __cplusplus
}
#endif

#endif /* _MD5_H_ */

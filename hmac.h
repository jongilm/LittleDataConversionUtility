
#ifndef _HMAC_H_
#define _HMAC_H_

#ifdef __cplusplus
extern "C" {
#endif

//typedef struct
//{
//  unsigned char a[16];
//} *caddr_t;

typedef unsigned char caddr_t[16];

extern void hmac_md5 ( unsigned char*  text,     // pointer to data stream
                       int             text_len, // length of data stream
                       unsigned char*  key,      // pointer to authentication key
                       int             key_len,  // length of authentication key
                       caddr_t         digest);  // caller digest to be filled in


#ifdef __cplusplus
}
#endif

#endif // _HMAC_H_
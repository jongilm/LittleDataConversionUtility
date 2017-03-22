
#ifndef _HEXDUMP_H_
#define _HEXDUMP_H_

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  HEX_UNSPECIFIED = 0,
  HEX_USE_CRLF,                   
  HEX_ASCII_ONLY,                
  HEX_DISPLAY_HEADERS,            
  HEX_DISPLAY_ADDRESSES,          
  HEX_DISPLAY_ASCII_CHARS,         
  HEX_DISPLAY_INTERMEDIATE_SPACES
} HEXDUMP_CONFIG;

extern void hexdump_config(HEXDUMP_CONFIG config_param, int Value);
extern int hexdump(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);
extern int hexundump(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);

#ifdef __cplusplus
}
#endif

#endif // _HEXDUMP_H_
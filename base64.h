
#ifndef _BASE64_H_
#define _BASE64_H_

#ifdef __cplusplus
extern "C" {
#endif

#define B64_OK             0
#define B64_ERR_CMDLINE    1
#define B64_ERR_INPUT      2
#define B64_ERR_OUTPUT     3
#define B64_ERR_MEMORY     4
#define B64_ERR_READING    5
#define B64_ERR_WRITING    6
#define B64_ERR_SYNTAX     7

#define ENCODE_BUFFER_SIZE_IN (8192*3)
#define ENCODE_BUFFER_SIZE_OUT (8192*6)

#define DECODE_BUFFER_SIZE_IN (8192*4)
#define DECODE_BUFFER_SIZE_OUT (8192*3)

typedef enum
{
  B64_UNSPECIFIED = 0,
  B64_USE_CRLF
} BASE64_CONFIG;

extern void base64_config(BASE64_CONFIG config_param, int Value);

extern int encodeB64(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);
extern int decodeB64(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut);

#ifdef __cplusplus
}
#endif

#endif // _BASE64_H_
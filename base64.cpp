
#include "stdafx.h"

#include "base64.h"

static const char* to_b64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static unsigned int g_nCharsPerLine = 64;

static int g_fUseCRLF = TRUE;

void base64_config(BASE64_CONFIG config_param, int Value)
{
  switch(config_param)
  {
    case B64_USE_CRLF:                    g_fUseCRLF = Value; break;
  }
}

int encodeB64(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
{
    int Erc = B64_OK;

    if( pBufferIn == NULL || pBufferOut == NULL )
    {
      Erc = B64_ERR_MEMORY;
      return Erc;
    }

    if (cbBufferIn >= ENCODE_BUFFER_SIZE_IN)
    {
      Erc = B64_ERR_READING;
      return Erc;
    }

    //for(;;)
    {
      unsigned long nDiv = 0;
      unsigned long nRem = 0;
      unsigned long nChars = 0;
      unsigned char *pIn = pBufferIn;
      unsigned int nOut = 0;

      nDiv = ((unsigned long)cbBufferIn) / 3;
      nRem = ((unsigned long)cbBufferIn) % 3;
      nChars = 0;

      while( nDiv > 0 )
      {
        pBufferOut[nOut+0] = to_b64[ (pIn[0] >> 2) & 0x3f];
        pBufferOut[nOut+1] = to_b64[((pIn[0] << 4) & 0x30) + ((pIn[1] >> 4) & 0xf)];
        pBufferOut[nOut+2] = to_b64[((pIn[1] << 2) & 0x3c) + ((pIn[2] >> 6) & 0x3)];
        pBufferOut[nOut+3] = to_b64[  pIn[2] & 0x3f];
        pIn += 3;
        nOut += 4;
        nDiv--;
        nChars += 4;
        if( nChars >= g_nCharsPerLine && g_nCharsPerLine != 0 )
        {
          nChars = 0;
          if( g_fUseCRLF )
          {
            pBufferOut[nOut++] = '\r';
          }
          pBufferOut[nOut++] = '\n';
        }
      }

      switch( nRem )
      {
        case 2:
          pBufferOut[nOut+0] = to_b64[ (pIn[0] >> 2) & 0x3f];
          pBufferOut[nOut+1] = to_b64[((pIn[0] << 4) & 0x30) + ((pIn[1] >> 4) & 0xf)];
          pBufferOut[nOut+2] = to_b64[ (pIn[1] << 2) & 0x3c];
          pBufferOut[nOut+3] = '=';
          nOut += 4;
          nChars += 4;
          if( nChars >= g_nCharsPerLine && g_nCharsPerLine != 0 )
          {
            nChars = 0;
            if( g_fUseCRLF )
            {
              pBufferOut[nOut++] = '\r';
            }
            pBufferOut[nOut++] = '\n';
          }
          break;
        case 1:
          pBufferOut[nOut+0] = to_b64[ (pIn[0] >> 2) & 0x3f];
          pBufferOut[nOut+1] = to_b64[ (pIn[0] << 4) & 0x30];
          pBufferOut[nOut+2] = '=';
          pBufferOut[nOut+3] = '=';
          nOut += 4;
          nChars += 4;
          if( nChars >= g_nCharsPerLine && g_nCharsPerLine != 0 )
          {
            nChars = 0;
            if( g_fUseCRLF )
            {
              pBufferOut[nOut++] = '\r';
            }
            pBufferOut[nOut++] = '\n';
          }
          break;
      }

      if( nRem > 0 /*|| feof( g_fIn )*/ )
      {
        if( nChars > 0 )
        {
          nChars = 0;
          if( g_fUseCRLF )
          {
            pBufferOut[nOut++] = '\r';
          }
          pBufferOut[nOut++] = '\n';
        }
      }

      *pcbBufferOut = (size_t)nOut;

      //if( nRem > 0 /*|| feof( g_fIn )*/ )
      //{
      //  break;
      //}
    }

    return Erc;
}

int decodeB64(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
{
  int Erc = B64_OK;
  int SkipMimeSeparator = 0;  // 0 Normal, 1 skip MIME separator (---) to end of line
  char c = '\0';
  unsigned char data[3];
  unsigned int nData = 0;
  unsigned int i;
  unsigned char *pOut = pBufferOut;

  *pcbBufferOut = 0;

  for(i=0;i<cbBufferIn;i++)
  {
    unsigned char bits = 'z';

    c = pBufferIn[i];

    if( SkipMimeSeparator > 0 )
    {
      if ( c == '\n' )
        SkipMimeSeparator = 0;
    }
    else if ( c >= 'A' && c <= 'Z' ) bits = (unsigned char) (c - 'A');
    else if ( c >= 'a' && c <= 'z' ) bits = (unsigned char) (c - 'a' + (char)26);
    else if ( c >= '0' && c <= '9' ) bits = (unsigned char) (c - '0' + (char)52);
    else if ( c == '+' )             bits = (unsigned char) 62;
    else if ( c == '/' )             bits = (unsigned char) 63;
    else if ( c == '-' )             SkipMimeSeparator = 1;
    else if ( c == '=' )             break;
    else                             bits = (unsigned char) 'y';

    if( bits < (unsigned char) 64 )
    {
      switch(nData++)
      {
        case 0: data[0] =  (bits << 2) & 0xFC; break;
        case 1: data[0] |= (bits >> 4) & 0x03;
                data[1] =  (bits << 4) & 0xF0; break;
        case 2: data[1] |= (bits >> 2) & 0x0F;
                data[2] =  (bits << 6) & 0xC0; break;
        case 3: data[2] |= (bits     ) & 0x3F; break;
      }

      if( nData == 4 )
      {
        memcpy(pOut,data,3);
        *pcbBufferOut += 3;
        pOut += 3;
        nData = 0;
      }
    }
  }

  if( Erc == B64_OK && nData > 0 )
  {
    if( nData == 1 )
    {
      Erc = B64_ERR_SYNTAX;
    }
    else
    {
      memcpy(pOut,data,nData-1);
      *pcbBufferOut += (nData-1);
      pOut += (nData-1);
    }
  }
  return Erc;
}




#include "stdafx.h"

#include "hexdump.h"


//--------------------------------------------------------------
// hexdump.cpp : Dump a file in hex format
// Jonnie Gilmore, 28/07/2005 16:12:51
//--------------------------------------------------------------

static int g_fUseCRLF = TRUE;
static int g_ASCII_only = FALSE;
static int g_DisplayHeaders = TRUE;
static int g_DisplayAddresses = TRUE;
static int g_DisplayAsciiChars = TRUE;
static int g_DisplayIntermediateSpaces = TRUE;


void hexdump_config(HEXDUMP_CONFIG config_param, int Value)
{
  switch(config_param)
  {
    case HEX_USE_CRLF:                    g_fUseCRLF = Value; break;
    case HEX_ASCII_ONLY:                  g_ASCII_only = Value; break;
    case HEX_DISPLAY_HEADERS:             g_DisplayHeaders = Value; break;
    case HEX_DISPLAY_ADDRESSES:           g_DisplayAddresses = Value; break;
    case HEX_DISPLAY_ASCII_CHARS:         g_DisplayAsciiChars = Value; break;
    case HEX_DISPLAY_INTERMEDIATE_SPACES: g_DisplayIntermediateSpaces = Value; break;
  }
}

int hexdump(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
//--------------------------------------------------------------
// 
//--------------------------------------------------------------
{
   unsigned int SizeOfFile;
   unsigned char *pIn;
   char *pOut;
   int n;

   pIn = pBufferIn;
   pOut = (char *)pBufferOut;
   *pcbBufferOut = 0;

   SizeOfFile = cbBufferIn;

   if (g_DisplayHeaders)
   {
     if( g_fUseCRLF )
       sprintf(pOut,"HEXDUMP: Size = %lu (0x%X) bytes\r\n",SizeOfFile,SizeOfFile); // Print file info
     else
       sprintf(pOut,"HEXDUMP: Size = %lu (0x%X) bytes\n",SizeOfFile,SizeOfFile); // Print file info
     n = strlen(pOut);
     *pcbBufferOut += n;
     pOut += n;

     //if( g_fUseCRLF )
     //  sprintf(pOut,"         Switches = ASCII_Only=%s\r\n",g_ASCII_only?"Yes":"No"); // Print file info
     //else
     //  sprintf(pOut,"         Switches = ASCII_Only=%s\n",g_ASCII_only?"Yes":"No"); // Print file info
     //n = strlen(pOut);
     //*pcbBufferOut += n;
     //pOut += n;
   }

   if (g_ASCII_only)
   {
      int CharsProcessed;
      char ch;
      unsigned int i;
      int LineOpen;

      CharsProcessed = 0;
      LineOpen = 0;
      //while (!feof(infile))
      for (i=0; i<SizeOfFile; i++)
      {
         ch = (char)(*pIn);
         pIn++;
         if (CharsProcessed%64 == 0)
         {
           sprintf(pOut,"%08X: \"",CharsProcessed); // Print offset
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
           LineOpen = 1;
         }
         CharsProcessed++;

         sprintf(pOut,"%c",isprint(ch)?ch:'.'); // Print byte in ascii 
         n = strlen(pOut);
         *pcbBufferOut += n;
         pOut += n;

         if (CharsProcessed%64 == 0)
         {
           if( g_fUseCRLF )
             sprintf(pOut,"\"\r\n");
           else
             sprintf(pOut,"\"\n");
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
           LineOpen = 0;
         }
      }
      if (LineOpen)
      {
        if( g_fUseCRLF )
          sprintf(pOut,"\"\r\n");
        else
          sprintf(pOut,"\"\n");
        n = strlen(pOut);
        *pcbBufferOut += n;
        pOut += n;
        LineOpen = 0;
      }
   }
   else
   {
      int StartAddress;
      int CharsProcessed;
      char ch;
      unsigned int i;
      char szAscii[17];

      CharsProcessed = 0;
      StartAddress = 0;
      memset(szAscii,0,17);

      //while (!feof(infile))
      for (i=0; i<SizeOfFile; i++)
      {
         ch = (char)(*pIn);
         pIn++;
         szAscii[CharsProcessed  ] = ch;
         szAscii[CharsProcessed+1] = 0;

         CharsProcessed++;

         if (CharsProcessed==16) // i.e. we alread have chars 0..15
         {
            int j;

            if (g_DisplayAddresses)
            {
              sprintf(pOut,"%08X: ",StartAddress); // Print offset
              n = strlen(pOut);
              *pcbBufferOut += n;
              pOut += n;
            }
            StartAddress += CharsProcessed;
            for (j=0;j<16;j++)
            {
               sprintf(pOut,"%02X%s",szAscii[j]&0xFF,g_DisplayIntermediateSpaces?" ":""); // Print byte in hex  (&0xff for annoying Tc hex bug)
               n = strlen(pOut);
               *pcbBufferOut += n;
               pOut += n;
               if (!isprint(szAscii[j]))
                  szAscii[j] = '.';
            }
            if (g_DisplayAsciiChars)
            {
              if( g_fUseCRLF )
                sprintf(pOut,"\"%s\"\r\n",szAscii); // Print string in ascii 
              else
                sprintf(pOut,"\"%s\"\n",szAscii); // Print string in ascii 
              n = strlen(pOut);
              *pcbBufferOut += n;
              pOut += n;
            }
            szAscii[0] = 0;
            CharsProcessed = 0;
         }
      }

      if (CharsProcessed>0) // Some still unreported
      {
         int i;

         if (g_DisplayAddresses)
         { 
           sprintf(pOut,"%08X: ",StartAddress); // Print offset
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
         }

         for (i=0;i<16;i++)
         {
            if (i < CharsProcessed)
            {
               sprintf(pOut,"%02X%s",szAscii[i]&0xFF,g_DisplayIntermediateSpaces?" ":""); // Print byte in hex 
               n = strlen(pOut);
               *pcbBufferOut += n;
               pOut += n;
               if (!isprint(szAscii[i]))
                  szAscii[i] = '.';
            } 
            else 
            {
               if (g_DisplayAsciiChars)
               {
                 sprintf(pOut,"   "); // Print filler
                 n = strlen(pOut);
                 *pcbBufferOut += n;
                 pOut += n;
               }
               szAscii[i]=' ';
            }
         }
         if (g_DisplayAsciiChars)
         { 
           if( g_fUseCRLF )
             sprintf(pOut,"\"%s\"\r\n",szAscii); // Print string in ascii 
           else
             sprintf(pOut,"\"%s\"\n",szAscii); // Print string in ascii 
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
         }
      }
   }

   return 0;
}

int HexCharToBin (char ch)
{
  ch = toupper(ch);
  if (ch>='0' && ch<='9')
    return ch-'0';
  else if (ch>='A' && ch<='F')
    return ch-'A' + 10;
  else
    return 0;
}

int hexundump(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
//--------------------------------------------------------------
// 
//--------------------------------------------------------------
{
   unsigned int SizeOfFile;
   unsigned char *pIn;
   char *pOut;
   //int n;

   pIn = pBufferIn;
   pOut = (char *)pBufferOut;
   *pcbBufferOut = 0;

   SizeOfFile = cbBufferIn;


   //if (g_ASCII_only)
   {
      int CharsProcessed;
      char ch;
      char ch1;
      char ch2;
      unsigned int i;
      int LineOpen;

      CharsProcessed = 0;
      LineOpen = 0;
      //while (!feof(infile))
      for (i=0; i<SizeOfFile/2; i++)
      {
         ch1 = (char)(*pIn);
         pIn++;
         ch2 = (char)(*pIn);
         pIn++;

         ch = HexCharToBin(ch1)*0x10 + HexCharToBin(ch2);
/*
         if (CharsProcessed%64 == 0)
         {
           sprintf(pOut,"%08X: \"",CharsProcessed); // Print offset
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
           LineOpen = 1;
         }
         CharsProcessed++;

         sprintf(pOut,"%c",isprint(ch)?ch:'.'); // Print byte in ascii 
         n = strlen(pOut);
         *pcbBufferOut += n;
         pOut += n;

         if (CharsProcessed%64 == 0)
         {
           if( g_fUseCRLF )
             sprintf(pOut,"\"\r\n");
           else
             sprintf(pOut,"\"\n");
           n = strlen(pOut);
           *pcbBufferOut += n;
           pOut += n;
           LineOpen = 0;
         }
*/
         *pOut = ch;
         pOut += 1;
         *pcbBufferOut += 1;

      }
/*
      if (LineOpen)
      {
        if( g_fUseCRLF )
          sprintf(pOut,"\"\r\n");
        else
          sprintf(pOut,"\"\n");
        n = strlen(pOut);
        *pcbBufferOut += n;
        pOut += n;
        LineOpen = 0;
      }
*/
   }
   
   
   return 0;
}

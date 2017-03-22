
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
static int g_DisplayC = 0; /* 0,1,2,4,8 */


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
      case HEX_DISPLAY_C:                   g_DisplayC = Value; break;
   }
}

static void IncrementPointersByStrlen(char **ppOut, unsigned int *pcbBufferOut)
{
   int n;
   n = strlen(*ppOut);
   *pcbBufferOut += n;
   *ppOut += n;
}

static void OutputLine(char **ppOut, int *pStartAddress, unsigned int *pcbBufferOut, int *pCharsProcessed, char *szAscii)
{
   int j;

   switch (g_DisplayC)
   {
      case 0:
         // Output Offset Address
         if (g_DisplayAddresses)
         {
            sprintf(*ppOut,"%08X: ", *pStartAddress); // Print offset
            IncrementPointersByStrlen(ppOut, pcbBufferOut);
         }

         *pStartAddress += *pCharsProcessed;

         for (j=0;j<16;j++)
         {
            char szHexChars[3];
            int DoOutput = FALSE;
            char szSeparatorChar[2];
            char HexPrefix[3];
            
            // Do we have a real character, or must we output a filler?
            if (j < *pCharsProcessed)
            {
               strcpy(HexPrefix,"0x");
               sprintf(szHexChars,"%02X",szAscii[j]&0xFF); // Print byte in hex
               strcpy(szSeparatorChar,",");
               DoOutput = TRUE;
               if (!isprint(szAscii[j]))
                  szAscii[j] = '.';
            }
            else
            {
               // Output filler so as to align the appended ASCII characters
               if (g_DisplayAsciiChars)
               {
                  strcpy(HexPrefix,"  ");
                  strcpy(szHexChars,"  "); // Filler of the byte in hex
                  strcpy(szSeparatorChar," ");
                  DoOutput = TRUE;
               }
               szAscii[j]=' ';
            }
            if (DoOutput)
            {
               sprintf(*ppOut,"%s%s%s%s",(g_DisplayC>0)?HexPrefix:"",
                                         szHexChars,
                                         (g_DisplayC>0)?szSeparatorChar:"",
                                         g_DisplayIntermediateSpaces?" ":"");
               IncrementPointersByStrlen(ppOut, pcbBufferOut);
            }
         }

         // Append the ASCII characters
         if (g_DisplayAsciiChars)
         {
            if( g_fUseCRLF )
               sprintf(*ppOut,"\"%s\"\r\n",szAscii); // Print string in ascii
            else
               sprintf(*ppOut,"\"%s\"\n",szAscii); // Print string in ascii
            IncrementPointersByStrlen(ppOut, pcbBufferOut);
         }

         break;
      case 1:
         for (j=0;j<16;j++)
         {
            char szHexChars[2+1];
            
            if (j < *pCharsProcessed)
            {
               sprintf(szHexChars,"%02X",szAscii[j]&0xFF); // Print byte in hex
               sprintf(*ppOut,"0x%s,", szHexChars);
               IncrementPointersByStrlen(ppOut, pcbBufferOut);
            }
         }
         break;
      case 2:
         for (j=0;j<16;j+=2)
         {
            char szHexChars[4+1];
            
            if (j < *pCharsProcessed)
            {
               sprintf(szHexChars,"%02X%02X", // Print word in hex
                                  szAscii[j  ]&0xFF,
                                  szAscii[j+1]&0xFF);
               sprintf(*ppOut,"0x%s,", szHexChars);
               IncrementPointersByStrlen(ppOut, pcbBufferOut);
            }
         }
         break;
      case 4:
         for (j=0;j<16;j+=4)
         {
            char szHexChars[8+1];
            
            if (j < *pCharsProcessed)
            {
               sprintf(szHexChars,"%02X%02X%02X%02X", // Print dword in hex
                                  szAscii[j  ]&0xFF,
                                  szAscii[j+1]&0xFF,
                                  szAscii[j+2]&0xFF,
                                  szAscii[j+3]&0xFF);
               sprintf(*ppOut,"0x%s,", szHexChars);
               IncrementPointersByStrlen(ppOut, pcbBufferOut);
            }
         }
         break;
      case 8:
         for (j=0;j<16;j+=8)
         {
            char szHexChars[16+1];
            
            if (j < *pCharsProcessed)
            {
               sprintf(szHexChars,"%02X%02X%02X%02X%02X%02X%02X%02X", // Print ddword in hex
                                  szAscii[j  ]&0xFF,
                                  szAscii[j+1]&0xFF,
                                  szAscii[j+2]&0xFF,
                                  szAscii[j+3]&0xFF,
                                  szAscii[j+4]&0xFF,
                                  szAscii[j+5]&0xFF,
                                  szAscii[j+6]&0xFF,
                                  szAscii[j+7]&0xFF);
               sprintf(*ppOut,"0x%s,", szHexChars);
               IncrementPointersByStrlen(ppOut, pcbBufferOut);
            }
         }
         break;
   }
   
   szAscii[0] = 0;
   *pCharsProcessed = 0;

}         

int hexdump(unsigned char *pBufferIn, unsigned int cbBufferIn, unsigned char *pBufferOut, unsigned int *pcbBufferOut)
//--------------------------------------------------------------
//
//--------------------------------------------------------------
{

   unsigned int SizeOfFile;
   unsigned char *pIn;
   char *pOut;

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
     IncrementPointersByStrlen(&pOut, pcbBufferOut);
  
     //if( g_fUseCRLF )
     //  sprintf(pOut,"         Switches = ASCII_Only=%s\r\n",g_ASCII_only?"Yes":"No"); // Print file info
     //else
     //  sprintf(pOut,"         Switches = ASCII_Only=%s\n",g_ASCII_only?"Yes":"No"); // Print file info
     //IncrementPointersByStrlen(&pOut, pcbBufferOut);
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
            IncrementPointersByStrlen(&pOut, pcbBufferOut);
            LineOpen = 1;
         }
         CharsProcessed++;

         sprintf(pOut,"%c",isprint(ch)?ch:'.'); // Print byte in ascii
         IncrementPointersByStrlen(&pOut, pcbBufferOut);

         if (CharsProcessed%64 == 0)
         {
            if( g_fUseCRLF )
               sprintf(pOut,"\"\r\n");
            else
               sprintf(pOut,"\"\n");
            IncrementPointersByStrlen(&pOut, pcbBufferOut);
            LineOpen = 0;
         }
      }
      if (LineOpen)
      {
         if( g_fUseCRLF )
            sprintf(pOut,"\"\r\n");
         else
            sprintf(pOut,"\"\n");
         IncrementPointersByStrlen(&pOut, pcbBufferOut);
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

         if (CharsProcessed==16) // i.e. we already have chars 0..15
         {
            OutputLine(&pOut, &StartAddress, pcbBufferOut, &CharsProcessed, szAscii);
         }
      }

      if (CharsProcessed>0) // Some still unreported
      {
         OutputLine(&pOut, &StartAddress, pcbBufferOut, &CharsProcessed, szAscii);
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
   unsigned char *pIn;
   char *pOut;
   char ch;
   char ch1;
   char ch2;
   unsigned int i;
   int HexBytesProcessed = 0;

   pIn = pBufferIn;
   pOut = (char *)pBufferOut;
   *pcbBufferOut = 0;

   for (i=0; i<cbBufferIn; i++)
   {
      switch(HexBytesProcessed%2)
      {
         case 0:
            ch1 = (char)(*pIn);
            pIn++;
            HexBytesProcessed++;

            // Skip over non-Hex characters
            if (ch1 == ' ' || ch1 == ',' || ch1 == ';' || ch1 == '{' || ch1 == '}' || ch1 == '\r' || ch1 == '\n' || ch1 == '\t')
            {
               HexBytesProcessed--;
               continue;
            }
            break;
            
         case 1:
            ch2 = (char)(*pIn);
            pIn++;
            HexBytesProcessed++;

            // Skip over "0x" or "0X"
            if (ch1 == '0' && (ch2 == 'x' || ch2 == 'X'))
            {
               HexBytesProcessed -= 2;
               continue;
            }
            // Skip over non-Hex characters
            if (ch2 == ' ' || ch2 == ',' || ch2 == ';' || ch2 == '{' || ch2 == '}' || ch2 == '\r' || ch2 == '\n' || ch2 == '\t')
            {
               HexBytesProcessed--;
               continue;
            }

   
            ch = HexCharToBin(ch1)*0x10 + HexCharToBin(ch2);
            
            *pOut = ch;
            pOut += 1;
            *pcbBufferOut += 1;
            break;
      }
   }

   return 0;
}

/*
DMGR get userSecurityBanner
yields...
0x3132330d 0x0a3c623e 0x626f6c64 0x3c2f623e 0x0d0a3c75 0x3e756e64 0x65726c69 0x6e653c2f 0x753e0d0a 0x3c693e69 0x74616c69 0x633c2f75
0x3e0d0a61 0x6d706572 0x73616e64 0x20262068 0x6572650d 0x0a616d70 0x65727361 0x6e642073 0x65717565 0x6e636520 0x26616d70 0x3b206865
0x72650d0a 0x71756f74 0x65202220 0x68657265 0x0d0a7175 0x6f746520 0x2671756f 0x743b2068 0x6572650d 0x0a6c6573 0x73746861 0x6e203c20
0x68657265 0x0d0a6c65 0x73737468 0x616e2026 0x6c743b20 0x68657265 0x0d0a6772 0x65617465 0x72746861 0x6e203e20 0x68657265 0x0d0a6772
0x65617465 0x72746861 0x6e202667 0x743b2068 0x6572650d 0x0a2d2d2d 0x2d2b2d2d 0x2d2d312d 0x2d2d2d2b 0x2d2d2d2d 0x322d2d2d 0x2d2b2d2d
0x2d2d332d 0x2d2d2d2b 0x2d2d2d2d 0x342d2d2d 0x2d2b2d2d 0x2d2d352d 0x2d2d2d2b 0x2d2d2d2d 0x362d2d2d 0x2d2b2d2d

*/
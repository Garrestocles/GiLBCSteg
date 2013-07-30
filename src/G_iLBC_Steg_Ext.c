/*********************************************************************
                    G_iLBC_Steg_Ext.c
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/

/**********************************************************************
This is the main steganography extraction file.  It is used to house all of
the functions that are used to extract the hidden file from the encoded
audio file as it is being decoded.
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "G_iLBC_Steg_Ext.h"

FILE *secret;   //The file handle of the file being extracted
int tracker; //Used to keep track of which bit is currently being extracted
int secretbyte[8];  //The current byte being worked on
int eof;    //Used to record when the EOF marker has been reached
char issecret;

/*  This function is used to initialize all fo the variables used
throughout these functions into their starting positions.  */

void initstegext(char *filename)
{
    if(filename != NULL) secret = fopen(filename,"w");
    else issecret = 'n';
    tracker = 0;
    eof = 0;
}


void getsecretbit(int lpc)
{
    short int secretbit;

/*  This section extracts the secret streganography bit from the LPC
coefficient and insert it into the current byte.*/

    secretbit = lpc % 2;
    secretbyte[tracker] = secretbit;
    tracker++;

//  Check to see if this byte is the EOF marker.

    if (secretbyte[0] == 1 && secretbyte[1] == 1 && secretbyte[2] == 1 && secretbyte[3] == 1 && secretbyte[4] == 1 && secretbyte[5] == 1 && secretbyte[6] == 1 && secretbyte[7] == 0)
    {
       eof = 7;
    }

/*  If the EOF marker has been reached, reset the tracker to something
benign. */

    if (eof == 7) tracker = 7;

/*  If the EOF marker hasn't been reached, write the current byte to the
output file.    */

    if (tracker == 8 && eof == 0)
    {
        if (issecret != 'n') writebyte(secretbyte);
        tracker = 0;
    }
}

/*  This function takes in the array containing the extracted
byte and writes it to a literal byte, and then writes that
byte to the file.   */

void writebyte(int *byte)
{
    char bytetowrite = 0x0000;
    int r;
    int g;

    for (r=0;r<8;r++)
    {
        if (byte[r] == 1)
        {
            g = 7 - r;
            bytetowrite |= 1 << g;
        }
    }

    fprintf(secret,"%c",bytetowrite);
}

//Used to close the file handler.

void closefile()
{
    fclose(secret);
}

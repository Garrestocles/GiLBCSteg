/*********************************************************************
                    G_iLBC_Steg.c
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/

/**********************************************************************
This is the main steganography insertion file.  It is used to house all
of the functions that are used to hide a file within the encoded audio
file.
***********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "G_iLBC_Steg.h"
#include "iLBC_define.h"

GStegByte *currentbyte; //The current byte being worked on.
GStegByte *firstbyte;   //The first byte that is being stored
GStegByte *newbyte; //Used to dynamically add a new byte
int count;  //Used to keep track of what bit is being worked on

//  This function initializes the count variable.

void initcount()
{
    count = 0;
}

/*  This function extracts each bit from a byte and puts
it into an array that records the bits. */

void binstring(char ch, int *bin)
{
    int x;

    for (x=0;x<8;x++)
    {
       bin[x]= ch & 0x8000 ? 1 : 0;
       ch<<= 1;
    }
}


void addbyte(int *bin)
{
    int r;

/*  Allocate memory for a new byte to be recorded from the
file to be hidden   */

    newbyte = (GStegByte *)malloc(sizeof(GStegByte));

/*  Check to see if the first byte is empty. If so,
work on this byte first.  If not work on the next
empty byte    */

    if (firstbyte == NULL)
        firstbyte = currentbyte = newbyte;

    else
    {
        currentbyte = firstbyte;

        while(currentbyte->nextbyte != NULL)
            currentbyte = currentbyte->nextbyte;

        currentbyte->nextbyte = newbyte;
        currentbyte = newbyte;
    }

//  Store the byte.

    for (r=0;r<8;r++)
    {
        currentbyte->byte[r] = bin[r];
    }

    currentbyte->nextbyte = NULL;

}

/*  This function retrieves the next bit that needs
to be hidden in the encoded audio.  */

int getbit()
{
    int rbit;

    if (firstbyte != NULL)
    {
        rbit = firstbyte->byte[count];
        count++;

/*  Since the byte, once hidden in the encoded audio is no
longer needed, this section deletes it. */

        if(count == 8)
        {
            count = 0;
            delfirstbyte();
        }
    }
    else rbit = 2;  //Indicates that there are no more bits to hide.

    return rbit;
}

/* This function deletes the first struct of the linked list
that contains the bytes of the hidden file.  Once a byte has been
encoded into the audio, it is no longer needed, so it is deleted
and the memory is freed.    */

void delfirstbyte(void)
{
    if(firstbyte == NULL)
        return;

    currentbyte = firstbyte;
    firstbyte = currentbyte->nextbyte;

    free(currentbyte);

    return;
}

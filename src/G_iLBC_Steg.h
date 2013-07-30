/*********************************************************************
                    G_iLBC_Steg.h
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/

/**********************************************************************
This file is used by the GiLBCSteg.c, iLBC_encode.c, and G_iLBC_Steg.c
to coordinate functions.
***********************************************************************/

#include "iLBC_define.h"

void binstring(char ch, int *bin);

void addbyte(int *bin);

int getbit();

void initcount();

void delfirstbyte(void);

typedef struct OneByte  {

        int byte[8];
        struct OneByte *nextbyte;

    } GStegByte;

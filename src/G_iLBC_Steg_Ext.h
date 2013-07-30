/*********************************************************************
                    G_iLBC_Steg_ext.h
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/

/**********************************************************************
This file is used by the GiLBCSteg.c, iLBC_decode.c, and
G_iLBC_Steg_Ext.c to coordinate functions.
***********************************************************************/

void initstegext(char *filename);

void writebyte(int *byte);

void getsecretbit(int lpc);

void closefile();

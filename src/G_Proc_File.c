/*********************************************************************
                        G_Proc_File.c
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/

void procbyte(int *secbyte)
{
    int indicator = 0;
    int r;

    for (r=0;r<8;r++)
    {
        if (secbyte[r] == 1) indicator ++;
    }

    if (indicator == 8) secbyte[0] = 0;
}

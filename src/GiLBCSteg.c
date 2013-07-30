/*********************************************************************
                        GiLBCSteg.c
        Audio Codec Steganography Application Source Code
                    By Garrestotle

            Utilizing the source code of WebRTC's
                    iLBC Audio Coder
Copyright (c) 2011, The WebRTC project authors. All rights reserved.
************************************************************************/


    #include <math.h>
    #include <stdlib.h>
    #include <stdio.h>
    #include <string.h>
    #include "iLBC_define.h"
    #include "iLBC_encode.h"
    #include "iLBC_decode.h"
    #include "G_iLBC_Steg.h"
    #include "G_iLBC_Steg_Ext.h"

    /* Runtime statistics */
    #include <time.h>

    #define ILBCNOOFWORDS_MAX   (NO_OF_BYTES_30MS/2)
   /*----------------------------------------------------------------*
    *  Encoder interface function
    *---------------------------------------------------------------*/

   short encode(   /* (o) Number of bytes encoded */
       iLBC_Enc_Inst_t *iLBCenc_inst,
                                   /* (i/o) Encoder instance */
       short *encoded_data,    /* (o) The encoded bytes */
       short *data                 /* (i) The signal block to encode*/
   ){
       float block[BLOCKL_MAX];
       int k;

       /* convert signal to float */

       for (k=0; k<iLBCenc_inst->blockl; k++)
           block[k] = (float)data[k];

       /* do the actual encoding */

       iLBC_encode((unsigned char *)encoded_data, block, iLBCenc_inst);


       return (iLBCenc_inst->no_of_bytes);
   }

      /*----------------------------------------------------------------*
    *  Decoder interface function
    *---------------------------------------------------------------*/

   short decode(       /* (o) Number of decoded samples */
       iLBC_Dec_Inst_t *iLBCdec_inst,  /* (i/o) Decoder instance */
       short *decoded_data,        /* (o) Decoded signal block*/
       short *encoded_data,        /* (i) Encoded bytes */
       short mode                       /* (i) 0=PL, 1=Normal */
   ){
       int k;
       float decblock[BLOCKL_MAX], dtmp;

       /* check if mode is valid */

       if (mode<0 || mode>1) {
           printf("\nERROR - Wrong mode - 0, 1 allowed\n"); exit(3);}

       /* do actual decoding of block */

       iLBC_decode(decblock, (unsigned char *)encoded_data,
           iLBCdec_inst, mode);

       /* convert to short */
       for (k=0; k<iLBCdec_inst->blockl; k++){
           dtmp=decblock[k];

           if (dtmp<MIN_SAMPLE)
               dtmp=MIN_SAMPLE;
           else if (dtmp>MAX_SAMPLE)
               dtmp=MAX_SAMPLE;
           decoded_data[k] = (short) dtmp;
       }

       return (iLBCdec_inst->blockl);
   }
/********************************************************
    Here is the main function that is used by GiLBCSteg.
    It processes all of the arugments and calls the
    various functions to operate the program.  It also
    provides information on its use.
********************************************************/

   int main(int argc, char* argv[])
   {

       /* Runtime statistics */

       float starttime;
       float runtime;
       float outtime;

       FILE *ifileid,*ofileid, *sfileid;
       short data[BLOCKL_MAX];
       short encoded_data[ILBCNOOFWORDS_MAX], decoded_data[BLOCKL_MAX];
       int len;
       short mode;
       int blockcount = 0;
       int packetlosscount = 0;

       char r;
       int bits[8]; //An array used to store 1 byte as individual bits.
       int endmark[] ={1,1,1,1,1,1,1,0};    //EOF marker for steg file.
       int encdec;  //Used to store whether in encode or decode mode.

       /* Create structs */
       iLBC_Enc_Inst_t Enc_Inst;
       iLBC_Dec_Inst_t Dec_Inst;

       mode = 30;

       /* get arguments and open files */

//  Make sure that the user is operating the application properly.
//  If not, provides instructions.

       if ((argc!=4) && (argc!=5)) {
           fprintf(stderr,
         "\n*------------------------------------------------------------------------------*\n");
           fprintf(stderr,
           "                                GiLBCSteg.exe\n");
           fprintf(stderr,
           "                     A Sneaky iLBC Steganography Application     \n");
           fprintf(stderr,
           "                                                By Garrestotle         \n\n");
           fprintf(stderr,
           "*------------------------------------------------------------------------------*\n");
           fprintf(stderr,
           "This program runs in two modes. Encode mode(1) and    \n");
           fprintf(stderr,
           "Decode mode(2). To run this program, type the following:\n");
           fprintf(stderr,
           "To encode: GiLBCSteg.exe 1 \"in.wav\" \"encoded file\" (text file to hide)\n");
           fprintf(stderr,
           "To decode: GiLBCSteg.exe 2 \"encoded file\" \"output audio file\" (Steg File)\n\n");
           fprintf(stderr,
           "*--------------------------------------=====-----------------------------------*\n");
           exit(1);
       }

        encdec = atoi(argv[1]);
       if (encdec != 1 && encdec != 2) {
           fprintf(stderr,"Wrong mode %s, must be 1 for encoding or 2 for decoding.\n",
               argv[1]);
           exit(2);
       }
       if ( (ifileid=fopen(argv[2],"rb")) == NULL) {
           fprintf(stderr,"Cannot open input file %s\n", argv[2]);
           exit(2);}
       if ( (ofileid=fopen(argv[3],"wb")) == NULL) {
           fprintf(stderr, "Cannot open output file %s\n",
               argv[3]); exit(1);}
       if (argc==5 && encdec == 1)
       {
           if( (sfileid=fopen(argv[4],"rb")) == NULL)
            {
               fprintf(stderr, "Cannot open steg file %s\n",argv[5]);exit(1);
            }
       } else   {
           sfileid=NULL;
                }

        /* print info */

       fprintf(stderr, "\n");
       fprintf(stderr,
           "*---------------------------------------------------*\n");
       fprintf(stderr,
           "*      Garrestotle's                          *\n");
       fprintf(stderr,
           "*           GiLBCSteg Steganography Program         *\n");
       fprintf(stderr,
           "*        Utilizing WebRTC's iLBC audio Codec        *\n");
       fprintf(stderr,
           "*                                                   *\n");
       fprintf(stderr,
           "*---------------------------------------------------*\n");

       fprintf(stderr,"\nMode           : %s\n", argv[1]);
       fprintf(stderr,"Input file     : %s\n", argv[2]);
       fprintf(stderr,"Output file    : %s\n", argv[3]);
       if (argc==5)
       {
            fprintf(stderr,"Steg file      : %s\n", argv[4]);
       }
       fprintf(stderr,"\n");

               /* Runtime statistics */

       starttime=clock()/(float)CLOCKS_PER_SEC;

    // Encoding mode (1)

       if (encdec == 1)
       {
            //Initialize data for encoding
            initEncode(&Enc_Inst, mode);
            initcount();

/*  If the encoder is going to be used for steganography, this section
of code reads the file given and writes it to a linked list used
to store the file in binary format, so that each bit can be individually
accessed.           */

            if (argc==5)
            {
                while ((r=fgetc(sfileid))!= EOF)
                {
                    binstring(r,bits);
                    addbyte(bits);
                }
                addbyte(endmark);
            }

            fclose(sfileid);

       /* loop over input blocks */

            while (fread(data,sizeof(short),Enc_Inst.blockl,ifileid)== Enc_Inst.blockl)
            {

            blockcount++;

            /* encoding */
            fprintf(stderr, "--- Encoding block %i --- ",blockcount);
            len=encode(&Enc_Inst, encoded_data, data);
            fprintf(stderr, "\r");

            /* write byte file */

            fwrite(encoded_data, sizeof(unsigned char), len, ofileid);
            }
        }

//  Decoding Mode (2)

        if (encdec == 2)
        {
            //Initialize data for decoding
            initDecode(&Dec_Inst, mode, 1);
            initstegext(argv[4]);

       /* loop over input blocks */

            while (fread(encoded_data,sizeof(unsigned char),50,ifileid)== 50)
            {

            blockcount++;

            fprintf(stderr, "--- Decoding block %i --- ",blockcount);

            len=decode(&Dec_Inst, decoded_data,encoded_data, 1);
            fprintf(stderr, "\r");

            /* write output file */

            fwrite(decoded_data,sizeof(short),len,ofileid);
            }
        closefile();
        }
        /* Runtime statistics */

       runtime = (float)(clock()/(float)CLOCKS_PER_SEC-starttime);
       outtime = (float)((float)blockcount*(float)mode/1000.0);
       outtime = outtime/2;
       printf("\n\nLength of speech file: %.1f s\n", outtime);
       printf("Packet loss          : %.1f%%\n", 100.0*(float)packetlosscount/(float)blockcount);
       printf("Time to run GiLBCSteg:");
       printf(" %.1f s (%.1f %% of realtime)\n\n", runtime, (100*runtime/outtime));

       /* close files */

       fclose(ifileid); fclose(ofileid);

       return(0);
   }

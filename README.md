GiLBCSteg
=========

A steganography algorithm utilizing WebRTC's internet low-bitrate audio codec (iLBC).

Installation Instructions
=========
In order to successfully compile GiLBCSteg, one must first attain the source code for the iLBC audio codec.   This can be obtained from http://www.ietf.org/rfc/rfc3951.txt.  To extract the source code from the RFC 3951 documentation, Columbia University's Wenju Jiang's extract-cfile.awk script is extremely helpful.  This can be obtained from http://ilbcfreeware.org/documentation/extract-cfile.txt.  Simply download the .txt file and change the extension to .awk.  Once this is completed, simply put the extract-cfile.awk file and the rfc3951.txt file in the same directory and run this command: "awk -f extract-cfile.awk rfc3951.txt".  All of the relevant C source code files and header files should appear.

To get GiLBCSteg to work, all you need to do is copy the src folder from this repository, and paste its contents into the file with the iLBC source code, replacing any files that conflict.  After that, delete the iLBC_test.c file, compile all the code and you should be good to go.  Run the program and it will tell you how to use it.

A Note About the Audio Files
=========
In order to successfully play the decoded audio, the audio that was originally encoded must have been encoded with a sampling rate of 8000 Hz in 16 bit PCM formatting.  This is generally worked with as a .wav file.  This was found to be easiest using the Audacity application, which can be acquired from http://audacity.sourceforge.net/download/.  In order to successfully play back the audio, the decoded binary file must be imported into Audacity as raw data with a sampling rate of 8000 Hz. Once the file has been successfully imported, one will then notice that the audio is twice as long as it should be.  This is solved by altering the playback speed.  Navigate to "Effect" > "Change Speed...".   Once the "Change Speed" window appears, double the speed by entering "100" into the percent change text box.  Hit "OK" and it should be ready for playback.

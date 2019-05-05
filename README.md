# WavEncode

C cross-platform WAV to MP3 encoder utilising Lame library & POSIX threads.

Requirements:

(1) application is called with pathname as argument, e.g.

<applicationname> F:\MyWavCollection\ all WAV-files contained directly in that folder are to be encoded to MP3

(2) use all available CPU cores for the encoding process in an efficient way by utilizing multi-threading

(3) statically link to lame encoder library

(4) application should be compilable and runnable on Windows and Linux

(5) the resulting MP3 files are to be placed within the same directory as the source WAV files, the filename extension should be changed appropriately to .MP3

(6) non-WAV files in the given folder shall be ignored

(7) multithreading shall be implemented by means of using Posix Threads (there exist implementations for Windows)

(8) the Boost library shall not be used

(9) the LAME encoder should be used with reasonable standard settings (e.g. quality based encoding with quality level "good")


Features:

Parallel Processing for multiple input files depending on system capabilty.
Input files support is limited to stereo WAV files
Portable - lame is statically linked with binary

Builds & runs on:

Linux (tested on Ubuntu)
Windows (tested on Windows 8 + mingw-w64)

Linux build instructions
Install lame along with it's development package, change to project dir:

$ gcc ListFilesInArray.c mythread.c main.c WavReadEncode.c -lpthread -l:libmp3lame.a -lm -o wavencode

The binary will be presented in same folder but this path can be configured by above command after -o flag.

Windows build instructions
Install msys2, launch mingw64 shell, add compiler and lame using

pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-lame

change to project dir and run

# /mingw64/bin/x86_64-w64-mingw32-gcc.exe main.c mythread.c ListFilesInArray.c WavReadEncode.c -l:libmp3lame.a

The binary will be presented in same folder.

Usage
Start program with supplying input directory with wav files as parameter which are wav stereo PCM:

./wavencoder /folder_path_of_wav_files/

*Do not miss to add last slash for Path

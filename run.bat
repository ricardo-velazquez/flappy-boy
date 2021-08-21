REM Automatically generated from Makefile
del dist/dist.gb
del dist/dist.map
del dist/dist.noi
C:\gbdk\bin\lcc -Wa-l -Wl-m -Wl-j -o dist/dist.gb src/main.c
C:\gbdk\emulators\vba.exe dist/dist.gb
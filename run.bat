REM Automatically generated from Makefile
del dist /Q
mkdir dist
C:\gbdk\bin\lcc -Wa-l -Wf-ba0 -c -o dist/savevars.o src/savevars.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/main.o src/main.c
C:\gbdk\bin\lcc -Wl-yt3 -Wl-yo4 -Wl-ya4 -o dist/dist.gb dist/main.o dist/savevars.o
C:\bgb\bgb.exe dist/dist.gb
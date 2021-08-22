REM Automatically generated from Makefile
del dist/dist.gb
del dist/dist.map
del dist/dist.noi
C:\gbdk\bin\lcc -c -o dist/main.o src/main.c
C:\gbdk\bin\lcc -c -o dist/tilemap.o src/tilemap.c
C:\gbdk\bin\lcc -o dist/dist.gb dist/main.o dist/tilemap.o
C:\gbdk\emulators\vba.exe dist/dist.gb



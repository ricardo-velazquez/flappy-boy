REM Automatically generated from Makefile
del dist /Q
mkdir dist
C:\gbdk\bin\lcc -c -o dist/main.o src/main.c
C:\gbdk\bin\lcc -c -o dist/tilemap.o src/res/tilemap.c
C:\gbdk\bin\lcc -c -o dist/sprites.o src/res/sprites.c
C:\gbdk\bin\lcc -c -o dist/bkgs.o src/res/bkgs.c
C:\gbdk\bin\lcc -c -o dist/player.o src/entities/player.c
C:\gbdk\bin\lcc -c -o dist/pipes.o src/entities/pipes.c
C:\gbdk\bin\lcc -o dist/dist.gb dist/main.o dist/tilemap.o dist/bkgs.o dist/sprites.o dist/player.o dist/pipes.o
C:\bgb\bgb.exe dist/dist.gb
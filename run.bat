REM Automatically generated from Makefile
REM del dist /Q
REM mkdir dist
C:\gbdk\bin\lcc -Wa-l -Wf-ba0 -c -o dist/savevars.o src/savevars.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/main.o src/main.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/tilemap.o src/res/tilemap.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/sprites.o src/res/sprites.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/bkgs.o src/res/bkgs.c
C:\gbdk\bin\lcc -Wa-l -c -o dist/player.o src/entities/player.c
REM C:\gbdk\bin\lcc -c -o dist/pipes.o src/entities/pipes.c
C:\gbdk\bin\lcc -Wl-yt3 -Wl-yo4 -Wl-ya4 -o dist/dist.gb dist/main.o dist/tilemap.o dist/bkgs.o dist/sprites.o dist/player.o dist/savevars.o
C:\bgb\bgb.exe dist/dist.gb
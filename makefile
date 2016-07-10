OUT=libthreedee.a
CFLAGS=-std=c99 -I../

all:
	cc $(CFLAGS) -c threedee.c -lm -o threedee.o
	ar rcs $(OUT) threedee.o

install:
	install $(OUT) /usr/local/lib

demo:
	cd t && cc $(CFLAGS) -o demo demo.c -lGLU -lGL -lglut -lm -lglfw -lGLEW

docs:
	pycco -i -d docs threedee.h

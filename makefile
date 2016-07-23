OUT=libthreedee.a
CFLAGS=-std=c99 -I../ -g

all:
	cc $(CFLAGS) -c threedee.c -lm -o threedee.o
	ar rcs $(OUT) threedee.o

install:
	install $(OUT) /usr/local/lib

demo:
	cd t; rm demo; cc $(CFLAGS) -o demo demo.c ../threedee.c ../mesh_loader.c -lGLU -lGL -lglut -lm -lglfw -lGLEW

docs:
	pycco -d docs *.h
	ls *.h | xargs -I% echo '*' "[%](%tml)" | smu > docs/doc_index.html

.PHONY: docs

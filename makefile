OUT=libsgl.a
CFLAGS=-std=c99 -g -I/usr/local/include -L/usr/local/lib

all:
	cc $(CFLAGS) -c sgl_draw.c -lm -o sgl_draw.o
	ar rcs $(OUT) sgl_draw.o

install:
	install $(OUT) /usr/local/lib

LDFLAGS=`pkg-config --libs glew` `pkg-config --libs glfw3` -lX11 -lglut -lm -lXrandr -lXxf86vm -lXi -lXinerama -lXcursor -lpthread
demo:
	-cd t && rm demo
	cd t && cc $(CFLAGS) -o demo demo.c ../sgl_draw.c ../sgl_mesh_loader.c -I.. $(LDFLAGS)

docs:
	pycco -d docs *.h
	ls *.h | xargs -I% echo '*' "[%](%tml)" | smu > docs/doc_index.html

.PHONY: docs

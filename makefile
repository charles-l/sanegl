all:
	cc -c lib3ds.c -o lib3ds.o
	ar rcs lib3ds.a lib3ds.o

install:
	install lib3ds.a /usr/local/lib

demo:
	cc -o demo display.c lib3ds.c -lGLU -lGL -lglut

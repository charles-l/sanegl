# sanegl
*bring some basic sanity into your GL code*

* limit boilerplate
* hide global state (at least until I switch to OpenGL 4.5 and start using DSA)
* BETTER FREAKING DOCUMENTATION

## docs
I'm experimenting with literate programming for this project, so run `make docs` to generate the docs directory. I'm using pycco for now, but docco and its varients should work as well.

## demo code
`make demo && ./t/demo`

See `t/demo.c` for example codes and stuff.

## what's here
* simple opengl helper library
* `.3ds` file loader

## TODO
* drop GLFW (only create a GL conetxt)
* other file formats
    * [MD2](https://en.wikipedia.org/wiki/MD2_(file_format))
    * [MD3](https://en.wikipedia.org/wiki/MD3_(file_format))
    * [MD5](http://www.3dgep.com/loading-and-animating-md5-models-with-opengl/)
    * [OBJ](https://en.wikipedia.org/wiki/Wavefront_.obj_file)
    * [STL](https://en.wikipedia.org/wiki/STL_(file_format))
    * MS3D
* Port OpenGL to 4.5 (yay for DSA!)
* Add Windows support (add `#ifdefs` and make the `makefile` more Windows friendly)

## FAQ
__What's the purpose of this project__
It's not a full graphics engine, it just limits the boilerplate for the majority of 3D rendering code you may have to write. Stuff like creating VAOs, VBOs, binding textures, initializing things and cleanup are all taken care of for you.

__Why is this project written in C__
I don't like C++

__Why do you have your own `3ds` parser built in (with more custom parsers to come)__
Because Assimp is bloated and written in C++

__Ewww you use snake_case__
Eww to CamelCase. It's C. C code should always use snake_case.

__You have a lot of opinions__
Yes.

#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "linmath.h"

// **Polygon type**
typedef struct {
    // `a`, `b`, and `c` map to the indexes of vertices in another array
    int a, b, c;
} poly_t;

// **Texture Map coordinates**
typedef struct {
    float u, v;
} mapco_t;

// **3D Object**
typedef struct {
    poly_t *polygon;
    vec3 *vertex;
    mapco_t *mapcoord;
    // Number of vertices
    int vlen;
    // Number of polygons
    int plen;
    // name of the object
    char name[20];
} load_obj_t;

// Some of these `#define`s might need to be tweaked, depending on the complexity of your scenes.
#define MAX_VERTS 20000
#define MAX_POLYS 20000
// Max number of objects in a single file
#define MAX_OBJS 16

// Loads an array of 3D objects from a `.3ds` file, and returns the array.
load_obj_t **load_3ds_objs(char *fname);

// Free a single load object
void free_load_obj(load_obj_t *o);

// Free the whole returned loaded obj array
void free_load_objs(load_obj_t **o);

#endif // MESH_LOADER_H

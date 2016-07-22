#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "sgl_math.h"

// Some of these `#define`s might need to be tweaked, depending on the complexity of your scenes.
#define MAX_VERTS 20000
#define MAX_POLYS 20000
// Max number of objects in a single file
#define MAX_OBJS 16

// **Polygon type**
typedef struct {
    // `a`, `b`, and `c` map to the indexes of vertices in another array
    int a, b, c;
} polygon_t;

// **Texture Map coordinates**
typedef struct {
    float u, v;
} uv_t;

// **3D Object**
typedef struct {
    polygon_t polygons[MAX_POLYS];
    v3 vertices[MAX_VERTS];
    v3 normals[MAX_VERTS];
    uv_t uvs[MAX_VERTS];

    // Number of vertices
    int vlen;

    // Number of polygons
    int plen;

    // Name of the object
    char name[20];
} raw_mesh_t;

// Loads an array of `raw_mesh_t` objs from a `.3ds` file, and returns the array.
// **NOTE**: you must remember to free the array when you're finished!
raw_mesh_t *load_3ds_objs(char *fname);

#endif // MESH_LOADER_H

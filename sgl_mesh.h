#ifndef SGL_MESH
#define SGL_MESH
#include "sgl_math.h"

// TODO: this is crap. make it not.
#define MAX_VERTS 20000
#define MAX_POLYS 20000
// Max number of objects in a single file
#define MAX_OBJS 16

typedef struct {
    // `a`, `b`, and `c` map to the indexes of vertices in another array
    int a, b, c;
} polygon_t;

typedef struct {
    float u, v;
} uv_t;

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
} mesh_t;

mesh_t create_cube(float r);

#endif // SGL_MESH

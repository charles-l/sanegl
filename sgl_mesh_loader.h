#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include <stdio.h>
#include <stdlib.h>
#include "sgl_math.h"
#include "sgl_mesh.h"

// Loads an array of objs from a `.3ds` file, and returns the array.
// **NOTE**: you must remember to free the array when you're finished!
mesh_t *load_3ds_objs(char *fname);

// Generates calculated smooth normals to a raw_mesh
void gen_normals(mesh_t *m);

#endif // MESH_LOADER_H

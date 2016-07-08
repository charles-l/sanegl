#ifndef LIB3DS_H
#define LIB3DS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linmath.h"

// initial starting point
#define MAX_VERTS 20000
#define MAX_POLYS 20000
#define MAX_OBJS 16 // max objects in a single file

typedef struct {
    int a, b, c;
} poly_t;

typedef struct {
    float u, v;
} mapco_t;

typedef struct {
    poly_t *polygon;
    vec3 *vertex;
    mapco_t *mapcoord;
    int vlen; // number of verts
    int plen; // number of polys
    int tex_id;
    char name[20];
} obj_t;

typedef struct {
    char *data;
    int w;
    int h;
} img_t;

obj_t **load_3ds_objs(char *fname);
void free_obj(obj_t *o);
void free_objs(obj_t **o); // free the obj array

#endif

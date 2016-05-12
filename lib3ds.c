#include "lib3ds.h"

static size_t get_fsize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return fsize;
}

static obj_t *new_obj() {
    obj_t *o = malloc(sizeof(obj_t));
    o->polygon = malloc(sizeof(poly_t) * MAX_POLYS);
    o->vertex = malloc(sizeof(vertex_t) * MAX_VERTS);
    o->mapcoord = malloc(sizeof(mapco_t) * MAX_VERTS);
    o->vlen = 0;
    o->plen = 0;
    o->tex_id = 0;
    return o;
}

obj_t **load_3ds_objs(char *fname) {
    FILE *f;
    unsigned short chunk_id;
    unsigned int chunk_len;
    unsigned short qty;
    unsigned short face_flags;
    if((f = fopen(fname, "rb")) == NULL) return NULL;

    int curo = -1; // hehehe the cleverness is overwhelming
    obj_t **objs = calloc(MAX_OBJS, sizeof(obj_t *));

    size_t fsize = get_fsize(f);

    while(ftell(f) < fsize) {
        int i;
        char c;

        fread(&chunk_id, 2, 1, f);
        fread(&chunk_len, 4, 1, f);

        switch(chunk_id)
        {
            case 0x4d4d: // main chunk
                break;
            case 0x3d3d: // 3d editor chunk
                break;
            case 0x4000: // object block
                i = 0;
                objs[++curo] = new_obj();
                do {
                    fread(&c, 1, 1, f);
                    objs[curo]->name[i] = c;
                } while (c != '\0' && i++ < 20);
                printf("%s\n", objs[curo]->name);
                break;
            case 0x4100: // triangle mesh (empty node)
                break;
            case 0x4110: // vertices list
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo]->vlen = qty;
                fprintf(stderr, "%d vertices\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->vertex[i].x, sizeof(float), 1, f);
                    fread(&objs[curo]->vertex[i].y, sizeof(float), 1, f);
                    fread(&objs[curo]->vertex[i].z, sizeof(float), 1, f);
                }
                break;
            case 0x4120: // chunk faces description
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo]->plen = qty;
                fprintf(stderr, "%d polys\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->polygon[i].a, sizeof(unsigned short), 1, f);
                    fread(&objs[curo]->polygon[i].b, sizeof(unsigned short), 1, f);
                    fread(&objs[curo]->polygon[i].c, sizeof(unsigned short), 1, f);
                    fread(&face_flags, sizeof(unsigned short), 1, f);
                }
                break;
            case 0x4140: // mapping coordinate list
                fread(&qty, sizeof(unsigned short), 1, f);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo]->mapcoord[i].u, sizeof(float), 1, f);
                    fread(&objs[curo]->mapcoord[i].v, sizeof(float), 1, f);
                }
                break;
            default:
                fseek(f, chunk_len - 6, SEEK_CUR);
        }
    }
    fclose(f);
    return objs;
}

void free_obj(obj_t *o) {
    free(o);
}

void free_objs(obj_t **o) {
    for(int i = 0; o[i] != NULL; i++) {
        free(o[i]);
    }
    free(o);
}

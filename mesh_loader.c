#include "mesh_loader.h"
#include "sgl_math.h"

inline static size_t get_fsize(FILE *f) {
    fseek(f, 0L, SEEK_END);
    size_t fsize = ftell(f);
    fseek(f, 0L, SEEK_SET);
    return fsize;
}

raw_mesh_t *load_3ds_objs(char *fname) {
    FILE *f;
    unsigned short chunk_id;
    unsigned int chunk_len;
    unsigned short qty;
    unsigned short face_flags;
    if((f = fopen(fname, "rb")) == NULL) return NULL;
    raw_mesh_t *objs = calloc(MAX_OBJS, sizeof(raw_mesh_t));

    int curo = -1;

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
                curo++;
                do {
                    fread(&c, 1, 1, f);
                    objs[curo].name[i] = c;
                } while (c != '\0' && i++ < 20);
                printf("%s\n", objs[curo].name);
                break;
            case 0x4100: // triangle mesh (empty node)
                break;
            case 0x4110: // vertices list
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo].vlen = qty;
                fprintf(stderr, "%d vertices\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo].vertices[i][0], sizeof(float), 1, f);
                    fread(&objs[curo].vertices[i][1], sizeof(float), 1, f);
                    fread(&objs[curo].vertices[i][2], sizeof(float), 1, f);
                }
                break;
            case 0x4120: // chunk faces description
                fread(&qty, sizeof(unsigned short), 1, f);
                objs[curo].plen = qty;
                fprintf(stderr, "%d polys\n", qty);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo].polygons[i].a, sizeof(unsigned short), 1, f);
                    fread(&objs[curo].polygons[i].b, sizeof(unsigned short), 1, f);
                    fread(&objs[curo].polygons[i].c, sizeof(unsigned short), 1, f);
                    fread(&face_flags, sizeof(unsigned short), 1, f);
                }
                break;
            case 0x4140: // mapping coordinate list
                fread(&qty, sizeof(unsigned short), 1, f);
                for(i = 0; i < qty; i++) {
                    fread(&objs[curo].uvs[i].u, sizeof(float), 1, f);
                    fread(&objs[curo].uvs[i].v, sizeof(float), 1, f);
                }
                break;
            default:
                fseek(f, chunk_len - 6, SEEK_CUR);
        }
    }
    fclose(f);
    return objs;
}

void gen_normals(raw_mesh_t *m) {
    for(int i = 0; i < m->plen; i++) {
        polygon_t *p = &(m->polygons[i]);
        v3 u = (v3) m->vertices[p->a] - (v3) m->vertices[p->b];
        v4 v = m->vertices[p->c] - m->vertices[p->b];
        //m->normals[p->a]
    }
}

#include "sgl_mesh.h"

mesh_t create_cube(float r) {
    mesh_t cube;
    cube.vertices = {
        (v3){r, -r, -r},
        (v3){r, -r, r},
        (v3){-r, -r, r},
        (v3){-r, -r, -r},
        (v3){r, r, -r},
        (v3){r, r, r},
        (v3){-r, r, r},
        (v3){-r, r, -r},
    };
    cube.polygons = {
        (polygon_t) {2, 4, 1},
        (polygon_t) {8, 6, 5},
        (polygon_t) {5, 2, 1},
        (polygon_t) {6, 3, 2},
        (polygon_t) {3, 8, 4},
        (polygon_t) {1, 8, 5},
        (polygon_t) {2, 3, 4},
        (polygon_t) {8, 7, 6},
        (polygon_t) {5, 6, 2},
        (polygon_t) {6, 7, 3},
        (polygon_t) {3, 7, 8},
        (polygon_t) {1, 4, 8}
    };
    return cube;
}

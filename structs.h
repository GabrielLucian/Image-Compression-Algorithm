#ifndef TEMA2_STRUCTS_H
#define TEMA2_STRUCTS_H
#include <inttypes.h>
    typedef struct QuadtreeNode {
    unsigned char blue, green, red;
    uint32_t area;
    int32_t top_left, top_right;
    int32_t bottom_left, bottom_right;
} __attribute__((packed)) QuadtreeNode;
    typedef struct Image{
        unsigned char **blue, **green, **red;
    }Image;
    typedef enum{
        TOPLEFT,TOPRIGHT,BOTTOMLEFT,BOTTOMRIGHT
    }Position;
#endif //TEMA2_STRUCTS_H

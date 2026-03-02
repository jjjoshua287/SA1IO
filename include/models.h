#ifndef MODELS_H
#define MODELS_H

#include <stdint.h>

struct Material {
    uint32_t DiffuseColor; // in 0xAARRGGBB format
    uint32_t SpecularColor; // in 0xAARRGGBB format
    float Exponent;
    uint32_t Texture; // 28-bit TextureID, 4 bits unused
    uint32_t MaterialFlags;
};

struct Triangles {
    short Vertice1;
    short Vertice2;
    short Vertice3;
};

struct Quads {
    short Vertice1;
    short Vertice2;
    short Vertice3;
    short Vertice4;
};

struct Strips {
    uint8_t NumStrips;
    uint8_t Direction;
    short vertices[];
};

struct Poly {
    Triangles triangles;
    Quads quads;
    Strips strips;
};

struct VColor {
    int color[]; // Array of int in 0xAARRGGBB format for each vertex in the strip
};

struct UV {
    int16_t uv[2];
};

typedef enum {
    POLY_TRIANGLES = 0,
    POLY_QUADS     = 1,
    POLY_STRIPS1   = 2,
    POLY_STRIPS2   = 3
} PolyType;

struct Mesh
{
    uint16_t MaterialID; // Low 14 bits = MaterialID, High 2 bits = PolyType
    uint16_t PolyTotal; // How many polys to load
    Poly *Polys;
    void *Pattr; // unused, but part of official model format
    float *PolyNormal; // points to array containing 2 floats for each poly if non-zero
    VColor *VertexColors;
    UV *UVCoordinates;
    uint32_t null; // Exclusive to DX version
};

typedef enum {
    OBJ_NO_POSITION  = 0x01, // Don't apply position values.
    OBJ_NO_ROTATION  = 0x02, // Don't apply rotation values.
    OBJ_NO_SCALE     = 0x04, // Don't apply scale values.
    OBJ_NO_RENDER    = 0x08, // Don't render this model.
    OBJ_NO_CHILDREN  = 0x10, // Don't process children.
    OBJ_ROT_ZYX      = 0x20, // Rotation values are in ZYX Order.
    OBJ_NO_ANIMATION = 0x40, // Don't include this model for animations.
    OBJ_NO_MORPHS    = 0x80  // Don't include this model for morphs.
} ObjectFlags;

struct Attach {
    // members here
};

struct Object {
    uint32_t ObjectFlags;
    // ...
};

#endif
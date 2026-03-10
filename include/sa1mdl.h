#include <stdlib.h>
#include <stdint.h>

typedef struct {
    uint8_t *data;
    size_t size;
} SA1ModelFile;

// All pointer fields are uint32 file offsets
typedef struct __attribute__((packed)) {
    uint32_t evalflags;
    uint32_t model;     // -> NJS_MODEL_SADX_FILE
    float    pos[3];
    int32_t  ang[3];
    float    scl[3];
    uint32_t child;     // -> NJS_OBJECT_FILE
    uint32_t sibling;   // -> NJS_OBJECT_FILE
} NJS_OBJECT_FILE;     // 0x34 bytes

typedef struct __attribute__((packed)) {
    uint32_t points;    // -> NJS_POINT3[]
    uint32_t normals;   // -> NJS_VECTOR[]
    int32_t  nbPoint;
    uint32_t meshsets;  // -> NJS_MESHSET_SADX_FILE[]
    uint32_t mats;      // -> NJS_MATERIAL[]
    uint16_t nbMeshset;
    uint16_t nbMat;
    float    center[3];
    float    r;
    uint32_t buffer;    // skip — always 0 in files
} NJS_MODEL_SADX_FILE; // 0x28 bytes

typedef struct __attribute__((packed)) {
    uint16_t type_matId;
    uint16_t nbMesh;
    uint32_t meshes;    // -> int16_t[]
    uint32_t attrs;     // -> uint32_t[]
    uint32_t normals;   // -> NJS_VECTOR[]
    uint32_t vertcolor; // -> NJS_COLOR[]
    uint32_t vertuv;    // -> NJS_TEX[]
    uint32_t buffer;    // skip
} NJS_MESHSET_SADX_FILE;
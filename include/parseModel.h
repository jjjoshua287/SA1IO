#ifndef PARSE_MODEL_H
#define PARSE_MODEL_H

#include <stdint.h>
#include <stddef.h>
#include "sa1mdl.h"


typedef uint32_t sa1_obj_off_t;
typedef uint32_t sa1mdl_off_t;

// File lifecycle
SA1ModelFile *sa1_load(const char *path);
void sa1_free(SA1ModelFile *f);
uint32_t sa1mdl_root_offset(const SA1ModelFile *f);

// Object tree navigation
sa1mdl_off_t sa1_object_model(const SA1ModelFile *f, sa1_obj_off_t offset);
sa1_obj_off_t sa1_object_child(const SA1ModelFile *f, sa1_obj_off_t offset);
sa1_obj_off_t sa1_object_sibling(const SA1ModelFile *f, sa1_obj_off_t offset);
sa1_obj_off_t sa1_object_evalflags(const SA1ModelFile *f, sa1_obj_off_t offset);
void sa1_object_transform(const SA1ModelFile *f, sa1_obj_off_t offset, float *out_pos, int32_t *out_ang, float *out_scl);

int32_t sa1_model_nbpoint(const SA1ModelFile *f, sa1mdl_off_t offset);
int sa1_model_points(const SA1ModelFile *f, sa1mdl_off_t offset, float *out_xyz, int max);

#endif
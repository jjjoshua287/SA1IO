#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../include/parseModel.h"
#include "../include/sa1-ninja.h"
#include "../include/sa1mdl.h"

#define MODEL_DEBUG 1

static void *sa1_ptr(const SA1ModelFile *f, uint32_t offset)
{
	if (offset == 0 || offset >= f->size) return NULL;
	return f->data + offset;
}

SA1ModelFile *sa1_load(const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp) return NULL;	
	
	fseek(fp, 0, SEEK_END);
	long len = ftell(fp);
	rewind(fp);
	if (len <= 0) { 
		fclose(fp); 
		return NULL; 
	}

	SA1ModelFile *f = malloc(sizeof(SA1ModelFile));
	if (!f) { 
		fclose(fp); 
		return NULL; 
	}

	f->size = (size_t)len;
	f->data = malloc(f->size);
	if (!f->data) { 
		free(f); 
		fclose(fp); 
		return NULL; 
	}

	fread(f->data, 1, f->size, fp);
	fclose(fp);
	return f;
}

/* This function is becoming deprecated for the functions below.
*  It should only be used for debugging and/or development
*/
void read_object(const SA1ModelFile *f, uint32_t offset, int depth)
{
	if (!offset) return;
	NJS_OBJECT_FILE *obj = sa1_ptr(f, offset);
	if (!obj) return;

	if (MODEL_DEBUG) {
		printf("%*sOBJECT @ 0x%X  evalflags=0x%X\n", depth*2, "", offset, obj->evalflags);
	}

	if(obj->model) {
		NJS_MODEL_SADX_FILE *mdl = sa1_ptr(f, obj->model);
		struct NJS_POINT3   *pts = sa1_ptr(f, mdl->points);
		if (MODEL_DEBUG) {
			printf("%*s  MODEL: %d verts, %d meshsets\n", depth*2, "", mdl->nbPoint, mdl->nbMeshset);
		}
	}

	read_object(f, obj->child, depth + 1);
	read_object(f, obj->sibling, depth);
}

void sa1_free(SA1ModelFile *f)
{
        free(f->data);
	free(f);
}

uint32_t sa1mdl_root_offset(const SA1ModelFile *f)
{
	uint32_t root_offset;
	memcpy(&root_offset, f->data + 0x08, 4);
	return root_offset;
}

static NJS_OBJECT_FILE *__sa1_object_file(const SA1ModelFile *f, sa1_obj_off_t offset)
{
	if (!offset) return; // No offset provided
	NJS_OBJECT_FILE *obj = sa1_ptr(f, offset);
	
	if (obj == NULL) return; // Invalid offset
	return obj;
}

sa1mdl_off_t sa1_object_model(const SA1ModelFile *f, sa1_obj_off_t offset)
{
	NJS_OBJECT_FILE *obj = __sa1_object_file(f, offset);
	if (!obj) return; // No mesh on this node
	return obj->model;
}

sa1_obj_off_t sa1_object_child(const SA1ModelFile *f, sa1_obj_off_t offset)
{
	NJS_OBJECT_FILE *obj = __sa1_object_file(f, offset);
	if (!obj) return; // No mesh on this node
	return obj->child;
}

sa1_obj_off_t get_sa1_object_sibling(const SA1ModelFile *f, sa1_obj_off_t offset)
{
	NJS_OBJECT_FILE *obj = __sa1_object_file(f, offset);
	if (!obj) return; // No mesh on this node
	return obj->sibling;
}

sa1_obj_off_t sa1_object_evalflags(const SA1ModelFile *f, sa1_obj_off_t offset)
{
	NJS_OBJECT_FILE *obj = __sa1_object_file(f, offset);
	if (!obj) return; // No mesh on this node
	return obj->evalflags;
}
void sa1_object_transform(const SA1ModelFile *f, sa1_obj_off_t offset, float *out_pos, int32_t *out_ang, float *out_scl)
{
	NJS_OBJECT_FILE *obj = __sa1_object_file(f, offset);
	if (!obj) return; // No mesh on this node
	
}

int32_t sa1_model_nbpoint(const SA1ModelFile *f, sa1mdl_off_t offset)
{
	/* casts f->data + offset to NJS_MODEL_SADX_FILE* and returns mdl->nbPoint */
	NJS_MODEL_SADX_FILE *mdl = sa1_ptr(f, offset);
	return mdl->nbPoint;
}

int sa1_model_points(const SA1ModelFile *f, sa1mdl_off_t offset, float *out_xyz, int max)
{
	NJS_MODEL_SADX_FILE *mdl = sa1_ptr(f, offset);
	struct NJS_POINT3 *pts = sa1_ptr(f, mdl->points);
	int n = mdl->nbPoint < max ? mdl->nbPoint : max;
	for (int i = 0; i < n; i++) {
		out_xyz[i*3 + 0] = pts[i].x;
		out_xyz[i*3 + 1] = pts[i].y;
		out_xyz[i*3 + 2] = pts[i].z;
	}
	return n;
}

// Temporary Entry Point
int main(int argc, char *argv[]){
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file.sa1mdl>\n", argv[0]);
	}

	SA1ModelFile *mdl = sa1_load(argv[1]);
	if (!mdl) {
		perror("sa1_load: failed to open file");
		return 1;
	}
	if (mdl->size < 12) {
		fprintf(stderr, "File too small\n");
		return 1;
	}
	uint32_t root_offset = get_root_offset_sa1mdl(mdl);
	read_object(mdl, root_offset, 0);
	return 0;
}
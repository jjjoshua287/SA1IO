#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
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

// Temporary Entry Point
int main(int argc, char *argv[]){
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file.sa1mdl>\n", argv[0]);
	}

	SA1ModelFile *f = sa1_load(argv[1]);
	if (!f) {
		perror("sa1_load: failed to open file");
		return 1;
	}
	if (f->size < 12) {
		fprintf(stderr, "File too small\n");
		return 1;
	}

	uint32_t root_offset;
	memcpy(&root_offset, f->data + 0x08, 4);
	read_object(f, root_offset, 0);
	return 0;
}
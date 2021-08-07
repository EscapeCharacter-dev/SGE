#include "internal.h"
#include "../lib/glad/glad.h"
#include "../include/sge.h"
#include <stdio.h>
#include <stdlib.h>

void createVAO(sge_object_prototype* dest){
	glGenVertexArrays(1, &dest->m_vao); // allocate a vertex array object
}

void createVBOS(
	sge_object_prototype* dest,
	uint_t NUM_BUFFERS /*How many buffers you want?*/
){
	glBindVertexArray(dest->m_vao);
	if(NUM_BUFFERS > SGE_OBJECT_MAX_VBOS){
		printf("<ERROR> Too many VBOs. cannot create %u VBOs for VAO.", NUM_BUFFERS);
		exit(1);
	}
	glGenBuffers(NUM_BUFFERS, dest->m_vbos); // Make NUM_BUFFERS buffers
	for(uint_t i = 0; i < NUM_BUFFERS; i++){
		dest->m_vboflags[i] = 0;
	}
}

void initArrayBuffer(
	sge_object_prototype* dest,
	uint_t index, //index into the vbo array to initialize
	uint_t elems,
	uint_t vboflags
){
	if(index > SGE_OBJECT_MAX_VBOS){
		printf("<ERROR> invalid vbo index %u", index);
		exit(1);
	}
	if(dest->m_vbos[index] == 0){
		printf("<ERROR> cannot initialize null VBO.");
		exit(1);
	}
	/*We have a valid index and VBO in that index, now we set it up.*/
	if(vboflags != 1 &&
		vboflags != 2 &&
		vboflags != 4 &&
		vboflags != 8 &&
		vboflags != 16 
	){
		printf("<ERROR> invalid vboflags %u", vboflags);
		exit(1);
	}
}

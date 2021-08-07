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
	unsigned int NUM_BUFFERS /*How many buffers you want?*/
){
	glBindVertexArray(dest->m_vao);
	if(NUM_BUFFERS > SGE_OBJECT_MAX_VBOS){
		printf("<ERROR> Too many VBOs. cannot create %u VBOs for VAO.", NUM_BUFFERS);
		exit(1);
	}
	glGenBuffers(NUM_BUFFERS, dest->m_vbos); // Make NUM_BUFFERS buffers
}

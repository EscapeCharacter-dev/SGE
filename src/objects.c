#include "internal.h"
#include "../lib/glad/glad.h"
#include "../include/sge.h"



void createVAO(struct sge_object_prototype* dest){
	glGenVertexArrays(1, &dest->m_vao); // allocate a vertex array object
}

void createVBOS(
	struct sge_object_prototype* dest,
	unsigned int 

){
	glBindVertexArray(dest->m_vao);
	glGenBuffers(NUM_BUFFERS, m_vertexArrayBuffers); // Make NUM_BUFFERS buffers
}

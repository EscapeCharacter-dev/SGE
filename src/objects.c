#include "internal.h"
#include "../lib/glad/glad.h"
#include "../include/sge.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
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


void bindObject(sge_object_prototype* me){
	glBindVertexArray(me->m_vao);
	{unsigned int i=0, j=0;
		for(i=0; i < SGE_OBJECT_MAX_VBOS; i++){
			if(me->m_vbos[i] == 0) {continue;}
			glEnableVertexAttribArray(j++);
			if(me->m_vboflags[i] == 8){ /*mat4.*/
				glEnableVertexAttribArray(j++);
				glEnableVertexAttribArray(j++);
				glEnableVertexAttribArray(j++);
				/*setup vertex attrib pointers.*/
				glVertexAttribPointer(j-4, 4, GL_FLOAT, GL_FALSE, 16 * (int)sizeof(GL_FLOAT), 0);
				glVertexAttribPointer(j-3, 4, GL_FLOAT, GL_FALSE, 16 * (int)sizeof(GL_FLOAT), (void*)(4 * sizeof(GL_FLOAT)));
				glVertexAttribPointer(j-2, 4, GL_FLOAT, GL_FALSE, 16 * (int)sizeof(GL_FLOAT), (void*)(8 * sizeof(GL_FLOAT)));
				glVertexAttribPointer(j-1, 4, GL_FLOAT, GL_FALSE, 16 * (int)sizeof(GL_FLOAT), (void*)(12 * sizeof(GL_FLOAT)));
			} else if(
				me->m_vboflags[i] == 16 || 
				me->m_vboflags[i] == 1 ){
				glVertexAttribPointer(j-1, 
					1, /*one float*/
					(me->m_vboflags[i] == 1)?GL_FLOAT:GL_INT, 
					GL_FALSE, 0, 0
				);
			} else if(me->m_vboflags[i] == 2){ /*vec2 attribute.*/
				glVertexAttribPointer(j-1, 
					2,  /*two floats*/
					GL_FLOAT, /*always floating point vec2.*/
					GL_FALSE, 0, 0
				);
			} else if(me->m_vboflags[i] == 4){
				glVertexAttribPointer(j-1, 
					4,  /*four floats*/
					GL_FLOAT, /*always floating point vec2.*/
					GL_FALSE, 0, 0
				);				
			} /*else error*/
		}
	}
	/*TODO: bind shader program if present.*/
	/*TODO: user callback?*/
}


void initArrayBuffer(
	sge_object_prototype* dest,
	uint_t index, //index into the vbo array to initialize
	uint_t elems,
	uint_t is_static, //use GL_DYNAMIC_DRAW or GL_STATIC_DRAW
	uint_t vboflags,
	void* init_data //data to use for initialization.
){
	if(index > SGE_OBJECT_MAX_VBOS){
		printf("<ERROR> invalid vbo index %u", index);
		exit(1);
	}
	if(dest->m_vbos[index] == 0){
		printf("<ERROR> cannot initialize null VBO.");
		exit(1);
	}
	if(elems == 0){
		printf("<ERROR> cannot init VBO with zero elements.");
		exit(1);
	}
	/*We have a valid index and VBO in that index, now we set it up.*/
	glBindBuffer(GL_ARRAY_BUFFER, dest->m_vbos[index]);
	if(vboflags == 1 || vboflags == 16) /*float or int. Assumed to be the same size... because they are in OpenGL.*/
	{
		
		/*assert(sizeof(float) == sizeof(int));*/
		glBufferData(
			GL_ARRAY_BUFFER, 
			elems* sizeof(float), init_data, is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
		);
	} else if(vboflags == 2){
		glBufferData(
			GL_ARRAY_BUFFER, 
			elems* sizeof(float) * 2, init_data, is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
		);
	} else if(vboflags == 4){
		glBufferData(
			GL_ARRAY_BUFFER, 
			elems* sizeof(float) * 4, init_data, is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
		);
	} else if(vboflags == 8){
		glBufferData(
			GL_ARRAY_BUFFER, 
			elems* sizeof(float) * 16, init_data, is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
		);
		/*TODO: setup vertex attrib pointers.*/
	} else {
		printf("<ERROR> invalid vboflags %u", vboflags);
		exit(1);
	}
}



void initElementArrayBuffer( /*Index buffer.*/
	sge_object_prototype* dest,
	uint_t index, //index into the vbo array to initialize
	uint_t elems,
	uint_t is_static, //use GL_DYNAMIC_DRAW or GL_STATIC_DRAW
	void* init_data //data to use for initialization.
){
	if(index > SGE_OBJECT_MAX_VBOS){
		printf("<ERROR> invalid vbo index %u", index);
		exit(1);
	}
	if(dest->m_vbos[index] == 0){
		printf("<ERROR> cannot initialize null VBO.");
		exit(1);
	}
	if(elems == 0){
		printf("<ERROR> cannot init VBO with zero elements.");
		exit(1);
	}
	/*We have a valid index and VBO in that index, now we set it up.*/
	glBindBuffer(GL_ARRAY_BUFFER, dest->m_vbos[index]);
	/*assert(sizeof(float) == sizeof(int));*/
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER, 
		elems* sizeof(float), init_data, is_static ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW
	);
}

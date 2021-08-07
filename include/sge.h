#ifndef GENGINE_H
#define GENGINE_H

#include "types.h"

/*
    required version: 0.0.1
    description: Initialization structure
*/
typedef struct {
    const char *title;
    int         wW; // Window Width
    int         wH; // Window Height
    int         vW; // Viewport Width
    int         vH; // Viewport Height
    bool_t      fs; // Fullscreen
} sge_init_struct;

/*
	SGE object prototype.
*/

#define SGE_OBJECT_MAX_VBOS 32
#define SGE_OBJECT_MAX_SHADERS 16

typedef struct  {
	unsigned int m_vao; //Vertex Array Object.
	unsigned int m_vbos[SGE_OBJECT_MAX_VBOS]; //VBOs
	unsigned int m_vboflags[SGE_OBJECT_MAX_VBOS]; //VBO flags
	//1: float property
	//2: vec2 property
	//4: vec4 property
	//8: mat4 property
	//16: int property
	unsigned int m_program; //compiled shader program.
	unsigned int m_shaders[SGE_OBJECT_MAX_SHADERS]; //shaders.
	void* udata; //user data
} sge_object_prototype;



/*
    required version: 0.0.1
    description: Update callback (used for render/events multithreading)
*/
typedef void    (*sge_update_callback)(uint64_t ticks);

/*
    required version: 0.0.1
    description: Initializes SGE's window
*/
void sge_init(sge_init_struct init_struct);

/*
    required version: 0.0.1
    description: Closes SGE's window
*/
void sge_end(void);

/*
    required version: 0.0.1
    description: Starts the game
*/
void sge_start(sge_update_callback callback);

/*
    required version: 0.0.1
    description: Loads a shader program
*/
uint32_t sge_load_shader(const char *json_filepath, uint32_t *out_shader_buffer);

#endif

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

typedef struct  {
	unsigned int m_vao; //Vertex Array Object.
	unsigned int m_vbos[32]; //VBOs
	unsigned int m_program; //compiled shader program.
	unsigned int m_shaders[16]; //shaders.
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
uint32_t sge_load_shader(const char *json_filepath);

#endif

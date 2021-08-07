#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef __GNUC__
#define PUBLIC  __attribute__((visibility("default")))
#elif _MSC_VER
#define PUBLIC  __declspec(dllexport)
#else
#error unsupported compiler
#endif

/*
    Shader code
*/
typedef struct {
    char    *source; /* shader source */
    int     type;    /* shader type */
} sge_shader_code;

/*
    Shader program
*/
typedef struct {
    sge_shader_code *arr;           /* array of shader code */
    int             shader_count;   /* number of shaders */
} sge_shader_program;

void sge_shader_free(uint_t *shaders, uint_t shader_count);
void sge_shader_program_free(uint_t program);
void sge_shader_source(const char *filepath, sge_shader_code *code);
uint_t sge_compile_program(sge_shader_program *program);
uint_t sge_compile_shader(sge_shader_code *code);

#endif

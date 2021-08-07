#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef __GNUC__
#define PUBLIC  __attribute__((visibility("default")))
#elif _MSC_VER
#define PUBLIC  __declspec(dllexport)
#else
#error unsupported compiler
#endif

#include "types.h"
#include "sge.h"

void sge_shader_free(uint_t *shaders, uint_t shader_count);
void sge_shader_program_free(uint_t program);
void sge_shader_source(const char *filepath, sge_shader_code *code);
uint_t sge_compile_program(sge_shader_program *program);
uint_t sge_compile_shader(sge_shader_code *code);

#endif

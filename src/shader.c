#include "internal.h"
#include "sge.h"
#include "glad.h"
#include "cjson.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint_t sge_compile_shader(sge_shader_code *code) {
    if (!code) {
        printf("<ERROR> shader code is a null pointer\n");
        exit(1);
    }

    uint_t shader = glCreateShader(code->type);
    glShaderSource(shader, strlen(code->source), code->source, NULL);
    glCompileShader(shader);
    uint_t status;
    static char errorLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(shader, 512, NULL, errorLog);
        printf("<ERROR> Shader compilation error:\n%s\n", errorLog);
        exit(1);
    }
    return shader;
}

uint_t sge_compile_program(sge_shader_program *program) {
    if (!program) {
        printf("<ERROR> shader program instance is a null pointer\n");
        exit(1);
    }

    uint_t p = glCreateProgram();
    for (int i = 0; i < program->shader_count; i++) {
        glAttachShader(p, sge_compile_shader(&program->arr[i]));
    }
    uint_t status;
    static char errorLog[512];
    glGetProgramiv(p, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(p, 512, NULL, errorLog);
        printf("<ERROR> Program linkage error:\n%s\n", errorLog);
        exit(1);
    }
    return p;
}

void sge_shader_source(const char *filepath, sge_shader_code *code) {
    if (!code) {
        printf("<ERROR> shader source to modify is a null pointer\n");
        exit(1);
    }

    FILE *fp = fopen(filepath, "r");
    if (!fp) {
        printf("<ERROR> Cannot open file '%s'\n", filepath);
        exit(1);
    }
    size_t len = ftell(fp);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    fclose(fp);
    code->source = buf;
}

void sge_shader_program_free(uint_t program) {
    glDeleteProgram(program);
}

void sge_shader_free(uint_t *shaders, uint_t shader_count) {
    if (!shaders) {
        printf("<ERROR> shader array in sge_shader_free() is a null pointer\n");
        exit(1);
    }
    for (int i = 0; i < shader_count; i++) {
        glDeleteShader(shaders[i]);
    }
}
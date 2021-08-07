#include "internal.h"
#include "sge.h"
#include "glad.h"
#include "cjson.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint32_t *objects = 0;
static uint16_t objects_used = 0;
static uint32_t *programs = 0;
static uint16_t programs_used = 0;

static void appendo(uint32_t val) {
    if (!objects) {
        objects = malloc(sizeof(uint32_t));
    } else {
        objects = realloc(objects, sizeof(uint32_t) * (objects_used + 2));
    }

    objects[objects_used++] = val;
}

static uint32_t appendp(uint32_t val) {
    if (!programs) {
        programs = malloc(sizeof(uint32_t));
    } else {
        programs = realloc(programs, sizeof(uint32_t) * (programs_used + 2));
    }

    objects[programs_used++] = val;
    return programs_used - 1;
}

PUBLIC uint32_t sge_load_shader(const char *json_filepath, uint32_t *out_shader_buffer, uint32_t *out_shader_count) {
    FILE *fp = fopen(json_filepath, "r");
    if (!fp) {
        printf("<ERROR> File %s is not found\n", json_filepath);
        exit(1);
    }
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    fclose(fp);
    const cJSON *json = cJSON_Parse(buf);
    free(buf);

    const cJSON *shaders = cJSON_GetObjectItem(json, "shaders");
    if (!shaders) {
        printf("<ERROR> Expected shaders element in shader JSON file\n");
        exit(1);
    }
    cJSON *shader;
    uint32_t total_shaders = 0;
    cJSON_ArrayForEach(shader, shaders) {
        if (total_shaders >= 32) {
            printf("<WARNING> Max number of shaders reached\n");
            break;
        }
        cJSON *type = cJSON_GetObjectItem(shader, "type");
        uint32_t shader_type;
        if (!type) {
            printf("<ERROR> Shader type is required\n");
            exit(1);
        }
        cJSON *path = cJSON_GetObjectItem(shader, "path");
        if (!path) {
            printf("<ERROR> Shader source filepath is required\n");
            exit(1);
        }
        if (!strcmp(type->valuestring, "v")) shader_type = GL_VERTEX_SHADER;
        else if (!strcmp(type->valuestring, "f")) shader_type = GL_FRAGMENT_SHADER;
        else if (!strcmp(type->valuestring, "g")) shader_type = GL_GEOMETRY_SHADER;
        else if (!strcmp(type->valuestring, "c")) shader_type = GL_COMPUTE_SHADER;
        fp = fopen(path, "r");
        if (!fp) {
            printf("<ERROR> Shader source '%s' not found\n", path);
            exit(1);
        }
        fseek(fp, 0, SEEK_END);
        len = ftell(fp);
        buf = malloc(len + 1);
        fread(buf, 1, len, fp);
        buf[len] = '\0';
        fclose(fp);
        uint32_t shader = glCreateShader(shader_type);
        glShaderSource(shader, len, buf, NULL);
        glCompileShader(shader);
        uint32_t status;
        static char *errorLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (!status) {
            glGetShaderInfoLog(shader, 512, NULL, errorLog);
            printf("<ERROR> Shader compilation error:\n%s\n", errorLog);
            exit(1);
        }
        appendo(shader);
        out_shader_buffer[total_shaders++] = shader;
    }
    uint32_t program = glCreateProgram();
    for (uint32_t i = 0; i < total_shaders; i++) glAttachShader(program, out_shader_buffer[i]);
    glLinkProgram(program);
    uint32_t status;
    static char *errorLog[512];
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(shader, 512, NULL, errorLog);
        printf("<ERROR> Shader linkage error:\n%s\n", errorLog);
        exit(1);
    }
    *out_shader_count = total_shaders;
    appendp(program);
    return program;
}

void sge_free_shaders(void) {
    for (int i = programs_used; i > 0; i--) glDeleteProgram(programs[i]);
    for (int i = objects_used; i > 0; i--) glDeleteShader(objects[objects_used]);
}
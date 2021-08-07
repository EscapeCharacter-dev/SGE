#include "internal.h"
#include "sge.h"
#include "glad.h"
#include "cjson.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

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

PUBLIC uint32_t sge_load_shader(const char *json_filepath) {
    FILE *fp = fopen(json_filepath, "r");
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    fclose(fp);
    const cJSON *json = cJSON_Parse(buf);
    free(buf);
    const cJSON *jvert = cJSON_GetObjectItem(json, "vertex");
    const cJSON *jfrag = cJSON_GetObjectItem(json, "fragment");
    const cJSON *jattr = cJSON_GetObjectItem(json, "attributes");
    const char *v_path = jvert->valuestring;
    const char *f_path = jfrag->valuestring;
    cJSON *attribute;
    char **attributes = malloc(sizeof(char **));
    int i = 0;
    cJSON_ArrayForEach(attribute, jattr) {
        char *attr = malloc(strlen(attribute->valuestring));
        strcpy(attr, attribute->valuestring);
        attributes[i++] = attr;
        attributes = realloc(attributes, i * sizeof(char **));
    }
    fp = fopen(v_path, "r");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    uint32_t v = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(v, 1, buf, NULL);
    glCompileShader(v);
    fclose(fp);
    free(buf);
    int status;
    static char log[2048];
    glGetShaderiv(v, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(v, 2048, NULL, log);
        fprintf(stderr, "Vertex shader compilation error:\n%s\n", log);
        cJSON_Delete(json);
        return;
    }
    appendo(v);
    fp = fopen(f_path, "r");
    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    uint32_t f = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(f, 1, buf, NULL);
    glCompileShader(f);
    fclose(fp);
    free(buf);
    glGetShaderiv(f, GL_COMPILE_STATUS, &status);
    if (!status) {
        glGetShaderInfoLog(f, 2048, NULL, log);
        fprintf(stderr, "Fragment shader compilation error:\n%s\n", log);
        cJSON_Delete(json);
        return;
    }
    appendo(f);
    uint32_t program = glCreateProgram();
    glAttachShader(program, v);
    glAttachShader(program, f);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (!status) {
        glGetProgramInfoLog(f, 2048, NULL, log);
        fprintf(stderr, "Shader linkage error:\n%s\n", log);
        cJSON_Delete(json);
        return;
    }
    uint32_t i = appendp(program);
    for (int j = 0; j < i; j++) free(attributes[j]);
    free(attributes);
    cJSON_Delete(json);
    return i;
}

void sge_free_shaders(void) {
    for (int i = 0; i < 2048; i++) glDeleteProgram(programs[i]);
    for (int i = objects_used; i > 0; i--) glDeleteShader(objects_used);
}
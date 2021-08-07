#include "internal.h"
#include "sge.h"
#include "glad.h"
#include "cjson.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

static uint32_t objects[2048];
static uint16_t objects_used = 0;
static uint32_t programs[2048];

PUBLIC void sge_load_shader(const char *json_filepath) {
    FILE *fp = fopen(json_filepath, "r");
    fseek(fp, 0, SEEK_END);
    size_t len = ftell(fp);
    char *buf = malloc(len + 1);
    fread(buf, 1, len, fp);
    buf[len] = '\0';
    fclose(fp);
    const cJSON *json = cJSON_Parse(buf);
    free(buf);
    const cJSON *jindex = cJSON_GetObjectItem(json, "index");
    const cJSON *jvert = cJSON_GetObjectItem(json, "vertex");
    const cJSON *jfrag = cJSON_GetObjectItem(json, "fragment");
    const cJSON *jattr = cJSON_GetObjectItem(json, "attributes");
    uint32_t index = jindex->valueint;
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
    objects[objects_used++] = v;
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
    objects[objects_used++] = f;
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
    for (int j = 0; j < i; j++) free(attributes[j]);
    free(attributes);
    cJSON_Delete(json);
}

void sge_free_shaders(void) {
    for (int i = 0; i < 2048; i++) glDeleteProgram(programs[i]);
    for (int i = objects_used; i > 0; i--) glDeleteShader(objects_used);
}
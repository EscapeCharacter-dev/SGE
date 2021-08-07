#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef __GNUC__
#define PUBLIC  __attribute__((visibility("default")))
#else
#error unsupported compiler
#endif

void sge_free_shaders(void);

#endif

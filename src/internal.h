#ifndef INTERNAL_H
#define INTERNAL_H

#ifdef __GNUC__
#define PUBLIC  __attribute__((visibility("default")))
#else
#error unsupported compiler
#endif

void me_free_shaders(void);

#endif

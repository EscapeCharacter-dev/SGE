CC	= gcc	# GCC/MinGW

INC	= -I./include -I./lib/glfw/include -I./lib/glad -I./lib/cjson

CFLAGS = -Wall -Wextra -O4 -DENGINE_BUILD $(INC) -fpic -fvisibility=hidden
ifeq ($(UNAME),Linux)
	TARGET = sge64.so
else
	TARGET = sge64.dll
endif

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o $@ -Wl,--output-def,sge64.def --no-undef -lopengl32 lib/glfw/libglfw3.a -lgdi32 -luser32 -lkernel32

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

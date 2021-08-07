CC	= gcc	# GCC/MinGW

INC	= -I./include -I./lib/glfw/include -I./lib/glad -I./lib/cjson

CFLAGS = -Wall -Wextra -O4 -DENGINE_BUILD $(INC) -fpic -fvisibility=hidden
ifeq ($(UNAME),Linux)
	TARGET = sge64.so
	LINKYDINK = -lgl -l
else
	TARGET = sge64.dll
	LINKYDINK = -lgdi32 -luser32 -lkernel32 -lopengl32
endif

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -shared $^ -o $@ -Wl,sge64.def -Wno-undef $(LINKYDINK)

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/*.o obj/*.o obj/*.dll obj/*.exe obj/*.so
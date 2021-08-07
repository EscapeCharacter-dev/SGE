CC	= gcc	# GCC/MinGW

INC	= -I./include -I./lib/glfw/include -I./lib/glad -I./lib/cjson

CFLAGS = -Wall -Wextra -O4 -DENGINE_BUILD $(INC) -fpic -fvisibility=hidden
ifeq ($(UNAME),Windows)
	TARGET = sge64.dll
	LINKYDINK = -lopengl32 lib/glfw/libglfw3.a -lgdi32 -luser32 -lkernel32 -Wl,sge64.def
else
	TARGET = sge64.so
	LINKYDINK = -lGL -lglfw -lm
endif

SOURCES = $(wildcard src/*.c)
OBJECTS = $(patsubst src/%.c, obj/%.o, $(SOURCES))

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -shared $(OBJECTS) -o $@  -Wno-undef $(LINKYDINK)
	$(AR) crs sge64.a $(OBJECTS)
	rm -f sge64.o

obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

test: $(TARGET)
	$(CC) test.c sge64.a -lglfw -lm -ldl
	
clean:
	rm -f src/*.o obj/*.o obj/*.dll obj/*.exe obj/*.so *.a *.so *.out

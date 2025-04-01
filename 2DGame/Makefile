RAYLIB_DIR?=external/raylib/src
RAYLIB_LIB?=$(RAYLIB_DIR)/libraylib.a

SOURCES=main.cpp player.cpp enemy.cpp item.cpp door.cpp gameutils.cpp
OBJECTS=$(SOURCES:.cpp=.o)

.PHONY: all clean
all: main.exe

main.exe: $(OBJECTS)
	g++ --std=c++20 -I$(RAYLIB_DIR) $^ -o $@ $(RAYLIB_LIB) -lopengl32 -lgdi32 -lwinmm

%.o: %.cpp
	g++ --std=c++20 -I$(RAYLIB_DIR) -c $< -o $@

clean:
	rm -f $(OBJECTS) main.exe

# Mac version
#RAYLIB_DIR?=/opt/homebrew/Cellar/raylib/5.5/include/
#RAYLIB_LIB?= -L "/opt/homebrew/Cellar/raylib/5.5/lib" -lraylib

#.PHONY: all
#all: main.exe

#%.exe: %.cpp
#	g++ --std=c++20 -I$(RAYLIB_DIR) $^ -o $@ $(RAYLIB_LIB) 
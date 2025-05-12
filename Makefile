# OBJS specifies which .cpp files to compile
OBJS = main.cpp \
       core/entity.cpp core/init.cpp core/game.cpp core/timeline.cpp \
       modules/collisions.cpp modules/movement.cpp modules/gravity.cpp modules/deathzone.cpp \
       modules/platform.cpp modules/sideboundary.cpp modules/spawnpoint.cpp \
       handlers/collision_handler.cpp handlers/control_handler.cpp handlers/death_handler.cpp handlers/sideboundary_handler.cpp \
       networking/peer.cpp \
       rendering/render.cpp rendering/scaling.cpp \
       core/eventmanager.cpp

# Compiler
CC = g++

# Compiler flags
COMPILER_FLAGS = -g -w

# Linker flags
LINKER_FLAGS = -lSDL2 -lzmq

# Executable output name
OBJ_NAME = main

# Default target
all: $(OBJ_NAME)

# Rule to build the executable
$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# Rules to run different modes
run-server:
	./$(OBJ_NAME) masterPeer

run-client1:
	./$(OBJ_NAME) peer2

run-client2:
	./$(OBJ_NAME) peer3

run-client3:
	./$(OBJ_NAME) peer4

# Clean target to remove binaries
clean:
	rm -f $(OBJ_NAME)

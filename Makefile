# Compiler
CC = g++

# Source files
OBJS = main.cpp \
       src/core/entity.cpp src/core/eventmanager.cpp src/core/init.cpp src/core/timeline.cpp \
       src/handlers/collision_handler.cpp src/handlers/control_handler.cpp src/handlers/death_handler.cpp src/handlers/sideboundary_handler.cpp \
       src/modules/collisions.cpp src/modules/control.cpp src/modules/deathzone.cpp src/modules/gravity.cpp src/modules/movement.cpp src/modules/platform.cpp src/modules/sideboundary.cpp src/modules/spawnpoint.cpp \
       src/networking/peer.cpp \
       src/rendering/render.cpp src/rendering/scaling.cpp src/core/game.cpp

# Include paths
INCLUDES = -Iinclude

# Compiler flags
COMPILER_FLAGS = -g -w

# Linker flags
LINKER_FLAGS = -lSDL2 -lzmq

# Output binary name
OBJ_NAME = main

# Build target
all: $(OBJ_NAME)

$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) $(INCLUDES) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# Run rules
run-server:
	./$(OBJ_NAME) masterPeer

run-client1:
	./$(OBJ_NAME) peer2

run-client2:
	./$(OBJ_NAME) peer3

run-client3:
	./$(OBJ_NAME) peer4

# Clean rule
clean:
	rm -f $(OBJ_NAME)

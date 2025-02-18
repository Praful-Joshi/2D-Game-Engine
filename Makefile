# OBJS specifies which files to compile as part of the project
OBJS = main.cpp entity.cpp collisions.cpp movement.cpp game.cpp gravity.cpp init.cpp render.cpp scaling.cpp \
       timeline.cpp platform.cpp control.cpp property.hpp peer.cpp spawnpoint.cpp deathzone.cpp sideboundary.cpp \
       event.hpp eventhandler.hpp collision_handler.cpp eventmanager.cpp control_handler.cpp death_handler.cpp \
       sideboundary_handler.cpp

# CC specifies which compiler we're using
CC = g++

# COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
COMPILER_FLAGS = -g -w

# LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS = -lSDL2 -lzmq

# OBJ_NAME specifies the name of our executable
OBJ_NAME = main

# This is the target that compiles our executable
all: $(OBJ_NAME)

# Rule to compile the object files and link them into the executable
$(OBJ_NAME): $(OBJS)
	$(CC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

# Rule to run the executable after building
run-server:
	./$(OBJ_NAME) masterPeer

run-client1:
	./$(OBJ_NAME) peer2

run-client2:
	./$(OBJ_NAME) peer3

run-client3:
	./$(OBJ_NAME) peer4

# Clean rule to remove generated files
clean:
	rm -f $(OBJ_NAME)

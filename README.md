# 2D Game Engine

A lightweight and raw 2D game engine written in C++ using SDL2 and ZeroMQ.  
Currently supports basic entity rendering, movement, simple physics, and event handling.

---

## ✨ Features
- Basic shape rendering (rectangles)
- Entity management with attributes (movement, collision, gravity)
- Event-driven updates (collision handling, control handling)
- Basic multiplayer support with peer-to-peer networking via ZeroMQ
- Proportional dynamic window scaling
- Sample games included (Bubble Shooter, Space Invader prototype)

---

## 🏗️ Project Structure

```
2D-Game-Engine/
├── include/            # All engine header files (Entity, Events, Game, Modules, Networking, Rendering)
├── src/
│   ├── core/           # Core engine systems (Entity, Event Manager, Game logic, Initialization, Timeline)
│   ├── handlers/       # Event handlers (Collision, Control, Death, Side Boundary)
│   ├── modules/        # Functional modules (Gravity, Movement, Spawnpoints, Platforms, Side Boundaries)
│   ├── networking/     # Multiplayer networking (Peer-to-Peer Communication using ZeroMQ)
│   ├── rendering/      # SDL2 rendering and dynamic scaling modules
├── main.cpp            # Example: Bubble Shooter game (entry point)
├── main2.cpp           # Another example: Space Invader prototype (alternate entry)
├── Makefile            # Build system
├── LICENSE             # Project License (MIT or GNU GPL depending)
├── README.md           # Project documentation
├── .gitignore          # Git ignore rules

```

---

## 🛠️ How to Build & Run

### Requirements:
- C++17 compatible compiler (e.g., `g++`)
- [SDL2](https://www.libsdl.org/download-2.0.php)
- [ZeroMQ](https://zeromq.org/)

### Building and Running:

1. Open a terminal and compile the project:
   ```bash
   make clean && make
   ```

2. To run the sample Bubble Shooter game:
   ```bash
   ./main masterPeer
   ```

3. To run other test games (like Space Invader):
   - Replace `main.cpp` with `space_invader.cpp` or modify `Makefile` to build different entry points manually.

> 🔥 Note: The engine itself is minimalistic — actual games are written inside `main.cpp` or other .cpp files.

### Controls:
- `Arrow Keys` — Move player-controlled shapes
- `Spacebar` — Shoot (in Bubble Shooter)
- `T` — Toggle scaling mode (constant or proportional)
- `P` — Pause/unpause the game
- `S + H` — Shrink player entity temporarily
- `Double tap arrow keys` — Quick teleportation
- `1/2/3` — Change game speed

---

## 📄 Notes
- This engine is still **in early/raw development**.
- Ideal for **learning** purposes or for **rapid prototyping** of simple games.
- Rendering currently supports **only basic shapes** (no textures or animations yet).
- Games are **implemented manually** inside `main.cpp` by using engine APIs.

---

## ⚡ Credits
- SDL2 for rendering
- ZeroMQ for multiplayer communication
- Licensed under GNU GPL v2

---

# ☀️ Interactive Sun Ray Casting Simulation (SDL2)

An interactive 2D ray-casting simulation built in **C** using **SDL2**. This project demonstrates how light rays propagate from a source and interact with obstacles in real time.

Users can drag both the light source and the obstacle to observe dynamic shadow generation and ray behavior.

---

## Features

* 🌞 360° ray casting from a movable light source
* 🎯 Real-time shadow generation using obstacle collision detection
* 🖱️ Drag-and-drop interaction for both the light source and obstacle
* 🌈 Smooth color gradient across rays
* ⚡ Built with SDL2 for fast rendering
* 📚 Beginner-friendly demonstration of ray casting concepts

---

## Preview

The simulation consists of:

* **Light Source** (white circle)

  * Emits rays in all directions.
  * Can be dragged using the mouse.

* **Obstacle** (white circle)

  * Blocks rays and creates shadows.
  * Can also be dragged freely.

As either object moves, the rays are recalculated instantly.

---

## How It Works

### Ray Generation

The light source generates `RAY_COUNT` rays evenly distributed around a full circle.

```c
double angle = ((double)i / RAY_COUNT) * 2.0 * M_PI;
```

---

### Ray Casting

Each ray advances in small increments:

```c
cx += RAY_STEP * cos(ray.angle);
cy += RAY_STEP * sin(ray.angle);
```

The ray continues until it:

* Leaves the window boundaries, or
* Collides with the obstacle.

---

### Obstacle Detection

Collision is determined using the circle equation:

```c
(dx * dx + dy * dy) < radius²
```

If true, the ray stops rendering.

---

### Color Gradient

A linear interpolation function blends between two colors:

```c
lerp_color(COLOR_SUN_RAY, COLOR_SUN_GLOW, t);
```

creating a glowing sunlight effect.

---

## Controls

| Action            | Control             |
| ----------------- | ------------------- |
| Move Light Source | Left-click and drag |
| Move Obstacle     | Left-click and drag |
| Exit              | Close window        |

---

## Project Structure

```text
.
├── main.c
└── README.md
```

---

## Requirements

* C Compiler (GCC / Clang)
* SDL2 Development Libraries

### Windows (MSYS2)

Install SDL2:

```bash
pacman -S mingw-w64-x86_64-SDL2
```

Compile:

```bash
gcc main.c -o raycast.exe -lSDL2 -lm
```

Run:

```bash
./raycast.exe
```

---

## Configuration

You can modify these constants:

```c
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define RAY_COUNT 250
#define RAY_STEP 1.0
```

### Increasing Ray Count

```c
#define RAY_COUNT 1000
```

Produces smoother lighting but increases CPU usage.

---

## Learning Concepts

This project demonstrates:

* Ray Casting
* Collision Detection
* Circle Mathematics
* SDL2 Rendering
* Interactive Graphics Programming
* Real-Time Simulation

---

## Future Improvements

* Soft shadows
* Multiple light sources
* Reflection rays
* Refraction effects
* Polygon obstacles
* GPU acceleration
* Dynamic lighting intensity

---

## License

This project is open source and free to use for learning, experimentation, and educational purposes.

#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define RAY_COUNT 250
#define RAY_STEP 1.0

#define COLOR_BLACK 0x00000000
#define COLOR_WHITE 0xFFFFFFFF
#define COLOR_SUN_RAY 0xFFFFD700
#define COLOR_SUN_GLOW 0xFFFFA500

typedef struct
{
    double x;
    double y;
    double radius;
} Circle;

typedef struct
{
    double x_start, y_start;
    double angle;
} Ray;

typedef enum
{
    DRAG_NONE,
    DRAG_LIGHT,
    DRAG_OBSTACLE
} DragTarget;

void fill_circle(SDL_Surface *surface, Circle circle, Uint32 color)
{
    double r_sq = circle.radius * circle.radius;
    for (double px = circle.x - circle.radius; px <= circle.x + circle.radius; px++)
    {
        for (double py = circle.y - circle.radius; py <= circle.y + circle.radius; py++)
        {
            double dist_sq = (px - circle.x) * (px - circle.x) + (py - circle.y) * (py - circle.y);
            if (dist_sq < r_sq)
            {
                SDL_Rect pixel = {(int)px, (int)py, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

static Uint32 lerp_color(Uint32 a, Uint32 b, double t)
{
    Uint8 ar = (a >> 16) & 0xFF, ag = (a >> 8) & 0xFF, ab = a & 0xFF;
    Uint8 br = (b >> 16) & 0xFF, bg = (b >> 8) & 0xFF, bb = b & 0xFF;
    return 0xFF000000 | ((Uint8)(ar + (br - ar) * t) << 16) | ((Uint8)(ag + (bg - ag) * t) << 8) | ((Uint8)(ab + (bb - ab) * t));
}

void cast_rays(SDL_Surface *surface, Ray rays[RAY_COUNT], Circle obstacle)
{
    double r_sq = obstacle.radius * obstacle.radius;

    for (int i = 0; i < RAY_COUNT; i++)
    {
        Ray ray = rays[i];
        double cx = ray.x_start;
        double cy = ray.y_start;

        double t = (double)i / RAY_COUNT;
        Uint32 color = lerp_color(COLOR_SUN_RAY, COLOR_SUN_GLOW, t);

        while (1)
        {
            cx += RAY_STEP * cos(ray.angle);
            cy += RAY_STEP * sin(ray.angle);

            if (cx < 0 || cx > WINDOW_WIDTH || cy < 0 || cy > WINDOW_HEIGHT)
                break;

            double dist_sq = (cx - obstacle.x) * (cx - obstacle.x) + (cy - obstacle.y) * (cy - obstacle.y);
            if (dist_sq < r_sq)
                break;

            SDL_Rect pixel = {(int)cx, (int)cy, 1, 1};
            SDL_FillRect(surface, &pixel, color);
        }
    }
}

void generate_rays(Circle source, Ray rays[RAY_COUNT])
{
    for (int i = 0; i < RAY_COUNT; i++)
    {
        double angle = ((double)i / RAY_COUNT) * 2.0 * M_PI;
        rays[i] = (Ray){source.x, source.y, angle};
    }
}

static int point_in_circle(double px, double py, Circle c)
{
    double dx = px - c.x;
    double dy = py - c.y;
    return (dx * dx + dy * dy) <= (c.radius * c.radius);
}

int main(void)
{
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(
        "Sun Ray Casting — drag both circles",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);
    SDL_Rect clear_rect = {0, 0, WINDOW_WIDTH, WINDOW_HEIGHT};

    Circle light = {200.0, 300.0, 50.0};
    Circle obstacle = {550.0, 300.0, 100.0};
    Ray rays[RAY_COUNT];
    generate_rays(light, rays);

    DragTarget dragging = DRAG_NONE;
    double drag_offset_x = 0.0;
    double drag_offset_y = 0.0;

    int running = 1;
    SDL_Event event;

    while (running)
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
                running = 0;
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    double mx = event.button.x;
                    double my = event.button.y;

                    if (point_in_circle(mx, my, light))
                    {
                        dragging = DRAG_LIGHT;
                        drag_offset_x = light.x - mx;
                        drag_offset_y = light.y - my;
                    }
                    else if (point_in_circle(mx, my, obstacle))
                    {
                        dragging = DRAG_OBSTACLE;
                        drag_offset_x = obstacle.x - mx;
                        drag_offset_y = obstacle.y - my;
                    }
                }
                break;

            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    dragging = DRAG_NONE;
                break;

            case SDL_MOUSEMOTION:
                if (dragging == DRAG_LIGHT)
                {
                    light.x = event.motion.x + drag_offset_x;
                    light.y = event.motion.y + drag_offset_y;
                    generate_rays(light, rays);
                }
                else if (dragging == DRAG_OBSTACLE)
                {
                    obstacle.x = event.motion.x + drag_offset_x;
                    obstacle.y = event.motion.y + drag_offset_y;
                }
                break;
            }
        }

        SDL_FillRect(surface, &clear_rect, COLOR_BLACK);
        cast_rays(surface, rays, obstacle);
        fill_circle(surface, light, COLOR_WHITE);
        fill_circle(surface, obstacle, COLOR_WHITE);

        SDL_UpdateWindowSurface(window);
        SDL_Delay(10);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
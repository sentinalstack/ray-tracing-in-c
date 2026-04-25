#define SDL_MAIN_HANDLED
#include <stdio.h>
#include <SDL2/SDL.h>
#include <math.h>

#define WIDTH 800
#define HEIGHT 600
#define COLOR_WHITE 0xffffffff

struct Circle
{
    double x;
    double y;
    double radius;
};

void FillCircle(SDL_Surface *surface, struct Circle circle, Uint32 color)
{
    double radius_squared = pow(circle.radius, 2);
    for (double i = circle.x - circle.radius; i <= circle.x + circle.radius; i++)
    {
        for (double j = circle.y - circle.radius; j <= circle.y + circle.radius; j++)
        {
            double distance_sqaured = pow(i - circle.x, 2) + pow(j - circle.y, 2);

            if (distance_sqaured < radius_squared)
            {
                SDL_Rect pixel = (SDL_Rect){i, j, 1, 1};
                SDL_FillRect(surface, &pixel, color);
            }
        }
    }
}

int main()
{
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Ray Tracing in C", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    SDL_Surface *surface = SDL_GetWindowSurface(window);

    // SDL_Rect rect = (SDL_Rect){200, 200, 200, 200};
    // SDL_FillRect(surface, &rect, COLOR_WHITE);

    struct Circle circle = {200, 200, 80};
    FillCircle(surface, circle, COLOR_WHITE);

    SDL_UpdateWindowSurface(window);
    SDL_Delay(5000);
}
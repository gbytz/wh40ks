#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#define ENTITIES_LIMIT 64

uint64_t entities = 0;
uint64_t create_entity()
{
    return ++entities;
}

struct Army
{
    uint64_t value;
};
typedef struct Army Army;

Army armies[ENTITIES_LIMIT];

void print_army(Army *a)
{
    printf("Army: %lu\n", a->value);
}

Army *add_army(uint64_t entityId)
{
    armies[entityId] = (Army){0};
    return &armies[entityId];
}

Army *get_army(uint64_t entityId)
{
    return &armies[entityId];
}

void remove_army(uint64_t entityId)
{
    armies[entityId] = (Army){0};
}

struct Unit
{
    uint64_t value;
};
typedef struct Unit Unit;

Unit units[ENTITIES_LIMIT];

void print_unit(Unit *u)
{
    printf("Unit: %lu\n", u->value);
}

Unit *add_unit(uint64_t entityId)
{
    units[entityId] = (Unit){0};
    return &units[entityId];
}

Unit *get_unit(uint64_t entityId)
{
    return &units[entityId];
}

void remove_unit(uint64_t entityId)
{
    units[entityId] = (Unit){0};
}

struct Position
{
    double x;
    double y;
    double z;
};
typedef struct Position Position;

Position positions[ENTITIES_LIMIT];

void print_position(Position *p)
{
    printf("Position: (%f, %f, %f)\n", p->x, p->y, p->z);
}

Position *add_position(uint64_t entityId)
{
    positions[entityId] = (Position){0};
    return &positions[entityId];
}

Position *get_position(uint64_t entityId)
{
    return &positions[entityId];
}

void remove_position(uint64_t entityId)
{
    positions[entityId] = (Position){0};
}

enum BaseShape
{
    None,
    Circular,
    Elliptical,
    Rectangular
};
typedef enum BaseShape BaseShape;

struct Base
{
    BaseShape shape;
    double r0;
    double r1;
};
typedef struct Base Base;

Base bases[ENTITIES_LIMIT];

void print_base(Base *b)
{
    printf("Base: ");
    switch(b->shape)
    {
        case Circular:
            printf("Circular: r = %f", b->r0);
            break;
        case Elliptical:
            printf("Elliptical: rx = %f, ry = %f", b->r0, b->r1);
            break;
        case Rectangular:
            printf("Rectangular: w = %f, h = %f", b->r0, b->r1);
            break;
        default:
            printf("Unknown BaseShape.");
    }
    printf("\n");
}

Base *add_base(uint64_t entityId)
{
    bases[entityId] = (Base){0};
    return &bases[entityId];
}

Base *get_base(uint64_t entityId)
{
    return &bases[entityId];
}

Base *remove_base(uint64_t entityId)
{
    bases[entityId] = (Base){0};
}

struct WindowSystem
{
    int (*start)(void);
    int (*stop)(void);
};
typedef struct WindowSystem WindowSystem;

const uint64_t SCREEN_WIDTH = 1280;
const uint64_t SCREEN_HEIGHT = 720;
SDL_Window* window = NULL;

int start_window_system()
{
    if(SDL_InitSubSystem(SDL_INIT_VIDEO) < 0)
    {
        printf("Failed to initialize SDL Video subsystem. SDL_Error: %s\n", SDL_GetError());
        return -2;
    }
    else
    {
        printf("Initialized SDL Video subsystem.\n");
        if(!(window = SDL_CreateWindow("WH40KS", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN)))
        {
            printf("Failed to create SDL Window. SDL_Error: %s\n", SDL_GetError());
            return -1;
        }
        else
        {
            printf("Created SDL Window.\n");
            return 0;
        }
    }
}

int stop_window_system()
{

    SDL_DestroyWindow(window);
    window = NULL;
    SDL_Quit();
}

struct RenderSystem
{
    int (*start)(void);
    int (*stop)(void);
    int (*update)(void);
};
typedef struct RenderSystem RenderSystem;

SDL_Renderer* renderer = NULL;

int start_render_system()
{
    if(!(renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED)))
    {
        printf("Failed to create SDL Renderer. SDL_Error: %s\n", SDL_GetError());
    }
    else
    {
        printf("Created SDL Renderer.\n");
    }
}

int stop_render_system()
{
    SDL_DestroyRenderer(renderer);
}

void clear_renderer(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
    SDL_RenderClear(renderer);
}

void present_renderer(SDL_Renderer* renderer)
{
    SDL_RenderPresent(renderer);
}

void render_positions(Position *positions)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (int i = 0; i < ENTITIES_LIMIT; ++i)
    {
        SDL_RenderDrawPoint(renderer, positions[i].x, positions[i].y);
    }
}

int update_render_system()
{
    clear_renderer(renderer);
    render_positions(positions);
    present_renderer(renderer);
}

int main(int argc, char const *argv[])
{
    printf("entities: %lu\n", entities);
    uint64_t id = create_entity();
    printf("entities: %lu\n", entities);
    printf("entity: %lu\n", id);

    Army *a = add_army(id);
    print_army(a);
    a->value = 8;
    print_army(get_army(id));
    remove_army(id);
    print_army(get_army(id));

    Unit *u = add_unit(id);
    print_unit(u);
    u->value = 8;
    print_unit(get_unit(id));
    remove_unit(id);
    print_unit(get_unit(id));

    Position *p = add_position(id);
    print_position(p);
    *p = (Position){ .z = 3.0, .y = 2.0, .x = 1.0};
    print_position(get_position(id));
    remove_position(id);
    print_position(get_position(id));

    Base *b = add_base(id);
    print_base(b);

    *b = (Base)
    {
        .shape = Circular,
        .r0 = 25.0
    };
    print_base(get_base(id));

    *b = (Base)
    {
        .shape = Elliptical,
        .r0 = 25.0,
        .r1 = 70.0
    };
    print_base(get_base(id));

    *b = (Base)
    {
        .shape = Rectangular,
        .r0 = 25.0,
        .r1 = 70.0
    };
    print_base(get_base(id));

    remove_base(id);
    print_base(get_base(id));

    WindowSystem windowSystem;
    windowSystem.start = start_window_system;
    windowSystem.stop = stop_window_system;

    RenderSystem renderSystem;
    renderSystem.start = start_render_system;
    renderSystem.stop = stop_render_system;
    renderSystem.update = update_render_system;

    windowSystem.start();
    renderSystem.start();

    renderSystem.update();

    renderSystem.stop();
    windowSystem.stop();
    return 0;
}

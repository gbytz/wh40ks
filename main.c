#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

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
    printf("Army: %"PRIu64"\n", a->value);
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
    printf("Unit: %"PRIu64"\n", u->value);
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

void remove_base(uint64_t entityId)
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
    printf("Destroyed SDL Window.\n");

    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    printf("Finalized SDL Video subsystem.\n");

    SDL_Quit();
    return 0;
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
    return 0;
}

int stop_render_system()
{
    SDL_DestroyRenderer(renderer);
    renderer = NULL;
    printf("Destroyed SDL Renderer.\n");
    return 0;
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

void render_positions(SDL_Renderer *renderer, Position *positions)
{
    SDL_SetRenderDrawColor(renderer, 0x00, 0xFF, 0x00, 0xFF);
    for (int i = 1; i <= entities; ++i)
    {
        SDL_RenderDrawPoint(renderer, positions[i].x, positions[i].y);
    }
}

void render_base_circular(SDL_Renderer *renderer, Position *position, Base *base)
{
    aacircleRGBA(renderer, position->x, position->y, base->r0, 0xFF, 0x00, 0x00, 0xFF);
}

void render_base_elliptical(SDL_Renderer *renderer, Position *position, Base *base)
{
    aaellipseRGBA(renderer, position->x, position->y, base->r0, base->r1, 0xFF, 0x00, 0x00, 0xFF);
}

void render_base_rectangular(SDL_Renderer *renderer, Position *position, Base *base)
{
    rectangleRGBA(renderer,
        position->x + base->r0,
        position->y + base->r1,
        position->x - base->r0,
        position->y - base->r1,
        0xFF, 0x00, 0x00, 0xFF
        );
}

void render_bases(SDL_Renderer *renderer, Position *positions, Base *bases)
{
    for (int i = 1; i <= entities; ++i)
    {
        switch(bases[i].shape)
        {
            case Circular:
                render_base_circular(renderer, &positions[i], &bases[i]);
                break;
            case Elliptical:
                render_base_elliptical(renderer, &positions[i], &bases[i]);
                break;
            case Rectangular:
                render_base_rectangular(renderer, &positions[i], &bases[i]);
                break;
            default:
                printf("Unknown BaseShape at index: %d\n", i);
        }
    }
}

int update_render_system()
{
    clear_renderer(renderer);
    render_positions(renderer, positions);
    render_bases(renderer, positions, bases);
    present_renderer(renderer);
    return 0;
}

#define INPUTS_LIMIT 64

struct InputSystem
{
    int (*start)(void);
    int (*stop)(void);
    int (*update)(void);
};
typedef struct InputSystem InputSystem;

typedef SDL_Event InputEvent;
InputEvent inputs[INPUTS_LIMIT];

int start_input_system()
{
    if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
    {
        printf("Failed to initialize SDL Events subsystem. SDL_Error: %s\n", SDL_GetError());
        return -1;
    }
    else
    {
        printf("Initialized SDL Events subsystem.\n");
        return 0;
    }
}

int stop_input_system()
{
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
    printf("Finalized SDL Events subsystem.\n");
    return 0;
}

int peep_inputs(InputEvent *inputs, int inputsLimit)
{
    SDL_PumpEvents();
    return SDL_PeepEvents(inputs, inputsLimit, SDL_PEEKEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
}

int get_inputs(InputEvent *inputs, int inputsLimit)
{
    SDL_PumpEvents();
    return SDL_PeepEvents(inputs, inputsLimit, SDL_GETEVENT, SDL_FIRSTEVENT, SDL_LASTEVENT);
}

void print_input_quit(InputEvent *e)
{
    printf("QUIT:{");
    printf(" type: %d,", e->quit.type);
    printf(" timestamp: %d", e->quit.timestamp);
    printf(" }\n");
}

void print_input_mouse_button(InputEvent *e)
{
    printf("%s:{", e->button.type == SDL_MOUSEBUTTONDOWN ? "MOUSEBUTTONDOWN" : "MOUSEBUTTONUP");
    printf(" type: %d,", e->button.type);           // the event type; SDL_MOUSEBUTTONDOWN or SDL_MOUSEBUTTONUP
    printf(" timestamp: %d,", e->button.timestamp); // timestamp of the event
    printf(" window: %d,", e->button.windowID);     // the window with mouse focus, if any
    printf(" which: %d,", e->button.which);         // the mouse instance id, or SDL_TOUCH_MOUSEID; see Remarks for details
    printf(" button: %d,", e->button.button);       // the button that changed; see Remarks for details
    printf(" state: %d,", e->button.state);         // the state of the button; SDL_PRESSED or SDL_RELEASED
    printf(" clicks: %d,", e->button.clicks);       // 1 for single-click, 2 for double-click, etc. (>= SDL 2.0.2)
    printf(" x: %d,", e->button.x);                 // X coordinate, relative to window
    printf(" y: %d", e->button.y);                  // Y coordinate, relative to window
    printf(" }\n");
}

void print_input_mouse_motion(InputEvent *e)
{
    printf("MOUSEMOTION:{");
    printf(" type: %d,", e->motion.type);            // the event type; SDL_MOUSEMOTION
    printf(" timestamp: %d,", e->motion.timestamp);  // timestamp of the event
    printf(" windowID: %d,", e->motion.windowID);    // the window with mouse focus, if any
    printf(" which: %d,", e->motion.which);          // the mouse instance id, or SDL_TOUCH_MOUSEID; see Remarks for details
    printf(" state: %d,", e->motion.state);          // the state of the button; see Remarks for details
    printf(" x: %d,", e->motion.x);                  // X coordinate, relative to window
    printf(" y: %d,", e->motion.y);                  // Y coordinate, relative to window
    printf(" xrel: %d,", e->motion.xrel);            // relative motion in the X direction
    printf(" yrel: %d", e->motion.yrel);            // relative motion in the Y direction
    printf(" }\n");
}

void print_input_unknown(InputEvent *e)
{
    printf("UNKNOWN INPUT:{");
    printf(" type: %d", e->type);
    printf(" }\n");
}

void print_inputs(InputEvent *inputs, int inputsLimit)
{
    for (int i = 0; i < inputsLimit; ++i)
    {
        switch(inputs[i].type)
        {
            case SDL_QUIT: print_input_quit(&inputs[i]); break;
            case SDL_MOUSEBUTTONDOWN: print_input_mouse_button(&inputs[i]); break;
            case SDL_MOUSEBUTTONUP: print_input_mouse_button(&inputs[i]); break;
            case SDL_MOUSEMOTION: print_input_mouse_motion(&inputs[i]); break;
            default: print_input_unknown(&inputs[i]); break;
        }
    }
}

int update_input_system()
{
    int inputsCount = peep_inputs(inputs, INPUTS_LIMIT);
    print_inputs(inputs, inputsCount);
    return inputsCount;
}

int main(int argc, char const *argv[])
{
    printf("entities: %"PRIu64"\n", entities);
    uint64_t id = create_entity();
    printf("entities: %"PRIu64"\n", entities);
    printf("entity: %"PRIu64"\n", id);

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

    a->value = 1;
    u->value = 1;
    *p = (Position){.x = 100.0, .y = 100.0, .z = 0.0};
    *b = (Base){.shape = Circular, .r0 = 25.0};

    uint64_t id1 = create_entity();
    Army *a1 = add_army(id1);
    Unit *u1 = add_unit(id1);
    Position *p1 = add_position(id1);
    Base *b1 = add_base(id1);
    a1->value = 2;
    u1->value = 1;
    *p1 = (Position){.x = 200.0, .y = 200.0, .z = 0.0};
    *b1 = (Base){.shape = Elliptical, .r0 = 25.0, .r1 = 70.0};

    uint64_t id2 = create_entity();
    Army *a2 = add_army(id2);
    Unit *u2 = add_unit(id2);
    Position *p2 = add_position(id2);
    Base *b2 = add_base(id2);
    a2->value = 2;
    u2->value = 1;
    *p2 = (Position){.x = 400.0, .y = 400.0, .z = 0.0};
    *b2 = (Base){.shape = Rectangular, .r0 = 25.0, .r1 = 70.0};

    WindowSystem windowSystem;
    windowSystem.start = start_window_system;
    windowSystem.stop = stop_window_system;

    RenderSystem renderSystem;
    renderSystem.start = start_render_system;
    renderSystem.stop = stop_render_system;
    renderSystem.update = update_render_system;

    InputSystem inputSystem;
    inputSystem.start = start_input_system;
    inputSystem.stop = stop_input_system;
    inputSystem.update = update_input_system;

    windowSystem.start();
    renderSystem.start();
    inputSystem.start();

    bool quit = false;
    SDL_Event e;
    while(!quit)
    {
        inputSystem.update();
        //Handle events on queue
        while( SDL_PollEvent( &e ) != 0 )
        {
            //User requests quit
            if( e.type == SDL_QUIT )
            {
                quit = true;
            }
        }
        renderSystem.update();
    }

    inputSystem.stop();
    renderSystem.stop();
    windowSystem.stop();
    return 0;
}

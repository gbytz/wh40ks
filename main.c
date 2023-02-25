#include <inttypes.h>
#include <stdio.h>

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

    return 0;
}

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

    return 0;
}

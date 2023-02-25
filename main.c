#include <inttypes.h>
#include <stdio.h>

#define ENTITIES_LIMIT 64

uint64_t entities = 0;
uint64_t create_entity()
{
    return ++entities;
}

int main(int argc, char const *argv[])
{
    printf("entities: %lu\n", entities);
    uint64_t id = create_entity();
    printf("entities: %lu\n", entities);
    printf("entity: %lu\n", id);

    return 0;
}

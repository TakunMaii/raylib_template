#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "miecs.h"
#include <raylib.h>

#define MAX_PARTICLES 100

typedef struct {
    float x, y;
} Position;

typedef struct {
    Texture2D texture;
    // the less the layer, the earlier it is drawn (background -> foreground)
    int layer;
    Shader shader;
} Sprite;

typedef struct {
    float vx, vy;
} Velocity;

typedef struct {
    float ax, ay;
} Accelaration;

typedef struct {
    struct {
        float life;
        Vector2 velocity;
        Vector2 position;
        float radius;
        Color color;
    } particles[MAX_PARTICLES];
    int count;
} ParticleCollection;

miecs_component_type Position_type;
miecs_component_type Sprite_type;
miecs_component_type Velocity_type;
miecs_component_type ParticleCollection_type;
miecs_component_type Accelaration_type;

void RegisterBasicComponents(miecs_world *world)
{
    Position_type = miecs_component_register(world, "Position", sizeof(Position));
    Sprite_type = miecs_component_register(world, "Sprite", sizeof(Sprite));
    Velocity_type = miecs_component_register(world, "Velocity", sizeof(Velocity));
    Accelaration_type = miecs_component_register(world, "Accelaration", sizeof(Accelaration));
    ParticleCollection_type = miecs_component_register(world, "ParticleCollection", sizeof(ParticleCollection));
}

#endif
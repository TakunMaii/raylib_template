#ifndef COMPONENTS_H_INCLUDED
#define COMPONENTS_H_INCLUDED

#include "miecs.h"
#include <raylib.h>

#define MAX_PARTICLES 100

typedef struct {
    float x, y;
} Position;

typedef struct {
    float scale;
} Scale;

typedef struct {
    float rotationInDegrees;
} Rotation;

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
} Acceleration;

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
miecs_component_type Scale_type;
miecs_component_type Rotation_type;
miecs_component_type Sprite_type;
miecs_component_type Velocity_type;
miecs_component_type ParticleCollection_type;
miecs_component_type Acceleration_type;

void RegisterBasicComponents(miecs_world *world)
{
    Position_type = miecs_component_register(world, "Position", sizeof(Position));
    Scale_type = miecs_component_register(world, "Scale", sizeof(Scale));
    Rotation_type = miecs_component_register(world, "Rotation", sizeof(Rotation));
    Sprite_type = miecs_component_register(world, "Sprite", sizeof(Sprite));
    Velocity_type = miecs_component_register(world, "Velocity", sizeof(Velocity));
    Acceleration_type = miecs_component_register(world, "Acceleration", sizeof(Acceleration));
    ParticleCollection_type = miecs_component_register(world, "ParticleCollection", sizeof(ParticleCollection));
}

#endif
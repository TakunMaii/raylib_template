#ifndef BASIC_SYSTEMS_H_INCLUDED
#define BASIC_SYSTEMS_H_INCLUDED

#include <raylib.h>
#include "miecs.h"
#include "basic_components.h"

void VelocitySystem(miecs_world *world, float delta_time)
{
    miecs_view_iter it;
    miecs_entity e;
    miecs_view_begin(&it, world, 2, Position_type, Velocity_type);
    while (miecs_view_next(&it, &e)) {
        Position *p = miecs_component_get(world, e, Position_type);
        Velocity *v = miecs_component_get(world, e, Velocity_type);

        p->x += v->vx * delta_time;
        p->y += v->vy * delta_time;
    }
}

void AccelarationSystem(miecs_world *world, float delta_time)
{
    miecs_view_iter it;
    miecs_entity e;
    miecs_view_begin(&it, world, 2, Velocity_type, Accelaration_type);
    while (miecs_view_next(&it, &e)) {
        Velocity *v = miecs_component_get(world, e, Velocity_type);
        Accelaration *a = miecs_component_get(world, e, Accelaration_type);

        v->vx += a->ax * delta_time;
        v->vy += a->ay * delta_time;
    }
}

void SpriteDrawingSystem(miecs_world *world)
{
    // find all entities with Position and Sprite components
    miecs_entity *entities = (miecs_entity *)malloc(sizeof(miecs_entity) * world->entity_count);
    int count = 0;

    miecs_view_iter it;
    miecs_entity e;
    miecs_view_begin(&it, world, 2, Position_type, Sprite_type);
    while (miecs_view_next(&it, &e)) {
        entities[count++] = e;
    }

    // Quick sort entities by Sprite.layer
    int i = 0;
    int j = count - 1;
    while (i < j) {
        Sprite *si = miecs_component_get(world, entities[i], Sprite_type);
        Sprite *sj = miecs_component_get(world, entities[j], Sprite_type);
        if (si->layer > sj->layer) {
            // swap
            miecs_entity temp = entities[i];
            entities[i] = entities[j];
            entities[j] = temp;
            i++;
            j--;
        } else {
            j--;
        }
    }

    // draw entities in sorted order
    for (int k = 0; k < count; ++k) {
        Sprite *s = miecs_component_get(world, entities[k], Sprite_type);
        Position *p = miecs_component_get(world, entities[k], Position_type);
        BeginShaderMode(s->shader);
        DrawTexture(s->texture, (int)p->x, (int)p->y, WHITE);
        EndShaderMode();
    }

    free(entities);
}

void ParticleUpdateSystem(miecs_world *world, float delta_time)
{
    miecs_view_iter it;
    miecs_entity e;
    miecs_view_begin(&it, world, 1, ParticleCollection_type);
    while (miecs_view_next(&it, &e)) {
        ParticleCollection *pc = miecs_component_get(world, e, ParticleCollection_type);
        for (int i = 0; i < pc->count; ++i) {
            pc->particles[i].life -= delta_time;
            if (pc->particles[i].life > 0) {
                pc->particles[i].position.x += pc->particles[i].velocity.x * delta_time;
                pc->particles[i].position.y += pc->particles[i].velocity.y * delta_time;
            }
            else {
                // remove dead particle by swapping with the last one
                pc->particles[i] = pc->particles[pc->count - 1];
                pc->count--;
                i--; // check the swapped particle in the next iteration
            }
        }

        if(pc->count <= 0) {
            miecs_entity_destroy(world, e);
        }
    }
}

void ParticleDrawingSystem(miecs_world *world)
{
    miecs_view_iter it;
    miecs_entity e;
    miecs_view_begin(&it, world, 1, ParticleCollection_type);
    while (miecs_view_next(&it, &e)) {
        ParticleCollection *pc = miecs_component_get(world, e, ParticleCollection_type);
        for (int i = 0; i < pc->count; ++i) {
            DrawCircleV(pc->particles[i].position, pc->particles[i].radius, pc->particles[i].color);
        }
    }
}

#endif
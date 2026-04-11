#define MIECS_IMPLEMENTATION
#include "miecs.h"
#include "basic_components.h"
#include "basic_systems.h"
#include <raylib.h>
#include <raymath.h>

int main(void)
{
    const int window_width = 800;
    const int window_height = 450;
    const float ball_radius = 24.0f;

    InitWindow(window_width, window_height, "raylib + miecs basic example");
    SetTargetFPS(60);

    miecs_world *world = miecs_world_create();
    RegisterBasicComponents(world);

    miecs_entity ball = miecs_entity_create(world);
    Position *p = (Position *)miecs_component_add(world, ball, Position_type);
    Velocity *v = (Velocity *)miecs_component_add(world, ball, Velocity_type);

    p->x = window_width * 0.5f;
    p->y = window_height * 0.5f;
    v->vx = 180.0f;
    v->vy = 130.0f;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        VelocitySystem(world, dt);

        // Handle ball bouncing on window edges
        miecs_view_iter move_it;
        miecs_entity e;
        miecs_view_begin(&move_it, world, 2, Position_type, Velocity_type);
        while (miecs_view_next(&move_it, &e)) {
            Position *pos = (Position *)miecs_component_get(world, e, Position_type);
            Velocity *vel = (Velocity *)miecs_component_get(world, e, Velocity_type);

            if (pos->x < ball_radius || pos->x > window_width - ball_radius) {
                vel->vx *= -1.0f;
                pos->x = Clamp(pos->x, ball_radius, window_width - ball_radius);
            }
            if (pos->y < ball_radius || pos->y > window_height - ball_radius) {
                vel->vy *= -1.0f;
                pos->y = Clamp(pos->y, ball_radius, window_height - ball_radius);
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        miecs_view_iter draw_it;
        miecs_view_begin(&draw_it, world, 1, Position_type);
        while (miecs_view_next(&draw_it, &e)) {
            Position *pos = (Position *)miecs_component_get(world, e, Position_type);
            DrawCircleV((Vector2){ pos->x, pos->y }, ball_radius, ORANGE);
        }

        DrawText("MIECS + raylib basic demo", 20, 20, 24, DARKGRAY);
        DrawText("Entity: Position + Velocity", 20, 52, 18, GRAY);
        EndDrawing();
    }

    miecs_world_destroy(world);
    CloseWindow();
    return 0;
}
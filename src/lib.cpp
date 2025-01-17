#include "common.hpp"


extern "C" {
    void init(State* state) {
        state->pos = { .x = 100.0, .y = 100.0 };
        state->vel = { .x = 0.0, .y = 0.0 };
    }

    void update(State* state) {
        static bool on_floor = false;

        auto bottom = GetScreenHeight();

        if (IsKeyDown(KEY_D)) {
            state->vel.x += 0.1;
        }
        if (IsKeyDown(KEY_A)) {
            state->vel.x -= 0.1;
        }

        if (on_floor && IsKeyDown(KEY_SPACE)) {
            state->vel.y -= 7.0;
        }

        state->vel.y += 0.1;

        state->pos.x += state->vel.x;
        state->pos.y += state->vel.y;

        if (state->pos.y + 16 >= bottom) {
            state->pos.y = (float)(bottom - 16);
            state->vel.y = 0.0;
            state->vel.x *= 0.7;
            on_floor = true;
        }
        else {
            on_floor = false;
        }
    }

    void render(State* state) {
        BeginDrawing();
            ClearBackground((Color) { .r=20, .g=20, .b=20, .a=255 });
            DrawRectangle(
                state->pos.x,
                state->pos.y,
                16,
                16,
                WHITE
            );
            DrawFPS(20, 20);
        EndDrawing();
    }

    void drop(State* state) {}
}

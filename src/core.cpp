#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/time.h>

#include <dlfcn.h>
#include <raylib.h>


/*
    Keybinds:
     - F5 : Lib reload
     - CTRL + F5 : Lib drop & re-init
     - SHIFT + F5 : Force app reload

*/

typedef void* State;

static State state = { 0 };

void* lib = nullptr;
const char* path;

void (*init)(State*) = nullptr;
void (*update)(State*) = nullptr;
void (*render)(State*) = nullptr;
void (*drop)(State*) = nullptr;

long prev = 0;


void reload() {
    bool valid = true;

    printf("\n\x1b[30m 𐬽 Reloading library...\x1b[0m\n");
    fflush(stdout);

    if (lib) dlclose(lib);

    void* temp_lib = dlopen(path, RTLD_NOW);
    if (!temp_lib) {
        printf(
            "\x1b[31m ✘\x1b[0;30m Couln't load library: %s\x1b[0m\n",
            dlerror()
        );
        return;
    }
    lib = temp_lib;

    void (*tmp_init)(State *) = (void (*)(State*)) dlsym(lib, "init");
    if (!tmp_init) {
        valid = false;
        printf(
            "\x1b[31m ✘\x1b[0;30m Couln't load init(): %s\x1b[0m\n",
            dlerror()
        );
    }
    init = tmp_init;

    void (*tmp_update)(State *) = (void (*)(State*)) dlsym(lib, "update");
    if (!tmp_update) {
        valid = false;
        printf(
            "\x1b[31m ✘\x1b[0;30m Couln't load update(): %s\x1b[0m\n",
            dlerror()
        );
    }
    update = tmp_update;

    void (*tmp_render)(State *) = (void (*)(State*)) dlsym(lib, "render");
    if (!tmp_render) {
        valid = false;
        printf(
            "\x1b[31m ✘\x1b[0;30m Couln't load render(): %s\x1b[0m\n",
            dlerror()
        );
    }
    render = tmp_render;

    void (*tmp_drop)(State *) = (void (*)(State*)) dlsym(lib, "drop");
    if (!tmp_drop) {
        valid = false;
        printf(
            "\x1b[31m ✘\x1b[0;30m Couln't load drop(): %s\x1b[0m\n",
            dlerror()
        );
    }
    drop = tmp_drop;

    if (valid) {
        puts("\x1b[32m ✓\x1b[0m \x1b[30mReloaded!\x1b[0m");
        fflush(stdout);
    }

    prev = GetFileModTime(path);
}

void hot() {
    static int cooldown = 0;

    if (cooldown >= 0) {
        cooldown -= 1;

        if (cooldown == 0) {
            // drop(&state);
            reload();
            // init(&state);
        }
    }

    else {
        if (GetFileModTime(path) > prev) {
            cooldown = 4;
            // printf("Changes detected!\n");
        }
    }
}

int main() {
    path = strdup("dist/libhot.so");

    reload();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Hot Reloading [example]");
    SetTargetFPS(60);

    init(&state);

    while (!WindowShouldClose()) {
        // Automatic hot-reloading
        hot();

        if (update) update(&state);
        
        if (render) {
            render(&state);
        }
        else {
            BeginDrawing();
                ClearBackground((Color) { 20, 20, 20, 255 });
                DrawText(
                    "Library failed to load properly...",
                    20,
                    20,
                    24,
                    WHITE
                );
            EndDrawing();
        }
    }

    if (drop) drop(&state);

    if (lib) dlclose(lib);

    CloseWindow();

    return 0;
}

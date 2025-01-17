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

bool reload() {
    printf("\x1b[30mReloading library...\x1b[0m");
    fflush(stdout);

    update = nullptr;
    if (lib) {
        dlclose(lib);
    }

    lib = dlopen(path, RTLD_NOW);

    if (!lib) {
        fprintf(stderr, "\nCouln't load library: %s\n", dlerror());
        return false;
    }

    init = (void (*)(State*)) dlsym(lib, "init");
    if (!init) {
        fprintf(stderr, "\nCouln't load init(): %s\n", dlerror());
        return false;
    }

    update = (void (*)(State*)) dlsym(lib, "update");
    if (!update) {
        fprintf(stderr, "\nCouln't load update(): %s\n", dlerror());
        return false;
    }

    render = (void (*)(State*)) dlsym(lib, "render");
    if (!render) {
        fprintf(stderr, "\nCouln't load render(): %s\n", dlerror());
        return false;
    }

    drop = (void (*)(State*)) dlsym(lib, "drop");
    if (!drop) {
        fprintf(stderr, "\nCouln't load drop(): %s\n", dlerror());
        return false;
    }

    puts("\x1b[32m ✓\x1b[0m");
    fflush(stdout);

    prev = GetFileModTime(path);

    return true;
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
        }
    }
}

int main() {
    path = strdup("dist/libhot.so");

    if (!reload()) {
        return 1;
    }

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Hot Reloading [example]");
    SetTargetFPS(60);

    init(&state);

    while (!WindowShouldClose()) {
        // Automatic hot-reloading
        hot();

        update(&state);
        render(&state);
    }

    drop(&state);

    if (lib) {
        dlclose(lib);
    }

    CloseWindow();

    return 0;
}

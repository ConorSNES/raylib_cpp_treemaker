/*
Starting boilerplate for raylib released by Jeffery Myers under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
*/

#include <map>
#include <random>
#include <string>

#include "ctime"
#include "raylib.h"
//#include "resource_dir.h"  // utility header for SearchAndSetResourceDir
#include "treemaker-lib.h"

using namespace std;

constexpr static int WIDTH = 1280;
constexpr static int HEIGHT = 800;

// manage tree algos
typedef void (*treealgo)(Vector2, mt19937);
void radial(Vector2 v, mt19937 r) { treemaker::draw_branching_angular(v,r); }
constexpr char radial_name[] = "radial";
void rectangular(Vector2 v, mt19937 r) { treemaker::draw_branching_rectangular(v,r); }
constexpr char rectangular_name[] = "rectangular";
static map<string, treealgo> algos;

const string next_algo(const string current) {
    if (current == radial_name) return rectangular_name;
    else return radial_name;
}

int main() {
    // populate algos at runtime
    algos[radial_name] = &radial;
    algos[rectangular_name] = &rectangular;

    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);

    // Create the window and OpenGL context
    InitWindow(WIDTH, HEIGHT, "Treemaker");

    // init randomness
    mt19937 metarand(time(NULL));
    int seed = metarand();

    // set algo
    string selected_algo = rectangular_name;

    // Determine tree root on canvas
    constexpr Vector2 ROOT = {(float)WIDTH / 2, (float)HEIGHT};

    // game loop
    while (!WindowShouldClose())
    {
    // Program continues until "esc" or "close" pressed

	// drawing
	BeginDrawing();

	// Setup the back buffer for drawing (clear color and depth buffers)
	ClearBackground({0x3b, 0x00, 0x19, 0xff});

    // debug draw (unused)
	// draw_indicator(0, 0);
	// draw_indicator(0, HEIGHT);
	// draw_indicator(ROOT.x, ROOT.y);

	// Seed tree algo randomness and draw branching.
    algos[selected_algo](ROOT, mt19937(seed));

	DrawText("Treemaker", 20, 20, 20, GRAY);
    DrawText(TextFormat("Tree generator: %s", selected_algo.c_str()), 20, HEIGHT - 120, 20,
		 GRAY);
	DrawText(TextFormat("Seed: %i", seed), 20, HEIGHT - 80, 20,
		 GRAY);
	DrawText("Press R to randomise.", 20, HEIGHT - 40, 20, GRAY);

    // As the tree is drawn on each frame, all that is needed to vary the tree is to assign a new random basis.
	if (IsKeyPressed(KeyboardKey::KEY_R)) seed = metarand();

    // The next algo will be selected when pressing T.
    if (IsKeyPressed(KeyboardKey::KEY_T)) selected_algo = next_algo(selected_algo);

	// end the frame and get ready for the next one  (display frame, poll
	// input, etc...)
	EndDrawing();
    }

    // cleanup;
    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}

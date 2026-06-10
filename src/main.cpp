/*
Starting boilerplate for raylib released by Jeffery Myers under CC0 1.0 (https://creativecommons.org/publicdomain/zero/1.0/)
*/

#include <random>

#include "ctime"
#include "raylib.h"
//#include "resource_dir.h"  // utility header for SearchAndSetResourceDir
#include "treemaker-lib.h"

using namespace std;

constexpr static int WIDTH = 1280;
constexpr static int HEIGHT = 800;

int main() {
    // Tell the window to use vsync and work on high DPI displays
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);

    // Create the window and OpenGL context
    InitWindow(WIDTH, HEIGHT, "Treemaker");

    // init randomness
    mt19937 metarand(time(NULL));
    int seed = metarand();

    // Determine 
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
	treemaker::draw_branching(ROOT, mt19937(seed));

	DrawText("Treemaker", 20, 20, 20, GRAY);
	DrawText(TextFormat("Current seed is: %i", seed), 20, HEIGHT - 80, 20,
		 GRAY);
	DrawText("Press R to randomise.", 20, HEIGHT - 40, 20, GRAY);

    // As the tree is drawn on each frame, all that is needed to vary the tree is to assign a new random basis.
	if (IsKeyPressed(KeyboardKey::KEY_R)) seed = metarand();

	// end the frame and get ready for the next one  (display frame, poll
	// input, etc...)
	EndDrawing();
    }

    // cleanup;
    // destroy the window and cleanup the OpenGL context
    CloseWindow();
    return 0;
}

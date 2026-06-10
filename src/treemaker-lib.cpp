#include <cstdlib>

#include "raylib.h"
#include "raymath.h"
#include "treemaker-lib.h"

class treemaker::lazy_arena {
   private:
    static Vector2* mem;
    static constexpr int count = 4;

   public:
    static constexpr int const get_count() { return count; }

    static Vector2* const get_ptr() {
	if (lazy_arena::mem == nullptr)
	    lazy_arena::mem = (Vector2*)malloc(sizeof(Vector2) * get_count());
	return lazy_arena::mem;
    }

    static void assign_abcd(Vector2 a, Vector2 b, Vector2 c, Vector2 d) {
	// collect address
	Vector2* arena = get_ptr();
	// assert values
	arena[0] = a;
	arena[1] = b;
	arena[2] = c;
	arena[3] = d;
    }
};
Vector2* treemaker::lazy_arena::mem = nullptr;

// these consts would probably be best in a module, but whatever
constexpr Color COL_START = BLUE;
constexpr Color COL_END = RED;
constexpr float THICK_START = 8.0;
constexpr float THICK_END = 2.0;
constexpr float ANGLE_MAX = PI / 3;
constexpr float LENGTH_MAX = 200.0f;

void treemaker::draw_branching_angular(Vector2 root, float lastangle, int depth,
			    float decay, mt19937 rand) {
    float depthdecay = logdecay((float)depth);

    float angle = lastangle + Lerp(-ANGLE_MAX, ANGLE_MAX, floatrand(rand));
    float length = LENGTH_MAX * depthdecay;

    Vector2 difference = {length * cos(angle), length * sin(angle)};
    difference = {-difference.y, difference.x};
    Vector2 end = {root.x - difference.x, root.y - difference.y};

    float thick = Lerp(THICK_START, THICK_END, 1 - depthdecay);
    Color color = ColorLerp(COL_START, COL_END, 1 - depthdecay);

    // Draw line.
    DrawSplineSegmentLinear(root, end, thick, color);
    //  DrawText(TextFormat("d%i_a%.0f_l%.0f_t%.0f", depth, angle, length,
    //  thick), end.x, end.y, 8, WHITE);

    if (depth > 10) return;

    // Recursive draw.
    if (floatrand(rand) < decay)
	// Sibling branches.
	draw_branching_angular(root, lastangle, depth, decay * 0.8, (mt19937)rand());
    if (floatrand(rand) < decay)
	// Nested branches.
	draw_branching_angular(end, angle, depth + 1, decay * 0.8, (mt19937)rand());
    else
	// Terminating branches draw leaves on the point.
	draw_leaves(end, (mt19937)rand());
					}

void treemaker::draw_branching_rectangular(Vector2 root, int depth, float decay,
		    mt19937 rand) {
    float depthdecay = logdecay((float)depth);

    // collect noise
    Vector2 noise = {
        static_cast<float>(floatrand(rand)*2.0 - 1.0),
        static_cast<float>(floatrand(rand)*0.5 + 0.5)
    };

    float length = LENGTH_MAX * depthdecay;
    
    Vector2 difference = {length * noise.x, length * noise.y};
    Vector2 end = root - difference;

    float thick = Lerp(THICK_START, THICK_END, 1 - depthdecay);
    Color color = ColorLerp(COL_START, COL_END, 1 - depthdecay);

    // Setup control points.
    lazy_arena::assign_abcd(root, {root.x, end.y}, {end.x, root.y}, end);

    // Draw line.
    DrawSplineBezierCubic(lazy_arena::get_ptr(), lazy_arena::get_count(), thick,
			  color);

    if (depth > 10) return;

    // Recursive draw.
    if (floatrand(rand) < decay)
	// Sibling branches.
	draw_branching_rectangular(root, depth, decay * 0.8, (mt19937)rand());
    if (floatrand(rand) < decay)
	// Nested branches.
	draw_branching_rectangular(end, depth + 1, decay * 0.8, (mt19937)rand());
    else
	// Terminating branches draw leaves on the point.
	draw_leaves(end, (mt19937)rand());
}

void treemaker::draw_leaves(Vector2 root, mt19937 rand) {
    constexpr float BGCIRC_SIZE = 32;
    constexpr float SUBCIRC_SIZE = BGCIRC_SIZE / 2;
    constexpr Color BGCIRC_COL = PINK;
    constexpr Color SUBCIRC_COL = {0xff, 0xcc, 0xff, 0xcc};
    constexpr int SUBCIRC_COUNT = 8;

    // Draw 5 random subcircles.
    for (int i = 0; i < SUBCIRC_COUNT; i++)
	DrawCircleV(
	    Vector2Add(root,
		       {Lerp(-BGCIRC_SIZE, BGCIRC_SIZE, floatrand(rand)),
			Lerp(-BGCIRC_SIZE, BGCIRC_SIZE, floatrand(rand))}),
	    SUBCIRC_SIZE, SUBCIRC_COL);

    // Draw background circle.
    // DrawCircleV(root, BGCIRC_SIZE, BGCIRC_COL);

    // Draw 5 more random circles.
    for (int i = 0; i < SUBCIRC_COUNT; i++)
	DrawCircleV(
	    Vector2Add(root,
		       {Lerp(-BGCIRC_SIZE, BGCIRC_SIZE, floatrand(rand)),
			Lerp(-BGCIRC_SIZE, BGCIRC_SIZE, floatrand(rand))}),
	    SUBCIRC_SIZE, SUBCIRC_COL);
}

void draw_indicator(int posx, int posy) {
    constexpr int INDICATOR_RADIUS = 8;
    constexpr float INDICATOR_FRADIUS = (float)INDICATOR_RADIUS;

    DrawCircle(posx, posy, INDICATOR_FRADIUS, RED);
    DrawLine(posx + INDICATOR_RADIUS, posy, posx - INDICATOR_RADIUS, posy,
	     BLACK);
    DrawLine(posx, posy + INDICATOR_RADIUS, posx, posy - INDICATOR_RADIUS,
	     BLACK);
}
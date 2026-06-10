#pragma once

#include <cmath>
#include <random>

#include "raylib.h"

using namespace std;

class treemaker {
private:
    // simple decay func
    static inline float logdecay(float x) { return 1.0f - log10f(x); }

    // collect random float between 0 and 1
    static inline float floatrand(mt19937& rng) {
	uniform_real_distribution<float> dist(0.0, 1.0);
	return dist(rng);
    }

    // method of producing a set of 4 heap allocated vectors 
    // for use by raylib algorithms while minimizing memory footprint
    // (note: this probably triggers leak safeguards. fix?)
    class lazy_arena;

public:

    // basic draw func
    static inline void draw_branching(Vector2 root, mt19937 rand) {
        draw_branching_rectangular(root, rand);
    }

    // draw tree using angular algorithm
    static void draw_branching_angular(Vector2 root, float lastangle, int depth,
				       float decay, mt19937 rand);

    static inline void draw_branching_angular(Vector2 root, mt19937 rand) {
	draw_branching_angular(root, 0.0f, 1, 1.0f, rand);
    }

    static void draw_branching_rectangular(Vector2 root, int depth, float decay, mt19937 rand);

    static inline void draw_branching_rectangular(Vector2 root, mt19937 rand) {
        draw_branching_rectangular(root, 1, 1.0f, rand);
    }

    // draw branch leaves on point
    static void draw_leaves(Vector2 root, mt19937 rand);

    // helper func for drawing an 8px indicator on a point
    static void draw_indicator(int posx, int posy);
};

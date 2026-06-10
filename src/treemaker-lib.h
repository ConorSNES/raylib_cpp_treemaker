#pragma once

#include <cmath>
#include <random>

#include "raylib.h"

using namespace std;

// simple decay func
inline float logdecay(float x) { return 1.0f - log10f(x); }

// collect random float between 0 and 1
inline float floatrand(mt19937 &rng) {
    uniform_real_distribution<float> dist(0.0, 1.0);
    return dist(rng);
}

// draw recursive branching tree on root point
void draw_branching(Vector2 root, float lastangle, int depth, float decay,
		    mt19937 rand);
// convenience alias for draw_branching
inline void draw_branching(Vector2 root, mt19937 rand) {
    draw_branching(root, 0.0f, 1, 1.0f, rand);
}

// draw branch leaves on point
void draw_leaves(Vector2 root, mt19937 rand);

// helper func for drawing an 8px indicator to show where a particular point is
void draw_indicator(int posx, int posy);
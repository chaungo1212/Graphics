#ifndef Utilis_H
#define Utilis_H
#include <string>
#include <memory>
#include <iostream>
#include "Geometry.h"
#include "Image.h"
using namespace std;

void perVertexColoring(int width, int height, vector<Triangle> ts, float s, float tx, float ty, string outFile); // Mode 0

#endif

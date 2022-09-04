#pragma once

#include "raylib.h"
#include "raymath.h"
#include <vector>
#include <math.h>
#include <string>
#include "Delauney.h"

class FibonacciSphereGenerator
{
public:
    std::vector<Vector3> vertices;
    std::vector<int> triangles;
    float radius = 10;
    int colourIndex = 0;

    FibonacciSphereGenerator(){};

    void DrawFibonacciSphere();
    void DrawTriangulation();
    std::vector<Vector3> GenerateVertices(int resolution);
    void TriangulateVertices();
    void PatchHole();
    void ScaleVerticesAndTriangles();
    std::vector<Vector2> StereographicProjection(const std::vector<Vector3> &pts);
    std::vector<Vector3> InverseStereographicProjection(const std::vector<Vector2> &pts);
    Mesh GenerateMesh();
};
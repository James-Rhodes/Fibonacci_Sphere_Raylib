#include "Fibonacci_Sphere.h"

void FibonacciSphereGenerator::TriangulateVertices()
{
    // Removing the first point which blows out to infinity
    std::vector<Vector2> projectedPts = StereographicProjection(std::vector<Vector3>(vertices.begin() + 1, vertices.end()));
    std::vector<int> triangulation = DelauneyTriangulateIndices(projectedPts);
    // triangles = InverseStereographicProjection(triangulation);
    triangles = triangulation;
    // ScaleVerticesAndTriangles();
    PatchHole();
}

std::vector<Vector2> FibonacciSphereGenerator::StereographicProjection(const std::vector<Vector3> &pts)
{
    std::vector<Vector2> projectedPts(pts.size());
    int index = 0;
    for (Vector3 pt : pts)
    {
        float den = (1 - pt.z);

        float x = pt.x / den;
        float y = pt.y / den;

        projectedPts[index] = {x, y};
        index++;
    }

    return projectedPts;
}

std::vector<Vector3> FibonacciSphereGenerator::InverseStereographicProjection(const std::vector<Vector2> &pts)
{
    std::vector<Vector3> sphericalCoords(pts.size());
    int index = 0;
    for (Vector2 pt : pts)
    {
        float den = (1 + (pt.x * pt.x) + (pt.y * pt.y));

        float x = 2 * pt.x / den;
        float y = 2 * pt.y / den;
        float z = (-1 + (pt.x * pt.x) + (pt.y * pt.y)) / den;

        sphericalCoords[index] = {x, y, z};
        index++;
    }

    return sphericalCoords;
}

void FibonacciSphereGenerator::DrawFibonacciSphere()
{
    // remember to call generate vertices before trying to draw.
    for (Vector3 pt : vertices)
    {
        DrawSphere(pt, 0.1f, BLACK);
    }
}

void FibonacciSphereGenerator::DrawTriangulation()
{
    int total = (int)triangles.size();
    for (int i = 0; i < total; i += 3)
    {
        DrawLine3D(vertices[triangles[i]], vertices[triangles[i + 1]], BLACK);
        DrawLine3D(vertices[triangles[i + 1]], vertices[triangles[i + 2]], BLACK);
        DrawLine3D(vertices[triangles[i]], vertices[triangles[i + 2]], BLACK);
    }
}

std::vector<Vector3> FibonacciSphereGenerator::GenerateVertices(int numPts)
{
    float turnFraction = (1 + pow(5, 0.5f)) / 2.0f;
    std::vector<Vector3> pts(numPts);
    for (int i = 0; i < numPts; i++)
    {

        float t = i / (numPts - 1.0f);
        float inclination = acosf(1 - 2 * t);
        float azimuth = 2 * PI * i / turnFraction;

        float x = sinf(inclination) * cosf(azimuth);
        float y = sinf(inclination) * sinf(azimuth);
        float z = cosf(inclination);

        pts[i] = {x, y, z};
    }
    vertices = pts;
    TriangulateVertices();
    ScaleVerticesAndTriangles();
    return vertices;
}

void FibonacciSphereGenerator::PatchHole()
{
    std::array<int, 15> trianglePatchVertices = {0, 4, 1, 0, 2, 4, 0, 5, 2, 0, 3, 5, 0, 1, 3};

    for (int index : trianglePatchVertices)
    {
        // triangles.push_back(vertices[index]);
        triangles.push_back(index);
    }
}

void FibonacciSphereGenerator::ScaleVerticesAndTriangles()
{
    for (size_t i = 0; i < triangles.size(); i++)
    {
        if (i < vertices.size())
        {
            vertices[i].x *= radius;
            vertices[i].y *= radius;
            vertices[i].z *= radius;
        }
    }
}

Mesh FibonacciSphereGenerator::GenerateMesh()
{
    Mesh mesh = {0};
    mesh.triangleCount = triangles.size() / 3;
    mesh.vertexCount = vertices.size();
    mesh.vertices = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));  // 3 vertices, 3 coordinates each (x, y, z)
    mesh.texcoords = (float *)MemAlloc(mesh.vertexCount * 2 * sizeof(float)); // 3 vertices, 2 coordinates each (x, y)
    mesh.normals = (float *)MemAlloc(mesh.vertexCount * 3 * sizeof(float));   // 3 vertices, 3 coordinates each (x, y, z)
    mesh.indices = (unsigned short *)MemAlloc(3 * mesh.triangleCount * sizeof(unsigned short));

    int index = 0;
    int texCoordsIndex = 0;
    for (size_t i = 0; i < triangles.size(); i++)
    {
        if (i < vertices.size())
        {
            mesh.vertices[index + 0] = vertices[i].x;
            mesh.vertices[index + 1] = vertices[i].y;
            mesh.vertices[index + 2] = vertices[i].z;

            Vector3 normal = Vector3Normalize(vertices[i]);
            mesh.normals[index + 0] = normal.x;
            mesh.normals[index + 1] = normal.y;
            mesh.normals[index + 2] = normal.z;

            mesh.texcoords[texCoordsIndex + 0] = asinf(normal.x) / PI + 0.5;
            mesh.texcoords[texCoordsIndex + 1] = asinf(normal.y) / PI + 0.5;
            texCoordsIndex += 2;
            index += 3;
        }

        mesh.indices[i] = triangles[i];
    }
    UploadMesh(&mesh, true);

    return mesh;
}
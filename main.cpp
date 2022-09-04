#include "raylib.h"
#include "Fibonacci_Sphere.h"
#include <chrono>
#include "raymath.h"

#define MAXSPHERES 200

int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Fibonacci Sphere");

    Camera camera = {0};
    camera.position = (Vector3){0.0f, 0.0f, 50.0f};
    camera.target = (Vector3){0.0f, 0.0f, 0.0f};
    camera.up = (Vector3){0.0f, 1.0f, 0.0f};
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    SetCameraMode(camera, CAMERA_FREE);
    SetTargetFPS(60); // Set our game to run at 60 frames-per-second
    FibonacciSphereGenerator fibGen;
    int numSpheres = 1000;
    // auto start = std::chrono::high_resolution_clock::now();
    Image checked = GenImageChecked(2, 2, 1, 1, RED, GREEN);
    Texture2D texture = LoadTextureFromImage(checked);
    UnloadImage(checked);
    fibGen.vertices = fibGen.GenerateVertices(numSpheres);
    std::cout << "Number of vertices: " << fibGen.vertices.size() << " , Number of tris: " << fibGen.triangles.size() << std::endl;
    Model model = LoadModelFromMesh(fibGen.GenerateMesh());
    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
    // auto end = std::chrono::high_resolution_clock::now();
    // auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    // std::cout << ms_int.count() << std::endl;
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        UpdateCamera(&camera);

        //----------------------------------------------------------------------------------
        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(RAYWHITE);

        BeginMode3D(camera);
        // fibGen.DrawFibonacciSphere();
        // fibGen.DrawTriangulation();

        DrawModel(model, {0, 0, 0}, 0.99f, WHITE); // Draw checkered model slightly smaller to ensure no z fighting between wire frame and model
        DrawModelWires(model, {0, 0, 0}, 1.0f, BLACK);

        EndMode3D();
        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadModel(model);
    CloseWindow(); // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

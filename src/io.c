// read Wavefront object file (.obj) format
// and interpret into arrays of vertices and edges
#include "io.h"

#define MAX_VERTICES 10000
#define MAX_INDICES 30000

void read_model(char* filepath, vec3f** vertices, int** indices, int* num_vertices, int* num_indices)
{
    FILE* file = fopen(filepath, "r");
    if (!file) {
        perror("Failed to open model file");
        return;
    }

    *vertices = malloc(MAX_VERTICES * sizeof(vec3f));
    *indices = malloc(MAX_INDICES * sizeof(int));

    if (!*vertices || !*indices) {
        perror("Failed to allocate memory");
        fclose(file);
        return;
    }

    char line[256];
    int vertex_count = 0;
    int face_count = 0;
    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, "v ", 2) == 0) {
            // Vertex line
            sscanf(line + 2, "%f %f %f", &(*vertices)[vertex_count].x, &(*vertices)[vertex_count].y, &(*vertices)[vertex_count].z);
            vertex_count++;
        } else if (strncmp(line, "f ", 2) == 0) {
            int v[4];
            int count = sscanf(line + 2, "%d/%*d/%*d %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d",
                            &v[0], &v[1], &v[2], &v[3]);

            if (count >= 3) {
                (*indices)[face_count++] = v[0] - 1;
                (*indices)[face_count++] = v[1] - 1;
                (*indices)[face_count++] = v[2] - 1;
            }
            if (count == 4) {
                (*indices)[face_count++] = v[0] - 1;
                (*indices)[face_count++] = v[2] - 1;
                (*indices)[face_count++] = v[3] - 1;
            }
        }
    }

    *num_vertices = vertex_count;
    *num_indices = face_count; // Each face consists of 3 indices

    *vertices = realloc(*vertices, vertex_count * sizeof(vec3f));
    *indices = realloc(*indices, face_count * sizeof(int));

    fclose(file);
}
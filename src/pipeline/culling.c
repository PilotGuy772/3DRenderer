#include "culling.h"


/* ------------------------------------------------------------------ */
/*  Helper – interpolate full vertex along edge  a ---- t ---- b       */
static vertex lerp_vertex(const vertex *a, const vertex *b, float t)
{
    vertex r = {0};
    r.position.x = a->position.x + t * (b->position.x - a->position.x);
    r.position.y = a->position.y + t * (b->position.y - a->position.y);
    r.position.z = a->position.z + t * (b->position.z - a->position.z);
    r.position.w = a->position.w + t * (b->position.w - a->position.w);

    r.normal.x   = a->normal.x   + t * (b->normal.x   - a->normal.x);
    r.normal.y   = a->normal.y   + t * (b->normal.y   - a->normal.y);
    r.normal.z   = a->normal.z   + t * (b->normal.z   - a->normal.z);

    r.world_pos.x= a->world_pos.x+ t*(b->world_pos.x - a->world_pos.x);
    r.world_pos.y= a->world_pos.y+ t*(b->world_pos.y - a->world_pos.y);
    r.world_pos.z= a->world_pos.z+ t*(b->world_pos.z - a->world_pos.z);

    /* simple colour blend (8-bit channels) */
    uint8_t ar = (a->color >> 24) & 0xFF, ag = (a->color >> 16) & 0xFF,
            ab = (a->color >>  8) & 0xFF, aa =  a->color        & 0xFF;
    uint8_t br = (b->color >> 24) & 0xFF, bg = (b->color >> 16) & 0xFF,
            bb = (b->color >>  8) & 0xFF, ba =  b->color        & 0xFF;
    uint32_t cr = (uint32_t)(ar + t * (br - ar));
    uint32_t cg = (uint32_t)(ag + t * (bg - ag));
    uint32_t cb = (uint32_t)(ab + t * (bb - ab));
    uint32_t ca = (uint32_t)(aa + t * (ba - aa));
    r.color = (cr << 24) | (cg << 16) | (cb << 8) | ca;
    return r;
}

/* ------------------------------------------------------------------ */
/*  Inside-test and edge intersection (homogeneous clipping)          */
static int inside(const vertex *v, int axis, float sign)
{
    float c = (axis==0? v->position.x : axis==1? v->position.y : v->position.z) * sign;
    return c <= v->position.w;
}

static vertex intersect(const vertex *a, const vertex *b, int axis, float sign)
{
    float a_c = (axis==0? a->position.x : axis==1? a->position.y : a->position.z) * sign;
    float b_c = (axis==0? b->position.x : axis==1? b->position.y : b->position.z) * sign;

    float t = (a->position.w - a_c) / ((a->position.w - a_c) - (b->position.w - b_c));
    return lerp_vertex(a, b, t);
}

/* ------------------------------------------------------------------ */
typedef struct { vertex v[MAX_VERTS_PER_TRI]; int count; } poly_t;

static void clip_poly_plane(const poly_t *in, poly_t *out, int axis, float sign)
{
    out->count = 0;
    for (int i=0; i<in->count; ++i) {
        const vertex *curr = &in->v[i];
        const vertex *prev = &in->v[(i-1+in->count)%in->count];

        int curr_in = inside(curr, axis, sign);
        int prev_in = inside(prev, axis, sign);

        if (curr_in && prev_in) { out->v[out->count++] = *curr; }
        else if (!prev_in && curr_in) {
            out->v[out->count++] = intersect(prev, curr, axis, sign);
            out->v[out->count++] = *curr;
        }
        else if (prev_in && !curr_in) {
            out->v[out->count++] = intersect(prev, curr, axis, sign);
        }
    }
}

/* ------------------------------------------------------------------ */
static void clip_triangle_full(const vertex *t0, const vertex *t1, const vertex *t2,
                               poly_t *poly_out)
{
    poly_t bufA = { .v = { *t0, *t1, *t2 }, .count = 3 };
    poly_t bufB = { .count = 0 };
    poly_t *in = &bufA, *out = &bufB, *tmp;

    for (int axis=0; axis<3; ++axis) {
        clip_poly_plane(in, out, axis, +1.f);
        if (out->count==0){ poly_out->count = 0; return; }
        tmp=in; in=out; out=tmp;

        clip_poly_plane(in, out, axis, -1.f);
        if (out->count==0){ poly_out->count = 0; return; }
        tmp=in; in=out; out=tmp;
    }
    *poly_out = *in;          /* copy result */
}

/* ------------------------------------------------------------------ */
/*  Fan-triangulate the polygon into faces                            */
static void emit_tris_from_poly(const poly_t *poly, uint32_t face_color,
                                face *out_faces, int *face_count,
                                int base_index)
{
    for (int i=1; i<poly->count-1; ++i) {
        face f;
        f.indices[0] = base_index;
        f.indices[1] = base_index + i;
        f.indices[2] = base_index + i + 1;
        f.color      = face_color;
        /* flat normal (optional): */
        vec3f a = (vec3f){poly->v[i].position.x, poly->v[i].position.y, poly->v[i].position.z};
        vec3f b = (vec3f){poly->v[0].position.x, poly->v[0].position.y, poly->v[0].position.z};
        vec3f c = (vec3f){poly->v[i+1].position.x, poly->v[i+1].position.y, poly->v[i+1].position.z};
        f.normal = vec3_normalize(vec3_cross(vec3_sub(b, a), vec3_sub(c, a)));
        f.material = 0;
        out_faces[(*face_count)++] = f;
    }
}

/* ================================================================== */
/*  PUBLIC ENTRY                                                      */
void culling_cull_triangle(vertex *vertices, int num_vertices,
                           face *faces,     int num_faces,
                           vertex **out_vertices, int *out_num_vertices,
                           face   **out_faces,    int *out_num_faces)
{
    /* start with generous allocations */
    int v_alloc = num_vertices * 4;
    int f_alloc = num_faces    * 4;
    *out_vertices    = (vertex*)malloc(v_alloc * sizeof(vertex));
    *out_faces       = (face  *)malloc(f_alloc * sizeof(face));
    *out_num_vertices= 0;
    *out_num_faces   = 0;

    if (!*out_vertices || !*out_faces) { fprintf(stderr,"alloc fail\n"); exit(1); }

    for (int fi=0; fi<num_faces; ++fi)
    {
        const face *src = &faces[fi];
        const vertex *v0 = &vertices[src->indices[0]];
        const vertex *v1 = &vertices[src->indices[1]];
        const vertex *v2 = &vertices[src->indices[2]];

        poly_t poly;  clip_triangle_full(v0,v1,v2,&poly);
        if (poly.count==0) continue;

        /* ensure space for new vertices */
        if (*out_num_vertices + poly.count > v_alloc) {
            v_alloc *= 2;
            *out_vertices = (vertex*)realloc(*out_vertices, v_alloc*sizeof(vertex));
        }
        /* copy vertices */
        int base = *out_num_vertices;
        memcpy(&(*out_vertices)[base], poly.v, poly.count*sizeof(vertex));
        *out_num_vertices += poly.count;

        /* ensure space for faces (fan produces n-2 triangles) */
        int add_faces = poly.count - 2;
        if (*out_num_faces + add_faces > f_alloc) {
            f_alloc = f_alloc*2 + add_faces;
            *out_faces = (face*)realloc(*out_faces, f_alloc*sizeof(face));
        }
        emit_tris_from_poly(&poly, src->color, *out_faces, out_num_faces, base);
    }

    /* trim to actual size (optional) */
    *out_vertices = (vertex*)realloc(*out_vertices, (*out_num_vertices)*sizeof(vertex));
    *out_faces    = (face  *)realloc(*out_faces,    (*out_num_faces)   *sizeof(face));
}

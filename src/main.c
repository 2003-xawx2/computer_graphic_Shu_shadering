#include "array.h"
#include "display.h"
#include "vector.h"
#include "triangle.h"
#include "mesh.h"
#include "matrix.h"
#include "light.h"
#include "texture.h"

bool is_running = false;
SDL_Event event;

direct_light_t direct_light = {.direction = {.x = 1, .y = 1, .z = 1}};
vec3_t camera_position = {.x = 0, .y = 0, .z = -5};
mat44_t project_mat;

float fov = M_PI / 2;
float camera_near_plane = .1;
float camera_far_plane = 100;

int previous_frame_time = 0;

triangle_t *triangles_to_render;

void set_up()
{
    set_up_color_buffer();
    project_mat = mat4_make_perspective(fov, (float)window_height / (float)window_width, camera_near_plane, camera_far_plane);
    mesh_texture = (uint32_t *)REDBRICK_TEXTURE;

    load_mesh_from_obj(&mesh, "assets/f22.obj");
}

void process_input()
{
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT: // quit button on the window
        is_running = false;
        break;
    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            is_running = false;
        break;
    }
}

void delay_until_fixed_frame_time()
{
    // lock the update until the delta time reach the target frame time
    int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
    if (time_to_wait > 0 && time_to_wait < FRAME_TARGET_TIME)
        SDL_Delay(time_to_wait);          // use while loop will occupy all the cpu
    previous_frame_time = SDL_GetTicks(); // return millisecond since SDL-init()
}

void update()
{
    delay_until_fixed_frame_time();

    triangles_to_render = NULL;

    mesh.tran.rotation.x += 0.01;
    mesh.tran.rotation.y += 0.001;
    mesh.tran.rotation.z += 0.005;

    // mesh.tran.scale.x += 0.001;

    // mesh.tran.position.x += 0.01;
    // mesh.tran.position.y += 0.01;

    mat44_t trans_mat = get_transform_matrix(mesh.tran);

    int mesh_faces_num = array_length(mesh.faces);
    // traverse all the cube faces
    for (int i = 0; i < mesh_faces_num; i++)
    {
        face_t face = mesh.faces[i];

        // convert the vertex index to the vertex poistion
        vec3_t face_vertices[3];
        face_vertices[0] = mesh.vertices[face.a - 1];
        face_vertices[1] = mesh.vertices[face.b - 1];
        face_vertices[2] = mesh.vertices[face.c - 1];

        vec3_t transformed_vertices[3];

        // apply the transform and make the camera at the origin
        for (int j = 0; j < 3; j++)
        {
            vec3_t transformed_vertex = face_vertices[j];

            transformed_vertex = vec3(mat4_mul_vec4(vec4(transformed_vertex), trans_mat));

            transformed_vertex = vec3_sub(transformed_vertex, camera_position);
            transformed_vertices[j] = transformed_vertex;
        }

        triangle_t projected_triangle = {
            .normal = vec3_cross(vec3_sub(transformed_vertices[0], transformed_vertices[1]),
                                 vec3_sub(transformed_vertices[0], transformed_vertices[2])),
            .avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3.0,
            .tex_coords = {face.a_uv, face.b_uv, face.c_uv},
        };

        // cull the back faces
        if (vec3_dot(projected_triangle.normal, transformed_vertices[0]) > 0)
            continue;

        // project to the screen
        for (int j = 0; j < 3; j++)
        {
            vec4_t vec4_projected_point = mat4_mul_vec4_project(project_mat, vec4(transformed_vertices[j]));
            vec2_t projected_point = {.x = vec4_projected_point.x, .y = vec4_projected_point.y};
            // scale and translate the projected points to the middle of the screen
            projected_point.x *= window_width / 2.0;
            projected_point.y *= -window_height / 2.0; // the y in color buffer is inverted conpared to the y in obj file
            projected_point.x += window_width / 2.0;
            projected_point.y += window_height / 2.0;

            projected_triangle.points[j] = projected_point;
        }

        array_push(triangles_to_render, projected_triangle);
    }

    // Sort the triangles to render by their avg_depth
    int num_triangles = array_length(triangles_to_render);
    for (int i = 0; i < num_triangles; i++)
    {
        for (int j = i; j < num_triangles; j++)
        {
            if (triangles_to_render[i].avg_depth < triangles_to_render[j].avg_depth)
            {
                // Swap the triangles positions in the array
                triangle_t temp = triangles_to_render[i];
                triangles_to_render[i] = triangles_to_render[j];
                triangles_to_render[j] = temp;
            }
        }
    }
}

void render()
{
    clear_color_buffer(0x000000FF);
    draw_grid(20, 20, 0xEEDFEEFF);
    int triangles_num = array_length(triangles_to_render);
    for (int i = 0; i < triangles_num; i++)
    {
        triangle_t triangle = triangles_to_render[i];
        vec2_t point_a = triangle.points[0];
        vec2_t point_b = triangle.points[1];
        vec2_t point_c = triangle.points[2];
        tex2_t uv_a = triangle.tex_coords[0];
        tex2_t uv_b = triangle.tex_coords[1];
        tex2_t uv_c = triangle.tex_coords[2];

        draw_filled_triangle(point_a.x, point_a.y,
                             point_b.x, point_b.y,
                             point_c.x, point_c.y,
                             shade_direct_light_color(0xFFFFFFFF, triangle.normal, direct_light));
        // draw_textured_triangle(point_a.x, point_a.y, uv_a.u, uv_a.v,
        //                        point_b.x, point_b.y, uv_b.u, uv_b.v,
        //                        point_c.x, point_c.y, uv_c.u, uv_c.v, mesh_texture);
        //     draw_line(point_a.x, point_a.y, point_b.x, point_b.y, 0x00FFFFFF);
        //     draw_line(point_c.x, point_c.y, point_b.x, point_b.y, 0x00FFFFFF);
        //     draw_line(point_a.x, point_a.y, point_c.x, point_c.y, 0x00FFFFFF);
    }
    array_free(triangles_to_render);

    render_color_buffer();
    SDL_RenderPresent(renderer);
}

void free_resources()
{
    array_free(mesh.vertices);
    array_free(mesh.faces);
}

int main(void)
{
    is_running = initialize_window();
    set_up();

    while (is_running)
    {
        process_input();
        update();
        render();
    }
    destory_window();
    free_resources();

    return 0;
}
#include "Bone_Animation.h"
#include <glm/gtx/euler_angles.hpp>

using namespace glm;

Bone_Animation::Bone_Animation()
{
}

Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
    root_position = { 2.0f, 1.0f, 2.0f };

    scale_vector =
    {
        {1.0f, 1.0f, 1.0f},
        {0.5f, 4.0f, 0.5f},
        {0.5f, 3.0f, 0.5f},
        {0.5f, 2.0f, 0.5f}
    };

    rotation_degree_vector =
    {
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 0.0f}
    };

    colors =
    {
        {0.7f, 0.0f, 0.0f, 1.0f},
        {0.7f, 0.7f, 0.0f, 1.0f},
        {0.7f, 0.0f, 0.7f, 1.0f},
        {0.0f, 0.7f, 0.7f, 1.0f}
    };
}

void Bone_Animation::update(float delta_time)
{
    bone_height = (int)scale_vector.size();

    for (int i = 0; i < bone_height; i++)
    {
        float x_rotation_angle = radians(rotation_degree_vector[i][2]);
        float y_rotation_angle = radians(rotation_degree_vector[i][0]);
        float z_rotation_angle = radians(rotation_degree_vector[i][1]);

        mat4 x_rotation_matrix = eulerAngleX(x_rotation_angle);
        mat4 y_rotation_matrix = eulerAngleY(y_rotation_angle);
        mat4 z_rotation_matrix = eulerAngleZ(z_rotation_angle);

        combined_rotation_matrix[i] = x_rotation_matrix * y_rotation_matrix * z_rotation_matrix;

        if (i == 0)
        {
            relative_translation_matrix[i] = translate(mat4(1.0f), { root_position[0], (root_position[1] - scale_vector[i][1] / 2.0f), root_position[2] });
            world_transformation[i] = relative_translation_matrix[i] * combined_rotation_matrix[i];
            bone_matrix[i] = translate(mat4(1.0f), root_position);
        }
        else
        {
            int index = i - 1;
            local_translation_matrix[i] = translate(mat4(1.0f), { 0, scale_vector[i][1] / 2.0f, 0 });
            relative_translation_matrix[i] = translate(mat4(1.0f), { 0, scale_vector[index][1], 0 });
            world_transformation[i] = world_transformation[index] * relative_translation_matrix[i] * combined_rotation_matrix[i];
            bone_matrix[i] = world_transformation[i] * local_translation_matrix[i];
        }
    }
}

void Bone_Animation::reset()
{
    for (int i = 0; i < bone_height; i++)
    {
        rotation_degree_vector[i] = { 0.0f, 0.0f, 0.0f };
        bone_matrix[i] = mat4(1.0f);
    }
}

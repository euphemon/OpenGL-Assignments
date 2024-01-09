#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

class Bone_Animation
{
public:
    Bone_Animation();
    ~Bone_Animation();

    void init();
    void update(float delta_time);

    bool movement_on;
    int bone_height;

    vector<vec3> scale_vector;
    vector<vec3> rotation_degree_vector;
    vector<vec4> colors;
    vector<mat4> bone_matrix = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };

    vec3 root_position;
    vec3 end_effector;
    vec3 target_position;
    vec4 target_colors;
 
    void rotation_reset();
    void target_position_reset();

private:
    int num_of_joints;

    vector<vec3> x_rotation_angle;
    vector<vec3> y_rotation_angle;
    vector<vec3> z_rotation_angle;
    vector<vec3> pivot_points;

    vector<mat4> rotated = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
    vector<mat4> origin_translate = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
    vector<mat4> origin_link = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
    vector<mat4> original_end = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
    vector<mat4> world_transformation = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };

    void rotation_update();
    void jacobian_calculation();
};

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Bone_Animation
{
public:
	Bone_Animation();
	~Bone_Animation();

	void init();
	void update(float delta_time);
	void reset();

	vector<vec3> scale_vector;
	vector<vec3> rotation_degree_vector;
	vector<vec4> colors;

	vec3 root_position;

	int bone_height;

	vector<mat4> bone_matrix = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> combined_rotation_matrix = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> local_translation_matrix = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> relative_translation_matrix = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
	vector<mat4> world_transformation = { mat4(1.0f), mat4(1.0f), mat4(1.0f), mat4(1.0f) };
};

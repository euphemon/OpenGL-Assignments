#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

class Curve {
public:
	Curve();
	~Curve();

	void init();
	void calculate_curve();

	float tau = 0.5; // Coefficient for catmull-rom spline
	int num_points_per_segment = 200;
	int count;
	bool box_clicked = true;

	//matrix used for computing points with Catmull-Rom 
	const float CatmullMatrix[16] = {
		-0.5f, 1.5f, -1.5f, 0.5f,
		1.0f, -2.5f, 2.0f, -0.5f,
		-0.5f, 0.0f, 0.5f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f
	};

	vector<vec3> control_points_pos;
	vector<vec3> curve_points_pos;
	vector<glm::quat> control_points_quaternion;

	vector<vec3> points(
		const vec3& P0,
		const vec3& P1,
		const vec3& P2,
		const vec3& P3);
	glm::mat4 rotate_with_quaternion(glm::mat4 rotating, int index);
	glm::mat4 quaternion_transformation_matrix(glm::quat quaterion, glm::vec3 position);
};

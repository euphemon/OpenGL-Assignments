#pragma once

#include <vector>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Curve.h"

class Aircraft_Animation
{

public:
	float total_moving_time = 10;
	float t1 = 0.3;
	float t2 = 0.7;
	float time_elapsed;
	float distance_traveled;
	float total_distance = 0;
	bool is_active, is_completed;

private:
	glm::mat4 m_model_mat;
	Curve* m_animation_curve = nullptr;

	std::vector<glm::vec4> time_to_distance;
	std::vector<glm::vec4>::iterator distance_table_iterator;
	glm::vec3 vector;
	glm::vec3 current_point;
	glm::vec3 next_point;


public:
	Aircraft_Animation();
	~Aircraft_Animation();

	float length_at_time(float elapsed_time);

	void init();
	void init(Curve* animation_curve);

	void update(float delta_time);

	void clear_move_data();
	void calculate_arc_length();

	glm::vec3 get_pos_at_distance(float distance);
	glm::vec3 interpolate_position(float distance, glm::vec4 s, glm::vec4 e);

	void reset();
	glm::mat4 get_model_mat() { return m_model_mat; };
};


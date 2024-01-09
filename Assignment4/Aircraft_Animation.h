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
	int segment_point_count;
	float total_moving_time = 10;
	float t1 = 0.3;
	float t2 = 0.7;
	float time_elapsed;
	float distance_traveled;
	float distance_total = 0;
	float length_at_time(float time_updated);
	bool is_active, is_completed;

private:
	glm::mat4 m_model_mat;
	glm::mat4 quaternion_matrix;
	glm::vec3 position;
	glm::quat quaternion;
	Curve* m_animation_curve = nullptr;

	//aamples of positions and quaternions along the path
	std::vector<glm::vec4> position_samples;
	std::vector<glm::vec3> quaternion_samples;
	//iterators for current position and quaternion samples
	std::vector<glm::vec4>::iterator current_position_sample;
	std::vector<glm::vec3>::iterator current_quaternion_sample;
	
	glm::vec3 find_next_position(float distance);
	glm::vec3 interpolate_position(float distance, glm::vec4 start, glm::vec4 end);
	glm::quat find_next_orientation(float distance);
	glm::quat interpolate_quaternions(float delta_length, float segment_length, glm::quat start, glm::quat end);

	//calculate the position of points
	void generate_position_samples();
	//calculate the orientation of points 
	void generate_orientation_samples();

public:
	Aircraft_Animation();
	~Aircraft_Animation();

	void init();
	void init(Curve* animation_curve);
	void update(float delta_time);
	void reset();

	glm::mat4 get_model_mat() { return m_model_mat; };
};





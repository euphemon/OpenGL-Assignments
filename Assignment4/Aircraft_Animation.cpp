#include <glm/gtx/quaternion.hpp>
#include "glm/ext.hpp"
#include "Aircraft_Animation.h"


Aircraft_Animation::Aircraft_Animation()
{
	this->m_model_mat = glm::mat4(1.0f);
}

Aircraft_Animation::~Aircraft_Animation()
{
}

void Aircraft_Animation::init()
{
	reset();
}

void Aircraft_Animation::init(Curve* animation_curve)
{
	m_animation_curve = animation_curve;
	segment_point_count = m_animation_curve->num_points_per_segment;
	time_elapsed = 0;
	reset();
}

void Aircraft_Animation::update(float delta_time)
{
	if (m_animation_curve->box_clicked && !m_animation_curve->curve_points_pos.empty() && position_samples.empty() && quaternion_samples.empty()) {
		generate_position_samples();
		generate_orientation_samples();
		current_position_sample = position_samples.begin();
		current_quaternion_sample = quaternion_samples.begin();
		distance_total = position_samples.back()[3];
	}
	else if (is_active && !position_samples.empty() && !quaternion_samples.empty()) {
		time_elapsed += delta_time;
		float time_normalized = time_elapsed / total_moving_time;
		float distance_normalized = length_at_time(time_normalized);
		distance_traveled = distance_total * distance_normalized;

		if (distance_traveled == 0.0 || distance_traveled >= distance_total) {
			position = m_animation_curve->control_points_pos.front();
			quaternion = m_animation_curve->control_points_quaternion.front();
		}
		else {
			position = find_next_position(distance_traveled);
			quaternion = find_next_orientation(distance_traveled);
		}

		quaternion_matrix = m_animation_curve->quaternion_transformation_matrix(quaternion, position);
		m_model_mat = quaternion_matrix * glm::mat4(1.0f);
	}
	else if (!is_active && distance_traveled >= distance_total) {
		reset();
	}
}

void Aircraft_Animation::reset()
{
	is_active = false;
	is_completed = true;
	time_elapsed = 0.0;
	distance_traveled = 0.0;
	current_position_sample = position_samples.begin();
	current_quaternion_sample = quaternion_samples.begin();
	position = m_animation_curve->control_points_pos[0];
	quaternion = m_animation_curve->control_points_quaternion[0];
	quaternion_matrix = m_animation_curve->quaternion_transformation_matrix(quaternion, position);
	m_model_mat = quaternion_matrix * glm::mat4(1.0f);
}

float Aircraft_Animation::length_at_time(float elapsed_time)
{
	float speed, traveled;
	float v = 2 / (1 - t1 + t2);

	if (elapsed_time > 0 && elapsed_time <= t1) {
		speed = v * (elapsed_time / t1);
		traveled = (v * elapsed_time * elapsed_time) / (2 * t1);
	}
	else if (elapsed_time > t1 && elapsed_time <= t2) {
		speed = v;
		traveled = (v * t1) / 2 + v * (elapsed_time - t1);
	}
	else if (elapsed_time > t2 && elapsed_time <= 1) {
		speed = v * (1 - (elapsed_time - t2) / (1 - t2));
		traveled = 1 - speed * (1 - elapsed_time) / 2;
	}
	else if (elapsed_time > 1) {
		speed = 0;
		traveled = 1;
	}
	else {
		speed = 0;
		traveled = 0;
	}

	return traveled;

}

glm::vec3 Aircraft_Animation::find_next_position(float distance)
{
	glm::vec3 position;

	vector<glm::vec4>::iterator foundSample = position_samples.end() - 1;

	for (vector<glm::vec4>::iterator it = position_samples.begin(); it != position_samples.end(); ++it) {
		if (distance == (*it)[3]) {
			foundSample = it;
			break;
		}
		if (distance < (*it)[3]) {
			foundSample = it == position_samples.begin() ? it : it - 1;
			break;
		}
	}

	if (foundSample != position_samples.end()) {
		position = { (*foundSample)[0], (*foundSample)[1], (*foundSample)[2] };
	}

	return position;
}

glm::vec3 Aircraft_Animation::interpolate_position(float distance, glm::vec4 start, glm::vec4 end)
{
	glm::vec3 interpolated_point;

	if (start[3] == end[3]) {
		interpolated_point = { start[0], start[1], start[2] };
	}
	else {
		float ratio = (distance - start[3]) / (end[3] - start[3]);
		interpolated_point = {
			start[0] + ratio * (end[0] - start[0]),
			start[1] + ratio * (end[1] - start[1]),
			start[2] + ratio * (end[2] - start[2])
		};
	}

	return interpolated_point;
}

void Aircraft_Animation::generate_position_samples()
{
	float total_length = 0.0;
	vector<glm::vec3> curve_points = m_animation_curve->curve_points_pos;

	for (size_t i = 0; i < curve_points.size(); i++) {
		if (i > 0) {
			float segment_length = glm::distance(curve_points[i], curve_points[i - 1]);
			total_length += segment_length;
		}
		position_samples.push_back({ curve_points[i], total_length });
	}
}

glm::quat Aircraft_Animation::find_next_orientation(float distance)
{
	int start_index = (*current_quaternion_sample)[0];
	int end_index;
	float segment_distance;
	float delta_distance;

	glm::quat orientation;

	if (distance > (*current_quaternion_sample)[1])
	{
		current_quaternion_sample = current_quaternion_sample + 1;
		return find_next_orientation(distance);
	}

	end_index = (start_index == segment_point_count - 1) ? 0 : start_index + 1;

	segment_distance = quaternion_samples[end_index * segment_point_count][1] - quaternion_samples[start_index * segment_point_count][1];
	delta_distance = distance - quaternion_samples[start_index * segment_point_count][1];

	orientation = interpolate_quaternions(
		delta_distance,
		segment_distance,
		m_animation_curve->control_points_quaternion[start_index],
		m_animation_curve->control_points_quaternion[end_index]);

	return orientation;
}

glm::quat Aircraft_Animation::interpolate_quaternions(float delta_length, float segment_length, glm::quat start, glm::quat end)
{
	glm::quat interpolate_quaternion;

	if (segment_length > 0.0f) {
		float ratio = delta_length / segment_length;
		interpolate_quaternion = glm::normalize(glm::mix(start, end, ratio));
	}
	else {
		//when segment_length is zero, avoid division by zero
		interpolate_quaternion = start;
	}

	return interpolate_quaternion;
}

void Aircraft_Animation::generate_orientation_samples()
{
	float total_length = 0.0f;
	int current_segment = 0;
	int current_u_index = 0;
	int point_index = 0;

	vector<glm::vec3> curve_points = m_animation_curve->curve_points_pos;
	float segment_length = 0.0f;

	for (vector<glm::vec3>::iterator it = curve_points.begin(); it != curve_points.end(); ++it, ++point_index) {
		if (it != curve_points.begin()) {
			segment_length = glm::distance(*it, *(it - 1));
			total_length += segment_length;
		}

		current_segment = point_index / segment_point_count;
		current_u_index = current_segment * segment_point_count + 1;

		quaternion_samples.push_back({ current_segment, total_length, point_index });
	}
}



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
	time_elapsed = 0;
	current_point = glm::vec3(0.0, 8.5, -2.0);
	next_point = glm::vec3(0.0, 8.5, -2.0);
	reset();
}

void Aircraft_Animation::update(float delta_time)
{
	if (m_animation_curve->box_clicked == true && m_animation_curve->curve_points_pos.empty() == false && time_to_distance.empty() == true) {
		calculate_arc_length();
		total_distance = time_to_distance.back()[3];
		distance_table_iterator = time_to_distance.begin();
	}

	else if (is_active == true && time_to_distance.empty() == false) {

		time_elapsed += delta_time;
		float time = time_elapsed / total_moving_time;

		float distance = length_at_time(time);
		distance_traveled = total_distance * distance;


		if (distance_traveled == 0.0) {
			current_point = glm::vec3(0.0, 8.5, -2.0);
			next_point = glm::vec3(0.0, 8.5, -2.0);

		}
		else {
			current_point = next_point;
			next_point = get_pos_at_distance(distance_traveled);
		}

		vector = next_point - current_point;
		m_model_mat = glm::translate(m_model_mat, vector);
	}

	else if (is_active == false && distance_traveled >= total_distance) {
		clear_move_data();
	}

}

void Aircraft_Animation::reset()
{
	m_model_mat = glm::mat4(1.0f);

	if (m_animation_curve != nullptr && m_animation_curve->control_points_pos.size() > 0)
	{
		m_model_mat = glm::translate(m_model_mat, m_animation_curve->control_points_pos[0]);
	}
	clear_move_data();
}

void Aircraft_Animation::clear_move_data()
{
	is_active = false;
	is_completed = true;
	time_elapsed = 0.0;
	distance_traveled = 0.0;
	distance_table_iterator = time_to_distance.begin();
	current_point = glm::vec3(0.0, 8.5, -2.0);
	next_point = glm::vec3(0.0, 8.5, -2.0);
}

void Aircraft_Animation::calculate_arc_length()
{
	float added_length = 0;
	std::vector<glm::vec3> points = m_animation_curve->curve_points_pos;
	std::vector<vec3>::iterator curve_points_iterator;

	for (curve_points_iterator = points.begin(); curve_points_iterator != points.end(); curve_points_iterator++) {

		if (curve_points_iterator == points.begin()) {
			added_length = 0;
		}
		else {
			float segment_length = sqrt(pow((*(curve_points_iterator))[0] - (*(curve_points_iterator - 1))[0], 2) +
				pow((*(curve_points_iterator))[1] - (*(curve_points_iterator - 1))[1], 2) +
				pow((*(curve_points_iterator))[2] - (*(curve_points_iterator - 1))[2], 2) * 1.0);
			added_length = added_length + segment_length;
			time_to_distance.push_back({ *curve_points_iterator, added_length });

			//showing the table values
			cout << "distance table values: " << (*curve_points_iterator)[0] << ", " << (*curve_points_iterator)[1] << ", " << (*curve_points_iterator)[2] << ", " << added_length << endl;
		}
	}
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

glm::vec3 Aircraft_Animation::get_pos_at_distance(float distance)
{
	glm::vec3 position;
	std::vector<glm::vec4>::iterator it = distance_table_iterator;

	while (it != time_to_distance.end() && distance > (*it)[3])
	{
		it++;
	}

	if (it == time_to_distance.begin())
	{
		position = interpolate_position(distance, glm::vec4(0.0, 8.5, -2.0, 0.0), *it);
	}
	else
	{
		std::vector<glm::vec4>::iterator prev = it - 1;
		position = interpolate_position(distance, *prev, *it);
	}

	distance_table_iterator = it;
	return position;
}


glm::vec3 Aircraft_Animation::interpolate_position(float distance, glm::vec4 s, glm::vec4 e)
{
	float length_total = e[3] - s[3];
	float length_delta = distance - s[3];
	float ratio = length_delta / length_total;

	glm::vec3 interpolated_point = {
		s[0] + ratio * (e[0] - s[0]),
		s[1] + ratio * (e[1] - s[1]),
		s[2] + ratio * (e[2] - s[2])
	};

	return interpolated_point;
}



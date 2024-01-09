#include "Bone_Animation.h"
#include <glm/gtx/euler_angles.hpp>


Bone_Animation::Bone_Animation()
{
}


Bone_Animation::~Bone_Animation()
{
}

void Bone_Animation::init()
{
	root_position = { 2.0f,0.5f,2.0f };
	target_position = { 3.0f,8.0f,3.0f };
	target_colors = { 0.0f, 1.0f, 0.0f, 1.0f };

	scale_vector =
	{
		{1.0f,1.0f,1.0f},
		{0.5f,4.0f,0.5f},
		{0.5f,3.0f,0.5f},
		{0.5f,2.0f,0.5f}
	};

	rotation_degree_vector =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f}
	};

	colors =
	{
		{0.7f,0.0f,0.0f,1.0f},
		{0.7f,0.7f,0.0f,1.0f},
		{0.7f,0.0f,0.7f,1.0f},
		{0.0f,0.7f,0.7f,1.0f}
	};

	movement_on = false;
	bone_matrix = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };
	bone_height = (int)scale_vector.size();
	num_of_joints = bone_height - 1;

	rotation_update();
}

void Bone_Animation::update(float delta_time)
{
	x_rotation_angle.clear();
	y_rotation_angle.clear();
	z_rotation_angle.clear();
	pivot_points.clear();

	rotation_update();

	if (movement_on)
	{
		jacobian_calculation();
	}
}

void Bone_Animation::target_position_reset()
{
	movement_on = false;
	target_position = { 3.0f,8.0f,3.0f };
}

void Bone_Animation::rotation_reset()
{
	movement_on = false;
	bone_matrix = { glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f), glm::mat4(1.0f) };

	rotation_degree_vector =
	{
		{0.0f,0.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f},
		{0.0f,30.0f,0.0f}
	};
}

void Bone_Animation::rotation_update()
{
	for (int i = 0; i < bone_height; i++)
	{
		int j = i - 1;

		mat4 euler_rotation = mat4(1.0f);
		float angleX = glm::radians(rotation_degree_vector[i][2]);
		float angleY = glm::radians(rotation_degree_vector[i][0]);
		float angleZ = glm::radians(rotation_degree_vector[i][1]);

		euler_rotation = glm::rotate(euler_rotation, angleX, vec3(1.0f, 0.0f, 0.0f));
		euler_rotation = glm::rotate(euler_rotation, angleY, vec3(0.0f, 1.0f, 0.0f));
		euler_rotation = glm::rotate(euler_rotation, angleZ, vec3(0.0f, 0.0f, 1.0f));

		rotated[i] = euler_rotation;

		if (i != 0)
		{
			origin_translate[i] = glm::translate(mat4(1.0f), vec3(0, scale_vector[i][1] / 2.0f, 0));
			origin_link[i] = glm::translate(mat4(1.0f), vec3(0, scale_vector[i - 1][1], 0));
			original_end[i] = glm::translate(mat4(1.0f), vec3(0, scale_vector[i][1], 0));

			world_transformation[i] = world_transformation[i - 1] * origin_link[i] * rotated[i];
			bone_matrix[i] = world_transformation[i] * origin_translate[i];

			pivot_points.push_back(vec3(world_transformation[j] * original_end[j] * vec4(0.0f, 0.0f, 0.0f, 1.0f)));
			x_rotation_angle.push_back(normalize(vec3(world_transformation[j] * vec4(1.0f, 0.0f, 0.0f, 0.0f))));
			y_rotation_angle.push_back(normalize(vec3(world_transformation[j] * rotated[i] * vec4(0.0f, 1.0f, 0.0f, 0.0f))));
			z_rotation_angle.push_back(normalize(vec3(world_transformation[j] * rotated[i] * vec4(0.0f, 0.0f, 1.0f, 0.0f))));
		}
		else
		{
			origin_translate[i] = glm::translate(mat4(1.0f), vec3(0, scale_vector[i][1] / 2.0f, 0));
			origin_link[i] = glm::translate(mat4(1.0f), vec3(root_position[0], (root_position[1] - scale_vector[i][1] / 2.0f), root_position[2]));
			original_end[i] = glm::translate(mat4(1.0f), vec3(0, scale_vector[i][1], 0));

			world_transformation[i] = origin_link[i] * rotated[i];
			bone_matrix[i] = glm::translate(mat4(1.0f), root_position);
		}
	}

	end_effector = world_transformation[3] * original_end[3] * vec4(0.0f, 0.0f, 0.0f, 1.0f);
	float distance = glm::distance(target_position, end_effector);
}

//inverse kinematics method
void Bone_Animation::jacobian_calculation()
{
	std::vector<glm::mat3> jacobian;
	std::vector<glm::mat3> transposed_jacobian;
	std::vector<glm::vec3> delta_angles;

	//calculate Jacobian and transpose
	for (int i = 0; i < num_of_joints; i++)
	{
		glm::vec3 axis_x_cross = glm::cross(x_rotation_angle[i], end_effector - pivot_points[i]);
		glm::vec3 axis_y_cross = glm::cross(y_rotation_angle[i], end_effector - pivot_points[i]);
		glm::vec3 axis_z_cross = glm::cross(z_rotation_angle[i], end_effector - pivot_points[i]);

		jacobian.push_back(glm::mat3(axis_x_cross, axis_y_cross, axis_z_cross));
		transposed_jacobian.push_back(glm::transpose(jacobian[i]));
	}

	glm::vec3 numerator(0.0f);
	glm::vec3 denominator(0.0f);

	for (int i = 0; i < num_of_joints; i++)
	{
		glm::vec3 diff = target_position - end_effector;
		glm::vec3 intermediate_result = transposed_jacobian[i] * diff;

		numerator += intermediate_result;
		denominator += jacobian[i] * intermediate_result;
	}

	//calculate step size alpha
	float alpha;
	if (glm::length(numerator) > 1e-6) {
		alpha = glm::dot(numerator, numerator) / glm::dot(denominator, denominator);
	}
	else {
		alpha = 0.0f;
	}

	//update values
	for (int i = 0; i < num_of_joints; i++)
	{
		delta_angles.push_back(alpha * transposed_jacobian[i] * (target_position - end_effector));
		rotation_degree_vector[i + 1] += glm::vec3(delta_angles[i][1], delta_angles[i][2], delta_angles[i][0]);
	}
}






#include <glm/gtx/quaternion.hpp>
#include "Curve.h"

using namespace glm;
using namespace std;

Curve::Curve()
{
}

Curve::~Curve()
{
}

void Curve::init()
{
	this->control_points_pos = {
		{ 0.0, 8.5, -2.0 },
		{ -3.0, 11.0, 2.3 },
		{ -6.0, 8.5, -2.5 },
		{ -4.0, 5.5, 2.8 },
		{ 1.0, 2.0, -4.0 },
		{ 4.0, 2.0, 3.0 },
		{ 7.0, 8.0, -2.0 },
		{ 3.0, 10.0, 3.7 }
	};

	this->control_points_quaternion = {
		{0.13964   , 0.0481732 , 0.831429 , 0.541043 },
		{0.0509038 , -0.033869 , -0.579695, 0.811295 },
		{-0.502889 , -0.366766 , 0.493961 , 0.592445 },
		{-0.636    , 0.667177  , -0.175206, 0.198922 },
		{0.693492  , 0.688833  , -0.152595, -0.108237 },
		{0.752155  , -0.519591 , -0.316988, 0.168866 },
		{0.542054  , 0.382705  , 0.378416 , 0.646269 },
		{0.00417342, -0.0208652, -0.584026, 0.810619 }
	};
}

vector<vec3> Curve::points(
	const vec3& P0,
	const vec3& P1,
	const vec3& P2,
	const vec3& P3)
{
	mat4 matrix = make_mat4(CatmullMatrix);
	mat4x3 control_points(P0, P1, P2, P3);
	vec4 u_var;
	vector<vec3> interpolated_points;

	for (int i = 0; i < num_points_per_segment; i++) {
		float u = (1.0 / (float)num_points_per_segment) * (i);
		vec4 u_var = vec4(u * u * u, u * u, u, 1);
		vec3 point = control_points * matrix * u_var;;
		interpolated_points.push_back(point);
		count++;
	}

	return interpolated_points;
}

void Curve::calculate_curve() {
	int num_of_control_points = control_points_pos.size();
	vector<vec3> interpolated_points;
	vector<vec3>::iterator curve_points_iterator;

	for (int i = 0; i < num_of_control_points; i++) {

		vec3 P0 = control_points_pos[(i + num_of_control_points - 1) % num_of_control_points];
		vec3 P1 = control_points_pos[i % num_of_control_points];
		vec3 P2 = control_points_pos[((i + 1) % num_of_control_points)];
		vec3 P3 = control_points_pos[((i + 2) % num_of_control_points)];
		interpolated_points = points(P0, P1, P2, P3);

		curve_points_iterator = curve_points_pos.end();
		curve_points_pos.insert(curve_points_iterator, interpolated_points.begin(), interpolated_points.end());
		if (i == num_of_control_points - 1) {
			curve_points_pos.push_back(P2);
		}
	}
}

//rotate matrix using quaternion
glm::mat4 Curve::rotate_with_quaternion(glm::mat4 rotating, int index) {
	mat4 quaternion_matrix = quaternion_transformation_matrix(control_points_quaternion[index], control_points_pos[index]);
	glm::mat4 rotated_matrix = quaternion_matrix * rotating;

	return rotated_matrix;
}

//transform the quaternion to matrix
glm::mat4 Curve::quaternion_transformation_matrix(glm::quat quaterion, glm::vec3 position) {
	glm::mat4 quaternion_matrix = toMat4(quaterion);
	mat4 quat_matrix = mat4_cast(quaterion);

	quat_matrix[3][0] = position[0];
	quat_matrix[3][1] = position[1];
	quat_matrix[3][2] = position[2];

	return quat_matrix;
}

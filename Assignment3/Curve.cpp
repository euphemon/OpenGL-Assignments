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
}

vector<vec3> Curve::points(
	const vec3& P0,
	const vec3& P1,
	const vec3& P2,
	const vec3& P3)
{
	mat4 matrix = make_mat4(CatmullMatrix);
	mat4x3 control_points(P0, P1, P2, P3);
	vector<vec3> interpolated_points;

	for (int i = 0; i < num_points_per_segment; i++) {
		float u = static_cast<float>(i) / num_points_per_segment;
		//u^3, u^2, u, 1
		vec4 u_var(u * u * u, u * u, u, 1.0f);
		vec3 point = control_points * matrix * u_var;
		interpolated_points.push_back(point);
	}

	return interpolated_points;
}

void Curve::calculate_curve()
{
	int num_of_control_points = static_cast<int>(control_points_pos.size());
	vector<vec3> curve_points;
	vector<vec3>::iterator curve_points_iterator;

	for (int i = 0; i < num_of_control_points; i++) {

		vec3 P0 = control_points_pos[(i + num_of_control_points - 1) % num_of_control_points];
		vec3 P1 = control_points_pos[i];
		vec3 P2 = control_points_pos[((i + 1) % num_of_control_points)];
		vec3 P3 = control_points_pos[((i + 2) % num_of_control_points)];
		curve_points = points(P0, P1, P2, P3);

		curve_points_iterator = curve_points_pos.end();
		curve_points_pos.insert(curve_points_iterator, curve_points.begin(), curve_points.end());
		curve_points_pos.push_back(P2);

	}

}

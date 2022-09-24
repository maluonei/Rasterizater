#pragma once
#ifndef PROJECTION_H
#define PROJECTION_H

#include "Eigen/Dense"
#include <math.h>
using namespace std;
using namespace Eigen;

const float MY_PI = 3.1415926535f;

Matrix4f GetViewportMatrix(int screen_width, int screen_height) {
	Eigen::Matrix4f Mvp;
	Mvp <<
		screen_height / 2.0f, 0, 0, (screen_height - 1) / 2.0f,
		0, screen_width / 2.0f, 0, (screen_width - 1) / 2.0f,
		0, 0, 1, 0,
		0, 0, 0, 1;
	return Mvp;
}

Matrix4f GetOrthographicMatrix(float l, float r, float b, float t, float n, float f) {
	Eigen::Matrix4f Morth;
	Morth <<
		2 / (r - l), 0, 0, -(r + l) / (r - l),
		0, 2 / (t - b), 0, -(t + b) / (t - b),
		0, 0, 2 / (n - f), -(n + f) / (n - f),
		0, 0, 0, 1;
	return Morth;
}

Matrix4f GetCameraMatrix(Vector3f eye_position, Vector3f gaze_direction, Vector3f view_up) {
	//eye_position <<
	//	0, 0, 2;
	//gaze_direction <<
	//	0, 0, -1;
	//view_up <<
	//	0, 1, 0;

	Vector3f w = -gaze_direction.normalized();
	Vector3f u = view_up.cross(w).normalized();
	Vector3f v = w.cross(u);

	Eigen::Matrix4f Mcam;
	Eigen::Matrix4f temp1;
	Eigen::Matrix4f temp2;

	temp1 <<
		u.x(), u.y(), u.z(), 0,
		v.x(), v.y(), v.z(), 0,
		w.x(), w.y(), w.z(), 0,
		0, 0, 0, 1;

	temp2 <<
		1, 0, 0, -eye_position.x(),
		0, 1, 0, -eye_position.y(),
		0, 0, 1, -eye_position.z(),
		0, 0, 0, 1;

	Mcam = temp1 * temp2;
	//Eigen::Matrix4f Mcam0;
	//Mcam0 <<
	//	1, 0, 0, 0,
	//	0, 1, 0, 0,
	//	0, 0, 1, 0,
	//	0, 0, 0, 1;

	return Mcam;
}

Matrix4f GetPerspectiveMatrix(float zNear, float zFar) {
	Eigen::Matrix4f P_inverse;
	P_inverse <<
		zFar, 0, 0, 0,
		0, zFar, 0, 0,
		0, 0, 0, (zFar * zNear),
		0, 0, -1, (zNear + zFar);
	return P_inverse.inverse();
}

Matrix4f GetPerspectiveProjectionMatrix(int screen_width, int screen_height, float eye_fov, float aspect_ratio, float zNear, float zFar) {
	float angle = eye_fov * MY_PI / 180.0f;

	float height = -2.f * tan(angle / 2.0f) * zNear;
	float width = height * aspect_ratio;
	float depth = zFar - zNear;

	float t = -zNear * tan(angle / 2.0f);
	float r = t * aspect_ratio;
	float l = -r;
	float b = -t;

	Eigen::Matrix4f Mvp = GetViewportMatrix(screen_width, screen_height);
	Eigen::Matrix4f Morth = GetOrthographicMatrix(l, r, b, t, zNear, zFar);

	Vector3f eye_position;
	Vector3f gaze_direction;
	Vector3f view_up;

	eye_position <<
		0.f, 0.f, 10.f;
	gaze_direction <<
		0.f, 0.f, -1.f;
	view_up <<
		0.f, 1.f, 0.f;

	Eigen::Matrix4f Mcam = GetCameraMatrix(eye_position, gaze_direction, view_up);
	Eigen::Matrix4f P = GetPerspectiveMatrix(zNear, zFar);
	return Mvp * Morth * P * Mcam;
}

Matrix4f GetOrthographicProjectionMatrix(int screen_width, int screen_height, float eye_fov, float aspect_ratio, float zNear, float zFar) {
	float angle = eye_fov * MY_PI / 180.0f;

	float height = 2 * tan(angle / 2.0f) * zNear;
	float width = height * aspect_ratio;
	float depth = zFar - zNear;

	float t = -zNear * tan(angle / 2.0f);
	float r = t * aspect_ratio;
	float l = -r;
	float b = -t;

	Eigen::Matrix4f Mvp = GetViewportMatrix(screen_width, screen_height);
	Eigen::Matrix4f Morth = GetOrthographicMatrix(l, r, b, t, zNear, zFar);

	Vector3f eye_position;
	Vector3f gaze_direction;
	Vector3f view_up;

	eye_position <<
		0, 0, -5.f;
	gaze_direction <<
		0, 0, -1.f;
	view_up <<
		0, 1.f, 0;

	Eigen::Matrix4f Mcam = GetCameraMatrix(eye_position, gaze_direction, view_up);

	return Mvp * Morth * Mcam;
}

#endif



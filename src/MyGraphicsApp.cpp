#include "../head/triangle.h"
#include "../head/quadrilateral.h"
#include "../head/projection.h"

int main() {
	Vector4f A(1, 2, 0.5, 1);
	Vector4f B(2, -2, -2, 1);
	Vector4f C(-2, -2, -2, 1);
	Vector4f D(-3, 3, -5, 1);

	Vector3f ca(1, 0, 1);
	Vector3f cb(0, 1, 1);
	Vector3f cc(0, 1, 0);
	Vector3f cd(1, 0, 0);

	int screen_width = 600;
	int screen_height = 800;

	cv::Mat M(screen_height, screen_width, CV_8UC3, cv::Scalar::all(255));
	Triangle triangle(A, B, C, ca, cb, cc); //create a triangle

	float eye_fov = 90;
	float aspect_ratio = 4.0f / 3.0f;
	float zNear = -1;
	float zFar = -5;
	Matrix4f Mpers = GetPerspectiveProjectionMatrix(screen_width, screen_height, eye_fov, aspect_ratio, zNear, zFar);

	Vector4f vntest = Vector4f(0, 0, zNear, 1);
	Vector4f vftest = Vector4f(0, 0, zFar, 1);
	Vector4f veye = Vector4f(0, 0, 9.9, 1);
	vntest = Mpers * vntest;
	vftest = Mpers * vftest;
	veye = Mpers * veye;
	vntest /= vntest.w();
	vftest /= vftest.w();
	veye /= veye.w();
	cout << vntest << endl;
	cout << vftest << endl;
	cout << veye << endl;
	cout << "******************\n";

	triangle.getPersprctiveProjection(Mpers);
	Graphic* graphic = triangle.clip(vntest.z(), vftest.z());
	if (graphic == nullptr) {
		cout << "DEBUG: graphic is nullptr\n";
	}
	else {
		graphic->drawSquare(M, vntest.z(), vftest.z());
	}
	Mat M1 = adjust(M);
	ShowImg(M1, "test");
}

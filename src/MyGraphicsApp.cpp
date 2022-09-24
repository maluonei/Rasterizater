#include "../head/triangle.h"
#include "../head/quadrilateral.h"
#include "../head/projection.h"

const int screen_width = 800;
const int screen_height = 600;

float*** zbuffer;
Vector3f*** cbuffer;

void InitZbuffer() {
	zbuffer = new float** [screen_width];
	for (int i = 0; i < screen_width; i++) {
		zbuffer[i] = new float* [screen_height];
		for (int j = 0; j < screen_height; j++) {
			zbuffer[i][j] = new float[sample_per_pixel];
			for (int k = 0; k < sample_per_pixel; k++) {
				zbuffer[i][j][k] = -10000.f;
			}
		}
	}
}

void InitCbuffer() {
	cbuffer = new Vector3f * *[screen_width];
	for (int i = 0; i < screen_width; i++) {
		cbuffer[i] = new Vector3f * [screen_height];
		for (int j = 0; j < screen_height; j++) {
			cbuffer[i][j] = new Vector3f[sample_per_pixel];
			for (int k = 0; k < sample_per_pixel; k++) {
				cbuffer[i][j][k] = Vector3f(1.0f, 1.0f, 1.0f);
			}
		}
	}
}

int main() {
	InitZbuffer();
	InitCbuffer();

	Vector4f A(6.f, 6.f, -3.f, 1.f); //Vertex
	Vector4f B(6.f, -6.f, -3.f, 1.f);
	Vector4f C(-6.f, -6.f, -3.f, 1.f);

	Vector4f D(4.f, 4.f, -4.f, 1.f);
	Vector4f E(4.f, -4.f, -2.f, 1.f);
	Vector4f F(-4.f, -4.f, -4.f, 1.f);

	Vector3f ca(0.f, 0.f, 1.f);  //Color
	Vector3f cb(0.f, 0.f, 1.f);
	Vector3f cc(0.f, 0.f, 1.f);
	Vector3f cd(1.f, 0.f, 0.f);
	Vector3f ce(1.f, 0.f, 0.f);
	Vector3f cf(1.f, 0.f, 0.f);

	//int screen_width = 600;
	//int screen_height = 800;

	cv::Mat M(screen_height, screen_width, CV_8UC3, cv::Scalar::all(255));

	//Triangle triangle1(A, B, C, ca, cb, cc);
	//Triangle triangle2(B, C, D, cd, cb, cc);
	Triangle triangle1(A, B, C, ca, cb, cc);
	Triangle triangle2(D, E, F, cd, ce, cf);
	//Graphic* graphic = new Triangle(A, B, C, ca, cb, cc);
	//Quadrilateral quadrilateral(A, B, C, D, ca, cb, cc, cd);

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
	//cout << vntest << endl;
	//cout << vftest << endl;
	//cout << veye << endl;
	//cout << "******************\n";

	//triangle1.getPersprctiveProjection(Mpers);
	//triangle1.drawSquare(M, vntest.w(), vftest.w());

	//quadrilateral.getPersprctiveProjection(Mpers);
	//quadrilateral.drawSquare(M, vntest.z(), vftest.z());
	triangle1.getPersprctiveProjection(Mpers);
	triangle2.getPersprctiveProjection(Mpers);
	//graphic->getPersprctiveProjection(Mpers);
	//Graphic* graphic = &triangle;
	Graphic* graphic1 = triangle1.clip(vntest.z(), vftest.z());
	Graphic* graphic2 = triangle2.clip(vntest.z(), vftest.z());

	if (graphic1 == nullptr) {
		cout << "DEBUG: graphic1 is nullptr\n";
	}
	else {
		graphic1->drawSquare(M, vntest.z(), vftest.z(), zbuffer, cbuffer);
	}

	if (graphic2 == nullptr) {
		cout << "DEBUG: graphic2 is nullptr\n";
	}
	else {
		graphic2->drawSquare(M, vntest.z(), vftest.z(), zbuffer, cbuffer);
	}

	draw(M, zbuffer, cbuffer);
	//Mat M1 = adjust(M);
	ShowImg(M, "test");

	return 0;
}
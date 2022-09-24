#pragma once
#ifndef GRAPHIC_H
#define GRAPHIC_H

#include "rasterization.h"
#include "basic.h"

#include <iostream>
#include <math.h>
#include <vector>
#include <random>
#include "Eigen/Dense"
#include "openCV2/core.hpp"
#include <opencv2/highgui.hpp>

using namespace std;
using namespace Eigen;
using namespace cv;

void ShowImg(Mat& M, string fileName) {
	imshow(fileName, M);
	waitKey(60000);
	destroyAllWindows();
}


Mat adjust(Mat M1) {
	int width = M1.cols;
	int height = M1.rows;

	Mat M(width, height, CV_8UC3, Scalar::all(255));
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			//cout << i << " " << j << endl;
			M.at<Vec3b>(i, j) = M1.at<Vec3b>(j, width - i - 1);
		}
	}
	return M;
}

class Graphic {
public:
	virtual void drawLine(Mat& M) const = 0;
	virtual void drawSquare(Mat& M, float zNear, float zFar, float*** zbuffer, Vector3f*** cbuffer) const = 0;
	virtual void getPersprctiveProjection(Matrix4f Mpers) = 0;
};

class TriangularPyramid : public Graphic {
public:
	Vector4f a, b, c, d;
	TriangularPyramid(Vector4f _a, Vector4f _b, Vector4f _c, Vector4f _d) :a(_a), b(_b), c(_c), d(_d) {}
	TriangularPyramid() {
		a = Vector4f(0, 0, 0, 0);
		b = Vector4f(0, 0, 0, 0);
		c = Vector4f(0, 0, 0, 0);
		d = Vector4f(0, 0, 0, 0);
	}

	virtual void drawLine(cv::Mat& M) const {
		BresenhamLine(M, a.x(), a.y(), b.x(), b.y());
		BresenhamLine(M, a.x(), a.y(), c.x(), c.y());
		BresenhamLine(M, a.x(), a.y(), d.x(), d.y());
		BresenhamLine(M, b.x(), b.y(), c.x(), c.y());
		BresenhamLine(M, b.x(), b.y(), d.x(), d.y());
		BresenhamLine(M, c.x(), c.y(), d.x(), d.y());
		cv::Mat M1 = adjust(M);
		cv::imshow("Projection", M1);
		cv::waitKey(60000);
		cv::destroyAllWindows();
	}

	void draw() {
		cout << "(" << a.x() << ",    " << a.y() << ",    " << a.z() << ",    " << a.w() << ")" << endl
			<< "(" << b.x() << ",    " << b.y() << ",    " << b.z() << ",    " << b.w() << ")" << endl
			<< "(" << c.x() << ",    " << c.y() << ",    " << c.z() << ",    " << c.w() << ")" << endl
			<< "(" << d.x() << ",    " << d.y() << ",    " << d.z() << ",    " << d.w() << ")" << endl;
	}
};

#endif



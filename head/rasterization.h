#pragma once
#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include <iostream>
#include "openCV2/opencv.hpp"
#include "Eigen/Dense"
#include "basic.h"

using namespace std;
using namespace cv;
using namespace Eigen;

void BresenhamLine(cv::Mat& M, int x1, int y1, int x2, int y2) {
	int x, y, dx, dy, p;

	if (x1 == x2) {
		if (y1 > y2) {
			std::swap(y1, y2);
		}
		for (int y = y1; y <= y2; y++) {
			M.at<cv::Vec3b>(x1, y) = cv::Vec3b(0, 0, 0);
		}
		return;
	}

	if (y1 == y2) {
		if (x1 > x2) {
			std::swap(x1, x2);
		}
		for (int x = x1; x <= x2; x++) {
			M.at<cv::Vec3b>(x, y1) = cv::Vec3b(0, 0, 0);
		}
	}

	if ((x1 > x2 && y1 > y2) || (x1 > x2 && y1 < y2 && (x1 - x2 > y2 - y1)) || (x2 > x1 && y2 < y1 && (y1 - y2 > x2 - x1))) {
		std::swap(x1, x2);
		std::swap(y1, y2);
	}

	x = x1;
	y = y1;
	dx = x2 - x1;
	dy = y2 - y1;
	if (0 < dy && dy <= dx) {
		p = dy + dy - dx;
		for (; x < x2; x++) {
			M.at<cv::Vec3b>(x, y) = cv::Vec3b(0, 0, 0);
			if (p >= 0) {
				y++;
				p += ((dy - dx) + (dy - dx));//+2dy -2dx
			}
			else {
				p += (dy + dy);          //+2dy
			}
		}
	}
	else if (0 < dx && dx <= dy) {
		p = dx + dx - dy;
		for (; y < y2; y++) {
			M.at<cv::Vec3b>(x, y) = cv::Vec3b(0, 0, 0);
			if (p >= 0) {
				x++;
				p += ((dx - dy) + (dx - dy));
			}
			else {
				p += (dx + dx);
			}
		}
	}
	else if (dy < 0 && dx > 0 && (-dy) <= dx) {
		dy = -dy;
		p = dy + dy - dx;
		for (; x < x2; x++) {
			M.at<cv::Vec3b>(x, y) = cv::Vec3b(0, 0, 0);
			if (p >= 0) {
				y--;
				p += ((dy - dx) + (dy - dx));
			}
			else {
				p += (dy + dy);
			}
		}
	}
	else if (dx < 0 && dy > 0 && (-dx) <= dy) {
		dx = -dx;
		p = dx + dx - dy;
		for (; y < y2; y++) {
			M.at<cv::Vec3b>(x, y) = cv::Vec3b(0, 0, 0);
			if (p >= 0) {
				x--;
				p += ((dx - dy) + (dx - dy));
			}
			else {
				p += (dx + dx);
			}
		}
	}
}

void draw(cv::Mat& M, float*** zbuffer, Vector3f*** cbuffer) {
	int znum = 0;
	Vector3f finalColor;
	//cout << "M.cols£º" << M.cols << endl;
	//cout << "M.rows£º" << M.rows << endl;
	for (int x = 0; x < M.cols; x++) {
		for (int y = 0; y < M.rows; y++) {
			znum = 0;
			finalColor = Vector3f(.0f, .0f, .0f);
			for (int i = 0; i < sample_per_pixel; i++) {
				if (zbuffer[x][y][i] != -10000.f) {
					znum++;
					finalColor += cbuffer[x][y][i];
				}
			}
			if (znum != 0) {
				//cout << "x:" << x << endl;
				//cout << "y:" << y << endl;
				//cout << "M.rows-y-1:" << M.rows - y - 1 << endl;
				//cout << "*******************\n";
				finalColor /= znum;
				//cout << "final_color:" << finalColor.x() << "," << finalColor.y() << "," << finalColor.z() << endl;
				//cout << x << "    " << y << endl;
				M.at<cv::Vec3b>(x, y) = cv::Vec3b(int(finalColor.x() * 255.999), int(finalColor.y() * 255.999), int(finalColor.z() * 255.999));
			}
		}
	}
}

#endif // !1



#pragma once
#ifndef RASTERIZATION_H
#define RASTERIZATION_H

#include "openCV2/opencv.hpp"

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


#endif // !1



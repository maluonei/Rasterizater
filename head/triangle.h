#pragma once
#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "graphic.h"

class Triangle : public Graphic {
public:
	Triangle(Vector4f _a, Vector4f _b, Vector4f _c, Vector3f _ca, Vector3f _cb, Vector3f _cc) :
		a(_a), b(_b), c(_c), ca(_ca), cb(_cb), cc(_cc) {}
	Triangle(const Triangle& triangle) {
		a = Vector4f(triangle.a);
		b = Vector4f(triangle.b);
		c = Vector4f(triangle.c);
		ca = Vector3f(triangle.ca);
		cb = Vector3f(triangle.cb);
		cc = Vector3f(triangle.cc);
	}
	Triangle() {
		a = Vector4f(0, 0, 0, 0);
		b = Vector4f(0, 0, 0, 0);
		c = Vector4f(0, 0, 0, 0);
		ca = Vector3f(0, 0, 0);
		cb = Vector3f(0, 0, 0);
		cc = Vector3f(0, 0, 0);
	}

	Vector4f A() const {
		return a;
	}

	Vector4f B() const {
		return b;
	}

	Vector4f C() const {
		return c;
	}

	Vector3f Ca()const {
		return ca;
	}

	Vector3f Cb()const {
		return cb;
	}

	Vector3f Cc()const {
		return cc;
	}

	void draw() {
		cout << "(" << a.x() << ",    " << a.y() << ",    " << a.z() << ",    " << a.w() << ")" << endl
			<< "(" << b.x() << ",    " << b.y() << ",    " << b.z() << ",    " << b.w() << ")" << endl
			<< "(" << c.x() << ",    " << c.y() << ",    " << c.z() << ",    " << c.w() << ")" << endl;
	}

	virtual void drawLine(Mat& M) const {
		BresenhamLine(M, a.x(), a.y(), b.x(), b.y());
		BresenhamLine(M, b.x(), b.y(), c.x(), c.y());
		BresenhamLine(M, a.x(), a.y(), c.x(), c.y());
	}

	virtual void drawSquare(Mat& M, float zNear, float zFar, float*** zbuffer, Vector3f*** cbuffer) const {
		float xmin = min(a.x(), min(b.x(), c.x()));
		float xmax = max(a.x(), max(b.x(), c.x()));
		float ymin = min(a.y(), min(b.y(), c.y()));
		float ymax = max(a.y(), max(b.y(), c.y()));
		//float zmin = min(a.z(), min(b.z(), c.z()));
		float zmax = max(a.z(), max(b.z(), c.z()));
		if (zmax > zNear) return;

		float falpha = f12(a.x(), a.y());
		float fbeta = f20(b.x(), b.y());
		float fgamma = f01(c.x(), c.y());

		if (falpha == 0) falpha = 1e-5;
		if (fbeta == 0) fbeta = 1e-5;
		if (fgamma == 0) fgamma = 1e-5;

		Vector2f samples[4] = {
			{0.25f, 0.25f},
			{0.25f, 0.75f},
			{0.75f, 0.25f},
			{0.75f, 0.75f}
		};

		//cout << "M.rows:" << M.rows << endl;
		for (int x = xmin; x <= xmax; x++) {
			for (int y = ymin; y <= ymax; y++) {
				Vector3f color(.0f, .0f, .0f);
				float zpFinal = 0.0f;
				int sampleNum = 0;
				for (int i = 0; i < sample_per_pixel; i++) {
					if (x >= 0 && x < M.cols && y >= 0 && y < M.rows) {
						//质心坐标系转换, (x0,y0,z0) = alpha*pa+beta*pb+gamma*pc;
						//缺少除以0的检查
						float alpha = f12(x + samples[i].x(), y + samples[i].y()) / falpha;
						if (alpha < 0) continue;
						float beta = f20(x + samples[i].x(), y + samples[i].y()) / fbeta;
						if (beta < 0) continue;
						float gamma = f01(x + samples[i].x(), y + samples[i].y()) / fgamma;
						if (gamma < 0) continue;
						float zp = alpha * a.z() + beta * b.z() + gamma * c.z();
						//cout << "zp:" << zp << endl;
						if (zp >= zNear + (1e-5) || zp <= zFar - (1e-5)) {  //判断小于zNear和远于zFar的点
							//cout << "zp again:" << zp << endl;
							continue;
						}
						if (zp > zbuffer[x][y][i]) {
							if (alpha >= 0 && beta >= 0 && gamma >= 0) {
								if ((alpha > 0 || falpha * f12(-10, -10) > 0) &&   //和（-1，-1）在同一边的点的三角形，绘制这条边
									(beta > 0 || fbeta * f20(-10, -10) > 0) &&
									(gamma > 0 || fgamma * f01(-10, -10) > 0)) {
									zbuffer[x][y][i] = zp;
									cbuffer[x][y][i] = alpha * ca + beta * cb + gamma * cc;
									//cout << "cbuffer:" << cbuffer[x][y][i] << endl;
								}
							}
						}
					}
				}
				//float zp = zpFinal / sampleNum;
				//color /= sampleNum;
				//if (zp >= zbuffer[x][y]) {
				//	zbuffer[x][y] = zp;
				//	M.at<Vec3b>(x, y) = Vec3b(int(color.x() * 255.999), int(color.y() * 255.999), int(color.z() * 255.999));
				//}
			}
		}
	}

	virtual void getPersprctiveProjection(Matrix4f Mpers) {
		this->a = Mpers * (this->a);
		this->b = Mpers * (this->b);
		this->c = Mpers * (this->c);

		this->a /= this->a.w();
		this->b /= this->b.w();
		this->c /= this->c.w();
	}

	Graphic* clip(float zNear, float zFar);

private:
	Vector4f a, b, c;
	Vector3f ca, cb, cc;
	//Vector3f vntest, vftest;

	float f01(float x, float y) const {
		return (a.y() - b.y()) * x + (b.x() - a.x()) * y + a.x() * b.y() - b.x() * a.y();
	}

	float f12(float x, float y) const {
		return (b.y() - c.y()) * x + (c.x() - b.x()) * y + b.x() * c.y() - c.x() * b.y();
	}

	float f20(float x, float y) const {
		return (c.y() - a.y()) * x + (a.x() - c.x()) * y + c.x() * a.y() - a.x() * c.y();
	}
};


#endif // !TRIANGLE_H


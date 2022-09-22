#pragma once
#ifndef QUADRILATERAL_H
#define QUADRILATERAL_H

#include "triangle.h"

class Quadrilateral :public Graphic {
public:
	Quadrilateral(Vector4f _a, Vector4f _b, Vector4f _c, Vector4f _d, Vector3f _ca, Vector3f _cb, Vector3f _cc, Vector3f _cd) :
		a(_a), b(_b), c(_c), d(_d), ca(_ca), cb(_cb), cc(_cc), cd(_cd) {}
	Quadrilateral() {}

	virtual void drawLine(Mat& M) const {

	}
	virtual void drawSquare(Mat& M, float zNear, float zFar) const {
		Triangle t1(a, b, d, ca, cb, cd);
		Triangle t2(b, c, d, cb, cc, cd);

		t1.drawSquare(M, zNear, zFar);
		t2.drawSquare(M, zNear, zFar);
	}

	virtual void getPersprctiveProjection(Matrix4f Mpers) {
		this->a = Mpers * (this->a);
		this->b = Mpers * (this->b);
		this->c = Mpers * (this->c);
		this->d = Mpers * (this->d);

		this->a /= this->a.w();
		this->b /= this->b.w();
		this->c /= this->c.w();
		this->d /= this->d.w();
	}

private:
	Vector4f a, b, c, d;// triangle(a,b,c) and (b,c,d), bd is the overlap line
	Vector3f ca, cb, cc, cd;
};


Graphic* Triangle::clip(float zNear, float zFar) {
	Graphic* graphic;

	Vector4f q(0, 0, zNear, 1.0f);
	Vector4f normal(0, 0, 1.0f, 0);

	float za = a.z();
	float zb = b.z();
	float zc = c.z();

	float t1, t2;
	float D = -normal.dot(q);
	float K1, K2;

	Vector4f innerP1, innerP2;
	Vector4f outerP1, outerP2;
	Vector3f c1, c2, c3;

	if (za < zNear && zb < zNear && zc < zNear) {
		//cout << "DEBUG:1\n";
		return this;// Triangle(a, b, c, ca, cb, cc);
	}
	else if (za > zNear && zb > zNear && zc > zNear) {
		return nullptr;
	}
	//一点内两点外
	else if ((za - zNear) * (zb - zNear) * (zc - zNear) > 0) {
		if (za < zNear && zb<zNear && zc > zNear) { //点C看不见
			innerP1 = a;
			c1 = ca;
			innerP2 = b;
			c2 = cb;
			outerP1 = c;
			c3 = cc;
		}
		else if (za < zNear && zb>zNear && zc < zNear) { //点B看不见
			innerP1 = a;
			c1 = ca;
			innerP2 = c;
			c2 = cc;
			outerP1 = b;
			c3 = cb;
		}
		else if (za > zNear && zb < zNear && zc < zNear) { //点A看不见
			innerP1 = b;
			c1 = cb;
			innerP2 = c;
			c2 = cc;
			outerP1 = a;
			c3 = ca;
		}

		K1 = normal.dot(innerP1 - outerP1);
		K2 = normal.dot(innerP2 - outerP1);
		if (K1 == 0 || K2 == 0) return this; // 这条边与zNear平面平行

		t1 = (normal.dot(innerP1) - D) / K1;
		t2 = (normal.dot(innerP2) - D) / K2;

		if (t1 > 0 && t1 < 1 && t2>0 && t2 < 1) {
			Vector4f temp1 = innerP1 + t1 * (outerP1 - innerP1);
			Vector4f temp2 = innerP2 + t2 * (outerP1 - innerP2);
			Vector3f C1 = t1 * c1 + (1 - t1) * c3;
			Vector3f C2 = t2 * c2 + (1 - t2) * c3;
			return new Quadrilateral(innerP1, innerP2, temp2, temp1, c1, c2, C2, C1);
		}
	}
	else {
		if (za < zNear && zb>zNear && zc > zNear) { //点C看不见
			innerP1 = a;
			c1 = ca;
			outerP1 = b;
			c2 = cb;
			outerP2 = c;
			c3 = cc;
		}
		else if (za > zNear && zb<zNear && zc > zNear) { //点B看不见
			innerP1 = b;
			c1 = cb;
			outerP1 = a;
			c2 = ca;
			outerP2 = c;
			c3 = cc;
		}
		else if (za > zNear && zb > zNear && zc < zNear) { //点A看不见
			innerP1 = c;
			c1 = cc;
			innerP2 = a;
			c2 = ca;
			outerP1 = b;
			c3 = cb;
		}

		K1 = normal.dot(innerP1 - outerP1);
		K2 = normal.dot(innerP1 - outerP2);
		if (K1 == 0 || K2 == 0) return this;//这条边与zNear平面平行

		t1 = (normal.dot(innerP1) - D) / K1;
		t2 = (normal.dot(innerP1) - D) / K2;

		if (t1 > 0 && t1 < 1 && t2>0 && t2 < 1) {
			Vector4f temp1 = innerP1 + t1 * (outerP1 - innerP1);
			Vector4f temp2 = innerP1 + t2 * (outerP2 - innerP1);
			Vector3f C1 = t1 * c1 + (1 - t1) * c2;
			Vector3f C2 = t2 * c1 + (1 - t2) * c3;
			return  new Triangle(innerP1, temp1, temp2, c1, C1, C2);
		}
	}
	return this;
}


#endif

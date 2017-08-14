#pragma once

class Vec3;

class Mat3
{
public:
	Mat3();
	Mat3(float a, float b, float c, float d, float e, float f, float g, float h, float i);
	Mat3(const Vec3 &col1, const Vec3 &col2, const Vec3 &col3);

	inline float operator[](int i) const { return values[i]; }
	inline float& operator[](int i) { return values[i]; }

	Mat3 operator+(const Mat3 &other) const;
	Mat3 operator-(const Mat3 &other) const;
	Mat3 operator*(const Mat3 &other) const;
	
	Mat3& operator+=(const Mat3 &other);
	Mat3& operator-=(const Mat3 &other);
	Mat3& operator*=(const Mat3 &other);
	Vec3 operator*=(const Vec3 &vec);

	//a d g
	//b e h
	//c f i
	//stored as a b c d e f g h i
	union {
		struct {
			float a, b, c, d, e, f, g, h, i;
		};
		struct {
			float el11, el21, el31, el12, el22, el32, el13, el23, el33; //el[line][col]
		};
		float values[9];
	};
};

Vec3 operator*(const Mat3 &mat, const Vec3 &vec);

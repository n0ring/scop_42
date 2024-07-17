#pragma once 
#include <math.h>
#include <cmath>

namespace nrg
{
	struct vec4
	{
		float x, y, z, w;
		vec4(){}
		vec4(float val) : x(val), y(val), z(val), w(val) {}
		vec4(float val_x, float val_y, float val_z, float val_w) : x(val_x), y(val_y), z(val_z), w(val_w) {}
		inline float& operator[](int i)
		{
			if (i == 0)
				return x;
			if (i == 1)
				return y;
			if (i == 2)
				return z;
			if (i == 3)
				return w;
			return x;
		}

		inline vec4 operator*(float n) {return vec4(x * n, y * n, z * n, w * n);}

		inline vec4 operator+(vec4 other) { return vec4(x + other.x, y + other.y, z + other.z, w + other.w) ;}
		inline vec4 operator*(vec4 o) {return vec4(x * o.x, y * o.y, z * o.z, w * o.w);}
		inline vec4 operator/(vec4 o) {return vec4(x / o.x, y / o.y, z / o.z, w / o.w);}
	};
	struct vec3
	{
		float x, y, z;

		vec3() {}
		vec3(float val) : x(val), y(val), z(val) {}
		vec3(float val_x, float val_y, float val_z) : x(val_x), y(val_y), z(val_z) {}
		vec3(nrg::vec4 v) : x(v.x), y(v.y), z(v.z) {}
		inline float& operator[](int i)
		{
			if (i == 0)
				return x;
			if (i == 1)
				return y;
			if (i == 2)
				return z;
			return x;
		}


		inline vec3 operator*(float n) {return vec3(x * n, y * n, z * n);}
		inline vec3 operator+(vec3 other) { return vec3(x + other.x, y + other.y, z + other.z) ;}
		inline vec3 operator-(vec3 other) { return vec3(x - other.x, y - other.y, z - other.z) ;}
		inline vec3 operator+(float f) { return vec3(x + f, y + f, z + f) ;}
		inline vec3 operator*(vec3 o) {return vec3(x * o.x, y * o.y, z * o.z);}
		inline vec3 operator/(vec3 o) {return vec3(x / o.x, y / o.y, z / o.z);}
		inline vec3 operator/(float f) {return vec3(x / f, y / f, z / f);}

	};

	struct mat4
	{
		vec4 columns[4];

		mat4()
		{
			setToZeros();
		}
		mat4(float f) 
		{
			setToZeros();
			columns[0][0] = f;
			columns[1][1] = f;
			columns[2][2] = f;
			columns[3][3] = f;
		}

		mat4(vec4 v1, vec4 v2, vec4 v3, vec4 v4) {
			setToZeros();
			columns[0] = v1;
			columns[1] = v2;
			columns[2] = v3;
			columns[3] = v4;
		}


		inline vec4& operator[](int i)
		{
			return columns[i];
		}

		

		inline void setToZeros()
		{
			for (int i = 0; i < 4; i++)
			{
				for (int k = 0; k < 4; k++)
					columns[i][k] = 0;
			}
		}

	};

	mat4 operator*(mat4 m1, mat4 m2);
	vec4 operator*(mat4 m, vec4 v);
	vec3 cross(vec3 a, vec3 b);
	mat4 translate(mat4 m, vec3 v);
	mat4 scale(mat4 m, vec3 v);
	float radians(float degrees);
	vec3 dot(vec3 a, vec3 b);
	vec3 normalize(vec3 vec);
	mat4 rotate(mat4 m, float angle, vec3 v);

}
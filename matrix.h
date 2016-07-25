#ifndef MATRIX_H
#define MATRIX_H

#include <utility>
#include "include\vectors.h"

class Matrix22
{
public:
	

	float data[4];
	Matrix22()
	{
		data[0]=data[1]=data[2]=data[3]= 0;
	};
	Matrix22(const Vec2D &x, const Vec2D &y)
	{
		data[0] = x.x;
		data[1] = x.y;
		data[2] = y.x;
		data[3] = y.y;
	};
	Matrix22(float radians)
	{
		float c = std::cos( radians );
		float s = std::sin( radians );
		
		data[0] =   c;
		data[1]  = -s;
		data[2]  =  s;
		data[3]  =  c;
	};
	float operator [](int i) const
	{
		return data[i];
	}

	Matrix22 operator *(const Matrix22 &m) const
	{
		Matrix22 result;

		result.data[0] = (data[0] * m.data[0]) + (data[1] * m.data[2]);
		result.data[1] = (data[0] * m.data[1]) + (data[1] * m.data[3]);
		result.data[2] = (data[2] * m.data[0]) + (data[3] * m.data[2]);
		result.data[3] = (data[2] * m.data[1]) + (data[3] * m.data[3]);

		return result;
	};
	Vec2D operator*(const Vec2D &vec) const
	{
		return CreateVec2D
			(
				vec.x * data[0] +
				vec.y * data[1],
				
				vec.x * data[2] +
				vec.y *	data[3]
			);
	};
	Vec2D Transform(const Vec2D &vec) const
	{
		return (*this) * vec;
	};

	Matrix22 Transpose() const
	{
		Matrix22 result;
		result.data[0] = data[3];
		result.data[1] = data[2];
		result.data[2] = data[1];
		result.data[3] = data[0];
		return result;
	}

	Vec2D GetRowVector(int i) const
	{
		return CreateVec2D(data[i*2],data[i*2+1]);
	};

	Matrix22 Inverse() const
	{
		Matrix22 result;
		float determinant = (data[0]*data[3]) - (data[1]*data[2]);
		result.data[0] = data[3];
		result.data[1] = -data[1];
		result.data[2] = -data[2];
		result.data[3] = data[0];

		result * (1/determinant);
		return result;

	}
	void Set(float radians)
	{
		float c = std::cos( radians );
		float s = std::sin( radians );
		
		data[0] =   c;
		data[1]  = -s;
		data[2]  =  s;
		data[3]  =  c;
	}
	Matrix22 Abs() const
	{
		Matrix22 result;
		result.data[0] = std::abs( data[0]);
		result.data[1] = std::abs( data[1]);
		result.data[2] = std::abs( data[2]);
		result.data[3] = std::abs( data[3]);
	  return result;
	}
};

#endif


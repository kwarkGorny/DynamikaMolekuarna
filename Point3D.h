#ifndef POINT3D_H
#define POINT3D_H
#include <cmath>
template<class T>
struct Point3D
{
	Point3D(T x, T y, T z)noexcept : x(x), y(y), z(z) {}
	Point3D()noexcept = default;
	Point3D(const Point3D& other)noexcept = default;
	Point3D(Point3D&& other) noexcept = default;
	Point3D& operator=(const Point3D& other)noexcept = default;
	Point3D& operator=(Point3D&& other)noexcept = default;

    constexpr inline Point3D operator-(const Point3D& v2) const noexcept{
        return {x - v2.x, y - v2.y, z - v2.z} ;
    }
    friend inline Point3D& operator-=(Point3D& v1, const Point3D& v2)noexcept{
        v1.x -= v2.x;
        v1.y -= v2.y;
        v1.z -= v2.z;
        return v1;
    }
    constexpr inline Point3D operator+(const Point3D& v2) const noexcept{
        return {x + v2.x, y + v2.y, z + v2.z} ;
    }
    friend inline Point3D& operator+=(Point3D& v1, const Point3D& v2)noexcept{
        v1.x += v2.x;
        v1.y += v2.y;
        v1.z += v2.z;
        return v1;
    }
    template<class Q>
    constexpr inline Point3D operator*(Q&& scalar)const noexcept{
        return {x * scalar, y * scalar, z * scalar};
    }

    template<class Q>
    inline Point3D& operator*=(Q&& scalar)noexcept{
        x *= scalar;
        y *= scalar;
        z *= scalar;
        return *this;
    }
    template<class Q>
    constexpr inline Point3D operator/(Q&& scalar)const noexcept{
        return {x / scalar, y / scalar, z / scalar};
    }
    template<class Q>
    inline Point3D& operator/=(Q&& scalar)noexcept{
        x /= scalar;
        y /= scalar;
        z /= scalar;
        return *this;
    }
    constexpr inline T GetMagitudeSqr()const noexcept{
        return  x * x + y * y + z * z;
    }
    constexpr inline T GetMagitude()const noexcept{
        return  std::sqrt(GetMagitudeSqr());
    }
    T x = T();
    T y = T();
    T z = T();
};
template<class T, class Q>
constexpr inline Point3D<T> operator * ( Q&& k, const Point3D<T>& m )noexcept
{
  return m * k;
}

typedef Point3D<double> PointF;
#endif // POINT3D_H

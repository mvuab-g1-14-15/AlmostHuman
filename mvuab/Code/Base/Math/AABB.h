#ifndef AABB_H
#define AABB_H

#include "Utils\Types.h"
#include "Vector3.h"

template<typename T>
class AABB
{
public:
    AABB()
    {
    }

    AABB(const Vector3<T>& min, const Vector3<T>& max) : MinPnt(min) , MaxPnt(max)
    {
    }
    
    const AABB operator*(const Vector3<T>& s) const
    {
        MinPnt.x *= s.x;
        MinPnt.y *= s.y;
        MinPnt.z *= s.z;
        MaxPnt.x *= s.x;
        MaxPnt.y *= s.y;
        MaxPnt.z *= s.z;

        return (*this);
    }

    inline AABB operator+(const Vector3<T>& p) const
    {
        MinPnt = p + MinPnt;
        MaxPnt = p + MaxPnt;
        return (*this);
    }

    inline float32 GetHeight() const
    {
        return MaxPnt.y-MinPnt.y;
    }
    
    // What is the center of the smallest sphere that will enclose this AABB?
    inline Vector3<T> GetCenter() const
    {
        return (MaxPnt+MinPnt)/2;
    }
    
    // What is the radius of the smallest sphere that will enclose this AABB?
    inline float32 GetRadius() const
    {
        return ((MaxPnt-MinPnt)/2).Length();
    }

public:
    Vector3<T> MinPnt;
    Vector3<T> MaxPnt;
};

////////////////////////////////////////////////////////////////////////////////////////
// TIPOS DE AABBCON TIPOS CONCRETOS DE DATOS
////////////////////////////////////////////////////////////////////////////////////////
typedef AABB<float32>   AABB3f;
typedef AABB<float64>   AABB3d;
typedef AABB<int32>     AABB3i;
typedef AABB<uint32>    AABB3u;
typedef AABB<uint16>    AABB3w;

#endif //AABB_H
#ifndef AABB_H
#define AABB_H

#include "Utils\Types.h"
#include "Vector3.h"

namespace Math
{
template<typename T>
class AABB
{
public:
    AABB()
    {
    }

    AABB( const Vector3<T>& min, const Vector3<T>& max ) : MinPnt( min ) , MaxPnt( max )
    {
    }

    const AABB operator*( const Vector3<T>& s ) const
    {
        MinPnt.x *= s.x;
        MinPnt.y *= s.y;
        MinPnt.z *= s.z;
        MaxPnt.x *= s.x;
        MaxPnt.y *= s.y;
        MaxPnt.z *= s.z;

        return ( *this );
    }

    inline AABB operator+( const Vector3<T>& p ) const
    {
        MinPnt = p + MinPnt;
        MaxPnt = p + MaxPnt;
        return ( *this );
    }

    inline float32 GetHeight() const
    {
        return MaxPnt.y - MinPnt.y;
    }
    inline float32 GetWidth() const
    {
        return MaxPnt.x - MinPnt.x;
    }
    inline float32 GetDepth() const
    {
        return MaxPnt.z - MinPnt.z;
    }

    // What is the center of the smallest sphere that will enclose this AABB?
    inline Vector3<T> GetCenter() const
    {
        return ( MaxPnt + MinPnt ) * 0.5f;
    }
    inline Vector3<T> GetMin() const
    {
        return MinPnt;
    }
    inline Vector3<T> GetMax() const
    {
        return MaxPnt;
    }

    // What is the radius of the smallest sphere that will enclose this AABB?
    inline float32 GetRadius() const
    {
        return ( MaxPnt - MinPnt ).Length() * 0.5f;
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

} //namespace Math
#endif //AABB_H
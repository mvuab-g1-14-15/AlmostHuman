#ifndef RAY_H
#define RAY_H

#include "Utils\Types.h"
#include "Vector3.h"

namespace Math
{
template<typename T>
class RAY
{
public:
  RAY()
  {
  }

  Ray( const Vector3<T>& Origin, const Vector3<T>& Direction ) : m_Origin( Origin ) ,
    m_Direction( Direction )
  {
  }

  inline Vector3<T> GetOrigin() const
  {
    return m_Origin;
  }

  inline Vector3<T> GetDirection() const
  {
    return m_Direction;
  }

public:
  Vector3<T> m_Origin;
  Vector3<T> m_Direction;
};

////////////////////////////////////////////////////////////////////////////////////////
// TIPOS DE  Ray CON TIPOS CONCRETOS DE DATOS
////////////////////////////////////////////////////////////////////////////////////////
typedef Ray<float32>   Ray3f;
typedef Ray<float64>   Ray3d;
} //namespace Math
#endif //RAY_H
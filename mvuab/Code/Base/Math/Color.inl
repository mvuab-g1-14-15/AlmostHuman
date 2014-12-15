#include "MathUtils.h"

inline CColor& CColor::Clamp()
{
    m_Color.x = mathUtils::Min(mathUtils::Max(m_Color.x, 0.0f), 1.0f);
    m_Color.y = mathUtils::Min(mathUtils::Max(m_Color.y, 0.0f), 1.0f);
    m_Color.z = mathUtils::Min(mathUtils::Max(m_Color.z, 0.0f), 1.0f);
    m_Color.w = mathUtils::Min(mathUtils::Max(m_Color.w, 0.0f), 1.0f);

    return (*this);
}

///
/// Construye un uint32 que codifica el color como 0xAARRGGBB (PC)
///
inline uint32 CColor::GetUint32Argb () const
{
    return  (uint32)(float32ToByte(m_Color[3])<<24) 
        + (float32ToByte(m_Color[0])<<16)
        + (float32ToByte(m_Color[1])<<8) 
        +  float32ToByte(m_Color[2]);
}

///
/// Construye un uint32 que codifica el color como 0xAABBGGRR (PC)
///
inline uint32 CColor::GetUint32Abgr () const
{
    return  (uint32)(float32ToByte(m_Color[3])<<24) 
        + (float32ToByte(m_Color[2])<<16)
        + (float32ToByte(m_Color[1])<<8) 
        +  float32ToByte(m_Color[0]);
}

///
/// Asigna el color a partir de un uint32 en formato  0xAARRGGBB
///
inline void    CColor::SetUint32Argb (int32 argb)
{
    m_Color[3] = ByteTofloat32(static_cast<uint8>(     (argb>>24))); // alpha
    m_Color[0] = ByteTofloat32(static_cast<uint8>(0xff&(argb>>16))); // red
    m_Color[1] = ByteTofloat32(static_cast<uint8>(0xff&(argb>> 8))); // green
    m_Color[2] = ByteTofloat32(static_cast<uint8>(0xff&(argb    ))); // blue
}

///
/// Asigna el color a partir de un uint32 en formato  0xAABBGGRR
///
inline void    CColor::SetUint32Abgr (int32 abgr)
{
    m_Color[3] = ByteTofloat32(static_cast<uint8>(     (abgr>>24))); // alpha
    m_Color[2] = ByteTofloat32(static_cast<uint8>(0xff&(abgr>>16))); // blue
    m_Color[1] = ByteTofloat32(static_cast<uint8>(0xff&(abgr>> 8))); // green
    m_Color[0] = ByteTofloat32(static_cast<uint8>(0xff&(abgr    ))); // red
}


// Funci??n de utilidad para convertir una componente de color en coma flotante a su versi??n de un byte (0..255)
inline uint8 CColor::float32ToByte(float32 fValue) const 
{ 
  int i = static_cast<int>(0xff * fValue); 
    i = mathUtils::Clamp(i, 0, 0xff); 

  return static_cast<uint8>(i); 
}

// Funci??n de utilidad para convertir un byte con una componente de color (0..255) a componente float32 (0.0..1.0)
inline float32 CColor::ByteTofloat32 (uint8 uValue) const 
{ 
  return static_cast<float32>(uValue)/255.0f; 
}

inline CColor& CColor::Lerp(const CColor& other, float32 f)
{
    m_Color.Lerp(other.GetArgb(), f);

    return *this;
}

// Operadores aritméticos   
inline CColor CColor::operator + (const CColor& otro)    const
{
    return CColor(otro.m_Color + m_Color);
}

inline CColor CColor::operator - (const CColor& otro)    const
{
    return CColor(m_Color - otro.m_Color);
}

inline CColor CColor::operator * (const CColor& otro)    const
{
    return CColor(otro.m_Color.x * m_Color.x,
                                otro.m_Color.y * m_Color.y,
                                otro.m_Color.z * m_Color.z,
                                otro.m_Color.w * m_Color.w);
}

inline CColor CColor::operator * (float32 escalar) const
{
    return CColor(m_Color * escalar);
}

// Operadores aritméticos de actualización   
inline CColor& CColor::operator += (const CColor& otro)
{
    m_Color += otro.m_Color;
    
    return (*this);
}

inline CColor& CColor::operator -= (const CColor& otro)
{
    m_Color -= otro.m_Color;

    return (*this);
}

inline CColor& CColor::operator *= (const CColor& otro)
{
    m_Color.x *= otro.m_Color.x;
    m_Color.y *= otro.m_Color.y;
    m_Color.z *= otro.m_Color.z;
    m_Color.w *= otro.m_Color.w;

    return (*this);
}

inline CColor& CColor::operator *= (float32 escalar)
{
    m_Color *= escalar;

    return (*this);
}

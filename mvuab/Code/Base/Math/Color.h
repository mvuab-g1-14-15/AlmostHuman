#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

namespace Math
{
class CColor
{
private:
  Math::Vect4f m_Color;

public:

  // Constructors
  explicit Math::CColor() : m_Color( 1.0f, 1.0f, 1.0f, 1.0f ) {}
  explicit Math::CColor( const Math::Vect4f& vect ) : m_Color( vect ) {}
  explicit Math::CColor( float32 red, float32 green, float32 blue,
                         float32 alpha = 1.0f ) : m_Color( red, green, blue, alpha ) {}

  // Getters and setters
  inline void Set( float32 red, float32 green, float32 blue, float32 alpha = 1.0f )
  {
    m_Color[0] = red;
    m_Color[1] = green;
    m_Color[2] = blue;
    m_Color[3] = alpha;
  }
  inline float32 GetRed() const
  {
    return m_Color[0];
  }
  inline void SetRed( float32 red )
  {
    m_Color[0] = red;
  }
  inline float32 GetGreen() const
  {
    return m_Color[1];
  }
  inline void SetGreen( float32 green )
  {
    m_Color[1] = green;
  }
  inline float32 GetBlue() const
  {
    return m_Color[2];
  }
  inline void SetBlue( float32 blue )
  {
    m_Color[2] = blue;
  }
  inline float32 GetAlpha() const
  {
    return m_Color[3];
  }
  inline void SetAlpha( float32 alpha )
  {
    m_Color[3] = alpha;
  }
  inline const Math::Vect4f& GetArgb() const
  {
    return m_Color;
  }
  inline void SetArgb( const Math::Vect4f& argb )
  {
    m_Color = argb;
  }

  inline uint32 GetUint32Abgr() const;
  inline uint32 GetUint32Argb() const;
  inline void SetUint32Argb( int32 argb );
  inline void    SetUint32Abgr( int32 argb );
  inline Math::CColor& Clamp();
  inline Math::CColor& Lerp( const Math::CColor& other, float32 f );

  // Aritmetic Operators
  inline        Math::CColor  operator + ( const Math::CColor& otro )    const;
  inline        Math::CColor  operator - ( const Math::CColor& otro )    const;
  inline        Math::CColor  operator * ( const Math::CColor& otro )    const;
  inline        Math::CColor  operator * ( float32 escalar )            const;

  // Aritmetic Operators
  inline Math::CColor& operator += ( const Math::CColor& otro );
  inline Math::CColor& operator -= ( const Math::CColor& otro );
  inline Math::CColor& operator *= ( const Math::CColor& otro );
  inline Math::CColor& operator *= ( float32 escalar );

private:
  inline uint8 float32ToByte( float32 fValue ) const;
  inline float32 ByteTofloat32( uint8 uValue ) const;
};

#include "Color.inl"

// Color constants
extern const Math::CColor colBLACK;
extern const Math::CColor colWHITE;
extern const Math::CColor colRED;
extern const Math::CColor colGREEN;
extern const Math::CColor colBLUE;
extern const Math::CColor colMAGENTA;
extern const Math::CColor colYELLOW;
extern const Math::CColor colCYAN;
extern const Math::CColor colORANGE;

} // namespace Math
#endif

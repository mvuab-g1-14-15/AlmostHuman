#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

namespace Math
{
    class CColor
    {
        public:

            float32 r, g, b, a;

            // Constructors
            explicit Math::CColor() : r( 1.0f), g(1.0f), b(1.0f), a(1.0f ) {}
            explicit Math::CColor( const Math::Vect4f& vect ) :
                r( vect.x ), g( vect.y), b( vect.z ), a( vect.w ) {}
            explicit Math::CColor( float32 red, float32 green, float32 blue,
                                   float32 alpha = 1.0f )
                : r( r ), g(green), b(blue), a( alpha ) {}

            // Getters and setters
            inline void Set( float32 red, float32 green, float32 blue, float32 alpha = 1.0f )
            {
                r = red;
                g = green;
                b = blue;
                a = alpha;
            }
            inline float32 GetRed() const
            {
                return r;
            }
            inline void SetRed( float32 red )
            {
                r = red;
            }
            inline float32 GetGreen() const
            {
                return g;
            }
            inline void SetGreen( float32 green )
            {
                g = green;
            }
            inline float32 GetBlue() const
            {
                return b;
            }
            inline void SetBlue( float32 blue )
            {
                b = blue;
            }
            inline float32 GetAlpha() const
            {
                return a;
            }
            inline void SetAlpha( float32 alpha )
            {
                a = alpha;
            }
            inline const Math::Vect4f GetArgb() const
            {
                return Math::Vect4f(r, g, b, a);
            }
            inline void SetArgb( const Math::Vect4f& argb )
            {
                r = argb.x;
                g = argb.y;
                b = argb.z;
                a = argb.w;
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
            inline Math::CColor & operator += ( const Math::CColor& otro );
            inline Math::CColor & operator -= ( const Math::CColor& otro );
            inline Math::CColor & operator *= ( const Math::CColor& otro );
            inline Math::CColor & operator *= ( float32 escalar );

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

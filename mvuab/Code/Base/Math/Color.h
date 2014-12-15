#ifndef COLOR_H_
#define COLOR_H_

#include "Vector4.h"

class CColor
{
private:
  Vect4f m_Color;

public:

    // Constructors
    explicit CColor () : m_Color(1.0f, 1.0f, 1.0f, 1.0f) {}
    explicit CColor (const Vect4f& vect) : m_Color(vect) {}
    explicit CColor (float32 red, float32 green, float32 blue, float32 alpha = 1.0f) :m_Color(red, green, blue, alpha) {}

    // Getters and setters
    inline void Set(float32 red, float32 green, float32 blue, float32 alpha = 1.0f)  { m_Color[0]=red; m_Color[1]=green; m_Color[2]=blue; m_Color[3]=alpha;  }
    inline float32 GetRed() const { return m_Color[0]; }
    inline void SetRed(float32 red) { m_Color[0] = red; }
    inline float32 GetGreen() const { return m_Color[1]; }
    inline void SetGreen(float32 green) { m_Color[1] = green; }
    inline float32 GetBlue() const { return m_Color[2]; }
    inline void SetBlue(float32 blue) { m_Color[2] = blue; }
    inline float32 GetAlpha() const { return m_Color[3]; }
    inline void SetAlpha(float32 alpha) { m_Color[3] = alpha; }
    inline const Vect4f& GetArgb () const { return m_Color; }
    inline void SetArgb(const Vect4f& argb) { m_Color = argb; }

    inline uint32 GetUint32Abgr() const;
    inline uint32 GetUint32Argb() const;
    inline void SetUint32Argb(int32 argb);
    inline void    SetUint32Abgr(int32 argb);
    inline CColor& Clamp();
    inline CColor& Lerp(const CColor& other, float32 f);
  
  // Aritmetic Operators
  inline        CColor  operator + (const CColor& otro)    const;
  inline        CColor  operator - (const CColor& otro)    const;
  inline        CColor  operator * (const CColor& otro)    const;
  inline        CColor  operator * (float32 escalar)            const;
  
  // Aritmetic Operators
  inline CColor& operator += (const CColor& otro);
  inline CColor& operator -= (const CColor& otro);
  inline CColor& operator *= (const CColor& otro);
  inline CColor& operator *= (float32 escalar);

private:
  inline uint8 float32ToByte (float32 fValue) const;
  inline float32 ByteTofloat32 (uint8 uValue) const;
};

#include "Color.inl"

// Color constants
extern const CColor colBLACK;
extern const CColor colWHITE;
extern const CColor colRED;
extern const CColor colGREEN;
extern const CColor colBLUE;
extern const CColor colMAGENTA;
extern const CColor colYELLOW;
extern const CColor colCYAN;
extern const CColor colORANGE;

#endif

//-----------------------------------------------------------------------
// Math Types
// Tipos y constantes de la librería matemática
//-----------------------------------------------------------------------

#ifndef __MATH_TYPES_H__
#define __MATH_TYPES_H__

// Constantes
#include "Utils/Types.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constantes numéricas
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const float64 ALG_EPSILON_float64 = (float64)1e-05;
const float32  ALG_EPSILON_float32  = (float32)ALG_EPSILON_float64;

const float64 QUAT_POLE_EPSILON_float64 = (float64)0.999950;
const float32  QUAT_POLE_EPSILON_float32  = (float32)0.99990f;

const float64 float64_PI_VALUE = 3.14159265358979323846;
const float32  float32_PI_VALUE  = (float32)float64_PI_VALUE;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Constantes numéricas con tipo templatizado
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T>  inline T      Epsilon           () { return 0; }
template<>            inline float32  Epsilon<float32>    () { return ALG_EPSILON_float32; }
template<>            inline float64 Epsilon<float64>   () { return ALG_EPSILON_float64; }

template<typename T>  inline T      One               () { return 1;    }
template<>            inline float32  One<float32>        () { return 1.0f; }
template<>            inline float64 One<float64>       () { return 1.0;  }

template<typename T>  inline T      MinusOne          () { return -1;    }
template<>            inline float32  MinusOne<float32>   () { return -1.0f; }
template<>            inline float64 MinusOne<float64>  () { return -1.0;  }

template<typename T>  inline T      Zero              () { return 0;    }
template<>            inline float32  Zero<float32>       () { return 0.0f; }
template<>            inline float64 Zero<float64>      () { return 0.0;  }

template<typename T>  inline T      Half              () { return (T)0.5; }
template<>            inline float32  Half<float32>       () { return 0.5f;   }
template<>            inline float64 Half<float64>      () { return 0.5;    }

template<typename T>  inline T      Two               () { return 2;    }
template<>            inline float32  Two<float32>        () { return 2.0f; }
template<>            inline float64 Two<float64>       () { return 2.0;  }

// Epsilon utilizado en la determinación de ángulos de Euler
template<typename T>  inline T      QuatPoleEpsilon         () { return 0; }
template<>            inline float32  QuatPoleEpsilon<float32>  () { return QUAT_POLE_EPSILON_float32; }
template<>            inline float64 QuatPoleEpsilon<float64> () { return QUAT_POLE_EPSILON_float64; }

// Constantes relacionadas con el número PI
template<typename T>  inline T      ePI            () { return (T)float64_PI_VALUE; }
template<>            inline float32  ePI<float32>     () { return float32_PI_VALUE;     }
template<>            inline float64 ePI<float64>    () { return float64_PI_VALUE;    }

template<typename T>  inline T      e2PI           () { return (T)(2.0 * float64_PI_VALUE); }
template<>            inline float32  e2PI<float32>    () { return (2.0f * float32_PI_VALUE);    }
template<>            inline float64 e2PI<float64>   () { return (2.0 * float64_PI_VALUE);    }

template<typename T>  inline T      ePI2           () { return (T)(0.5 * float64_PI_VALUE); }
template<>            inline float32  ePI2<float32>    () { return (0.5f * float32_PI_VALUE);    }
template<>            inline float64 ePI2<float64>   () { return (0.5 * float64_PI_VALUE);    }

template<typename T>  inline T      eInPI          () { return (T)(1.0 / float64_PI_VALUE); }
template<>            inline float32  eInPI<float32>   () { return (1.0f / float32_PI_VALUE);    }
template<>            inline float64 eInPI<float64>  () { return (1.0 / float64_PI_VALUE);    }

template<typename T>  inline T      eIn2PI         () { return (T)(1.0 / (2.0 * float64_PI_VALUE)); }
template<>            inline float32  eIn2PI<float32>  () { return (1.0f / (2.0f * float32_PI_VALUE));   }
template<>            inline float64 eIn2PI<float64> () { return (1.0 / (2.0 * float64_PI_VALUE));    }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// CONSTANTES DE PI CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static const float32 ePIf       = float32_PI_VALUE;
static const float32 e2PIf      = 2.0f * ePIf;
static const float32 ehalfPIf   = 0.5f * ePIf;
static const float32 eInPIf     = 1.0f / ePIf;
static const float32 eIn2PIf    = 1.0f / e2PIf;

static const float64 ePId    = float64_PI_VALUE;
static const float64 e2PId   = 2.0 * ePId;
static const float64 ePI2d   = 0.5 * ePId;
static const float64 eInPId  = 1.0 / ePId;
static const float64 eIn2PId = 1.0 / e2PId;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x3] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template<typename T> class Matrix33;
typedef Matrix33<float32>   Mat33f;
typedef Matrix33<float64>  Mat33d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [3x4] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Matrix34;
typedef Matrix34<float32>   Mat34f;
typedef Matrix34<float64>  Mat34d;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE MATRICES [4x4] CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Matrix44;
typedef Matrix44<float32>   Mat44f;
typedef Matrix44<float64>  Mat44d;
/*
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE CUATERNIONES CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Quatn;
typedef Quatn<float32>  Quatf;
typedef Quatn<float64> Quatd;
typedef Quatn<int32>  Quati;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE CUATERNION+POSICION CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class QuatPos;
typedef QuatPos<float32>  Transformf;
typedef QuatPos<float64> Transformd;
*/

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 2D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector2;
typedef Vector2<float32>  Vect2f;
typedef Vector2<float64> Vect2d;
typedef Vector2<int32>  Vect2i;
typedef Vector2<uint32> Vect2u;
typedef Vector2<uint16> Vect2w;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 3D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector3;
typedef Vector3<float32>  Vect3f;
typedef Vector3<float64> Vect3d;
typedef Vector3<uint16> Vect3w;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// TIPOS DE VECTORES 4D CON TIPOS CONCRETOS DE DATOS
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename T> class Vector4;
typedef Vector4<float32>  Vect4f;
typedef Vector4<float64> Vect4d;
typedef Vector4<int32>  Vect4i;
typedef Vector4<uint8>  Vect4u8;


#endif

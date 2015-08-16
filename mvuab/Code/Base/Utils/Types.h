#ifndef _TYPES_H_
#define _TYPES_H_

typedef signed char         int8;   // integer de 8 bits
typedef short               int16;  // integer de 16 bits
typedef int                 int32;  // integer de 32 bits

typedef __int64             int64;  // integer de 64 bits
typedef unsigned __int64    uint64; // integer positivo de 64 bits

typedef unsigned char       uint8;  // integer positivo de 8 bits
typedef unsigned short      uint16; // integer positivo de 16 bits
typedef unsigned int        uint32; // integer positivo de 32 bits

typedef float               float32;    // float32 de 32 bits
typedef double              float64;    // float32 de 64 bits

// use for functions, global at the context of the file
//#define internal static
//
//// normal static variable
//#define local_persist static
//
//// use for global vars, global at the context of the file, also sets the var to zero
//#define global_variable static

#ifndef float4
#define float4             Math::Vect4f;
#endif

#ifndef float3
#define float3             Math::Vect3f;
#endif

#ifndef float2
#define float2             Math::Vect2f;
#endif

#endif

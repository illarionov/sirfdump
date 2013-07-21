#ifndef SIRFDUMP_WINCE_H
#define SIRFDUMP_WINCE_H

#ifdef WINCE

typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;

typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;

typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;

typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;

#define snprintf _snprintf

#endif

#endif // SIRFDUMP_WINCE_H
/**
 * \file
 * \author Timothy B. Costa
 * \brief poreScale general definitions
 */

#ifndef _PORESCALE_DEFINE_H_
#define _PORESCALE_DEFINE_H_

#include <cstdint>
#include <cstddef>
#include <cstdarg>
#include <unordered_set>
#include <cassert>
#include "math.h"

#define MASTER_NODE 0
#define PORESCALE_COMM MPI_COMM_WORLD

#if defined(__PORESCALE_INT8__)

typedef int8_t			psInt;
typedef uint8_t			psUInt;
#define	PORESCALE_INTMAX	INT8_MAX
#define PORESCALE_UINTMAX	UINT8_MAX
#define	PORESCALE_INTMIN	INT8_MIN

#elif defined(__PORESCALE_INT16__)

typedef int16_t			psInt;
typedef uint16_t		psUInt;
#define	PORESCALE_INTMAX	INT16_MAX
#define PORESCALE_UINTMAX	UINT16_MAX
#define	PORESCALE_INTMIN	INT16_MIN

#elif defined(__PORESCALE_INT64__)

typedef int64_t			psInt;
typedef uint64_t		psUInt;
#define	PORESCALE_INTMAX	INT64_MAX
#define PORESCALE_UINTMAX	UINT64_MAX
#define	PORESCALE_INTMIN	INT64_MIN

#else // Default to 32bit integer types

/** \brief psInt_t default definition. */
typedef int32_t			psInt;
/** \brief psUInt_t default definition. */
typedef uint32_t		psUInt;
/** \brief Maximum integer size. */
#define	PORESCALE_INTMAX	INT32_MAX
/** \brief Maximum unsigned integer size. */
#define PORESCALE_UINTMAX	UINT32_MAX
/** \brief Minimum integer size. */
#define	PORESCALE_INTMIN	INT32_MIN

#endif

typedef int8_t			psInt8;
typedef uint8_t			psUInt8;
#define	PORESCALE_INT8MAX	INT8_MAX
#define PORESCALE_UINT8MAX	UINT8_MAX
#define	PORESCALE_INT8MIN	INT8_MIN

#define vBlock 1024

// 1d->2d index
#define idx2(i, j, ldi) ((i * ldi) + j)

// 1d->3d index
#define idx3(i, j, k, ldi1, ldi2) (k + (ldi2 * (j + ldi1 * i)))

#endif

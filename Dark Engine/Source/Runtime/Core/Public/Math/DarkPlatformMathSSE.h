#pragma once

#include "CoreTypes.h"
#include <xmmintrin.h>
#include <emmintrin.h>

#if !PLATFORM_ENABLE_VECTORINTRINSICS
template <class Base>
struct TDarkPlatformMathSSEBase : public BASE
{ };
#endif

namespace DE
{
	namespace SSE
	{
		FORCEINLINE float InvSqrt(float InValue)
		{
			const __m128 One = _mm_set_ss(1.0f);
			const __m128 Y0 = _mm_set_ss(InValue);
			const __m128 X0 = _mm_sqrt_ss(Y0);
			const __m128 R0 = _mm_div_ss(One, X0);
			float temp;
			_mm_store_ss(&temp, R0);
			return temp;
		}

		FORCEINLINE int32 TruncToInt32(float F)
		{
			return _mm_cvtt_ss2si(_mm_set_ss(F));
		}
		FORCEINLINE int32 TruncToInt32(double InValue)
		{
			return _mm_cvttsd_si32(_mm_set_sd(InValue));
		}
		FORCEINLINE int64 TruncToInt64(double InValue)
		{
			return _mm_cvttsd_si64(_mm_set_sd(InValue));
		}
	}
}

template<class Base>
struct TDarkPlatformMathSSEBase : public Base
{
	template<typename T>
	static FORCEINLINE int32 TruncToInt32(T F)
	{
		return DE::SSE::TruncToInt32(F);
	}
	
	static FORCEINLINE int64 TruncToInT64(double F)
	{
		return DE::SSE::TruncToInt64(F);
	}




	static FORCEINLINE int32 TruncToInt(float F) { return TruncToInt32(F); }
	static FORCEINLINE int64 TruncToInt(double F) { return TruncToInT64(F); }
};
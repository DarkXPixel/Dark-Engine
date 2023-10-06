#pragma once
#include <HAL/Platform.h>

namespace DE::Math
{
	template<typename T> struct TVector;
	template<typename T> struct TVector2;
}


// ISPC forward declarations
namespace ispc
{
	struct FVector;
	struct FVector2D;
	struct FVector4;
	struct FQuat;
	struct FMatrix;
	struct FPlane;
	struct FTransform;
}


using FVector =					DE::Math::TVector<double>;
using FVector2D =				DE::Math::TVector2<double>;


using FVector3d =				DE::Math::TVector<double>;
using FVector2d =				DE::Math::TVector<double>;

using FVector3f =				DE::Math::TVector<float>;
using FVector2f =				DE::Math::TVector2<float>;
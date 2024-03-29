#pragma once
#include "Memory/TUniquePtr.h"
#include "GenericWindow.h"
#include "Math/MathFwd.h"


namespace EMouseButtons
{
	enum Type
	{
		Left,
		Middle,
		Right,
		Thumb01,
		Thumb02,
		Invalid
	};
}



class FGenericApplicationHandler
{
public:
	virtual bool OnMouseDown(const TSharedPtr<FGenericWindow>& Window,
		const EMouseButtons::Type Button, const FVector2D CursorPos)
	{
		return false;
	}
	virtual bool OnMouseUp(const TSharedPtr<FGenericWindow>& Window,
		const FVector2D CursorPos)
	{
		return false;
	}

	virtual bool OnMouseMove()
	{
		return false;
	}
	virtual bool OnRawMouseMove(const int32 X, const int32 Y)
	{
		return false;
	}

	virtual bool OnSizeChanged(const TSharedPtr<FGenericWindow>& Window, const int32 Width, const int32 Height, bool bWasMinimized = false)
	{
		return false;
	}







};
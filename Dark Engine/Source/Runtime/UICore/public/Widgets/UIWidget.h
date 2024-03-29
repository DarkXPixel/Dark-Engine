#pragma once
#include "Math/MathFwd.h"
#include "Containers/Array.h"
#include "Memory/TUniquePtr.h"
#include <functional>


class UIWidget
{
	friend class UIWindow;
public:
	UIWidget() {}
	virtual void Update(float DeltaTime)
	{
		ForEachChild([DeltaTime](UIWidget* Widget)
			{
				Widget->Update(DeltaTime);
			});
	}
	virtual void DrawImGui() {}
	
	void SetSize(FIntPoint InSize) { Size = InSize; }
	void SetPostion(FIntPoint InPosition) { Position = InPosition; }
	FString	GetName() const { return Name; }
	void SetName(FString InName) { Name = InName; }
	void AddChild(TSharedPtr<UIWidget> Child) { ChildWidgets.Add(Child); }
	FIntRect GetRect() { return Rect; }

protected:
	//void ForEachChild(void(*Func)(UIWidget*));
	void ForEachChild(std::function<void(UIWidget*)> Func);

private:
	void InitWindow(UIWindow* InWindow) { Owner = InWindow; }

protected:
	FIntPoint Position;
	FIntRect Rect;
	FIntPoint Size;
	TArray<TSharedPtr<UIWidget>> ChildWidgets;
	UIWindow* Owner;
	FString Name;





};
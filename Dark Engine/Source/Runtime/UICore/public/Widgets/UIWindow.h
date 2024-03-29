#pragma once
#include "GenericPlatform/GenericWindow.h"
#include "Memory/TUniquePtr.h"
#include "CoreMinimal.h"
#include "UIWidget.h"
#include "DarkClient.h"
#include "imgui.h"


class UIWindow
{
public:
	UIWindow() {}
	~UIWindow();

public:
	TSharedPtr<FGenericWindow> GetNativeWindow() const { return NativeWindow; }
	void SetNativeWindow(TSharedPtr<FGenericWindow> InNativeWindow);

	void ShowWindow();
	void Maximize() { NativeWindow->Maximize(); }

	void Update(float DeltaTime);

	FVector2f GetInitSizeInScreen() const { return InitSizeScreen; }
	FVector2f GetInitPositionInScreen() const { return InitPositionScreen; }
	bool HasWindowBorder() const { return bHasWindowBorder; }
	void SetbWindowBorder(bool InbHasWindowBorder) { bHasWindowBorder = InbHasWindowBorder; }
	FString GetTitle() const { return Title; }
	void SetTitle(FString InTitle) { Title = InTitle; }
	void AddWidget(TSharedPtr<UIWidget> InWidget) { InWidget->InitWindow(this); Widgets.Add(InWidget); }
	//void SetViewportClient(TSharedPtr<FViewportClient> InViewportClient) { ViewportClient = InViewportClient; }
	void SetImGuiContext(ImGuiContext* InContext) { IMGUIContext = InContext; }
	FVector2f GetViewportSize() { return GetInitSizeInScreen(); }
	const TArray<TSharedPtr<UIWidget>>& GetWidgets() const { return Widgets; }

	TSharedPtr<FViewportClient> CreateViewportClient();
	void CreateViewport();
	void DrawWindow();

	FIntPoint GetMousePosition() const;


private:
	bool bHasWindowBorder = true;
	TSharedPtr<FGenericWindow> NativeWindow;
	FVector2f InitSizeScreen = {1280, 720};
	FVector2f InitPositionScreen;
	FString Title;
	//TSharedPtr<FViewportClient>
	TArray<TSharedPtr<UIWidget>> Widgets;
	//TSharedPtr<FUIWindowViewport> Viewport;
	TSharedPtr<FRHIViewport> Viewport;
	ImGuiContext* IMGUIContext;
};
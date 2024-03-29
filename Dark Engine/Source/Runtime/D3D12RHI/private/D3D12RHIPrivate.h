#pragma once
#include "D3D12RHI.h"
#include "D3D12Adapter.h"
#include "D3D12Viewport.h"
#include "D3D12Descriptors.h"
#include "DynamicRHI.h"
#include "RHIContext.h"
#include "D3D12Queue.h"




class FD3D12DynamicRHI final :public ID3D12PlatformDynamicRHI
{
	static FD3D12DynamicRHI* SingleD3D12RHI;

public:
	static FD3D12DynamicRHI* GetD3D12RHI() { return SingleD3D12RHI; }

	FD3D12DynamicRHI(const TArray<TSharedPtr<FD3D12Adapter>>& InChosenAdapters);

	~FD3D12DynamicRHI();

	FD3D12Adapter& GetAdapter(int32 Index = 0) { return *ChosenAdapters[Index]; }
	const FD3D12Adapter& GetAdapter(int32 Index = 0) const { return *(ChosenAdapters[Index]); }

	const uint64 GetNumAdapters() const { return /*ChosenAdapters.GetSize()*/0; }

	void Init() override;

	void PostInit() override;

	//virtual void PostInit();
	virtual void Shutdown() {}

	virtual TSharedPtr<FRHIViewport> RHICreateViewport(void* WindowHandle, uint32 SizeX, uint32 SizeY, bool bIsFullscreen) override;


	virtual IRHIComputeContext* RHIGetCommandContext() override;
	virtual IRHICommandContext* RHIGetDefaultContext() override;

	class FD3D12CommandContext* CreateCommandContext(FD3D12Device* InParent, ED3D12QueueType InQueueType, bool InIsDefaultContext);;

	virtual FRHITexture* RHIGetViewportBackBuffer(FRHIViewport* Viewport) override;


	virtual ID3D12CommandQueue* RHIGetCommandQueue() override { return nullptr; }
	virtual ID3D12Device* RHIGetDevice() { return nullptr; }
	virtual ID3D12GraphicsCommandList* RHIGetGraphicsCommandList(uint32 InDeviceIndex) { return nullptr; }
	virtual DXGI_FORMAT RHIGetSwapChainFormat() const { return DXGI_FORMAT_UNKNOWN; }



#ifdef IMGUI
	FD3D12DescriptorHeap* ImGuiDescriptorHeap;
	uint32 ImGuiDescriptorHandle = 0;
#endif

public:
	FD3D12Texture* CreateD3D12Texture(const FRHITextureCreateDesc& CreateDesc, FD3D12Device* Device)
	{
		return new FD3D12Texture(CreateDesc, Device);
	}


private:
	TArray<TSharedPtr<FD3D12Adapter>> ChosenAdapters;
	D3D_FEATURE_LEVEL FeatureLevel;





};


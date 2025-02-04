#pragma once
#include <GameEngineBase/GameEngineMath.h>
#include <GameEngineBase/GameEngineWindow.h>

// ���� :
class GameEngineRenderTarget;
class GameEngineDevice
{
public:
	~GameEngineDevice() {}

	// delete Function
	GameEngineDevice(const GameEngineDevice& _Other) = delete;
	GameEngineDevice(GameEngineDevice&& _Other) noexcept = delete;
	GameEngineDevice& operator=(const GameEngineDevice& _Other) = delete;
	GameEngineDevice& operator=(GameEngineDevice&& _Other) noexcept = delete;

	static void Initialize();
	static void DeviceCreate();
	static void CreateSwapChain();

	static void RenderStart();
	static void RenderEnd();

	static ID3D11Device* GetDevice()
	{
		return Device_;
	}
	static ID3D11DeviceContext* GetContext()
	{
		return Context_;
	}

	static GameEngineRenderTarget* GetBackBuffer()
	{
		return BackBufferTarget;
	}

	static IDXGIAdapter* GetHighPerformanceAdapter();

	static void Destroy();

protected:

private:
	static ID3D11Device* Device_;
	static ID3D11DeviceContext* Context_;
	
	static IDXGISwapChain* SwapChain_;

	static class GameEngineRenderTarget* BackBufferTarget;

	GameEngineDevice();
};


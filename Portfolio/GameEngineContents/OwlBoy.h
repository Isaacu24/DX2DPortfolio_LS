#pragma once
#include <GameEngineCore/GameEngineCore.h>

class OwlBoy : public GameEngineCore
{
public:
	OwlBoy();
	~OwlBoy();

	OwlBoy(const OwlBoy& _Other) = delete;
	OwlBoy(OwlBoy&& _Other) noexcept = delete;
	OwlBoy& operator=(const OwlBoy& _Other) = delete;
	OwlBoy& operator=(OwlBoy&& _Other) noexcept = delete;

	std::string GetWindowTitle() override
	{
		return "MyGame";
	}

protected:
	void UserStart() override;
	void UserUpdate(float _DeltaTime) override;
	void UserEnd() override;

	void ResourceInit();

private:

};


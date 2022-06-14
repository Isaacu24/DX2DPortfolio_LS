#pragma once
#include <GameEngineCore/GameEngineCore.h>

class Blasphemous : public GameEngineCore
{
public:
	Blasphemous();
	~Blasphemous();

	Blasphemous(const Blasphemous& _Other) = delete;
	Blasphemous(Blasphemous&& _Other) noexcept = delete;
	Blasphemous& operator=(const Blasphemous& _Other) = delete;
	Blasphemous& operator=(Blasphemous&& _Other) noexcept = delete;

	std::string GetWindowTitle() override
	{
		return "Blasphemous";
	}

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:

};


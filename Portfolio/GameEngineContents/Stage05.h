#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include "StageBase.h"

class Stage05 : public GameEngineLevel, public StageBase
{
public:
	Stage05();
	~Stage05();

	Stage05(const Stage05& _Other) = delete;
	Stage05(Stage05&& _Other) noexcept = delete;
	Stage05& operator=(const Stage05& _Other) = delete;
	Stage05& operator=(Stage05&& _Other) noexcept = delete;

protected:
	void Start() override;
	void Update(float _DeltaTime) override;
	void End() override;

private:
};


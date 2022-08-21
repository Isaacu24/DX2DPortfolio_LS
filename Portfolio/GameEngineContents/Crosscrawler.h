#pragma once
#include "NormalMonster.h"

class Crosscrawler : public NormalMonster
{
public:
	Crosscrawler();
	~Crosscrawler();

	Crosscrawler(const Crosscrawler& _Other) = delete;
	Crosscrawler(Crosscrawler&& _Other) noexcept = delete;
	Crosscrawler& operator=(const Crosscrawler& _Other) = delete;
	Crosscrawler& operator=(Crosscrawler&& _Other) noexcept = delete;

    void IdleStart(const StateInfo& _Info);
    void IdleUpdate(float _DeltaTime, const StateInfo& _Info);
    void IdleEnd(const StateInfo& _Info);

protected:
    void Start() override;
    void Update(float _DeltaTime) override;
    void End() override;

    void PatrolMoveX(float _DeltaTime) override;

private:

};


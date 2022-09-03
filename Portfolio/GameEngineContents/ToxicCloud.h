#pragma once
#include "Projectile.h"
#include <GameEngineCore/GameEngineActor.h>

class ToxicCloud
    : public Projectile
    , public GameEngineActor
{
public:
    ToxicCloud();
    ~ToxicCloud();

    ToxicCloud(const ToxicCloud& _Other)                = delete;
    ToxicCloud(ToxicCloud&& _Other) noexcept            = delete;
    ToxicCloud& operator=(const ToxicCloud& _Other)     = delete;
    ToxicCloud& operator=(ToxicCloud&& _Other) noexcept = delete;

    void ShootStart(const StateInfo& _Info);
    void ShootUpdate(float _DeltaTime, const StateInfo& _Info);

    void ExplosionStart(const StateInfo& _Info);
    void ExplosionUpdate(float _DeltaTime, const StateInfo& _Info);

    bool Explosion(GameEngineCollision* _This, GameEngineCollision* _Other)
    {
        State_.ChangeState("Explosion");

        return true;
    }

protected:
    void Start() override;
    void Update(float _DeltaTime) override;
    void End() override;

    void Shoot(float _DeltaTime) override 
    { 
        Speed_ += _DeltaTime * 300;
        GetTransform().SetWorldMove({(Dir_.x * Speed_ * _DeltaTime), (Dir_.y * Speed_ * _DeltaTime)}); 
    }

private:
    GameEngineTextureRenderer* ExplsionV1_;
};

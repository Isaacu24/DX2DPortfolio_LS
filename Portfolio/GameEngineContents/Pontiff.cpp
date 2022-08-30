#include "PreCompile.h"
#include "Pontiff.h"
#include "GiantSword.h"
#include "PlatformSpawner.h"
#include "PontiffMainBody.h"


Pontiff::Pontiff() {}

Pontiff::~Pontiff() {}

void Pontiff::Start()
{
    Helmet_ = CreateComponent<GameEngineTextureRenderer>();
    Body_   = CreateComponent<GameEngineTextureRenderer>();
    Face_   = CreateComponent<GameEngineTextureRenderer>();

    MainBody_ = GetLevel()->CreateActor<PontiffMainBody>();
    MainBody_->Off();

    Helmet_->CreateFrameAnimationCutTexture("pontiff_idle_helmet", {"pontiff_idle_helmet.png", 0, 30, 0.2f, true});

    Helmet_->CreateFrameAnimationCutTexture("pontiff_opening_helmet",
                                            {"pontiff_opening_helmet.png", 0, 0, 0.0f, false});

    Helmet_->CreateFrameAnimationCutTexture("pontiff_closing_helmet",
                                            {"pontiff_closing_helmet.png", 0, 0, 0.0f, false});
    Helmet_->GetTransform().SetLocalScale({900, 1300});
    Helmet_->GetTransform().SetLocalMove({0, 50});

    Body_->CreateFrameAnimationCutTexture("pontiff_idle_torso", {"pontiff_idle_torso.png", 0, 30, 0.2f, true});
    Body_->CreateFrameAnimationCutTexture("pontiff_opening_torso", {"pontiff_opening_torso.png", 0, 17, 0.1f, true});
    Body_->CreateFrameAnimationCutTexture("pontiff_closing_torso", {"pontiff_closing_torso.png", 0, 14, 0.1f, true});
    Body_->GetTransform().SetLocalScale({900, 1100});
    Body_->GetTransform().SetLocalMove({0, 35});

    Face_->CreateFrameAnimationCutTexture("pontiff_openIdle_face", {"pontiff_openIdle_face.png", 0, 30, 0.2f, true});

    Face_->CreateFrameAnimationCutTexture("pontiff_opening_face", {"pontiff_opening_face.png", 0, 17, 0.1f, false});

    Face_->CreateFrameAnimationCutTexture("pontiff_closing_face", {"pontiff_closing_face.png", 0, 14, 0.1f, false});

    Face_->CreateFrameAnimationCutTexture("pontiff_openedIdle_face_DEATH",
                                          {"pontiff_openedIdle_face_DEATH.png", 0, 64, 0.1f, false});

    Face_->AnimationBindFrame("pontiff_openedIdle_face_DEATH",
                                [&](const FrameAnimation_DESC& _Info)
                                {
                                    if (35 == _Info.CurFrame)
                                    {
                                        MainBody_->On();
                                        MainBody_->GetTransform().SetWorldPosition(
                                            {GetTransform().GetWorldPosition().x,
                                             GetTransform().GetWorldPosition().y - 200,
                                             static_cast<int>(ACTORORDER::BeforeLayer5)});

                                        IsAscension_ = true;
                                    }
                                });


    Face_->GetTransform().SetLocalScale({950, 1300});
    Face_->GetTransform().SetLocalMove({0, 50, -1});

    PlatformSpawner_ = GetLevel()->CreateActor<PlatformSpawner>();

    State_.CreateStateMember("Appear",
                             std::bind(&Pontiff::AppearUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::AppearStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::AppearEnd, this, std::placeholders::_1));

    State_.CreateStateMember("Opening",
                             std::bind(&Pontiff::OpeningUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::OpeningStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::OpeningEnd, this, std::placeholders::_1));

    State_.CreateStateMember("Closing",
                             std::bind(&Pontiff::ClosingUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::ClosingStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::ClosingEnd, this, std::placeholders::_1));

    State_.CreateStateMember("OpenIdle",
                             std::bind(&Pontiff::OpenIdleUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::OpenIdleStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::OpenIdleEnd, this, std::placeholders::_1));

    State_.CreateStateMember("CloseIdle",
                             std::bind(&Pontiff::CloseIdleUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::CloseIdleStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::CloseIdleEnd, this, std::placeholders::_1));

    State_.CreateStateMember("Death",
                             std::bind(&Pontiff::DeathUpdate, this, std::placeholders::_1, std::placeholders::_2),
                             std::bind(&Pontiff::DeathStart, this, std::placeholders::_1),
                             std::bind(&Pontiff::DeathEnd, this, std::placeholders::_1));
    State_.ChangeState("Appear");

    GiantSword_ = GetLevel()->CreateActor<GiantSword>();
    GiantSword_->GetTransform().SetWorldPosition({1400, -600, static_cast<int>(ACTORORDER::BossMonster)});

    BossUI_ = GetLevel()->CreateActor<BossUI>();
    BossUI_->SetBossMonster(this);
    BossUI_->SetBossUI();
    BossUI_->AllOff();

    BodyCollider_ = CreateComponent<GameEngineCollision>();
    BodyCollider_->ChangeOrder(COLLISIONORDER::BossMonster);
    BodyCollider_->SetDebugSetting(CollisionType::CT_OBB2D, float4{1.0f, 0.0f, 1.0f, 0.5f});
    BodyCollider_->GetTransform().SetWorldScale({150.f, 150.f, 1.0f});
    BodyCollider_->GetTransform().SetWorldMove({-30, 70});
}

void Pontiff::Update(float _DeltaTime)
{
    State_.Update(_DeltaTime);

    if ("Death" != State_.GetCurStateStateName())
    {
        MonsterBase::DamageCheck(10.f);
    }
}

void Pontiff::End() {}


void Pontiff::AppearStart(const StateInfo& _Info)
{
    Helmet_->ChangeFrameAnimation("pontiff_opening_helmet");
    Body_->ChangeFrameAnimation("pontiff_idle_torso");
    Face_->Off();
}

void Pontiff::AppearUpdate(float _DeltaTime, const StateInfo& _Info)
{
    Time_ += _DeltaTime;

    if (3.f <= Time_)
    {
        Time_ = 0.f;
        State_.ChangeState("CloseIdle");
    }
}

void Pontiff::AppearEnd(const StateInfo& _Info)
{
    Face_->On();
    BossUI_->AllOn();
    BossUI_->SetBossName("마지막 기적의 아들");
    BossUI_->SetFontPosition({480, 590, -100.f});
    BossUI_->SetFontSize(35);

    PlatformSpawner_->CreateFristPattern();
}

void Pontiff::OpeningStart(const StateInfo& _Info)
{
    Helmet_->ChangeFrameAnimation("pontiff_opening_helmet");
    Body_->ChangeFrameAnimation("pontiff_opening_torso");
    Face_->ChangeFrameAnimation("pontiff_opening_face");
}

void Pontiff::OpeningUpdate(float _DeltaTime, const StateInfo& _Info) {}

void Pontiff::OpeningEnd(const StateInfo& _Info) {}

void Pontiff::OpenIdleStart(const StateInfo& _Info)
{
    Helmet_->ChangeFrameAnimation("pontiff_idle_helmet");
    Body_->ChangeFrameAnimation("pontiff_idle_torso");
    Face_->ChangeFrameAnimation("pontiff_openIdle_face");
}

void Pontiff::OpenIdleUpdate(float _DeltaTime, const StateInfo& _Info) {}
void Pontiff::OpenIdleEnd(const StateInfo& _Info) {}

void Pontiff::ClosingStart(const StateInfo& _Info)
{
    Helmet_->ChangeFrameAnimation("pontiff_closing_helmet");
    Body_->ChangeFrameAnimation("pontiff_closing_torso");
    Face_->ChangeFrameAnimation("pontiff_closing_face");
}

void Pontiff::ClosingUpdate(float _DeltaTime, const StateInfo& _Info) {}
void Pontiff::ClosingEnd(const StateInfo& _Info) {}

void Pontiff::CloseIdleStart(const StateInfo& _Info)
{
    Face_->Off();
    Helmet_->ChangeFrameAnimation("pontiff_idle_helmet");
    Body_->ChangeFrameAnimation("pontiff_idle_torso");
}

void Pontiff::CloseIdleUpdate(float _DeltaTime, const StateInfo& _Info) {}

void Pontiff::CloseIdleEnd(const StateInfo& _Info) { Face_->On(); }


void Pontiff::DeathStart(const StateInfo& _Info)
{
    AscensionSpeed_ = 100;

    PlatformSpawner_->SetSpawnerOrder(SpawnerOrder::Death);

    Face_->ChangeFrameAnimation("pontiff_openedIdle_face_DEATH");

    BossUI_->AllOff();

    Body_->CurAnimationPauseOn();
    Helmet_->CurAnimationPauseOn();
}

void Pontiff::DeathUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (true == IsAscension_)
    {
        AscensionSpeed_ += 500.f * _DeltaTime;
        MainBody_->GetTransform().SetWorldMove(float4::UP * AscensionSpeed_ * _DeltaTime);
    }
}

void Pontiff::DeathEnd(const StateInfo& _Info) {}

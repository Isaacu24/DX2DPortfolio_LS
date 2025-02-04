﻿#include "PreCompile.h"
#include "PlayerUI.h"
#include "LeftTopUI.h"

PlayerUI::PlayerUI() {}

PlayerUI::~PlayerUI() {}

void PlayerUI::UIAllOn()
{
    HPBar_->On();
    MPBar_->On();
    BarFrame_->On();
    TearFrame_->On();
    ScreenRenderer_->On();
    BackRenderer_->On();

    for (size_t i = 0; i < TearRenderers_.size(); i++)
    {
        if (nullptr != TearRenderers_[i])
        {
            TearRenderers_[i]->On();
        }
    }

    for (size_t i = 0; i < Flasks_.size(); i++)
    {
        if (nullptr != Flasks_[i])
        {
            Flasks_[i]->On();
        }
    }
}
void PlayerUI::UIAllOff()
{
    HPBar_->Off();
    MPBar_->Off();
    BarFrame_->Off();
    Inventory_->Off();
    TearFrame_->Off();
    ScreenRenderer_->Off();
    BackRenderer_->Off();

    for (size_t i = 0; i < TearRenderers_.size(); i++)
    {
        if (nullptr != TearRenderers_[i])
        {
            TearRenderers_[i]->Off();
        }
    }

    for (size_t i = 0; i < Flasks_.size(); i++)
    {
        if (nullptr != Flasks_[i])
        {
            Flasks_[i]->Off();
        }
    }
}

void PlayerUI::OnInventoryTear()
{
    for (size_t i = 0; i < TearRenderers_.size(); i++)
    {
        if (nullptr != TearRenderers_[i])
        {
            TearRenderers_[i]->GetTransform().SetWorldMove({80, 20, -10});
        }
    }
}

void PlayerUI::OffInventoryTear()
{
    for (size_t i = 0; i < TearRenderers_.size(); i++)
    {
        if (nullptr != TearRenderers_[i])
        {
            TearRenderers_[i]->GetTransform().SetWorldMove({-80, -20, 10});
        }
    }
}


void PlayerUI::Start()
{
    HPBar_ = GetLevel()->CreateActor<LeftTopUI>();
    HPBar_->Renderer_->SetTexture("Player_HP.png");
    HPBar_->Renderer_->ScaleToTexture();
    HPBar_->SetLevelOverOn();

    float HP = Penitent::GetMainPlayer()->GetHP() / 100.f;
    HPBar_->Renderer_->SetUVData(HP);

    MPBar_ = GetLevel()->CreateActor<LeftTopUI>();
    MPBar_->Renderer_->SetTexture("Player_MP.png");
    MPBar_->Renderer_->ScaleToTexture();
    MPBar_->SetLevelOverOn();

    Penitent::GetMainPlayer()->SetMP(100);
    float MP = Penitent::GetMainPlayer()->GetMP() / 100.f;

    MPBar_->Renderer_->SetUVData(MP);

    BarFrame_ = GetLevel()->CreateActor<LeftTopUI>();
    BarFrame_->Renderer_->SetTexture("Player_HpBar.png");
    BarFrame_->Renderer_->ScaleToTexture();
    BarFrame_->SetLevelOverOn();

    TearFrame_ = CreateComponent<GameEngineUIRenderer>();
    TearFrame_->SetTexture("TearFrame.png");
    TearFrame_->ScaleToTexture();

    Inventory_ = GetLevel()->CreateActor<Inventory>(2000);
    Inventory_->SetLevelOverOn();
    Inventory_->Off();

    HPBar_->GetTransform().SetWorldPosition({-499, 308, static_cast<int>(UIORDER::PlayerUI)});
    MPBar_->GetTransform().SetWorldPosition({-442, 284, static_cast<int>(UIORDER::PlayerUI)});
    BarFrame_->GetTransform().SetWorldPosition({-600, 330, static_cast<int>(UIORDER::PlayerUIFrame)});
    TearFrame_->GetTransform().SetWorldPosition({500, 270, static_cast<int>(UIORDER::PlayerUIFrame)});

    Flasks_.resize(3);

    for (size_t i = 0; i < 3; i++)
    {
        Flasks_[i] = CreateComponent<GameEngineUIRenderer>();
        Flasks_[i]->SetTexture("Full_Flask.png");
        Flasks_[i]->ScaleToTexture();

        Flasks_[i]->GetTransform().SetWorldPosition({-440.f + (40.f * i), 240, static_cast<int>(UIORDER::PlayerUI)});
    }

    TearRenderers_.resize(10);

    BackRenderer_ = CreateComponent<GameEngineUIRenderer>();
    BackRenderer_->SetTexture("BlackBackground.png");
    BackRenderer_->ScaleToTexture();
    BackRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, 0.0f};
    BackRenderer_->GetTransform().SetWorldPosition({0, 0, static_cast<int>(UIORDER::BackScreenUI)});

    ScreenRenderer_ = CreateComponent<GameEngineUIRenderer>();
    ScreenRenderer_->SetTexture("Death_Screen.png");
    ScreenRenderer_->ScaleToTexture();
    ScreenRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, 0.0f};
    ScreenRenderer_->GetTransform().SetWorldPosition({0, 0, static_cast<int>(UIORDER::ScreenUI)});

    ScreenState_.CreateStateMember(
        "PlayerDeath",
        std::bind(&PlayerUI::PlayerDeathUpdate, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&PlayerUI::PlayerDeathStart, this, std::placeholders::_1),
        std::bind(&PlayerUI::PlayerDeathEnd, this, std::placeholders::_1));

    ScreenState_.CreateStateMember(
        "BossDeath",
        std::bind(&PlayerUI::BossDeathUpdate, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&PlayerUI::BossDeathStart, this, std::placeholders::_1),
        std::bind(&PlayerUI::BossDeathEnd, this, std::placeholders::_1));

    ScreenState_.CreateStateMember(
        "FinalBossDeath",
        std::bind(&PlayerUI::FinalBossDeathUpdate, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&PlayerUI::FinalBossDeathStart, this, std::placeholders::_1),
        std::bind(&PlayerUI::FinalBossDeathEnd, this, std::placeholders::_1));

    ScreenState_.CreateStateMember(
        "BehindScreen",
        std::bind(&PlayerUI::BehindScreenUpdate, this, std::placeholders::_1, std::placeholders::_2),
        std::bind(&PlayerUI::BehindScreenStart, this, std::placeholders::_1),
        std::bind(&PlayerUI::BehindScreenEnd, this, std::placeholders::_1));

    ScreenState_.ChangeState("BehindScreen");
}

void PlayerUI::Update(float _DeltaTime)
{
    ScreenState_.Update(_DeltaTime);

    DamageCheck(_DeltaTime);
}

void PlayerUI::End() {}

void PlayerUI::LevelStartEvent() {}

void PlayerUI::LevelEndEvent() { ScreenState_.ChangeState("BehindScreen"); }


void PlayerUI::SetTear(int _Value)
{
    std::vector<int> Numbers;

    for (size_t i = 0; i < TearRenderers_.size(); i++)
    {
        if (nullptr != TearRenderers_[i])
        {
            TearRenderers_[i]->Off();
        }
    }

    while (true)
    {
        int Temp = _Value % 10;

        Numbers.push_back(Temp);

        _Value /= 10;

        if (_Value < 10)
        {
            if (0 != _Value)
            {
                Numbers.push_back(_Value);
            }

            break;
        }
    }

    for (size_t i = 0; i < Numbers.size(); i++)
    {
        if (nullptr == TearRenderers_[i])
        {
            TearRenderers_[i] = CreateComponent<GameEngineUIRenderer>();
        }

        if (i == Numbers.size())
        {
            if (0 == Numbers[i])
            {
                return;
            }
        }

        TearRenderers_[i]->On();
        TearRenderers_[i]->SetTexture(std::to_string(Numbers[i]) + ".png");
        TearRenderers_[i]->ScaleToTexture();
        TearRenderers_[i]->GetTransform().SetWorldPosition(
            {505.f - (10.f * i), 270.f, static_cast<int>(UIORDER::NumberUI)});
    }
}

void PlayerUI::UseFlask(int _Index)
{
    Flasks_[_Index]->SetTexture("Empty_Flask.png");

    float HP = Penitent::GetMainPlayer()->GetHP() / 100.f;
    HPBar_->Renderer_->SetUVData(HP);
}

void PlayerUI::FillFlask(int _Index) 
{
    SoundPlayer_ = GameEngineSound::SoundPlayControl("FLASK_REFILL.wav");
    Flasks_[_Index]->SetTexture("Full_Flask.png"); 
}


void PlayerUI::DamageCheck(float _DeltaTime)
{
    PlayerCurHp_ = Penitent::GetMainPlayer()->GetHP();

    if (PlayerPrevHp_ != PlayerCurHp_)
    {
        HPAlpha_ += _DeltaTime * 2;

        float CurHP  = static_cast<float>(PlayerCurHp_);
        float PrevHP = static_cast<float>(PlayerPrevHp_);

        LerpHp_ = GameEngineMath::LerpLimit(PrevHP, CurHP, HPAlpha_);

        if (LerpHp_ - CurHP < 0.05f)
        {
            HPAlpha_      = 0.f;
            LerpHp_       = CurHP;
            PlayerPrevHp_ = PlayerCurHp_;
        }
    }

    HPBar_->Renderer_->SetUVData(LerpHp_ / 100.f);
}


void PlayerUI::PlayerDeathStart(const StateInfo& _Info)
{
    SoundPlayer_ = GameEngineSound::SoundPlayControl("Player_Death.wav");

    ScreenRenderer_->SetTexture("Death_Screen.png");
    ScreenRenderer_->ScaleToTexture();
    ScreenRenderer_->GetTransform().SetWorldScale(ScreenRenderer_->GetTransform().GetWorldScale());

    Alpha_      = 0.f;
    ScreenTime_ = 0.f;

    IsReverse_ = false;
}

void PlayerUI::PlayerDeathUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (false == IsReverse_)
    {
        Alpha_ += _DeltaTime * 0.4f;

        if (1 < Alpha_)
        {
            Alpha_ = 1.0f;
            ScreenTime_ += _DeltaTime;
        }

        if (2.f < ScreenTime_)
        {
            ScreenTime_ = 0.f;
            IsReverse_  = true;
        }
    }

    else
    {
        Penitent::GetMainPlayer()->Off();
        GEngine::ChangeLevel(RespawnLevelName_);

        ScreenState_.ChangeState("BehindScreen");
    }

    ScreenRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, Alpha_};
}

void PlayerUI::PlayerDeathEnd(const StateInfo& _Info) {}


void PlayerUI::BossDeathStart(const StateInfo& _Info)
{
    SoundPlayer_ = GameEngineSound::SoundPlayControl("Boss_Fight_Ending.wav");

    ScreenRenderer_->SetTexture("boss-defeated-screen-title.png");
    ScreenRenderer_->ScaleToTexture();
    ScreenRenderer_->GetTransform().SetWorldScale(ScreenRenderer_->GetTransform().GetWorldScale() * 2);

    Alpha_      = 0.f;
    BackAlpha_  = 0.f;
    ScreenTime_ = 0.f;

    IsReverse_ = false;
}

void PlayerUI::BossDeathUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (false == IsReverse_)
    {
        Alpha_ += _DeltaTime * 0.4f;
        BackAlpha_ += _DeltaTime * 0.2f;

        if (1 < Alpha_)
        {
            Alpha_ = 1.0f;
            ScreenTime_ += _DeltaTime;
        }

        if (0.5f < BackAlpha_)
        {
            BackAlpha_ = 0.5f;
        }

        if (2.f < ScreenTime_)
        {
            ScreenTime_ = 0.f;
            IsReverse_  = true;
        }
    }

    else
    {
        Alpha_ -= _DeltaTime * 0.4f;
        BackAlpha_ -= _DeltaTime * 0.2f;

        if (0 > Alpha_)
        {
            Alpha_ = 0.0f;
            ScreenState_.ChangeState("BehindScreen");
        }

        if (0 > BackAlpha_)
        {
            BackAlpha_ = 0.0f;
            ScreenState_.ChangeState("BehindScreen");
        }
    }

    ScreenRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, Alpha_};
    BackRenderer_->GetColorData().MulColor   = float4{1.f, 1.f, 1.f, BackAlpha_};
}

void PlayerUI::BossDeathEnd(const StateInfo& _Info) {}

void PlayerUI::FinalBossDeathStart(const StateInfo& _Info)
{
    SoundPlayer_ = GameEngineSound::SoundPlayControl("Boss_Fight_Ending.wav");

    ScreenRenderer_->SetTexture("pontiff-defeated-screen-title.png");
    ScreenRenderer_->ScaleToTexture();
    ScreenRenderer_->GetTransform().SetWorldScale(ScreenRenderer_->GetTransform().GetWorldScale() * 2);

    Alpha_      = 0.f;
    BackAlpha_  = 0.f;
    ScreenTime_ = 0.f;

    IsReverse_ = false;
}

void PlayerUI::FinalBossDeathUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (false == IsReverse_)
    {
        Alpha_ += _DeltaTime * 0.4f;
        BackAlpha_ += _DeltaTime * 0.2f;

        if (1 < Alpha_)
        {
            Alpha_ = 1.0f;
            ScreenTime_ += _DeltaTime;
        }

        if (0.5f < BackAlpha_)
        {
            BackAlpha_ = 0.5f;
        }

        if (2.f < ScreenTime_)
        {
            ScreenTime_ = 0.f;
            IsReverse_  = true;
        }
    }

    else
    {
        Alpha_ -= _DeltaTime * 0.4f;
        BackAlpha_ -= _DeltaTime * 0.2f;

        if (0 > Alpha_)
        {
            Alpha_ = 0.0f;
            ScreenState_.ChangeState("BehindScreen");
        }

        if (0 > BackAlpha_)
        {
            BackAlpha_ = 0.0f;
            ScreenState_.ChangeState("BehindScreen");
        }
    }

    ScreenRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, Alpha_};
    BackRenderer_->GetColorData().MulColor   = float4{1.f, 1.f, 1.f, BackAlpha_};
}

void PlayerUI::FinalBossDeathEnd(const StateInfo& _Info) {}


void PlayerUI::BehindScreenStart(const StateInfo& _Info)
{
    ScreenRenderer_->GetColorData().MulColor = float4{1.f, 1.f, 1.f, 0};
    BackRenderer_->GetColorData().MulColor   = float4{1.f, 1.f, 1.f, 0};
}

void PlayerUI::BehindScreenUpdate(float _DeltaTime, const StateInfo& _Info) {}

void PlayerUI::BehindScreenEnd(const StateInfo& _Info) {}

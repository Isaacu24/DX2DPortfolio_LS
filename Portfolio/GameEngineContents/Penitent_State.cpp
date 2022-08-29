#include "PreCompile.h"
#include "Penitent.h"
#include "PlayerUI.h"
#include "MetaTextureRenderer.h"
#include "MetaSpriteManager.h"
#include "MoveEffect.h"
#include "HitEffect.h"
#include "AttackEffect.h"

void Penitent::FreezeStart(const StateInfo& _Info) { MetaRenderer_->ChangeMetaAnimation("penitent_sheathedIdle"); }

void Penitent::FreezeUpdate(float _DeltaTime, const StateInfo& _Info) {}

void Penitent::FreezeEnd(const StateInfo& _Info) {}

void Penitent::IdleStart(const StateInfo& _Info) { MetaRenderer_->ChangeMetaAnimation("penintent_idle_anim"); }

void Penitent::IdleUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (20000 < GameEngineInput::GetInst()->GetThumbLX()
        || true == GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        State_.ChangeState("Move");
    }

    else if (0 > GameEngineInput::GetInst()->GetThumbLX()
             || true == GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        State_.ChangeState("Move");
    }

    //사다리 내려가기 or 앉기
    else if (true == GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        State_.ChangeState("Crouch");
    }

    else if (true == GameEngineInput::GetInst()->IsPressKey("PenitentSlide"))
    {
        State_.ChangeState("Slide");
    }

    else if (true == GameEngineInput::GetInst()->IsDownKey("PenitentJump"))
    {
        State_.ChangeState("Jump");
    }

    else if (true == GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
    {
        State_.ChangeState("Attack");
    }

    else if (true == GameEngineInput::GetInst()->IsDownKey("PenitentParry"))
    {
        State_.ChangeState("Parrying");
    }

    //내리막길
    if (false == IsGround_)
    {
        if (false == FallCollisionCheck())
        {
            JumpForce_ = 10.f;
            State_.ChangeState("Fall");
        }
    }

    Gravity_->SetActive(!IsGround_);
}

void Penitent::IdleEnd(const StateInfo& _Info) {}

void Penitent::MoveStart(const StateInfo& _Info)
{
    RunTime_ = 0.f;
    MetaRenderer_->ChangeMetaAnimation("penintent_start_run_anim");
}

void Penitent::MoveUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        GetTransform().PixLocalPositiveX();
        Dir_ = GetTransform().GetRightVector();

        RealXDir_ = 1;

        if (false == RightObstacleCheck())
        {
            RunTime_ += _DeltaTime;

            GetTransform().SetWorldMove(Dir_ * Speed_ * _DeltaTime);
        }
    }

    else if (GameEngineInput::GetInst()->IsUpKey("PenitentRight"))
    {
        if (1.f <= RunTime_)
        {
            MetaRenderer_->ChangeMetaAnimation("penintent_stop_run_anim");

            MoveEffect_->Renderer_->On();
            MoveEffect_->GetTransform().PixLocalPositiveX();
            MoveEffect_->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition()
                                                         + float4{RealXDir_ * 50.f, 0});
            MoveEffect_->Renderer_->ChangeMetaAnimation("penitent-stop-running-dust");
        }

        else
        {
            ChangeState("Idle");
        }
        return;
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        GetTransform().PixLocalNegativeX();
        Dir_ = -(GetTransform().GetLeftVector());

        RealXDir_ = -1;

        if (false == LeftObstacleCheck())
        {
            RunTime_ += _DeltaTime;
            GetTransform().SetWorldMove(Dir_ * Speed_ * _DeltaTime);
        }
    }

    else if (GameEngineInput::GetInst()->IsUpKey("PenitentLeft"))
    {
        if (1.f <= RunTime_)
        {
            MetaRenderer_->ChangeMetaAnimation("penintent_stop_run_anim");

            MoveEffect_->Renderer_->On();
            MoveEffect_->GetTransform().PixLocalNegativeX();
            MoveEffect_->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition()
                                                         + float4{RealXDir_ * 50.f, 0});
            MoveEffect_->Renderer_->ChangeMetaAnimation("penitent-stop-running-dust");
        }

        else
        {
            ChangeState("Idle");
        }
        return;
    }

    if (GameEngineInput::GetInst()->IsDownKey("PenitentJump"))
    {
        State_.ChangeState("Jump");
    }

    else if (GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        State_.ChangeState("Crouch");
    }

    else if (GameEngineInput::GetInst()->IsPressKey("PenitentSlide"))
    {
        State_.ChangeState("Slide");
    }

    else if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
    {
        State_.ChangeState("Attack");
    }

    //내리막길
    if (false == IsGround_)
    {
        if (false == FallCollisionCheck())
        {
            JumpForce_ = 10.f;
            State_.ChangeState("Fall");
        }
    }

    Gravity_->SetActive(!IsGround_);
}

void Penitent::MoveEnd(const StateInfo& _Info) {}

void Penitent::JumpStart(const StateInfo& _Info)
{
    JumpForce_ = 100.f;
    MetaRenderer_->ChangeMetaAnimation("penitent_jump_anim");

    MoveEffect_->Renderer_->On();
    MoveEffect_->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition());
    MoveEffect_->Renderer_->ChangeMetaAnimation("penitent-jumping-landing-dust-anim");
}

void Penitent::JumpUpdate(float _DeltaTime, const StateInfo& _Info)
{
    JumpForce_.y -= _DeltaTime * 100.f;

    Dir_ = GetTransform().GetUpVector() * 10.f;

    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        GetTransform().PixLocalPositiveX();
        RealXDir_ = 1;

        MetaRenderer_->ChangeMetaAnimation("penitent_jum_forward_anim");

        if (false == RightObstacleCheck())
        {
            Dir_ += GetTransform().GetRightVector() * 3.f;
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        GetTransform().PixLocalNegativeX();
        RealXDir_ = -1;

        MetaRenderer_->ChangeMetaAnimation("penitent_jum_forward_anim");

        if (false == LeftObstacleCheck())
        {
            Dir_ += -(GetTransform().GetLeftVector() * 3.f);
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack")
        && GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        ChangeState("VerticalAttack");
        return;
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
    {
        ChangeState("JumpAttack");
        return;
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        JumpForce_.y -= _DeltaTime * 100.f;
    }

    GetTransform().SetWorldMove(Dir_ * JumpForce_ * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::JumpEnd(const StateInfo& _Info) {}

void Penitent::FallStart(const StateInfo& _Info)
{
    MetaRenderer_->ChangeMetaAnimation("penitent_falling_loop_anim");

    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight")
        || GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_jum_forward_fall_anim");
    }
}

void Penitent::FallUpdate(float _DeltaTime, const StateInfo& _Info)
{
    JumpForce_.y -= _DeltaTime * 100.f;
    FallTime_ += _DeltaTime;

    Dir_ = GetTransform().GetUpVector() * 10.f;

    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        GetTransform().PixLocalPositiveX();
        RealXDir_ = 1;

        if (false == RightObstacleCheck())
        {
            Dir_ += -(GetTransform().GetLeftVector() * 3.f);
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        GetTransform().PixLocalNegativeX();
        RealXDir_ = -1;

        if (false == LeftObstacleCheck())
        {
            Dir_ += -(GetTransform().GetLeftVector() * 3.f);
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
    {
        ChangeState("JumpAttack");
        return;
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        JumpForce_.y -= _DeltaTime * 100.f;
        FallTime_ += _DeltaTime / 2;
    }

    if (true == IsGround_)
    {
        ChangeState("Landing");
        return;
    }

    GetTransform().SetWorldMove(Dir_ * JumpForce_ * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::FallEnd(const StateInfo& _Info) {}


void Penitent::JumpAttackStart(const StateInfo& _Info)
{
    //일단 인정하지도 증가하지도 않음.
    FallTime_ = 0.f;

    MetaRenderer_->ChangeMetaAnimation("penitent_jumping_attack_noslashes");

    if (0 < RealXDir_)  //오른쪽
    {
        AttackCollider_->GetTransform().SetWorldMove({RealXDir_ * 80.f, 50.f});
    }

    else if (0 > RealXDir_)  //왼쪽
    {
        AttackCollider_->GetTransform().SetWorldMove({RealXDir_ * 80.f, 50.f});
    }
}

void Penitent::JumpAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
    JumpForce_.y -= _DeltaTime * 80.f;

    Dir_ = GetTransform().GetUpVector() * 10.f;

    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        GetTransform().PixLocalPositiveX();
        RealXDir_ = 1;

        if (false == RightObstacleCheck())
        {
            Dir_ += -(GetTransform().GetLeftVector() * 3.f);
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        GetTransform().PixLocalNegativeX();
        RealXDir_ = -1;

        if (false == LeftObstacleCheck())
        {
            Dir_ += -(GetTransform().GetLeftVector() * 3.f);
        }
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentUp"))
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_upward_attack_jump");

        AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
        AttackCollider_->GetTransform().SetWorldMove({0.f, 150.f});
    }

    if (true == IsGround_)
    {
        ChangeState("Landing");
        return;
    }

    GetTransform().SetWorldMove(Dir_ * JumpForce_ * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::JumpAttackEnd(const StateInfo& _Info)
{
    AttackCollider_->Off();
    AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
}

void Penitent::KnockBackStart(const StateInfo& _Info)
{
    if ("LadderClimb" == _Info.PrevState)
    {
        State_.ChangeState("LadderClimb");
        return;
    }

    FallTime_ = 0.f;
    MetaRenderer_->ChangeMetaAnimation("Penitent_pushback_grounded");
    BodyCollider_->Off();
}

void Penitent::KnockBackUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (true == RightObstacleCheck() || true == LeftObstacleCheck())
    {
        return;
    }

    GetTransform().SetWorldMove(float4{-(RealXDir_), 0} * 150.f * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::KnockBackEnd(const StateInfo& _Info) 
{ 
    BodyCollider_->On();
}

void Penitent::KnockUpStart(const StateInfo& _Info)
{
    if ("LadderClimb" == _Info.PrevState)
    {
        State_.ChangeState("LadderClimb");
        return;
    }

    FallTime_ = 0.f;

    MetaRenderer_->ChangeMetaAnimation("penitent_throwback_anim");

    BodyCollider_->Off();
}

void Penitent::KnockUpUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (true == RightObstacleCheck() || true == LeftObstacleCheck())
    {
        return;
    }

    GetTransform().SetWorldMove(float4{RealXDir_, 0} * 150.f * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::KnockUpEnd(const StateInfo& _Info) { BodyCollider_->On(); }

void Penitent::LandingStart(const StateInfo& _Info)
{
    if ("VerticalAttack" == _Info.PrevState)
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_verticalattack_landing");

        float4 PlayerPos = GetTransform().GetWorldPosition();

        AttackEffect_->Renderer_->On();
        AttackEffect_->GetTransform().SetWorldPosition(
            {PlayerPos.x, PlayerPos.y, static_cast<int>(ACTORORDER::PlayerEffect)});
        AttackEffect_->Renderer_->ChangeMetaAnimation("penitent_verticalattack_landing_effects_anim");
        AttackEffect_->Renderer_->CurAnimationReset();
        return;
    }

    if (0.9f <= FallTime_)
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_hardlanding_rocks_anim");
    }

    else
    {
        MetaRenderer_->ChangeMetaAnimation("penintent_standing_up");

        //모션 캔슬
        if (GameEngineInput::GetInst()->IsPressKey("PenitentRight")
            || GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
        {
            ChangeState("Move");
        }

        else if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
        {
            ChangeState("Attack");
        }
    }
}

void Penitent::LandingUpdate(float _DeltaTime, const StateInfo& _Info) { Gravity_->SetActive(!IsGround_); }

void Penitent::LandingEnd(const StateInfo& _Info) { FallTime_ = 0.f; }


void Penitent::CrouchStart(const StateInfo& _Info)
{
    MetaRenderer_->ChangeMetaAnimation("penitent_crouch_anim");

    BodyCollider_->GetTransform().SetWorldScale({ColScale_.y, ColScale_.x});
    BodyCollider_->GetTransform().SetWorldMove({0, -50});
}

void Penitent::CrouchUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (GameEngineInput::GetInst()->IsUpKey("PenitentDown"))
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_crouch_up_anim");
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentRight"))
    {
        GetTransform().PixLocalPositiveX();
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentLeft"))
    {
        GetTransform().PixLocalNegativeX();
    }
}

void Penitent::CrouchEnd(const StateInfo& _Info)
{
    BodyCollider_->GetTransform().SetWorldScale(ColScale_);
    BodyCollider_->GetTransform().SetWorldMove({0, 50});
}

void Penitent::SlideStart(const StateInfo& _Info)
{
    SlideForce_ = 500.f;
    MetaRenderer_->ChangeMetaAnimation("penitent_dodge_anim");

    BodyCollider_->GetTransform().SetWorldScale({ColScale_.y, ColScale_.x});
    BodyCollider_->GetTransform().SetWorldMove({0, -50});

    MoveEffect_->Renderer_->On();

    if (1 == RealXDir_)
    {
        MoveEffect_->GetTransform().PixLocalPositiveX();
    }

    else if (-1 == RealXDir_)
    {
        MoveEffect_->GetTransform().PixLocalNegativeX();
    }

    MoveEffect_->GetTransform().SetWorldPosition(GetTransform().GetWorldPosition() + float4{RealXDir_ * 50.f, 0});
    MoveEffect_->Renderer_->ChangeMetaAnimation("penitent_start_dodge_dust_anim");
}

void Penitent::SlideUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (GameEngineInput::GetInst()->IsPressKey("PenitentAttack"))
    {
        State_.ChangeState("DodgeAttack");
        return;
    }

    if (true == RightObstacleCheck() || true == LeftObstacleCheck())
    {
        return;
    }

    SlideForce_ -= _DeltaTime * 350.f;
    GetTransform().SetWorldMove(Dir_ * SlideForce_ * _DeltaTime);

    Gravity_->SetActive(!IsGround_);

    //내리막길
    if (false == IsGround_)
    {
        if (false == FallCollisionCheck())
        {
            MoveEffect_->Renderer_->Off();

            JumpForce_ = 10.f;
            State_.ChangeState("Fall");
        }
    }
}

void Penitent::SlideEnd(const StateInfo& _Info)
{
    BodyCollider_->GetTransform().SetWorldScale(ColScale_);
    BodyCollider_->GetTransform().SetWorldMove({0, 50});
}

void Penitent::DangleStart(const StateInfo& _Info)
{
    Gravity_->SetActive(false);
    MetaRenderer_->ChangeMetaAnimation("penitent_hangonledge_anim");

    //누적된 추락 시간 초기화
    FallTime_ = 0.f;
}

void Penitent::DangleUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (GameEngineInput::GetInst()->IsPressKey("PenitentUp"))
    {
        IsDangle_     = true;
        IsClimbLedge_ = true;

        MetaRenderer_->ChangeMetaAnimation("penitent_climbledge_reviewed");
    }

    else if (GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
    {
        State_.ChangeState("Fall");
        IsDangle_  = true;
        JumpForce_ = 10.f;
    }

    if (true == IsClimbLedge_)
    {
        float4 StartPos = GetTransform().GetWorldPosition();
        float4 EndPos   = GetTransform().GetWorldPosition() + float4{RealXDir_ * 100.f, 270.f};

        GetTransform().SetWorldPosition(float4::Lerp(StartPos, EndPos, _DeltaTime));
    }
}

void Penitent::DangleEnd(const StateInfo& _Info) { IsClimbLedge_ = false; }

void Penitent::LadderClimbStart(const StateInfo& _Info)
{
    //한 프레임 false 상태로 만들고 다음 상태일 때 체크
    IsGround_ = false;

    //위아래 분기 나누어야 함@
    MetaRenderer_->ChangeMetaAnimation("penintent_ladder_climb_loop_anim");
}

void Penitent::LadderClimbUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (false == IsLadder_)
    {
        if (GameEngineInput::GetInst()->IsPressKey("PenitentUp"))
        {
            CilmbY_ = 30.f;

            if (true == MetaRenderer_->IsCurAnimationPause())
            {
                MetaRenderer_->CurAnimationPauseSwitch();
            }

            MetaRenderer_->ChangeMetaAnimation("penintent_ladder_climb_loop_anim");
            GetTransform().SetWorldMove(GetTransform().GetUpVector() * Speed_ * _DeltaTime);
        }

        if (GameEngineInput::GetInst()->IsPressKey("PenitentDown"))
        {
            CilmbY_ = -50;


            if (true == MetaRenderer_->IsCurAnimationPause())
            {
                MetaRenderer_->CurAnimationPauseSwitch();
            }


            MetaRenderer_->ChangeMetaAnimation("penintent_ladder_climb_loop_anim");
            GetTransform().SetWorldMove(GetTransform().GetDownVector() * Speed_ * _DeltaTime);
        }

        else if (GameEngineInput::GetInst()->IsUpKey("PenitentUp")
                 || GameEngineInput::GetInst()->IsUpKey("PenitentDown"))
        {
            //애니메이션 멈춤
            MetaRenderer_->CurAnimationPauseSwitch();
        }
    }

    if (true == IsGround_)
    {
        if (0 > CilmbY_)  //아래 사다리
        {
            IsLadder_ = true;
            Gravity_->SetActive(true);
            MetaRenderer_->ChangeMetaAnimation("penintent_ladder_up_from_ground");
        }

        else if (0 < CilmbY_)  //위 사다리
        {
            IsLadder_ = true;

            GetTransform().SetWorldUpMove(500.f, _DeltaTime);
            MetaRenderer_->ChangeMetaAnimation("penitent_ladder_down_from_ground_anim");
        }
    }
}

void Penitent::LadderClimbEnd(const StateInfo& _Info) { IsLadder_ = false; }

void Penitent::AttackStart(const StateInfo& _Info)
{
    MetaRenderer_->ChangeMetaAnimation("penitent_three_hits_attack_combo_no_slashes");
    AttackCollider_->GetTransform().SetWorldMove({RealXDir_ * 80.f, 50.f});
}

void Penitent::AttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (GameEngineInput::GetInst()->IsDownKey("PenitentAttack"))
    {
        ++AttackStack_;
    }

    if (GameEngineInput::GetInst()->IsPressKey("PenitentUp"))
    {
        MetaRenderer_->ChangeMetaAnimation("penitent_upward_attack_clamped_anim");

        AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
        AttackCollider_->GetTransform().SetWorldMove({0.f, 150.f});
    }

    Gravity_->SetActive(!IsGround_);
}

void Penitent::AttackEnd(const StateInfo& _Info)
{
    AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
    AttackStack_ = 0;
}

void Penitent::DodgeAttackStart(const StateInfo& _Info)
{
    MetaRenderer_->ChangeMetaAnimation("penitent_dodge_attack_LVL3");
    AttackCollider_->GetTransform().SetWorldMove({RealXDir_ * 80.f, 50.f});
    AttackCollider_->ChangeOrder(COLLISIONORDER::PlayerSkill);
    BodyCollider_->Off();
}

void Penitent::DodgeAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (true == RightObstacleCheck() || true == LeftObstacleCheck())
    {
        return;
    }

    GetTransform().SetWorldMove({RealXDir_ * 350.f * _DeltaTime, 0.f});
    Gravity_->SetActive(!IsGround_);
}

void Penitent::DodgeAttackEnd(const StateInfo& _Info)
{
    AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
    AttackCollider_->Off();
    AttackCollider_->ChangeOrder(COLLISIONORDER::PlayerAttack);
    BodyCollider_->On();
}

void Penitent::VerticalAttackStart(const StateInfo& _Info)
{
    MetaRenderer_->ChangeMetaAnimation("penitent_verticalattack_start_anim");
    FallTime_ = 0;

    ReadySkill_ = false;
    Gravity_->SetActive(false);

    AttackCollider_->On();
    AttackCollider_->GetTransform().SetWorldMove({0.f, -100.f});
    BodyCollider_->Off();
}

void Penitent::VerticalAttackUpdate(float _DeltaTime, const StateInfo& _Info)
{
    if (false == ReadySkill_)
    {
        return;
    }

    JumpForce_.y -= _DeltaTime * 150.f;

    Dir_ = GetTransform().GetUpVector() * 10.f;

    if (true == IsGround_)
    {
        ChangeState("Landing");
        return;
    }

    GetTransform().SetWorldMove(Dir_ * JumpForce_ * _DeltaTime);
    Gravity_->SetActive(!IsGround_);
}

void Penitent::VerticalAttackEnd(const StateInfo& _Info)
{
    AttackCollider_->GetTransform().SetLocalPosition({0.f, 0.f});
    AttackCollider_->Off();
}

void Penitent::ParryingStart(const StateInfo& _Info) { MetaRenderer_->ChangeMetaAnimation("penitent_parry_failed"); }

void Penitent::ParryingUpdate(float _DeltaTime, const StateInfo& _Info) {}

void Penitent::ParryingEnd(const StateInfo& _Info) {}

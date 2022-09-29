#include "PreCompile.h"
#include "DistortionEffect.h"

DistortionEffect::DistortionEffect() {}

DistortionEffect::~DistortionEffect() {}

void DistortionEffect::EffectInit()
{
    CopyTarget_ = new GameEngineRenderTarget();
    CopyTarget_->CreateRenderTargetTexture(
        GameEngineWindow::GetScale(), DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT, float4::ZERO);

    EffectSet_.SetPipeLine("Distortion");

    EffectSet_.ShaderResources.SetConstantBufferLink("DeltaTimeData", Data_);

    Data_.ScreenXY[0] = 1280.f;
    Data_.ScreenXY[1] = 720.f;
}

void DistortionEffect::Effect(GameEngineRenderTarget* _Target)
{
    AccTime_ += GameEngineTime::GetDeltaTime();
    Data_.AccTime = AccTime_;

    if (1.f <= AccTime_)
    {
        Off();
        AccTime_ = 0.f;
    }

    CopyTarget_->Copy(_Target);
    EffectSet_.ShaderResources.SetTexture("Tex", CopyTarget_->GetRenderTargetTexture(0));

    _Target->Copy(CopyTarget_);
    _Target->Setting();
    _Target->Effect(EffectSet_);
}

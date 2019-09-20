#include "GstAppSrcComponent.h"
#include "GstPipelineImpl.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Runtime/Core/Public/Async/Async.h"

UGstAppSrcComponent::UGstAppSrcComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGstAppSrcComponent::UninitializeComponent()
{
	ResetState();
}

void UGstAppSrcComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGstAppSrcComponent::ResetState()
{
	if (AppSrc)
		AppSrc->Disconnect();
	SafeDestroy(AppSrc);
}

void UGstAppSrcComponent::CbPipelineStart(IGstPipeline *Pipeline)
{
	ResetState();

	if (AppSrcEnabled && !AppSrcName.IsEmpty())
	{
		AppSrc = IGstAppSrc::CreateInstance(TCHAR_TO_ANSI(*AppSrcName));
		AppSrc->Connect(Pipeline, TCHAR_TO_ANSI(*AppSrcName), this);
	}
}

void UGstAppSrcComponent::CbPipelineStop()
{
	ResetState();
}

void UGstAppSrcComponent::CbGstPushTexture()
{
	NeedsData = true;
}

void UGstAppSrcComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (AppSrc)
	{
		AActor *Actor = GetOwner();
		USceneCaptureComponent2D *CaptureComponent = Cast<USceneCaptureComponent2D>(AppSrcCapture.GetComponent(Actor));
		if (CaptureComponent)
		{
			UTextureRenderTarget2D *TextureTarget = CaptureComponent->TextureTarget;
			if (TextureTarget)
			{
				if (NeedsData)
				{
					NeedsData = false;
					TArray<FColor> TextureData;
					FTextureRenderTargetResource *TextureResource = TextureTarget->GameThread_GetRenderTargetResource();
					TextureResource->ReadPixels(TextureData);
					AppSrc->PushTexture((uint8_t *)TextureData.GetData(), TextureData.Num() * 4);
				}
			}
			else if (AppSrc->GetTextureFormat() == EGstTextureFormat::GST_VIDEO_FORMAT_BGRA)
			{
				UTextureRenderTarget2D *NewRenderTarget2D = NewObject<UTextureRenderTarget2D>();
				check(NewRenderTarget2D);
				NewRenderTarget2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
				NewRenderTarget2D->InitAutoFormat(AppSrc->GetTextureWidth(), AppSrc->GetTextureHeight());
				NewRenderTarget2D->UpdateResourceImmediate(true);
				CaptureComponent->TextureTarget = NewRenderTarget2D;
			}
			else
			{
				GST_LOG_ERR(TEXT("GstAppSrc: Missing TextureTarget"));
			}
		}
		else
		{
			GST_LOG_ERR(TEXT("GstAppSrc: AppSrcCapture is not a USceneCaptureComponent2D"));
		}
	}
}

void UGstAppSrcComponent::SetKlv(TArray<FGstKlv> _Klv)
{
	Klv = _Klv;
}

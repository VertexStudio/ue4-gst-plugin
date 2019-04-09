#include "GstAppSrcComponent.h"
#include "GstPipelineImpl.h"
// #include "GstSampleImpl.h"
#include "Runtime/Core/Public/Async/Async.h"

UGstAppSrcComponent::UGstAppSrcComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGstAppSrcComponent::UninitializeComponent()
{
	ResetState();
}

void UGstAppSrcComponent::ResetState()
{
	if (AppSrc) AppSrc->Disconnect();
	// SafeDestroy(Texture);
	SafeDestroy(AppSrc);
}

void UGstAppSrcComponent::CbPipelineStart(IGstPipeline* Pipeline)
{
	ResetState();

	if (AppSrcEnabled && !AppSrcName.IsEmpty())
	{
		AppSrc = IGstAppSrc::CreateInstance();
		AppSrc->Connect(Pipeline, TCHAR_TO_ANSI(*AppSrcName));
	}
}

void UGstAppSrcComponent::CbPipelineStop()
{
	ResetState();
}

void UGstAppSrcComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

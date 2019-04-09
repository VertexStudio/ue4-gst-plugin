#pragma once

#include "GstElementComponent.h"
#include "GstAppSrcImpl.h"
// #include "GstTexture.h"
#include "GstVideoFormat.h"
#include "GstAppSrcComponent.generated.h"

// DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FGstTextureCreatedSignature, UGstAppSrcComponent*, AppSrc, UTexture2D*, NewTexture, EGstVideoFormat, Format, int, Width, int, Height);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSTREAMER_API UGstAppSrcComponent : public UGstElementComponent
{
	GENERATED_BODY()

public:	

	UGstAppSrcComponent();

	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void CbPipelineStart(class IGstPipeline* Pipeline);
	virtual void CbPipelineStop();
	// virtual void CbGstSampleReceived(class IGstSample* Sample);
	// virtual void CbGstTextureCreated();

	UPROPERTY(Category="GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString PipelineName;

	UPROPERTY(Category="GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString AppSrcName;

	UPROPERTY(Category="GstAppSrc", EditAnywhere, BlueprintReadWrite)
	bool AppSrcEnabled;

	// UFUNCTION(Category="GstAppSrc", BlueprintCallable)
	// UTexture2D* GetTexture() { return Texture->GetTextureObject(); }

	// UPROPERTY(Category="GstAppSrc", BlueprintAssignable)
	// FGstTextureCreatedSignature OnGstTextureCreated;

protected:

	void ResetState();

	IGstAppSrc* AppSrc = nullptr;
	// FGstTexture* Texture = nullptr;
};

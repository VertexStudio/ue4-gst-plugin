#pragma once

#include "GstElementComponent.h"
#include "GstAppSrcImpl.h"
#include "GstVideoFormat.h"
#include "GstKlv.h"
#include "GstAppSrcComponent.generated.h"

UCLASS(ClassGroup = (Simbotic), meta = (BlueprintSpawnableComponent))
class GSTREAMER_API UGstAppSrcComponent : public UGstElementComponent, public IGstAppSrcCallback
{
	GENERATED_BODY()

public:
	UGstAppSrcComponent();

	virtual void BeginPlay() override;
	virtual void UninitializeComponent() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	virtual void CbPipelineStart(class IGstPipeline *Pipeline);
	virtual void CbPipelineStop();
	virtual void CbGstPushTexture();

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString PipelineName;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	FString AppSrcName;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere, BlueprintReadWrite)
	bool AppSrcEnabled;

	UPROPERTY(Category = "GstAppSrc", EditAnywhere)
	FComponentReference AppSrcCapture;

	void SetKlv(TArray<FGstKlv> Klv);

protected:
	void ResetState();

	IGstAppSrc *AppSrc = nullptr;
	TArray<FGstKlv> Klv;

	bool NeedsData = false;
};

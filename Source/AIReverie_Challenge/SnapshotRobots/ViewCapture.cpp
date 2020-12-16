#include "ViewCapture.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetRenderingLibrary.h"
#include "Engine/World.h"

// Sets default values
AViewCapture::AViewCapture(const FObjectInitializer& ObjectInitializer) 
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	// Create our SceneCaptureComponent2D and make it the root component of this Actor.
	Camera = ObjectInitializer.CreateDefaultSubobject<USceneCaptureComponent2D>(this, TEXT("Camera"));
	SetRootComponent(Camera);
	// Make sure we don't capture every frame for performance, and because our render target will be made to be GC'd.
	Camera->bCaptureEveryFrame = false;
	// Set the right format to produce png files
	Camera->CaptureSource = ESceneCaptureSource::SCS_FinalColorLDR;
}

//Make the camera component look at pawn`s camera
bool AViewCapture::SetCameraToPlayerView(FVector PawnLocation, FRotator PawnRotation, float CameraFOV)
{

	SetActorLocationAndRotation(PawnLocation, PawnRotation);
	Camera->SetWorldLocationAndRotation(PawnLocation, PawnRotation);

	Camera->FOVAngle = CameraFOV;

	return true;
}

//Captures player`s view and stores it to a file
bool AViewCapture::CapturePlayersView(int32 Resolution, FString FilePath, FString FileName, FVector PawnLocation, FRotator PawnRotation, float CameraFOV)
{
	// Make the resolution a power of two.
	Resolution = FGenericPlatformMath::Pow(2, FGenericPlatformMath::FloorLog2(FGenericPlatformMath::Max(Resolution, 1) * 2 - 1));

	// Move our actor and its camera component to player's camera.
	if (!SetCameraToPlayerView(PawnLocation, PawnRotation, CameraFOV)) return false;

	// Create a temporary object that we will let die in GC in a moment after this scope ends.
	UTextureRenderTarget2D * TextureRenderTarget = NewObject<UTextureRenderTarget2D>();
	TextureRenderTarget->InitAutoFormat(Resolution, Resolution);
	//This format allows UE4 to produce PNG files
	TextureRenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	// Take the capture.
	Camera->TextureTarget = TextureRenderTarget;
	Camera->CaptureScene();
	UKismetRenderingLibrary::ExportRenderTarget(GetWorld(), Camera->TextureTarget, FilePath, FileName);
	
	return true;
}


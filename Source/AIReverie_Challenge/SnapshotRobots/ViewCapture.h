// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Camera/CameraComponent.h"
#include "ViewCapture.generated.h"

/*
This class holds the functionality to create render targets to click snapshots,
capture viewport data, store it to the rendertarget and export it to a png file
*/
UCLASS()
class AIREVERIE_CHALLENGE_API AViewCapture : public AActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	AViewCapture(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	bool SetCameraToPlayerView(FVector PawnLocation, FRotator PawnRotation, float CameraFOV);

	UFUNCTION(BlueprintCallable)
	bool CapturePlayersView(int32 Resolution, FString FilePath, FString FileName, FVector PawnLocation, FRotator PawnRotation, float CameraFOV);
	
	// The pointer to our "Camera" USceneCaptureComponent2D. 
	UPROPERTY(EditAnywhere, Transient)
	class USceneCaptureComponent2D * Camera;
};
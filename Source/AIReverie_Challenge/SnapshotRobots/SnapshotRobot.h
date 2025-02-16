// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ViewCapture.h"
#include "GameFramework/Pawn.h"
#include "Components/StaticMeshComponent.h"
#include "Camera/CameraComponent.h"
#include "SnapshotRobot.generated.h"

/*
This class serves as the base class for setting up pawns that go around an environment
to capture scene data. The class has two main components: a static mesh, and 
a camera. The camera component is used as the point of view and to click snapshots
*/
UCLASS()
class AIREVERIE_CHALLENGE_API ASnapshotRobot : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ASnapshotRobot();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Root comp")
	UStaticMeshComponent* mMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* mCamera;

	//View capture actor
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snapshot")
	AViewCapture* mSnapshotActor;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input; Overriding it and keeping it empty so that inputs can`t move this robot pawn.
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Blueprint accessible variables

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Customize")
	float mCameraRelativePlacement;

	//Movement functions

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void RotateRootInYaw(float Angle, float Direction);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	void MoveRootForward(float MovementSpeed);

	UFUNCTION(BlueprintCallable, Category = "Movement")
	bool IsObstacleAhead(float ObstacleDistance);

	//Image Capture
	UFUNCTION(BlueprintCallable, Category = "Snapshot")
	bool CaptureSnapshot(int32 Resolution, FString Directory, FString Filename);

	//This is to implement the function in Robot 2
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Snapshot")
	void SpawnSphere();

	//Helper functions
	void EnsureCaptureActorReference();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "SnapshotRobot.h"
#include "SnapshotRobotController.generated.h"

/**
 This class controls the custom pawn. This class takes the help of pawn functions to make
 decisions. This class also contains variables that have been exposed to the Editor and 
 blueprints to change the simulation parameters
 */
UCLASS()
class AIREVERIE_CHALLENGE_API ASnapshotRobotController : public APlayerController
{
	GENERATED_BODY()
	
public:
	// Sets default values for this controller's properties
	ASnapshotRobotController();

protected:
	//Constructor
	virtual void BeginPlay() override;

	//Components

	//Pawn to control: We control its root static mesh to move the pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pawn")
	ASnapshotRobot *mPawn;

	//Timer accumulator
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Decision")
	float mTimeSinceLastDecision;

	//Array to trace actors in the viewport
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snapshot")
	TArray<FString> mActorsInViewport;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snapshot")
	FString mActorsInViewportCombinedString;

	//Save backslash Data directory
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snapshot")
	FString mGameSavedDataDir;

	//Index to capture the number of simulations captured since Begin play
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Snapshot")
	int mDataCaptureIndex;

	//Bot movement functions
	void MakeDecision();
	void CaptureData();
	float GetRandomNumber(float Min, float Max);
	int GetRandomNumber(int32 Min, int32 Max);

	//Snapshot helpers
	void GetActorsInViewport(TArray<FString>& CurrentlyRenderedActors, FString &CombinedString);
	bool SaveStringTextToFile(FString SaveDirectory, FString FileName, FString SaveText);
	bool CreateDataDirectory(FString SaveDirectory);
	
public:
	//Methods
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//Public components
	//These can be edited in blueprints. Ideally, these should be defined in the game mode, so that they can be altered without creating an inherited blueprint. But since the gamemode was provided with the test as a blueprint, a design decision was made to keep constants in the player controller.

	//Wait time between robot`s decisions
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decision")
	float mDecisionTimerSeconds;

    //Units to move in forward direction
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decision")
	float mMoveDistance;

	//Min rotation when an obstacle is found
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decision")
	float mMinRotation;

	//Max rotation when an obstacle is found
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decision")
	float mMaxRotation;

	//Obstacle check distance
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Decision")
	float mObstacleDistance;

	//Snapshot resolution
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Snapshot")
	int32 mSnapshotResolution;

};

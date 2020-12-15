// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotRobotController.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ASnapshotRobotController::ASnapshotRobotController()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	//Defaults, taken from the instructions page
	mDecisionTimerSeconds = 1.0f;
	
	mMoveDistance = 10.0f;

	mMinRotation = 5.0f;

	mMaxRotation = 25.0f;

	mObstacleDistance = 15.0f;

}

void ASnapshotRobotController::BeginPlay()
{
	Super::BeginPlay();

	//resetting time accumulator at start
	mTimeSinceLastDecision = mDecisionTimerSeconds;

	//Possess the snapshot robot (pawn) in the scene, so that the starting position of a bot (1 or 2) can be adjusted before simulating. 
	for (TObjectIterator<ASnapshotRobot> Itr; Itr; ++Itr)
	{
		if (Itr->IsA(ASnapshotRobot::StaticClass()))
		{
			mPawn = Cast<ASnapshotRobot>(*Itr);
			Possess(mPawn);
			break;
		}
		
	}
}

void ASnapshotRobotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Counting time during ticks
	mTimeSinceLastDecision += DeltaTime;
	if (mTimeSinceLastDecision >= mDecisionTimerSeconds)
	{
		//reset timer
		mTimeSinceLastDecision = 0.0f;

		//Move the bot
		MakeDecision();
	}
}

void ASnapshotRobotController::MakeDecision()
{
	if (mPawn->IsObstacleAhead(mObstacleDistance)) //Rotate
	{
		//Generate random numbers
		int32 Direction = GetRandomNumber(0, 1);
		float DirectionFloat = Direction == 1 ? DirectionFloat = 1.0f : -1.0f;
		float AngleOfRotation = GetRandomNumber(mMinRotation, mMaxRotation);
		
		mPawn->RotateRootInYaw(AngleOfRotation, DirectionFloat);
	}
	else //Move forward
	{
		mPawn->MoveRootForward(mMoveDistance);
	}
	
}


float ASnapshotRobotController::GetRandomNumber(float Min, float Max)
{
	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	return FMath::FRandRange(Min, Max);
}

int ASnapshotRobotController::GetRandomNumber(int32 Min, int32 Max)
{
	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	return FMath::RandRange(Min, Max);
}
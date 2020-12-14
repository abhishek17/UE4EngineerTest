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
			mPawnStaticMesh = mPawn->GetRootStaticMesh();
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
	if (IsObstacleAhead()) //Rotate
	{
		int32 Direction = GetRandomNumber(0, 1);
		float DirectionFloat = Direction == 1 ? DirectionFloat = 1.0f : -1.0f;
		float AngleOfRotation = GetRandomNumber(mMinRotation, mMaxRotation);
		
		FRotator NewRotation = FRotator(0.0f, DirectionFloat*AngleOfRotation, 0.0f);

		FQuat QuatRotation = FQuat(NewRotation);

		mPawnStaticMesh->AddLocalRotation(QuatRotation, false, 0, ETeleportType::None);
	}
	else //Move forward
	{
		FVector CurrentBotFowardVector = mPawnStaticMesh->GetForwardVector();
		mPawnStaticMesh->AddRelativeLocation(CurrentBotFowardVector * mMoveDistance);
	}
	
}

bool ASnapshotRobotController::IsObstacleAhead()
{
	FHitResult OutHit;
	FVector Start = mPawnStaticMesh->GetComponentLocation();

	FVector ForwardVector = mPawnStaticMesh->GetForwardVector();
	FVector End = ((ForwardVector * mObstacleDistance) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			UE_LOG(LogTemp, Display, TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName());
			return true;
		}
	}
	return false;
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
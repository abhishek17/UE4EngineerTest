// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotRobotController.h"
#include "Engine/World.h"

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

	mSnapshotResolution = 512;

}

//Controller`s Begin play
void ASnapshotRobotController::BeginPlay()
{
	Super::BeginPlay();

	//resetting time accumulator at start
	mTimeSinceLastDecision = mDecisionTimerSeconds;
	//Reset the data capture index
	mDataCaptureIndex = 0;

	//Setup pawn
	mPawn = Cast<ASnapshotRobot> (GetPawn());
	if (mPawn == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("No Pawn in the Scene!!!"));
	}

	//Create Saved\Data directory
	FString GameSavedDir = FPaths::ProjectSavedDir();
	mGameSavedDataDir = GameSavedDir + "\\Data";
	CreateDataDirectory(mGameSavedDataDir);
	
}

//Controller`s tick
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

		//Capture data
		CaptureData();
	}
}

//This function is called whenever the wait period is over and it`s time for a robot pawn to make a decision
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

//This function is responsible for capturing data, including a snapshot and a text file
void ASnapshotRobotController::CaptureData()
{
	//Increment data capture index
	mDataCaptureIndex++;

	//Click snapshots and save data
	FString DataCaptureBaseName = "image_";
	DataCaptureBaseName += FString::FromInt(mDataCaptureIndex);

	//Capture snapshot
	FString ImageCapturename = DataCaptureBaseName + ".png";
	mPawn->CaptureSnapshot(mSnapshotResolution, mGameSavedDataDir, ImageCapturename);

	//Save Actors
	GetActorsInViewport(mActorsInViewport, mActorsInViewportCombinedString);
	FString TextFileName = DataCaptureBaseName + "_actors.txt";
	SaveStringTextToFile(mGameSavedDataDir, TextFileName, mActorsInViewportCombinedString);
}

//Function to generate a random float
float ASnapshotRobotController::GetRandomNumber(float Min, float Max)
{
	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	return FMath::FRandRange(Min, Max);
}

//Function to generate a random integer
int ASnapshotRobotController::GetRandomNumber(int32 Min, int32 Max)
{
	int32 seed = (int32)(FDateTime::Now().GetTicks() % INT_MAX);
	return FMath::RandRange(Min, Max);
}

//This function goes through the scene proxies for all the actors in the scene to compare with the view frustum. After that all the overlapping actors are stored in an array
void ASnapshotRobotController::GetActorsInViewport(TArray<FString>& CurrentlyRenderedActors, FString &CombinedString)
{
	//reset data arrays
	CurrentlyRenderedActors.Empty();
	CombinedString.Empty();

	//Get the view frustum and see if any primitive collides with it. This helps find out what has not been clipped in rendering
	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	FSceneViewFamilyContext ViewFamily(FSceneViewFamily::ConstructionValues(
		LocalPlayer->ViewportClient->Viewport,
		GetWorld()->Scene,
		LocalPlayer->ViewportClient->EngineShowFlags)
		.SetRealtimeUpdate(true));

	FVector ViewLocation;
	FRotator ViewRotation;
	FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamily, /*out*/ ViewLocation, /*out*/ ViewRotation, LocalPlayer->ViewportClient->Viewport);

	bool IsFirstActor = true;
	for (TObjectIterator<UPrimitiveComponent> ScenePrimitsItr; ScenePrimitsItr; ++ScenePrimitsItr)
	{
		// Using the Object iterator so we have to skip objects not in the 
		// relevant world
		if (ScenePrimitsItr->GetWorld() == GetWorld())
		{
			FPrimitiveSceneProxy* Proxy = ScenePrimitsItr->SceneProxy;
			if (Proxy)
			{
				bool bIsShown =
					SceneView->ViewFrustum.IntersectSphere(Proxy->GetBounds().Origin, Proxy->GetBounds().SphereRadius) ||
					SceneView->ViewFrustum.IntersectBox(Proxy->GetBounds().Origin, Proxy->GetBounds().BoxExtent);
				if (bIsShown)
				{
					//If collided, see if the actor that owns this primitive has not been stored
					AActor *OwnerActor = ScenePrimitsItr->GetOwner();
					if (OwnerActor != nullptr)
					{
						if (!CurrentlyRenderedActors.Contains(OwnerActor->GetName()))
						{
							CurrentlyRenderedActors.Add(OwnerActor->GetName());
							if (IsFirstActor)
							{
								CombinedString = OwnerActor->GetName();
								IsFirstActor = false;
							}
							else
							{
								CombinedString += " ";
								CombinedString += OwnerActor->GetName();
							}
						}
					}
				}
			}
		}
	}
}

//This function saves the string containing actors to a file
bool ASnapshotRobotController::SaveStringTextToFile(FString SaveDirectory, FString FileName, FString SaveText)
{

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!platformFile.DirectoryExists(*SaveDirectory))
	{
		platformFile.CreateDirectory(*SaveDirectory);
	}
	//still could not make directory?
	if (!platformFile.DirectoryExists(*SaveDirectory))
	{
		//Could not make the specified directory
		return false;
		//~~~~~~~~~~~~~~~~~~~~~~
	}

	//get complete file path
	SaveDirectory += "\\";
	SaveDirectory += FileName;

	return FFileHelper::SaveStringToFile(SaveText, *SaveDirectory);
}

//This function creates the Data folder under the Saved directory, if it does not exist
bool ASnapshotRobotController::CreateDataDirectory(FString SaveDirectory)
{
	//Create data folder in saved
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	// Directory Exists?
	if (!platformFile.DirectoryExists(*SaveDirectory))
	{
		platformFile.CreateDirectory(*SaveDirectory);
	}
	//still could not make directory?
	if (!platformFile.DirectoryExists(*SaveDirectory))
	{
		//Could not make the specified directory
		return false;
		//~~~~~~~~~~~~~~~~~~~~~~
	}
	return true;
}
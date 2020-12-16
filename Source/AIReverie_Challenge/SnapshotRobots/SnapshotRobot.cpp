// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotRobot.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/Paths.h"

// Sets default values
ASnapshotRobot::ASnapshotRobot()
{
	//Create components
	mMesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	mCamera = CreateDefaultSubobject<UCameraComponent>("Camera");

	//Set up component heirarchy 
	RootComponent = mMesh;
	mCamera->SetupAttachment(mMesh);

	//set defaults
	mCameraRelativePlacement = -50.0f;

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnapshotRobot::BeginPlay()
{
	Super::BeginPlay();

	//Push camera component a bit behind the root mesh to allow for better snapshots
	mCamera->SetRelativeLocation(FVector(mCameraRelativePlacement, 0.0f, 0.0f));
}


// Called every frame
void ASnapshotRobot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASnapshotRobot::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//Helps to rotate the pawn in Yaw
void ASnapshotRobot::RotateRootInYaw(float Angle, float Direction)
{
	FRotator NewRotation = FRotator(0.0f, Direction*Angle, 0.0f);
	FQuat QuatRotation = FQuat(NewRotation);
	mMesh->AddLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

//Helps the pawn to move forward
void ASnapshotRobot::MoveRootForward(float MovementSpeed)
{
	FVector CurrentBotFowardVector = mMesh->GetForwardVector();
	mMesh->AddRelativeLocation(CurrentBotFowardVector * MovementSpeed);
}

//This function checks if there is an obstacle right in the front
bool ASnapshotRobot::IsObstacleAhead(float ObstacleDistance)
{
	FHitResult OutHit;
	FVector Start = mMesh->GetComponentLocation();

	FVector ForwardVector = mMesh->GetForwardVector();
	FVector End = ((ForwardVector * ObstacleDistance) + Start);
	FCollisionQueryParams CollisionParams;

	if (GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams))
	{
		if (OutHit.bBlockingHit)
		{
			UE_LOG(LogTemp, Display, TEXT("You are hitting: %s"), *OutHit.GetActor()->GetName());
			return true; //We hit something
		}
	}
	return false;//Nothing in the front
}

void ASnapshotRobot::EnsureCaptureActorReference()
{
		//If not found in the scene, instantiate a new actor
		if (mSnapshotActor == nullptr)
		{
			//Instantiate view capture actor
			FVector Location(0.0f, 0.0f, 89.f);
			FRotator Rotation(0.0f, 0.0f, 0.0f);
			FActorSpawnParameters SpawnInfo;

			mSnapshotActor = GetWorld()->SpawnActor<AViewCapture>(AViewCapture::StaticClass(), Location, Rotation, SpawnInfo);
		}
}

//This is the base class implementation of the spawnSphere function
void ASnapshotRobot::SpawnSphere_Implementation()
{
	UE_LOG(LogTemp, Display, TEXT("SpawnSphere base class"));
}

//Function to help with capturing snapshots as png files
bool ASnapshotRobot::CaptureSnapshot(int32 Resolution, FString Directory, FString Filename)
{
	//make sure that we have the View Capture actor
	EnsureCaptureActorReference();

	//Capture and save the image
	bool SuccessCapture =  mSnapshotActor->CapturePlayersView(Resolution, Directory, Filename, GetActorLocation(), GetActorRotation(), mCamera->FieldOfView);
	if (SuccessCapture)
	{
		//Call the spawn sphere function, which will be implemented in the blueprints by a child class of snapshot robot
		SpawnSphere();
		return true;
	}
	else
	{
		return false;
	}
}
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
	mSpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	mCamera = CreateDefaultSubobject<UCameraComponent>("Camera");

	//Set up component heirarchy 
	RootComponent = mMesh;
	mSpringArm->SetupAttachment(mMesh);
	mCamera->SetupAttachment(mSpringArm);

 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASnapshotRobot::BeginPlay()
{
	Super::BeginPlay();

	//Instantiate view capture actor
	FVector Location(0.0f, 0.0f, 89.f); 
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	
	mSnapshotActor = GetWorld()->SpawnActor<AViewCapture>(AViewCapture::StaticClass(), Location, Rotation, SpawnInfo); 
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

void ASnapshotRobot::RotateRootInYaw(float Angle, float Direction)
{
	FRotator NewRotation = FRotator(0.0f, Direction*Angle, 0.0f);
	FQuat QuatRotation = FQuat(NewRotation);
	mMesh->AddLocalRotation(QuatRotation, false, 0, ETeleportType::None);
}

void ASnapshotRobot::MoveRootForward(float MovementSpeed)
{
	FVector CurrentBotFowardVector = mMesh->GetForwardVector();
	mMesh->AddRelativeLocation(CurrentBotFowardVector * MovementSpeed);
}

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
			return true;
		}
	}
	return false;
}

bool ASnapshotRobot::CaptureSnapshot(int32 Resolution, FString Directory, FString Filename)
{
	if (mSnapshotActor == nullptr)
	{
		//Instantiate view capture actor
		FVector Location(0.0f, 0.0f, 89.f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;

		mSnapshotActor = GetWorld()->SpawnActor<AViewCapture>(AViewCapture::StaticClass(), Location, Rotation, SpawnInfo);
	}
	return mSnapshotActor->CapturePlayersView(Resolution, Directory, Filename, GetActorLocation(), GetActorRotation(), mCamera->FieldOfView);

}
// Fill out your copyright notice in the Description page of Project Settings.


#include "SnapshotRobot.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

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


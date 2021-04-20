// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnTank.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

APawnTank::APawnTank()
{
    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
    SpringArm->SetupAttachment(RootComponent);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm);
}

// Called when the game starts or when spawned
void APawnTank::BeginPlay()
{
    Super::BeginPlay();

    PlayerControllerRef = Cast<APlayerController>(GetController());
}

void APawnTank::HandleDestruction() 
{
    Super::HandleDestruction();
    bIsPlayerAlive = false;

    SetActorHiddenInGame(true);
    SetActorTickEnabled(false);
}

bool APawnTank::GetIsPlayerAlive() 
{
    return bIsPlayerAlive;
}

// Called every frame
void APawnTank::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    
    Rotate();
    Move();

    //if(PlayerControllerRef)
    //{
    //    FHitResult TraceHitResult;
    //    PlayerControllerRef->GetHitResultUnderCursor(ECC_Visibility, false, TraceHitResult);
    //   FVector HitLocation = TraceHitResult.ImpactPoint;

    //    RotateTurret(HitLocation); 
    //}
}

// Called to bind functionality to input
void APawnTank::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAxis("MoveForward", this, &APawnTank::CalculateMoveInput);
    PlayerInputComponent->BindAxis("Turn", this, &APawnTank::CalculateRotateInput);
    PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APawnTank::Fire);

    PlayerInputComponent->BindAxis("Dip", IE_Pressed, this, &APawnTank::BeginDip);
    //PlayerInputComponent->BindAxis("Dip", IE_Released, this, &APawnTank::LeaveDip);
    //PlayerInputComponent->BindAxis("Lift", IE_Pressed, this, &APawnTank::BeginLift);
    //PlayerInputComponent->BindAxis("Lift", IE_Released, this, &APawnTank::LeaveLift);
}

UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Move", meta = (AllowPrivateAccess = "true"))
float Thrust = 1.0f;

void APawnTank::CalculateMoveInput(float Value) 
{
    MoveDirection = FVector( Thrust * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0); //Value
}

void APawnTank::CalculateRotateInput(float Value) 
{
    float RotateAmount = Value * RotateSpeed * GetWorld()->DeltaTimeSeconds;
    FRotator Rotation = FRotator(0, RotateAmount, 0); //RotateAmount
    RotationDirection = FQuat(Rotation);
}

void APawnTank::BeginDip(float Value)
{
    MoveDirection = FVector( Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);
}

void APawnTank::LeaveDip(float Value)
{
    MoveDirection = FVector( Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);

}

void APawnTank::BeginLift(float Value)
{
    MoveDirection = FVector( Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 500, 0);

}

void APawnTank::LeaveLift(float Value)
{
    MoveDirection = FVector( Value * MoveSpeed * GetWorld()->DeltaTimeSeconds, 0, 0);

}

void APawnTank::Move() 
{
    AddActorLocalOffset(MoveDirection, true);
}

void APawnTank::Rotate() 
{
    AddActorLocalRotation(RotationDirection, true);
}
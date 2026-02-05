// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	FollowCamera  = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	FollowCamera ->SetupAttachment(CameraBoom);
	FollowCamera ->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) { UE_LOG(LogTemp, Warning, TEXT("[Input] No PlayerController")); return; }

	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP) { UE_LOG(LogTemp, Warning, TEXT("[Input] No LocalPlayer")); return; }

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) { UE_LOG(LogTemp, Warning, TEXT("[Input] No EnhancedInput subsystem")); return; }

	if (!IMC_Player) { UE_LOG(LogTemp, Warning, TEXT("[Input] IMC_Player is null (assign in BP)")); return; }

	Subsystem->AddMappingContext(IMC_Player, 0);
	UE_LOG(LogTemp, Log, TEXT("[Input] IMC_Player added"));
	
	if (CameraBoom)
	{
		TargetArmLength = CameraBoom->TargetArmLength;
	}
		
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) { UE_LOG(LogTemp, Warning, TEXT("[Input] EnhancedInputComponent cast failed")); return; }

	if (!IA_Move) { UE_LOG(LogTemp, Warning, TEXT("[Input] IA_Move is null (assign in BP)")); return; }

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);
	
	UEnhancedInputComponent* EnhancedInputComponent =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent) return;

	// 이동
	EnhancedInputComponent->BindAction(
		IA_Move,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::OnMove
	);

	// 카메라 줌
	EnhancedInputComponent->BindAction(
		IA_CameraZoom,
		ETriggerEvent::Triggered,
		this,
		&APlayerCharacter::OnZoom
	);

}

// 캐릭터 이동
void APlayerCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D Move = Value.Get<FVector2D>();
	
	if (!Controller) return;
	
	// 월드 기준 X, Y 사용
	const FVector Forward = FVector(1.f, 0.f, 0.f);
	const FVector Right = FVector(0.f, 1.f, 0.f);
	
	AddMovementInput(Forward, Move.Y);
	AddMovementInput(Right, Move.X);
}

// 카메라 줌
void APlayerCharacter::OnZoom(const FInputActionValue& Value)
{
	const float ZoomValue = Value.Get<float>();
	if (ZoomValue == 0.f || !CameraBoom) return;

	TargetArmLength = FMath::Clamp(
		TargetArmLength - ZoomValue * ZoomSpeed,
		MinArmLength,
		MaxArmLength
	);
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!CameraBoom) return;

	CameraBoom->TargetArmLength = FMath::FInterpTo(
		CameraBoom->TargetArmLength,
		TargetArmLength,
		DeltaTime,
		ZoomInterpSpeed
	);
}


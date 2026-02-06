// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Animation/AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.f;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->bUsePawnControlRotation = false;
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));

	FollowCamera  = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera ->SetupAttachment(CameraBoom);
	FollowCamera ->bUsePawnControlRotation = false;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] No PlayerController")); return;
	}

	ULocalPlayer* LP = PC->GetLocalPlayer();
	if (!LP)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] No LocalPlayer")); return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] No EnhancedInput subsystem")); return;
	}

	if (!IMC_Player)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] IMC_Player is null (assign in BP)")); return;
	}

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
	if (!EIC)
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] EnhancedInputComponent cast failed"));
		return;
	}

	// 이동
	if (IA_Move)
	{
		EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] IA_Move is null"));
	}

	// 카메라 줌
	if (IA_CameraZoom)
	{
		EIC->BindAction(IA_CameraZoom, ETriggerEvent::Triggered, this, &APlayerCharacter::OnZoom);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] IA_CameraZoom is null"));
	}

	// 대시
	if (IA_Dash)
	{
		EIC->BindAction(IA_Dash, ETriggerEvent::Started, this, &APlayerCharacter::OnDash);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[Input] IA_Dash is null"));
	}
	
}

// 캐릭터 이동
void APlayerCharacter::OnMove(const FInputActionValue& Value)
{
	if (bIsDashing)
	{
		return;
	}
	
	const FVector2D Move = Value.Get<FVector2D>();
	
	if (!Controller) return;
	
	if (!Move.IsNearlyZero()) LastMoveInput = Move;
	
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
	
	if (bDashMoving)
	{
		DashElapsed += DeltaTime;

		const float Duration = FMath::Max(DashTime, KINDA_SMALL_NUMBER);
		const float Speed = DashDistance / Duration;         
		float Step = Speed * DeltaTime;                     
		Step = FMath::Min(Step, DashRemaining);

		FVector Delta = DashDir * Step;

		// 충돌 유지 이동
		FHitResult Hit;
		AddActorWorldOffset(Delta, true, &Hit);

		DashRemaining -= Step;

		// 벽에 부딪히면 즉시 종료
		if (Hit.bBlockingHit || DashRemaining <= 0.f || DashElapsed >= Duration)
		{
			bDashMoving = false;
			DashRemaining = 0.f;
		}
	}
}

// 캐릭터 대시
void APlayerCharacter::OnDash(const FInputActionValue& Value)
{
	if (!bCanDash || bIsDashing || !DashMontage)
	{
		return;
	}
	
	UAnimInstance* AnimInst = (GetMesh() ? GetMesh()->GetAnimInstance() : nullptr);
	if (!AnimInst)
	{
		return;
	}
	
	if (AnimInst->Montage_IsPlaying(DashMontage))
	{
		return;
	}
	
	bCanDash = false;
	bIsDashing = true;
	bDashImpulseFired = false;

	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		SavedGroundFriction = MoveComp->GroundFriction;
		SavedBrakingDecel   = MoveComp->BrakingDecelerationWalking;
		
		MoveComp->GroundFriction = 0.f;
		MoveComp->BrakingDecelerationWalking = 0.f;

		MoveComp->StopMovementImmediately();
	}
	
	DashDir = FVector(LastMoveInput.Y, LastMoveInput.X, 0.f);
	if (DashDir.SizeSquared() < 1e-4f)
	{
		DashDir = GetActorForwardVector();
	}
	DashDir = DashDir.GetSafeNormal();
	
	const float Len = PlayAnimMontage(DashMontage);
	UE_LOG(LogTemp, Warning, TEXT("[Dash] PlayAnimMontage Len=%f"), Len);
	
	if (Len <= 0.f)
	{
		bIsDashing = false;
		bCanDash = true;
		return;
	}
	
	GetWorldTimerManager().SetTimer(
		DashEndHandle, 
		this, 
		&APlayerCharacter::EndDash, 
		DashTime, 
		false
		);
	
	// 쿨타임
	GetWorldTimerManager().SetTimer(
		DashCooldownHandle,
		this,
		&APlayerCharacter::ResetDash,
		DashCooldown,
		false
		);

	UE_LOG(LogTemp, Log, TEXT("[Dash] Dir=(%.2f, %.2f) Strength=%.0f"),
		DashDir.X, DashDir.Y, DashStrength);
}

void APlayerCharacter::EndDash()
{
	if (!bIsDashing)
	{
		return;
	}
	
	bIsDashing = false;
	bDashMoving = false;
	GetWorldTimerManager().ClearTimer(DashEndHandle);
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->GroundFriction = SavedGroundFriction;
		MoveComp->BrakingDecelerationWalking = SavedBrakingDecel;
		MoveComp->StopMovementImmediately();
	}
	
	if (UAnimInstance* AnimInst = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr)
	{
		if (DashMontage)
		{
			AnimInst->Montage_Stop(0.05f, DashMontage);
		}
	}
}

// 대시 초기화
void APlayerCharacter::ResetDash()
{
	bCanDash = true;
}


void APlayerCharacter::DashImpulse()
{
	if (!bIsDashing)
	{
		return;
	}
	if (bDashMoving) 
	{
		return;
	}
	bDashMoving = true;
	DashElapsed = 0.f;
	DashRemaining = DashDistance;
	
	UE_LOG(LogTemp, Warning, TEXT("[Dash] Impulse Fired"));

}
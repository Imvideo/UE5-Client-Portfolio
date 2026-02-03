// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "GameFramework/PlayerController.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!EIC) { UE_LOG(LogTemp, Warning, TEXT("[Input] EnhancedInputComponent cast failed")); return; }

	if (!IA_Move) { UE_LOG(LogTemp, Warning, TEXT("[Input] IA_Move is null (assign in BP)")); return; }

	EIC->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMove);
}

void APlayerCharacter::OnMove(const FInputActionValue& Value)
{
	const FVector2D Move = Value.Get<FVector2D>();
	UE_LOG(LogTemp, Log, TEXT("[Move] X=%.2f Y=%.2f"), Move.X, Move.Y);
}


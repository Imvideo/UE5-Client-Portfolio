#include "EnemyCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"
#include  "GameFramework/CharacterMovementComponent.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HP = MaxHP;
	
	USkeletalMeshComponent* MeshComp = GetMesh();
	if (!MeshComp) return;
	
	const int32 MatCount = MeshComp->GetNumMaterials();
	for (int32 i = 0; i < MatCount; i++)
	{
		UMaterialInstanceDynamic* MID = MeshComp->CreateDynamicMaterialInstance(i);
		
		if (MID)
		{
			HitFlashMIDs.Add(MID);
			
			FLinearColor OriginalColor;
			if (MID->GetVectorParameterValue(TEXT("Paint Tint"), OriginalColor))
			{
				OriginalPaintTints.Add(OriginalColor);
			}
			else
			{
				OriginalPaintTints.Add(FLinearColor::White);
			}
		}
	}
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = MoveSpeed;   
		MoveComp->bOrientRotationToMovement = true; 
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bDead) return;
	if (bIsHitStunned) return;
	
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) return;
	
	const FVector ToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	const float Distance = ToPlayer.Size();
	
	if (Distance <= StopDistance) return;
	
	const FVector Direction = ToPlayer.GetSafeNormal();
	
	AddMovementInput(Direction, 1.0f);
}

float AEnemyCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (bDead) return 0.f;
	
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	HP -= DamageAmount;
	
	UE_LOG(LogTemp, Log, TEXT("[Enemy] %s took %.1f damage. HP=%.1f"),
		*GetName(), DamageAmount, HP);
	
	
	
	if (HP <= 0.f)
	{
		Die();
		return DamageAmount;
	}
	
	StartHitFlash();
	StartHitStun();
	return DamageAmount;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Die()
{
	if (bDead) return;
	bDead = true;
	
	UE_LOG(LogTemp, Warning, TEXT("[Enemy] %s died"), *GetName());
	
	GetWorldTimerManager().ClearTimer(HitFlashHandle);
	GetWorldTimerManager().ClearTimer(HitStunHandle);
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
		MoveComp->DisableMovement();
	}
	
	SetActorTickEnabled(false);
	
	if (UCapsuleComponent* Cap = GetCapsuleComponent())
	{
		Cap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	if (USkeletalMeshComponent* MeshComp = GetMesh())
	{
		MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
	SetLifeSpan(DeathDelay);
}

void AEnemyCharacter::StartHitStun()
{
	if (bIsHitStunned) return;
	
	bIsHitStunned = true;
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}
	
	GetWorldTimerManager().ClearTimer(HitStunHandle);
	GetWorldTimerManager().SetTimer(HitStunHandle, this, &AEnemyCharacter::EndHitStun, HitStunTime, false);
}

void AEnemyCharacter::EndHitStun()
{
	bIsHitStunned = false;
}

void AEnemyCharacter::StartHitFlash()
{
	for (UMaterialInstanceDynamic* MID : HitFlashMIDs)
	{
		if (MID)
		{
			MID->SetVectorParameterValue(TEXT("Paint Tint"), HitFlashColor);
		}
	}
	
	GetWorldTimerManager().ClearTimer(HitFlashHandle);
	GetWorldTimerManager().SetTimer(
		HitFlashHandle,
		this,
		&AEnemyCharacter::EndHitFlash,
		HitFlashTime,
		false
		);
}

void AEnemyCharacter::EndHitFlash()
{
	const int32 Count = HitFlashMIDs.Num();
	for (int32 i = 0; i < Count; ++i)
	{
		if (HitFlashMIDs[i])
		{
			HitFlashMIDs[i]->SetVectorParameterValue(
				TEXT("Paint Tint"),
				OriginalPaintTints.IsValidIndex(i) ? OriginalPaintTints[i] : FLinearColor::White
				);
		}
	}
}

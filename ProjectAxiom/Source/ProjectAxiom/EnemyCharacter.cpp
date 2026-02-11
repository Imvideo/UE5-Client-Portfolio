#include "EnemyCharacter.h"

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
	
	if (UCharacterMovementComponent* MoveComp = GetCharacterMovement())
	{
		MoveComp->MaxWalkSpeed = MoveSpeed;   
		MoveComp->bOrientRotationToMovement = true; 
	}
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	HP -= DamageAmount;
	
	UE_LOG(LogTemp, Log, TEXT("[Enemy] %s took %.1f damage. HP=%.1f"),
		*GetName(), DamageAmount, HP);
	
	if (HP <= 0.f)
	{
		Die();
	}
	
	return DamageAmount;
}

void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("[Enemy] %s died"), *GetName());
	Destroy();
}


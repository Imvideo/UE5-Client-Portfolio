#include "EnemyCharacter.h"

AEnemyCharacter::AEnemyCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	HP = MaxHP;
}

void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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


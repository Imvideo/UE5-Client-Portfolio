// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class AEnemyCharacter;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEnemyDead, AEnemyCharacter*, Enemy);

UCLASS()
class PROJECTAXIOM_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float MaxHP = 30.f;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float HP = 0.f;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float HitStunTime = 0.2f;
	
	bool bIsHitStunned = false;
	FTimerHandle HitStunHandle;
	
	UPROPERTY(EditAnywhere, Category="Combat|HitFlash")
	float HitFlashTime = 0.2f;
	
	UPROPERTY(EditAnywhere, Category="Combat|HitFlash")
	FLinearColor HitFlashColor = FLinearColor::Red;
	
	TArray<UMaterialInstanceDynamic*> HitFlashMIDs;
	TArray<FLinearColor> OriginalPaintTints;
	
	FTimerHandle HitFlashHandle;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float StopDistance = 120.f;

	UPROPERTY(EditAnywhere, Category="Death")
	float DeathDelay = 1.5f;
	
	bool bDead = false;
	
	

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
		) override;
	
	// Enemy Spawn
	UPROPERTY(BlueprintAssignable, Category="Wave")
	FOnEnemyDead OnEnemyDead;
private:
	void Die();
	void StartHitStun();
	void EndHitStun();
	void StartHitFlash();
	void EndHitFlash();
};



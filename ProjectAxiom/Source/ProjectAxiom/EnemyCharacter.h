// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

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
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float MoveSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, Category="Movement")
	float StopDistance = 120.f;

public:	
	virtual void Tick(float DeltaTime) override;
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	virtual float TakeDamage(
		float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser
		) override;
private:
	void Die();
};



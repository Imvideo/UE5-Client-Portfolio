// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "PlayerCharacter.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;


UCLASS()
class PROJECTAXIOM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* IMC_Player;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Move;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float MinArmLength = 300.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float MaxArmLength = 1200.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomSpeed = 100.f;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_CameraZoom;
	
	UPROPERTY(EditAnywhere, Category="Camera|Zoom")
	float ZoomInterpSpeed = 12.f;
	
	float TargetArmLength = 0.f;


private:
	void OnMove(const FInputActionValue& Value);
	void OnZoom(const FInputActionValue& Value);
};

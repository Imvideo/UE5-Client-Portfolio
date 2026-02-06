#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "InputAction.h"
#include "InputMappingContext.h"
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
	
	// IMC_Player
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputMappingContext* IMC_Player;
	
	// IA_Move
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* IA_Move;
	
	// IA_CamerZoom
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_CameraZoom;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* FollowCamera;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float MinArmLength = 300.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float MaxArmLength = 1200.f;
	
	UPROPERTY(EditAnywhere, Category="Camera")
	float ZoomSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, Category="Camera|Zoom")
	float ZoomInterpSpeed = 12.f;
	
	float TargetArmLength = 0.f;
	
	// IA_Dash
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* IA_Dash;
	
	UPROPERTY(EditAnywhere, Category="Dash")
	float DashStrength = 1400.f;

	UPROPERTY(EditAnywhere, Category="Dash")
	float DashCooldown = 1.f;

	UPROPERTY(EditAnywhere, Category="Dash")
	float DashDuration = 0.10f; // 대시 무적 판정 시간
	
	UPROPERTY(EditAnywhere, Category="Dash")
	float DashSpeed = 900.f;

	UPROPERTY(EditAnywhere, Category="Dash")
	float DashTime = 0.12f;

	UPROPERTY(EditAnywhere, Category="Dash")
	float DashWindupTime = 0.05f; // 0.03~0.08 추천
	
	UPROPERTY(EditAnywhere, Category="Dash")
	float DashLockTime = 0.12f;
	FTimerHandle DashLockHandle;
	
	UPROPERTY(EditAnywhere, Category="Dash")
	float DashDistance = 800.f; 
	
	UPROPERTY(EditDefaultsOnly, Category="Dash|Animation")
	UAnimMontage* DashMontage = nullptr;
	
	bool bCanDash = true;
	FTimerHandle DashCooldownHandle;
	
	bool bIsDashing = false;
	FVector DashDirection = FVector::ZeroVector;
	
	bool bDashMoving = false;
	float DashElapsed = 0.f;
	float DashRemaining = 0.f;
	
	FTimerHandle DashEndHandle;
	FTimerHandle DashWindupHandle;
	
	
	FVector DashDir = FVector::ZeroVector;
	// 마지막 이동 입력(대시 방향용)
	FVector2D LastMoveInput = FVector2D(0.f, 1.f); // 기본 전방
	
	float SavedGroundFriction = 0.f;
	float SavedBrakingDecel = 0.f;
	bool bDashImpulseFired = false;
	
	UFUNCTION(BlueprintCallable, Category="Dash")
	void DashImpulse();
	

private:
	void OnMove(const FInputActionValue& Value);
	void OnZoom(const FInputActionValue& Value);
	void OnDash(const FInputActionValue& Value);
	void EndDash();
	void ResetDash();
	
	
};

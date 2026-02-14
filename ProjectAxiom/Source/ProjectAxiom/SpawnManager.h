#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnManager.generated.h"

class AEnemyCharacter;

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	int32 EnemyCount = 5;
	
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 0.3f;
};

UCLASS()
class PROJECTAXIOM_API ASpawnManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnManager();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Wave")
	TSubclassOf<AEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<AActor*> SpawnPoints;

	UPROPERTY(EditAnywhere, Category="Wave")
	TArray<FWaveData> Waves;

	int32 CurrentWaveIndex = 0;
	int32 AliveCount = 0;
	int32 SpawnedCount = 0;

	FTimerHandle SpawnTimerHandle;

	

	

	
public:	
	virtual void Tick(float DeltaTime) override;
	
	void StartWave(int32 WaveIndex);
	void SpawnEnemy();
	
	UFUNCTION()
	void HandleEnemyDead(AEnemyCharacter* Enemy);

};

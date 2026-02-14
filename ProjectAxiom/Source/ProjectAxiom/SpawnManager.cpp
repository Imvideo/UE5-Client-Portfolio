#include "SpawnManager.h"
#include "EnemyCharacter.h"
#include "TimerManager.h"

ASpawnManager::ASpawnManager()
{
	PrimaryActorTick.bCanEverTick = false;

}

void ASpawnManager::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("[Wave] SpawnManager BeginPlay: %s"), *GetName());
	StartWave(0);
}

void ASpawnManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnManager::StartWave(int32 WaveIndex)
{
	if (!Waves.IsValidIndex(WaveIndex) || !EnemyClass || SpawnPoints.Num() == 0)
		return;

	CurrentWaveIndex = WaveIndex;
	SpawnedCount = 0;
	AliveCount = 0;

	GetWorldTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&ASpawnManager::SpawnEnemy,
		Waves[WaveIndex].SpawnInterval,
		true
	);
}

void ASpawnManager::SpawnEnemy()
{
	const FWaveData& Wave = Waves[CurrentWaveIndex];

	if (SpawnedCount >= Wave.EnemyCount)
	{
		GetWorldTimerManager().ClearTimer(SpawnTimerHandle);
		return;
	}

	AActor* SpawnPoint = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
	if (!SpawnPoint) return;

	AEnemyCharacter* Enemy = GetWorld()->SpawnActor<AEnemyCharacter>(
		EnemyClass,
		SpawnPoint->GetActorLocation(),
		SpawnPoint->GetActorRotation()
	);

	if (!Enemy) return;

	AliveCount++;
	SpawnedCount++;

	Enemy->OnEnemyDead.AddDynamic(this, &ASpawnManager::HandleEnemyDead);
}

void ASpawnManager::HandleEnemyDead(AEnemyCharacter* Enemy)
{
	AliveCount = FMath::Max(AliveCount - 1, 0);

	const bool bWaveSpawnFinished =
		SpawnedCount >= Waves[CurrentWaveIndex].EnemyCount;

	if (bWaveSpawnFinished && AliveCount == 0)
	{
		const int32 NextWave = CurrentWaveIndex + 1;
		if (Waves.IsValidIndex(NextWave))
		{
			StartWave(NextWave);
		}
	}
}


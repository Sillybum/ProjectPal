// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EMyPlayerState : uint8
{
	Idle = 0 UMETA(DisplayName = "Idle"),	// 평소 상태
	Rolling UMETA(DisplayName = "Rolling"),	// 구르기 상태
	SpawningPal UMETA(DisplayName = "Spawning"),	// 팰 소환 중
	DespawningPal UMETA(DisplayName = "Despawning"),	// 팰 회수 중
	Exhaust UMETA(DisplayName = "Exhaust"),	// 스테미나를 다 사용 했을 때
	Dead UMETA(DisplayName = "Dead")	// (HP == 0)일 때
};

UCLASS()
class PROJECTPAL_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	EMyPlayerState PlayerState;
	FString StateString = UEnum::GetValueAsString(PlayerState);
	// 상태 변경 함수
	UFUNCTION(BlueprintCallable, Category = "PlayerState")
	FORCEINLINE void SetActionState(EMyPlayerState NewState) { PlayerState = NewState; }

	// 상태 확인 함수
	UFUNCTION(BlueprintPure, Category = "PlayerState")
	FORCEINLINE EMyPlayerState GetActionState() const { return PlayerState; }

	UFUNCTION()
	void PrintStateOnScreen()
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, *StateString);
	}
};

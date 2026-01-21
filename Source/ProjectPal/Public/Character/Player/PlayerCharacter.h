// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class PROJECTPAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// 카메라 붐 스틱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CCameraArm;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* CCamera;

	// 구르기 몽타주 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UAnimMontage* RollMontage;	
	
	// 카메라 관련 변수
	bool bIsAiming;
	// 시야각
	float DefaultFOV = 90.f;
	float AimFOV = 60.f;
	// 스프링 암
	float DefaultArmLength = 200.0f;
	float AimArmLength = 150.0f;
	
	
	// 속도 변화를 위한 변수
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float WalkSpeed = 50.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float JogSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float SprintSpeed = 600.0f;
	
	// 구르기 속도 함수 : Montage Play Rate값
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float RollSpeed = 1.0f;

// protected:
public:
	// 바인딩할 함수
	void Move(const FInputActionValue& Value);	// 기본 움직임(Jogging)
	void Look(const FInputActionValue& Value);	// 기본 시야
	void StartSprint(const FInputActionValue& Value);	// 달리기 시작
	void StopSprint(const FInputActionValue& Value);	// 달리기 끝
	void Roll();
	void SetAiming(bool bInAiming);
};

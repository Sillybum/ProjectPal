// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

struct FInputActionValue;
class UInputAction;
class UInputMappingContext;
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

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	// 카메라 붐 스틱
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	USpringArmComponent* CCameraArm;
	// 카메라
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess="true"))
	UCameraComponent* CCamera;

	// 액션 맵핑을 위한 변수
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* CMappingContext;

	// 점프
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* JumpAction;

	// 움직임
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* MoveAction;
	
	// 시야
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* LookAction;
	
	// 달리기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* SprintAction;
	
	// 구르기
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputAction* RollAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UAnimMontage* RollMontage;	// 구르기 몽타주 변수
	
	// 속도 변화를 위한 변수
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float JogSpeed = 400.0f;
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float SprintSpeed = 600.0f;
	
	// 구르기 속도 함수 : Montage Play Rate값
	UPROPERTY(EditAnywhere, Category="Movement", meta=(AllowPrivateAccess="true"))
	float RollSpeed = 1.0f;

protected:
	// 바인딩할 함수
	void Move(const FInputActionValue& Value);	// 기본 움직임(Jogging)
	void Look(const FInputActionValue& Value);	// 기본 시야
	void StartSprint(const FInputActionValue& Value);	// 달리기 시작
	void StopSprint(const FInputActionValue& Value);	// 달리기 끝
	void Roll();
};

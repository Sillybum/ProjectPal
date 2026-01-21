// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/Player/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/MyPlayerController.h"
#include "GameFramework/MyPlayerState.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 스켈레탈메시 로드
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> MeshAsset(
		TEXT("/Game/_Pal/Import/Player/Pal_Player.Pal_Player"));
	if (MeshAsset.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(MeshAsset.Object);
	}
	// Mesh의 Location, Rotation 설정
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.f, 0.f, -85.0f), FRotator(0.0f, -90.0f, 0.f));

	//캡슐 콜리전 크기 조절
	GetCapsuleComponent()->InitCapsuleSize(34.0f, 85.0f);

	// SpringArm 오브젝트 생성
	CCameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CCameraArm"));
	CCameraArm->SetupAttachment(RootComponent);
	CCameraArm->TargetArmLength = DefaultArmLength;
	CCameraArm->bUsePawnControlRotation = true; // 컨트롤러 회전에 따라 카메라 회전

	// Camera 오브젝트 생성
	CCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("CCamera"));
	CCamera->SetupAttachment(CCameraArm, USpringArmComponent::SocketName);

	// 이동 설정
	bUseControllerRotationYaw = false; // 마우스 회전해도 캐릭터 회전하지않음

	GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 캐릭터 회전
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // 캐릭터 회전 속도
	GetCharacterMovement()->JumpZVelocity = 500.0f; // 점프시 z축 속도
	// GetCharacterMovement()->AirControl = 0.35f;	// 공중에 떠있는 속도
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed; // 최대 이동속도
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f; // 최소 아날로그 속도
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f; // 제동속도
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 카메라 줌 수치 결정
	float TargetFOV = bIsAiming ? AimFOV : DefaultFOV;
	float TargetArmLength = bIsAiming ? AimArmLength : DefaultArmLength;

	// FInterpTo를 사용해 부드럽게 전환
	CCamera->FieldOfView = FMath::FInterpTo(CCamera->FieldOfView, TargetFOV, DeltaTime, 10.f);
	CCameraArm->TargetArmLength = FMath::FInterpTo(CCameraArm->TargetArmLength, TargetArmLength, DeltaTime, 10.f);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// 카메라 방향을 기준으로 앞, 뒤, 왼쪽, 오른쪽 계산
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// 앞 구하기
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// 오른쪽 구하기
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 움직임 추가
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// 컨트롤러의 축 입력 추가
		AddControllerYawInput(LookAxisVector.X); // X축 : Yaw
		AddControllerPitchInput(LookAxisVector.Y); // Y축 : Pitch
	}
}

void APlayerCharacter::StartSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed; // 최대 이동속도 변경
}

void APlayerCharacter::StopSprint(const FInputActionValue& Value)
{
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed; // 최대 이동속도 변경
}

void APlayerCharacter::Roll()
{
	// PlayerController를 받아옴
	AMyPlayerController* PC = Cast<AMyPlayerController>(GetController());
	// PlayerState를 받아옴
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();

	// PlayerState와 PlayerController를 받아오지 못했다면 실행하지 않음
	if (!PS || !PC) return;
	// 공중에서는 구르지 못함
	if (GetCharacterMovement()->IsFalling()) return;
	// 이미 구르기 중이라면 구르지 못하게 함 (PlayerState 이용)
	if (PS->GetActionState() == EMyPlayerState::Rolling) return;

	// --- 즉시 회전 로직 추가 ---
	// 현재 이동 입력 벡터를 가져옵니다 (MoveAction의 현재 값)
	// Controller에서 현재 입력 중인 축 값을 확인하거나, 마지막 이동 입력을 참조합니다.
	FVector LastInput = GetLastMovementInputVector();
	// 만약 입력이 있다면 (방향키를 누르고 있다면)
	if (!LastInput.IsNearlyZero())
	{
		// 입력 방향을 바라보도록 즉시 회전
		FRotator NewRotation = LastInput.Rotation();
		NewRotation.Pitch = 0.f;
		NewRotation.Roll = 0.f;
		SetActorRotation(NewRotation);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && RollMontage)
	{
		// EMyPlayerState를 Rolling으로 변경
		PS->SetActionState(EMyPlayerState::Rolling);
		// 현재 State를 기준으로 IMC 교체
		PC->UpdateInputContext();

		// 몽타주 실행 : 실행 시간을 Duration으로 저장 해줌
		float Duration = AnimInstance->Montage_Play(RollMontage, RollSpeed);

		// 몽타주가 끝날 때 상태를 복구하기 위한 타이머 (또는 Montage_SetEndDelegate 사용 가능)
		FTimerHandle TimerHandle;
		GetWorldTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([PS, PC]()
		{
			if (PC && PS && PS->GetActionState() == EMyPlayerState::Rolling)
			{
				PS->SetActionState(EMyPlayerState::Idle); // 또는 NONE
				PC->UpdateInputContext();
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("State: Rolling Finished"));
			}
		}), Duration, false);
	}
}

void APlayerCharacter::SetAiming(bool isAiming)
{
	bIsAiming = isAiming;
}

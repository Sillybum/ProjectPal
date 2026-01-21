// Fill out your copyright notice in the Description page of Project Settings.


#include "GameFramework/MyPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Character/Player/PlayerCharacter.h"
#include "GameFramework/MyPlayerState.h"

AMyPlayerController::AMyPlayerController()
{
}

void AMyPlayerController::UpdateInputContext()
{
	// 상태 정보 받아오기
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	ULocalPlayer* player = GetLocalPlayer();
	
	if (!PS || !player) return;
	
	// Local Player에서 EnhancedInputLocalPlayerSubsystem을 얻어옴
	UEnhancedInputLocalPlayerSubsystem* Subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;
	
	// 현재 적용중인 IMC를 제거 (중복 방지)
	Subsystem->ClearAllMappings();
	
	// 현재 상태에 따라 적절한 IMC 할당
	if (PS->GetActionState() == EMyPlayerState::Rolling)
	{
		if (RollingIMC)
		{
			Subsystem->AddMappingContext(RollingIMC, 0);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Input Mode: Rolling"));
		}
	}
	else
	{
		if (IdleIMC)
		{
			Subsystem->AddMappingContext(IdleIMC, 0);
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Input Mode: Idle"));
		}
	}
}

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	// 현재 PlayerController에 연결된 Local Player 객체를 가져옴
	ULocalPlayer* player = GetLocalPlayer();
	if (!player) return;
	
	// Local Player에서 EnhancedInputLocalPlayerSubsystem을 얻어옴
	UEnhancedInputLocalPlayerSubsystem* Subsystem = player->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (!Subsystem) return;
	if (!IdleIMC) return;
	
	// Subsystem을 통해 할당한 InputMappingContext를 활성화
	Subsystem->AddMappingContext(IdleIMC, 0);	// Priority : 0 -> 우선순위가 가장 높음
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move & Look
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Input_Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AMyPlayerController::Input_Look);

		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AMyPlayerController::Input_Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AMyPlayerController::Input_StopJump);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &AMyPlayerController::Input_StartSprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AMyPlayerController::Input_StopSprint);

		// Roll
		EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Started, this, &AMyPlayerController::Input_Roll);
		
		// Zoom
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &AMyPlayerController::Input_StartAim);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Completed, this, &AMyPlayerController::Input_StopAim);
	}
}

void AMyPlayerController::Input_Move(const FInputActionValue& Value)
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Move(Value); // 캐릭터의 Move 함수 호출
	}
}

void AMyPlayerController::Input_Look(const FInputActionValue& Value)
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Look(Value);
	}
}

void AMyPlayerController::Input_Jump()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Jump();
	}
}

void AMyPlayerController::Input_StopJump()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->StopJumping();
	}
}

void AMyPlayerController::Input_StartSprint()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->StartSprint(FInputActionValue()); // 매개변수 필요시 전달
	}
}

void AMyPlayerController::Input_StopSprint()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->StopSprint(FInputActionValue());
	}
}

void AMyPlayerController::Input_Roll()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->Roll();
	}
}

void AMyPlayerController::Input_StartAim()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->SetAiming(true);
	}
}

void AMyPlayerController::Input_StopAim()
{
	if (APlayerCharacter* ControlledChar = Cast<APlayerCharacter>(GetPawn()))
	{
		ControlledChar->SetAiming(false);
	}
}



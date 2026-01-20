// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/PlayerAnimInstance.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<ACharacter>(TryGetPawnOwner());
	if (Player)
	{
		MovementComponent = Player->GetCharacterMovement();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	if (MovementComponent)
	{
		Speed = MovementComponent->Velocity.Size(); // 속도
		
		Acceleration = MovementComponent->GetCurrentAcceleration(); // 가속도
		IsMoving = Acceleration.SizeSquared() > 0.0f;	// 움직임 여부 판단(빠른 계산을 위한 SizeSquared사용)
		
		IsFalling = MovementComponent->IsFalling(); // 공중 확인
		if (IsFalling) // 공중에 떴을 때 위로 가는중인지 아래로 떨어지는중인지 확인
		{
			// 올라가는지 내려가는지 판단
			if (Player->GetVelocity().Z > 0) { IsRising = true; }
			else if (Player->GetVelocity().Z <= 0) { IsRising = false; }
		}
		else { IsRising = false; }
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "MarksmanAnimInstance.h"
#include "MarksmanCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"

void UMarksmanAnimInstance::NativeInitializeAnimation()
{
    Super::NativeInitializeAnimation();
    MarksmanCharacter = Cast<AMarksmanCharacter>(TryGetPawnOwner());
}

void UMarksmanAnimInstance::NativeUpdateAnimation(float DeltaTime) 
{
    Super::NativeUpdateAnimation(DeltaTime);
    
    if (!MarksmanCharacter)
    {
        MarksmanCharacter = Cast<AMarksmanCharacter>(TryGetPawnOwner());
    }

    if (!MarksmanCharacter)
    {
        return;
    }

    FVector Velocity = MarksmanCharacter->GetVelocity();
    Velocity.Z = 0.f;
    Speed = Velocity.Size();

    bIsInAir = MarksmanCharacter->GetCharacterMovement()->IsFalling();
    
    bIsAccelerating = MarksmanCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0 ? true : false; 
}
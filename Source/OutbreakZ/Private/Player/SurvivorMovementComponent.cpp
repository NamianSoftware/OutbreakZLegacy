// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/SurvivorMovementComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Net/UnrealNetwork.h"

#pragma region SAVED_DATA
USurvivorMovementComponent::FSavedMove_CMP::FSavedMove_CMP()
{
	Saved_bWantsToJog = 0;
}

bool USurvivorMovementComponent::FSavedMove_CMP::CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter,
                                                                float MaxDelta) const
{
	const FSavedMove_CMP* NewCMPMove = static_cast<FSavedMove_CMP*>(NewMove.Get());

	if (NewCMPMove->Saved_bWantsToJog != Saved_bWantsToJog) return false;

	return Super::CanCombineWith(NewMove, InCharacter, MaxDelta);
}

void USurvivorMovementComponent::FSavedMove_CMP::Clear()
{
	FSavedMove_Character::Clear();

	Saved_bWantsToJog = 0;
}

uint8 USurvivorMovementComponent::FSavedMove_CMP::GetCompressedFlags() const
{
	uint8 Result = Super::GetCompressedFlags();

	if (Saved_bWantsToJog) Result |= FLAG_Custom_1;

	return Result;
}

void USurvivorMovementComponent::FSavedMove_CMP::SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel,
                                                            FNetworkPredictionData_Client_Character& ClientData)
{
	Super::SetMoveFor(C, InDeltaTime, NewAccel, ClientData);

	const auto CharacterMovement = Cast<USurvivorMovementComponent>(C->GetCharacterMovement());

	Saved_bWantsToJog = CharacterMovement->Safe_bWantsToJog;
}

void USurvivorMovementComponent::FSavedMove_CMP::PrepMoveFor(ACharacter* C)
{
	Super::PrepMoveFor(C);

	const auto CharacterMovement = Cast<USurvivorMovementComponent>(C->GetCharacterMovement());
	CharacterMovement->Safe_bWantsToJog = Saved_bWantsToJog;
}
#pragma endregion

#pragma region CLIENT_NETWORK_PREDICTION_DATA
USurvivorMovementComponent::FNetworkPredictionData_Client_CMP::FNetworkPredictionData_Client_CMP(
	const UCharacterMovementComponent& ClientMovement) : Super(ClientMovement)
{
}

FSavedMovePtr USurvivorMovementComponent::FNetworkPredictionData_Client_CMP::AllocateNewMove()
{
	return FSavedMovePtr(new FSavedMove_CMP());
}
#pragma endregion

USurvivorMovementComponent::USurvivorMovementComponent()
{
	NavAgentProps.bCanCrouch = true;

	MaxAcceleration = 250.f;
	BrakingFriction = 0.f;

	GroundFriction = 4.f;
	MaxWalkSpeedCrouched = 165.f;
	BrakingDecelerationWalking = 1000.f;

	AirControl = 0.f;
	AirControlBoostMultiplier = 0.f;
	AirControlBoostVelocityThreshold = 0.f;

	RotationRate = FRotator::ZeroRotator;
}

void USurvivorMovementComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(USurvivorMovementComponent, Safe_bWantsToJog);
}

FNetworkPredictionData_Client* USurvivorMovementComponent::GetPredictionData_Client() const
{
	check(PawnOwner);

	if (ClientPredictionData == nullptr)
	{
		USurvivorMovementComponent
			* MutableThis = const_cast<USurvivorMovementComponent
				*>(this);

		MutableThis->ClientPredictionData = new FNetworkPredictionData_Client_CMP(*this);
		MutableThis->ClientPredictionData->MaxSmoothNetUpdateDist = 92.f;
		MutableThis->ClientPredictionData->NoSmoothNetUpdateDist = 140.f;
	}

	return ClientPredictionData;
}

void USurvivorMovementComponent::UpdateFromCompressedFlags(uint8 Flags)
{
	Super::UpdateFromCompressedFlags(Flags);

	Safe_bWantsToJog = (Flags & FSavedMove_Character::FLAG_Custom_1) != 0;
}

void USurvivorMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation,
                                                   const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	const auto CurrentAcceleration = UKismetMathLibrary::MakeRotFromX(GetCurrentAcceleration());
	const auto Delta = UKismetMathLibrary::NormalizedDeltaRotator(CurrentAcceleration, GetPawnOwner()->GetActorRotation());
	const float MovementAngle = Delta.Yaw;
		
	if (MovementMode == MOVE_Walking)
	{
		float NewMaxSpeed;
		if (Safe_bWantsToJog)
		{
			NewMaxSpeed = JogDirectionSpeed->GetFloatValue(MovementAngle);
		}
		else
		{
			NewMaxSpeed = WalkDirectionSpeed->GetFloatValue(MovementAngle);
		}
		
		MaxWalkSpeed = NewMaxSpeed;
	}
}

void USurvivorMovementComponent::StartJog()
{
	Safe_bWantsToJog = true;
}

void USurvivorMovementComponent::StopJog()
{
	Safe_bWantsToJog = false;
}

void USurvivorMovementComponent::ToggleCrouch()
{
	bWantsToCrouch = !bWantsToCrouch;
}

void USurvivorMovementComponent::StartCrouch()
{
	bWantsToCrouch = true;
}

void USurvivorMovementComponent::StopCrouch()
{
	bWantsToCrouch = false;
}

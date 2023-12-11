// OutbreakZ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SurvivorMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class OUTBREAKZ_API USurvivorMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
	typedef UCharacterMovementComponent Super;

	class FSavedMove_CMP : public FSavedMove_Character
	{
		typedef FSavedMove_Character Super;

		uint8 Saved_bWantsToJog:1;

	public:
		FSavedMove_CMP();
		
		virtual bool CanCombineWith(const FSavedMovePtr& NewMove, ACharacter* InCharacter, float MaxDelta) const override;
		virtual void Clear() override;
		virtual uint8 GetCompressedFlags() const override;
		virtual void SetMoveFor(ACharacter* C, float InDeltaTime, FVector const& NewAccel, class FNetworkPredictionData_Client_Character & ClientData) override;
		virtual void PrepMoveFor(ACharacter* C) override;
	};

	class FNetworkPredictionData_Client_CMP : public FNetworkPredictionData_Client_Character
	{	
		typedef FNetworkPredictionData_Client_Character Super;
		
	public:
		FNetworkPredictionData_Client_CMP(const UCharacterMovementComponent& ClientMovement);
		
		virtual FSavedMovePtr AllocateNewMove() override;
	};

	UPROPERTY(Replicated)
	bool Safe_bWantsToJog;

	UPROPERTY(EditDefaultsOnly) UCurveFloat* WalkDirectionSpeed;
	UPROPERTY(EditDefaultsOnly) UCurveFloat* JogDirectionSpeed;

public:
	USurvivorMovementComponent();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	virtual FNetworkPredictionData_Client* GetPredictionData_Client() const override;

protected:
	virtual void UpdateFromCompressedFlags(uint8 Flags) override;
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;

public:
	UFUNCTION(BlueprintCallable) void StartJog();
	UFUNCTION(BlueprintCallable) void StopJog();
	
	UFUNCTION(BlueprintCallable) void ToggleCrouch();
	UFUNCTION(BlueprintCallable) void StartCrouch();
	UFUNCTION(BlueprintCallable) void StopCrouch();
};

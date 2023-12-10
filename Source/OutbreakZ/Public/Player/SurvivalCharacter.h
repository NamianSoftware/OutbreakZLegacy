// OutbreakZ

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "SurvivalCharacter.generated.h"


class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;


UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	ECM_FirstPerson UMETA(DisplatName = "FirstPerson"),
	ECM_ThirdPerson UMETA(DisplatName = "ThirdPerson"),
	ECM_NumCameraModes UMETA(Hidden)
};


UCLASS()
class OUTBREAKZ_API ASurvivalCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ASurvivalCharacter();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

#pragma region COMPONENTS

#pragma region CAMERA
private:
	UPROPERTY(EditDefaultsOnly, Category="Components|Camera|FirtsPersonCamera")
	UCameraComponent* FPCamera;

	UPROPERTY(EditDefaultsOnly, Category="Components|Camera|FirtsPersonCamera")
	FName CamaraSocketName = "CameraSocket";
	
	UPROPERTY(EditDefaultsOnly, Category="Components|Camera|ThirdPersonCamera")
	USpringArmComponent* TPCameraBoom;
	
	UPROPERTY(EditDefaultsOnly, Category="Components|Camera|FirtsPersonCamera")
	UCameraComponent* TPCamera;

	UPROPERTY(EditDefaultsOnly, Category="Components|Camera")
	ECameraMode CameraMode = ECameraMode::ECM_FirstPerson;

	void SwitchCameraMode(ECameraMode NewCameraMode);
#pragma endregion

#pragma region MESHES
	UPROPERTY
	(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* HeadMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* BodyMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* UpperBodyMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* VestMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* LowerBodyMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* SocksMesh;

	UPROPERTY(EditDefaultsOnly, Category="Components|Meshes")
	USkeletalMeshComponent* FeetMesh;
#pragma endregion
#pragma endregion

private:
	UPROPERTY()
	TArray<USkeletalMeshComponent*> SkeletalMeshComponents;
	
	UPROPERTY()
	TArray<USkeletalMeshComponent*> HiddenFirstPersonMeshComponents;

	
#pragma region INPUT

protected:
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

private:
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Jog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JogAction;

	/** Crouch Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	/** Jog Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchCameraAction;

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for jog input */
	void JogStarted(const FInputActionValue& Value);
	void JogFinished(const FInputActionValue& Value);

	/** Called for run input */
	void RunStarted(const FInputActionValue& Value);
	void RunFinished(const FInputActionValue& Value);

	/** Called for crouch input */
	void CrouchPressed(const FInputActionValue& Value);

	/** Called for jump input */
	void JumpPressed(const FInputActionValue& Value);
	void JumpReleased(const FInputActionValue& Value);

	/** Called for crouch input */
	void ToggleNextCameraMode(const FInputActionValue& Value);

#pragma endregion
};

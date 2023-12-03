// OutbreakZ

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SurvivalCharacter.generated.h"


class UCameraComponent;


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
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma region COMPONENTS

#pragma region Camera
private:
	UPROPERTY(EditDefaultsOnly, Category="Components|FirtsPersonCamera")
	UCameraComponent* FPCamera;

	UPROPERTY(EditDefaultsOnly, Category="Components|FirtsPersonCamera")
	FName CamaraSocketName = "CameraSocket";
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
};

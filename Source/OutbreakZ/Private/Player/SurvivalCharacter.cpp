// OutbreakZ


#include "Player/SurvivalCharacter.h"

#include "Camera/CameraComponent.h"

ASurvivalCharacter::ASurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

#pragma region INIT_MESHES
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	HeadMesh->bOwnerNoSee = true;
	SkeletalMeshComponents.Add(HeadMesh);
	
	BodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Body");
	SkeletalMeshComponents.Add(BodyMesh);

	UpperBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("UpperBody");
	SkeletalMeshComponents.Add(UpperBodyMesh);

	VestMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Vest");
	SkeletalMeshComponents.Add(VestMesh);

	LowerBodyMesh = CreateDefaultSubobject<USkeletalMeshComponent>("LowerBody");
	SkeletalMeshComponents.Add(LowerBodyMesh);

	SocksMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SocksMesh");
	SkeletalMeshComponents.Add(SocksMesh);

	FeetMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Feet");
	SkeletalMeshComponents.Add(FeetMesh);

	for (const auto MeshComp : SkeletalMeshComponents)
	{
		MeshComp->SetupAttachment(GetMesh());
	}
#pragma endregion

	FPCamera = CreateDefaultSubobject<UCameraComponent>("FPCamera");
	FPCamera->SetupAttachment(HeadMesh, CamaraSocketName);
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

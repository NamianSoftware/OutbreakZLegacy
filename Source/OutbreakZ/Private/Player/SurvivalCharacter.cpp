// OutbreakZ


#include "Player/SurvivalCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

ASurvivalCharacter::ASurvivalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

#pragma region INIT_MESHES
	HeadMesh = CreateDefaultSubobject<USkeletalMeshComponent>("Head");
	SkeletalMeshComponents.Add(HeadMesh);
	HiddenFirstPersonMeshComponents.Add(HeadMesh);

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
		MeshComp->SetLeaderPoseComponent(GetMesh());
	}

	for (const auto MeshComp : HiddenFirstPersonMeshComponents)
	{
		MeshComp->bCastHiddenShadow = true;
	}

#pragma endregion


#pragma region CAMERA
	FPCamera = CreateDefaultSubobject<UCameraComponent>("FPCamera");
	FPCamera->SetupAttachment(HeadMesh, CamaraSocketName);

	TPCameraBoom = CreateDefaultSubobject<USpringArmComponent>("TPCameraBoom");
	TPCameraBoom->SetupAttachment(GetRootComponent());
	TPCameraBoom->bEnableCameraLag = true;
	TPCameraBoom->bEnableCameraRotationLag = true;
	TPCameraBoom->CameraRotationLagSpeed = 20.f;
	TPCameraBoom->CameraLagMaxDistance = 30.f;

	TPCamera = CreateDefaultSubobject<UCameraComponent>("TPCamera");
	TPCamera->SetupAttachment(TPCameraBoom, TPCameraBoom->SocketName);
#pragma endregion
}

void ASurvivalCharacter::BeginPlay()
{
	Super::BeginPlay();

	SwitchCameraMode(CameraMode);

	if (const APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ASurvivalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateActorRotation();
}

void ASurvivalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASurvivalCharacter::JumpPressed);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this,
		                                   &ASurvivalCharacter::JumpReleased);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASurvivalCharacter::Look);

		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Started, this, &ASurvivalCharacter::JogStarted);
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Canceled, this, &ASurvivalCharacter::JogFinished);
		EnhancedInputComponent->BindAction(JogAction, ETriggerEvent::Completed, this, &ASurvivalCharacter::JogFinished);

		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this,
		                                   &ASurvivalCharacter::CrouchPressed);

		EnhancedInputComponent->BindAction(SwitchCameraAction, ETriggerEvent::Started, this,
		                                   &ASurvivalCharacter::ToggleNextCameraMode);
	}
}

void ASurvivalCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D MovementVector = Value.Get<FVector2D>();

	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void ASurvivalCharacter::Look(const FInputActionValue& Value)
{
	if (!Controller) return;

	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);
}

void ASurvivalCharacter::JogStarted(const FInputActionValue& Value)
{
}

void ASurvivalCharacter::JogFinished(const FInputActionValue& Value)
{
}

void ASurvivalCharacter::RunStarted(const FInputActionValue& Value)
{
}

void ASurvivalCharacter::RunFinished(const FInputActionValue& Value)
{
}

void ASurvivalCharacter::CrouchPressed(const FInputActionValue& Value)
{
}

void ASurvivalCharacter::JumpPressed(const FInputActionValue& Value)
{
	ACharacter::Jump();
}

void ASurvivalCharacter::JumpReleased(const FInputActionValue& Value)
{
	ACharacter::StopJumping();
}

void ASurvivalCharacter::ToggleNextCameraMode(const FInputActionValue& Value)
{
	const ECameraMode NextCameraMode = static_cast<ECameraMode>((static_cast<uint8>(CameraMode) + 1) % static_cast<
		uint8>(ECameraMode::ECM_NumCameraModes));
	SwitchCameraMode(NextCameraMode);
}

void ASurvivalCharacter::UpdateActorRotation()
{
	const auto CurrentAcceleration = GetCharacterMovement()->GetCurrentAcceleration();
	const auto CurrentSpeed = CurrentAcceleration.Size();
	if (UKismetMathLibrary::LessEqual_DoubleDouble(CurrentSpeed, 0.f)) return;

	const auto RotatorAcceleration = UKismetMathLibrary::MakeRotFromX(CurrentAcceleration);
	const auto BaseAimRotation = GetBaseAimRotation();

	const auto DirectionAngle = UKismetMathLibrary::NormalizedDeltaRotator(RotatorAcceleration, BaseAimRotation).Yaw;
	const auto Offset = DirectionCurve->GetFloatValue(DirectionAngle);
	const auto NewYaw = BaseAimRotation.Yaw + Offset;

	const auto DeltaTime = GetWorld()->GetDeltaSeconds();
	const auto NewRotation = UKismetMathLibrary::RInterpTo(
		GetActorRotation(),
		FRotator(0.f, NewYaw, 0.f),
		DeltaTime, RotationInterpSpeed);
	
	SetActorRotation(NewRotation);
}

void ASurvivalCharacter::SwitchCameraMode(ECameraMode NewCameraMode)
{
	CameraMode = NewCameraMode;

	bool bActiveFPCamera = false;
	bool bActiveTPCamera = false;
	bool bHideMeshes = false;

	switch (CameraMode)
	{
	case ECameraMode::ECM_FirstPerson:
		bActiveFPCamera = true;
		bActiveTPCamera = false;
		bHideMeshes = true;
		break;

	case ECameraMode::ECM_ThirdPerson:
		bActiveFPCamera = false;
		bActiveTPCamera = true;
		bHideMeshes = false;
		break;

	default:
		break;
	}

	FPCamera->SetActive(bActiveFPCamera);
	FPCamera->bUsePawnControlRotation = bActiveFPCamera;

	TPCamera->SetActive(bActiveTPCamera);
	TPCameraBoom->bUsePawnControlRotation = bActiveTPCamera;

	for (const auto MeshComp : HiddenFirstPersonMeshComponents)
	{
		MeshComp->SetOwnerNoSee(bHideMeshes);
	}
}

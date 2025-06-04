// Fill out your copyright notice in the Description page of Project Settings.


#include "GCPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PlayerController.h"
#include "GCPlayerController.h"

// Sets default values
AGCPlayer::AGCPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContext(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Input/IMC_Input.IMC_Input'"));
	if (InputMappingContext.Succeeded())
	{
		DefaultMappingContext = InputMappingContext.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_MOVE(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Move.IA_Move'"));
	if (IA_MOVE.Succeeded())
	{
		MoveAction = IA_MOVE.Object;
	}
	static ConstructorHelpers::FObjectFinder<UInputAction> IA_ZOOM(TEXT("/Script/EnhancedInput.InputAction'/Game/Input/IA_Zoom.IA_Zoom'"));
	if (IA_ZOOM.Succeeded())
	{
		ZoomAction = IA_ZOOM.Object;
	}

	CameraMoveSpeed = 20000.0f;
	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Create a camera boom...
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	RootComponent = Capsule;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(Capsule);
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->TargetArmLength = 3000;
	TargetArmTo = CameraBoom->TargetArmLength;
	CameraBoom->bDoCollisionTest = false;

	// Create a camera...
	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCameraComponent->SetProjectionMode(ECameraProjectionMode::Perspective);

	TopDownCameraComponent->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	TopDownCameraComponent->SetupAttachment(CameraBoom);
	TopDownCameraComponent->SetFieldOfView(45);
	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetCanBeDamaged(false);
	

}

// Called when the game starts or when spawned
void AGCPlayer::BeginPlay()
{
	Super::BeginPlay();
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem< UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
}

// Called every frame
void AGCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(FVector(
		FMath::Clamp(GetActorLocation().X + MovementVector.X * CameraMoveSpeed * DeltaTime, -38400, 38400),
		FMath::Clamp(GetActorLocation().Y + MovementVector.Y * CameraMoveSpeed * DeltaTime, -38400, 38400),
		GetActorLocation().Z
	));
	CameraBoom->TargetArmLength = FMath::FInterpConstantTo(CameraBoom->TargetArmLength, TargetArmTo, DeltaTime, 50000.0f);
}

// Called to bind functionality to input
void AGCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (Input != nullptr)
	{
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGCPlayer::Move);
		Input->BindAction(MoveAction, ETriggerEvent::Completed, this, &AGCPlayer::Move);
		Input->BindAction(ZoomAction, ETriggerEvent::Started, this, &AGCPlayer::Zoom);
	}
}

void AGCPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (TopDownCameraComponent != nullptr)
	{

		TopDownCameraComponent->SetAutoCalculateOrthoPlanes(true);
		TopDownCameraComponent->SetWorldRotation(FRotator(0.f,0.f, 0.0f));
		TopDownCameraComponent->AutoPlaneShift = -10000.f;
		CameraBoom->SetWorldRotation(FRotator(-25.f, 45.f, 0.0f));
	}
}

void AGCPlayer::Move(const FInputActionInstance& Instance)
{

	FVector2D VectorTmp = Instance.GetValue().Get<FVector2D>();
	FVector VectorTmp3D(-VectorTmp.X, VectorTmp.Y, .0f);
	FQuat QuatRotation = FQuat(FRotator(0.0f, -45.0f, 0.0f));
	FVector Rotated = QuatRotation.RotateVector(VectorTmp3D);
	MovementVector = FVector2D(Rotated.X, Rotated.Y);
}

void AGCPlayer::Zoom(const FInputActionInstance& Instance)
{
	FVector2d MovementVectorWheel = Instance.GetValue().Get<FVector2d>();
	
	if ((FMath::RoundToInt(CameraBoom->TargetArmLength) == 60000.f) 
		&& MovementVectorWheel.X > 0 
		&& TopDownCameraComponent->ProjectionMode == ECameraProjectionMode::Perspective)
	{

		TopDownCameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
		CameraMoveSpeed = 0;
		SetActorLocation(FVector(0.f, 0.f, 100.f));
		TopDownCameraComponent->OrthoWidth = 140000.f;
		CameraBoom->SetWorldRotation(FRotator(-90.f, 0.f, 0.0f));
	}
	if (FMath::RoundToInt(CameraBoom->TargetArmLength) == 60000.f
		&& MovementVectorWheel.X < 0
		&& TopDownCameraComponent->ProjectionMode == ECameraProjectionMode::Orthographic)
	{
		TopDownCameraComponent->SetProjectionMode(ECameraProjectionMode::Perspective);
		CameraMoveSpeed = 20000.f;
		
		CameraBoom->SetWorldRotation(FRotator(-25.f, 45.f, 0.0f));
	}
	TargetArmTo = FMath::Clamp(
		(CameraBoom->TargetArmLength + (MovementVectorWheel.X)*15000), 3000, 60000.f
	);
	
}


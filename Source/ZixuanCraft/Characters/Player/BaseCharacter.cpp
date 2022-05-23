// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Player/BaseCharacter.h"
#include "UI/ZixuanCraftWidgetBase.h"
#include "GameplayComponents/InventoryComponent.h"
#include "GameObjects/ZixuanCraftProjectile.h"
#include "Utils/TypeDefs.h"

#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MotionControllerComponent.h"
#include "XRMotionControllerBase.h" // for FXRMotionControllerBase::RightHandSourceId

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(45.0f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-33.8f, -6.96f, 56.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	// Create a gun mesh component
	FP_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Gun"));
	FP_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	FP_Gun->bCastDynamicShadow = false;
	FP_Gun->CastShadow = false;
	// FP_Gun->SetupAttachment(Mesh1P, TEXT("GripPoint"));
	FP_Gun->SetupAttachment(RootComponent);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(FP_Gun);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 100.0f, -10.6f));

	// Note: The ProjectileClass and the skeletal mesh/anim blueprints for Mesh1P, FP_Gun, and VR_Gun 
	// are set in the derived blueprint asset named MyCharacter to avoid direct content references in C++.

	// Create VR Controllers.
	R_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("R_MotionController"));
	R_MotionController->MotionSource = FXRMotionControllerBase::RightHandSourceId;
	R_MotionController->SetupAttachment(RootComponent);
	L_MotionController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("L_MotionController"));
	L_MotionController->SetupAttachment(RootComponent);

	// Create a gun and attach it to the right-hand VR controller.
	// Create a gun mesh component
	VR_Gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("VR_Gun"));
	VR_Gun->SetOnlyOwnerSee(false);			// otherwise won't be visible in the multiplayer
	VR_Gun->bCastDynamicShadow = false;
	VR_Gun->CastShadow = false;
	VR_Gun->SetupAttachment(R_MotionController);
	VR_Gun->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));

	VR_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("VR_MuzzleLocation"));
	VR_MuzzleLocation->SetupAttachment(VR_Gun);
	VR_MuzzleLocation->SetRelativeLocation(FVector(0.000004, 53.999992, 10.000000));
	VR_MuzzleLocation->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));		// Counteract the rotation of the VR gun model.

#if PLATFORM_ANDROID || PLATFORM_IOS
	GetCharacterMovement()->MaxStepHeight = 100.0f;
	BaseTurnRate *= 2.0f;
	BaseLookUpRate *= 2.0f;
#else
	GetCharacterMovement()->MaxStepHeight = 50.0f;
#endif

	GetCharacterMovement()->JumpZVelocity = 500.0f;
	
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

	// Uncomment the following line to turn motion controllers on by default:
	//bUsingMotionControllers = true;
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Attach gun mesh component to Skeleton, doing it here because the skeleton is not yet created in the constructor
	FP_Gun->AttachToComponent(Mesh1P, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));

	// Show or hide the two versions of the gun based on whether or not we're using motion controllers.
	if (bUsingMotionControllers)
	{
		VR_Gun->SetHiddenInGame(false, true);
		Mesh1P->SetHiddenInGame(true, true);
	}
	else
	{
		VR_Gun->SetHiddenInGame(true, true);
		Mesh1P->SetHiddenInGame(false, true);
	}
}

void ABaseCharacter::Attack()
{
	// try and fire a projectile
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			//Set Spawn Collision Handling Override
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.Instigator = GetInstigator();
			ActorSpawnParams.Owner = this;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			if (bUsingMotionControllers)
			{
				const FRotator SpawnRotation = VR_MuzzleLocation->GetComponentRotation();
				const FVector SpawnLocation = VR_MuzzleLocation->GetComponentLocation();
				World->SpawnActor<AZixuanCraftProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
			else
			{
				const FRotator SpawnRotation = GetControlRotation();
				// MuzzleOffset is in camera space, so transform it to world space before offsetting from the character location to find the final muzzle position
				const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
				// spawn the projectile at the muzzle
				World->SpawnActor<AZixuanCraftProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}

	// try and play the sound if specified
	if (FireSound != nullptr)
	{
		UGameplayStatics::PlaySoundAtLocation(this, FireSound, GetActorLocation(), 0.05f);
	}

	// try and play a firing animation if specified
	if (FireAnimation != nullptr)
	{
		// Get the animation object for the arms mesh
		UAnimInstance* AnimInstance = Mesh1P->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			AnimInstance->Montage_Play(FireAnimation, 1.f);
		}
	}
}

void ABaseCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed *= SpeedMultiplier;
	GetCharacterMovement()->MaxStepHeight *= 2.0f;
}

void ABaseCharacter::SlowDown()
{
	GetCharacterMovement()->MaxWalkSpeed /= SpeedMultiplier;
	GetCharacterMovement()->MaxStepHeight /= 2.0f; 
}

float ABaseCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Health -= Damage;
	UpdateHealthUI();

	return Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
}

void ABaseCharacter::InitWidget(UZixuanCraftWidgetBase* InWidget)
{
	Widget = InWidget;
	UpdateHealthUI();
	InitInventoryUI();
}

void ABaseCharacter::UpdateHealthUI()
{
	if (Widget)
	{
		Widget->UpdateHealthBarPercent(Health / MaxHealth);
	}
}

void ABaseCharacter::InitInventoryUI()
{
	// For every loot in my inventory, add it to the widget
	int32 Index = 0;
	for (const FLootSlot& Slot : InventoryComponent->GetInventory())
	{
		if (Index < Widget->GetGameplayInventoryNum())
		{
			Widget->SetButtonDataAt(Slot, Index);
		}

		Widget->SetButtonDataAt(Slot, Index + Widget->GetGameplayInventoryNum());
		++Index;
	}
}

void ABaseCharacter::ToggleInventory()
{
	if (Widget)
	{
		Widget->IToggleInventoryCrafting();
	}
}

void ABaseCharacter::ToggleInstructions()
{
	if (Widget)
	{
		Widget->ToggleInstructions();
	}
}

void ABaseCharacter::ScrollInventoryUp()
{
	if (Widget)
	{
		Widget->ScrollInventory(false);
	}
}

void ABaseCharacter::ScrollInventoryDown()
{
	if (Widget)
	{
		Widget->ScrollInventory(true);
	}
}

void ABaseCharacter::UpdateInventoryUI(int32 Index)
{
	check(Index != InvalidIndex);
	check(Widget->IsGameplayInventory(Index));
	const TArray<FLootSlot>& Inventory = InventoryComponent->GetInventory();
	Widget->SetButtonDataAt(Inventory[Index], Index);
	Widget->SetButtonDataAt(Inventory[Index], Index + Widget->GetGameplayInventoryNum());
}

void ABaseCharacter::LookUpAtRate(float Rate)
{
	if (!Widget || !Widget->IsDisplayingInventoryPanel())
	{
		// calculate delta for this frame from the rate information
		AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	// Attack
	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABaseCharacter::Attack);

	// Change speed event
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ABaseCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ABaseCharacter::SlowDown);

	// Enable touchscreen input
	EnableTouchscreenMovement(PlayerInputComponent);

	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ABaseCharacter::OnResetVR);

	// Bind movement events
	PlayerInputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);

	// UI
	PlayerInputComponent->BindAction("ToggleInstructions", IE_Pressed, this, &ABaseCharacter::ToggleInstructions);
	PlayerInputComponent->BindAction("ToggleInstructions", IE_Released, this, &ABaseCharacter::ToggleInstructions);
	PlayerInputComponent->BindAction("ShowInventory", IE_Pressed, this, &ABaseCharacter::ToggleInventory);
	PlayerInputComponent->BindAction("ScrollInventoryDown", IE_Pressed, this, &ABaseCharacter::ScrollInventoryDown);
	PlayerInputComponent->BindAction("ScrollInventoryUp", IE_Pressed, this, &ABaseCharacter::ScrollInventoryUp);
}

bool ABaseCharacter::EnableTouchscreenMovement(UInputComponent* PlayerInputComponent)
{
	if (FPlatformMisc::SupportsTouchInput() || GetDefault<UInputSettings>()->bUseMouseForTouch)
	{
		PlayerInputComponent->BindTouch(EInputEvent::IE_Pressed, this, &ABaseCharacter::BeginTouch);
		PlayerInputComponent->BindTouch(EInputEvent::IE_Released, this, &ABaseCharacter::EndTouch);

		//Commenting this out to be more consistent with FPS BP template.
		PlayerInputComponent->BindTouch(EInputEvent::IE_Repeat, this, &ABaseCharacter::TouchUpdate);
		return true;
	}

	return false;
}

void ABaseCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		//Attack();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void ABaseCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//Commenting this section out to be consistent with FPS BP template.
//This allows the user to turn without using the right virtual joystick
void ABaseCharacter::TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if ((TouchItem.bIsPressed == true) && (TouchItem.FingerIndex == FingerIndex))
	{
		if (TouchItem.bIsPressed)
		{
			if (GetWorld() != nullptr)
			{
				UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
				if (ViewportClient != nullptr)
				{
					FVector MoveDelta = Location - TouchItem.Location;
					FVector2D ScreenSize;
					ViewportClient->GetViewportSize(ScreenSize);
					FVector2D ScaledDelta = FVector2D(MoveDelta.X, MoveDelta.Y) / ScreenSize;
					if (FMath::Abs(ScaledDelta.X) >= 4.0 / ScreenSize.X)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.X * BaseTurnRate;
						AddControllerYawInput(Value);
					}
					if (FMath::Abs(ScaledDelta.Y) >= 4.0 / ScreenSize.Y)
					{
						TouchItem.bMoved = true;
						float Value = ScaledDelta.Y * BaseTurnRate;
						AddControllerPitchInput(Value);
					}
					TouchItem.Location = Location;
				}
				TouchItem.Location = Location;
			}
		}
	}
}

void ABaseCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ABaseCharacter::MoveForward(float Value)
{
	if (!Widget || !Widget->IsDisplayingInventoryPanel())
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorForwardVector(), Value);
		}
	}
}

void ABaseCharacter::MoveRight(float Value)
{
	if (!Widget || !Widget->IsDisplayingInventoryPanel())
	{
		if (Value != 0.0f)
		{
			// add movement in that direction
			AddMovementInput(GetActorRightVector(), Value);
		}
	}
}

void ABaseCharacter::TurnAtRate(float Rate)
{
	if (!Widget || !Widget->IsDisplayingInventoryPanel())
	{ 
		// calculate delta for this frame from the rate information
		AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
	}
}


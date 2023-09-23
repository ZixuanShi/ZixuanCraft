// Fill out your copyright notice in the Description page of Project Settings.
// BaseCharacter.h
// Author: Zixuan Shi
// Base character class used in the game. All characters should be inherited from this

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameObjects/Loot/LootData.h"
#include "BaseCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UMotionControllerComponent;
class UAnimMontage;
class USoundBase;
class UZixuanCraftWidgetBase;
class UInventoryComponent;

/**
 * Base FPS shooter character with minimal data to be controlled by the player. 
 * Attached a camera in the world to display the game
 * Inventory component
 * Widget UI which mirrors the inventory and health data
 */
UCLASS()
class ZIXUANCRAFT_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Touch */
	struct TouchData
	{
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;

		TouchData()
		{
			bIsPressed = false;
			Location = FVector::ZeroVector;
		}
	};

protected:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Gun mesh: 1st person view (seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* FP_Gun;

	/** Location on gun mesh where projectiles should spawn. */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USceneComponent* FP_MuzzleLocation;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate = 45.0f;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate = 45.0f;

	/** Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class AZixuanCraftProjectile> ProjectileClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* FireSound;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/** Whether to use motion controller location for aiming. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	uint8 bUsingMotionControllers : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = MaxHealth;

	/** Used when sprinting/slowing down */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	float SpeedMultiplier = 2.0f;

	/** Back reference of this character's widget */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UZixuanCraftWidgetBase* Widget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent = nullptr;

public:
	ABaseCharacter();

	virtual void BeginPlay() override;
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	void HealToMaxHealth();

	/** UI */
	UFUNCTION(BlueprintCallable)
	void InitWidget(UZixuanCraftWidgetBase* InWidget);

	void UpdateHealthUI();
	void InitInventoryUI();
	void ToggleInventory();
	void ToggleInstructions();
	void ScrollInventoryUp();
	void ScrollInventoryDown();

	/**
	* Update a single slot in inventory UI
	* @param Index			The index of inventory in widget to update
	*/
	void UpdateInventoryUI(int32 Index);

	UZixuanCraftWidgetBase* GetWidget() const { return Widget; }
	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

protected:
	/** Touches */
	TouchData TouchItem;
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);

	/** Input actions */
	virtual void Attack();
	void Sprint();
	void SlowDown();

	/** Handles moving forward/backward */
	void MoveForward(float Value);

	/** Handles stafing movement, left and right */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	/*
	 * Configures input for touchscreen devices if there is a valid touch interface for doing so
	 *
	 * @param	InputComponent	The input component pointer to bind controls to
	 * @returns true if touch controls were enabled.
	 */
	bool EnableTouchscreenMovement(UInputComponent* PlayerInputComponent);
};

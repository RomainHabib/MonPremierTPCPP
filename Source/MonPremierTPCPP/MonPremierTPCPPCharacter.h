// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MonPremierTPCPPCharacter.generated.h"

UCLASS(config=Game)
class AMonPremierTPCPPCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
public:
	AMonPremierTPCPPCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float playerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool isDead = false;

	UPROPERTY(EditAnywhere)
		float timeBeforeRespawn;

	UPROPERTY(EditAnywhere)
    class UStaticMeshComponent* ShootPointMesh;
	
	UPROPERTY(VisibleAnywhere)
	bool hasSomethingInHand = false;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* PickupPointMesh;
	
	UPROPERTY(VisibleAnywhere)
	AActor* actorInHand;

	UPROPERTY(EditAnywhere)
	UAnimBlueprint* animClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isCrouching = false;
	
	float internDeathTimer;
	bool hasRespawned = false;


protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
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

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION()
		void GetDamage(float healthToRemove);

	UFUNCTION()
		void DoRagdoll();

	UFUNCTION()
		void Die();
	
	UFUNCTION()
		void Respawn();

	UFUNCTION()
		void Pickup();

	UFUNCTION()
	void Hold();
	
	UFUNCTION()
	void Release();

	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;

	UFUNCTION()
	void GetCrouch();

	UFUNCTION()
	void Shoot();
	

	void CheckLifeState();
	void UpdateDeathTimer(float deltaTime);
};


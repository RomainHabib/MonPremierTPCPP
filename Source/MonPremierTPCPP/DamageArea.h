// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MonPremierTPCPPCharacter.h"
#include "GameFramework/Actor.h"
#include "DamageArea.generated.h"

UCLASS()
class MONPREMIERTPCPP_API ADamageArea : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageArea();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnEnter(AActor* enteredActor, AActor* otherActor);

	UFUNCTION()
	void OnExit(AActor* exitedActor, AActor* otherActor);
	void DealDamage(float deltaTime);

	UPROPERTY(VisibleAnywhere)
	AMonPremierTPCPPCharacter* enteredCharacter;

	UPROPERTY(VisibleAnywhere)
	bool hasSomeoneIn;

	UPROPERTY(EditAnywhere)
	bool canOneShot = false;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	float damageAmount;
	
	UPROPERTY(EditAnywhere)
	float damageTickSpeed;

	float internTimer = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

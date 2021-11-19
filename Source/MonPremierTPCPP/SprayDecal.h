// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SprayDecal.generated.h"

UCLASS()
class MONPREMIERTPCPP_API ASprayDecal : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASprayDecal();

	UFUNCTION()
	void DestroySprayDecal();

	UPROPERTY(EditAnywhere)
	UDecalComponent* Decal;

	UPROPERTY(VisibleAnywhere)
	UMaterial* SelectionMaterial;
	
	UPROPERTY(VisibleAnywhere)
	UMaterialInstance* MaterialInstance;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

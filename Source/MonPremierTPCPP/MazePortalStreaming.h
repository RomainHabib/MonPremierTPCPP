// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MazePortalStreaming.generated.h"

UCLASS()
class MONPREMIERTPCPP_API AMazePortalStreaming : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMazePortalStreaming();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
    	void OnEnter(AActor* enteredActor, class AActor* otherActor);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	TArray<FName> levelsToLoad;

	UPROPERTY(EditAnywhere)
	TArray<FName> levelsToUnload;


};

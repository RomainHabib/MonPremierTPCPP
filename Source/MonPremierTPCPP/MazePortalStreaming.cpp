// Fill out your copyright notice in the Description page of Project Settings.


#include "MazePortalStreaming.h"
#include "MonPremierTPCPPCharacter.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AMazePortalStreaming::AMazePortalStreaming()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/Geometry/Meshes/1M_Cube.1M_Cube'"));
    
    	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
    	
    	if (CubeMesh.Succeeded()) 
        	{
        		StaticMesh->SetStaticMesh(CubeMesh.Object);
        	}

	OnActorBeginOverlap.AddDynamic(this, &AMazePortalStreaming::OnEnter);

}

void AMazePortalStreaming::OnEnter(AActor* enteredActor, class AActor* otherActor)
{
	AMonPremierTPCPPCharacter* character = Cast<AMonPremierTPCPPCharacter>(otherActor);

	if (character == nullptr) 
	{
		return;
	}
	
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("player has stepped on a portal"));

	
	// for(FName levelName : levelsToLoad)
	// {
	for (int i = 0; i < levelsToLoad.Num(); i++)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevel(this, levelsToLoad[i], true, false, LatentInfo);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Emerald, TEXT("loading a level"));
	}
	// }

	// for(FName levelName : levelsToUnload)
	// {
	for (int i = 0; i < levelsToUnload.Num(); i++)
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, levelsToUnload[i], LatentInfo, false);
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Turquoise, TEXT("unloading a level"));
	}
	// }
}


// Called when the game starts or when spawned
void AMazePortalStreaming::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMazePortalStreaming::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


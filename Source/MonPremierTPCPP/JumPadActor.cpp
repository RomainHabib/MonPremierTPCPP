// Fill out your copyright notice in the Description page of Project Settings.


#include "JumPadActor.h"
#include "GameFramework/Character.h"
#include "MonPremierTPCPPCharacter.h"

// Sets default values
AJumPadActor::AJumPadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Aller chercher un objet depuis les fichiers
	//static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMesh(TEXT("StaticMesh'/Game/ThirdPerson/Meshes/Linear_Stair_StaticMesh.Linear_Stair_StaticMesh'"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	StaticMesh->SetSimulatePhysics(false);
	RootComponent = StaticMesh;

	// Assigner l'objet trouvé dans les fichiers
	/*if (CubeMesh.Succeeded()) 
	{
		StaticMesh->SetStaticMesh(CubeMesh.Object);
	}*/

	StaticMesh->OnComponentHit.AddDynamic(this, &AJumPadActor::OnHit);
}



void AJumPadActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (character == nullptr) 
	{
		return;
	}

	character->LaunchCharacter(FVector(0.0f, 0.0f, 1000.0f), false, true);
	
	// AMonPremierTPCPPCharacter* player = Cast<AMonPremierTPCPPCharacter>(OtherActor);
	// player->GetDamage(10);
	
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green, TEXT("Yahoouuu"));
}


// Called when the game starts or when spawned
void AJumPadActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AJumPadActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//StaticMesh->OnComponentHit.AddDynamic(this, &AJumPadActor::OnHit);
}



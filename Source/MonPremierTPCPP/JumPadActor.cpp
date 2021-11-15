// Fill out your copyright notice in the Description page of Project Settings.


#include "JumPadActor.h"
#include "GameFramework/Character.h"

// Sets default values
AJumPadActor::AJumPadActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	//StaticMesh->SetSimulatePhysics(true);
	RootComponent = StaticMesh;

	StaticMesh->OnComponentHit.AddDynamic(this, &AJumPadActor::OnHit);
}



void AJumPadActor::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!character) 
	{
		return;
	}

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Yahoouuu"));
	character->LaunchCharacter(FVector(0.0f, 0.0f, 1000.0f), true, true);
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

}


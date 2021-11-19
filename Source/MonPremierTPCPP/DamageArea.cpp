// Fill out your copyright notice in the Description page of Project Settings.


#include "DamageArea.h"

#include "MonPremierTPCPPCharacter.h"

// Sets default values
ADamageArea::ADamageArea()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	StaticMesh->SetSimulatePhysics(false);
	RootComponent = StaticMesh;

	OnActorBeginOverlap.AddDynamic(this, &ADamageArea::OnEnter);
	OnActorEndOverlap.AddDynamic(this, &ADamageArea::OnExit);
}

// Called when the game starts or when spawned
void ADamageArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADamageArea::OnEnter(AActor* enteredActor, class AActor* otherActor)
{
	AMonPremierTPCPPCharacter* character = Cast<AMonPremierTPCPPCharacter>(otherActor);

	if (character == nullptr) 
	{
		return;
	}
	
	ADamageArea::hasSomeoneIn = true;
	enteredCharacter = character;
	internTimer = damageTickSpeed;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("player has stepped on the zone"));
}

void ADamageArea::OnExit(AActor* exitedActor, class AActor* otherActor)
{
	enteredCharacter = nullptr;
	ADamageArea::hasSomeoneIn = false;
}

void ADamageArea::DealDamage(float deltaTime)
{
	if(!hasSomeoneIn)
	{
		return;
	}

	if(!canOneShot)
	{
		if(internTimer > 0)
		{
			internTimer -= deltaTime;
		}
		else
		{
			enteredCharacter->GetDamage(damageAmount);
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, FString::Printf(TEXT("Zone dealed : %f"), damageAmount));
			internTimer = damageTickSpeed;
		}
	}
	else
	{
		enteredCharacter->GetDamage((enteredCharacter->playerHealth));
	}
}

// Called every frame
void ADamageArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DealDamage(DeltaTime);
}


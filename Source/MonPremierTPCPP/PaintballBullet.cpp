// Fill out your copyright notice in the Description page of Project Settings.


#include "PaintballBullet.h"
#include "SprayDecal.h"

// Sets default values
APaintballBullet::APaintballBullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereMesh(TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere'"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeSM"));
	StaticMesh->SetSimulatePhysics(false);
	RootComponent = StaticMesh;
	
	// Assigner l'objet trouvé dans les fichiers
	if (SphereMesh.Succeeded()) 
	{
		StaticMesh->SetStaticMesh(SphereMesh.Object);
	}

	StaticMesh->SetWorldScale3D(FVector(0.2f,0.2f,0.2f));
	
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->InitialSpeed = 2500;
	
	
	StaticMesh->OnComponentHit.AddDynamic(this, &APaintballBullet::OnHit);
}

// Called when the game starts or when spawned
void APaintballBullet::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &APaintballBullet::DestroyPaintball, 3, false);
}

void APaintballBullet::DestroyPaintball()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Magenta, TEXT("Destroying a paintball"));
	Destroy();
}

void APaintballBullet::ApplyPaintTag(FHitResult hitInfo)
{
	FVector Location = hitInfo.Location;
	FRotator Rotation = hitInfo.ImpactNormal.Rotation();
	
	ASprayDecal* newDecal = GetWorld()->SpawnActor<ASprayDecal>(Location, Rotation);

	USceneComponent* hitSceneComponent = Cast<USceneComponent>(hitInfo.GetActor());

	if(hitSceneComponent == nullptr)
	{
		return;
	}
	
	FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, true);
	newDecal->GetRootComponent()->AttachToComponent(hitSceneComponent, rules, NAME_None);
}

void APaintballBullet::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& hit)
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, TEXT("A paintball touched something"));
	ApplyPaintTag(hit);
	DestroyPaintball();
}

// Called every frame
void APaintballBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Copyright Epic Games, Inc. All Rights Reserved.

#include "MonPremierTPCPPCharacter.h"

#include "DrawDebugHelpers.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "PaintballBullet.h"
#include "GameFramework/GameModeBase.h"

//////////////////////////////////////////////////////////////////////////
// AMonPremierTPCPPCharacter

AMonPremierTPCPPCharacter::AMonPremierTPCPPCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	AMonPremierTPCPPCharacter::playerHealth = 100.0f;
	AMonPremierTPCPPCharacter::timeBeforeRespawn = 2.0f;

	// FVector shootPointLocation = GetCapsuleComponent()->GetComponentLocation();
	// shootPointLocation.X = GetCapsuleComponent()->GetComponentLocation().X + 175;

	ShootPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShootPoint"));
	ShootPointMesh->SetupAttachment(GetCapsuleComponent());
	// ShootPointMesh->SetWorldLocation(shootPointLocation);

	// FVector pickupPointLocation = GetCapsuleComponent()->GetComponentLocation();
	// pickupPointLocation.X = GetCapsuleComponent()->GetComponentLocation().X + 200;

	PickupPointMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupPoint"));
	PickupPointMesh->SetupAttachment(GetCapsuleComponent());
	// PickupPointMesh->SetWorldLocation(pickupPointLocation);
	
    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletonMesh(TEXT("SkeletalMesh'/Game/Animations/T-Pose__2_.T-Pose__2_'"));
 
	if(SkeletonMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SkeletonMesh.Object, false);
	}

	GetMesh()->SetWorldLocation(FVector(0,0,-90.0f));
	GetMesh()->SetWorldRotation(FRotator(0,-90.0f,0));

	// GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//
	// if(animClass != nullptr)
	// {
	// 	GetMesh()->SetAnimClass(animClass->GetClass());
	// }

	// static ConstructorHelpers::FClassFinder<ACharacter> character(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Player_BP.Player_BP'"));
	//
	// if(character.Succeeded() && character.Class != nullptr)
	// {
		// AMonPremierTPCPPCharacter* characterForAnim = Cast<AMonPremierTPCPPCharacter>(character.Class);
	// }
		
	
	//Temporaire, ça va eviter de faire crasher le lancement d'Unreal quand c'est dans le constructeur du personnage
	// static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimationInstance(TEXT("AnimBlueprint'/Game/Animations/WrestlerAnim_BP.WrestlerAnim_BP'"));
	

	 // if(animClass == nullptr)
	 // {
	 // 	 static ConstructorHelpers::FObjectFinder<UAnimInstance> AnimationInstance(TEXT("AnimBlueprint'/Game/Animations/WrestlerAnim_BP.WrestlerAnim_BP'"));
	 // 	 // static ConstructorHelpers::FClassFinder<UAnimBlueprintGeneratedClass> AnimInstance(TEXT("AnimBlueprint'/Game/Animations/WrestlerAnim_BP.WrestlerAnim_BP'"));
	 // 	
	 //   	if(AnimationInstance.Succeeded())
	 //   	{
	 //   		animClass = AnimationInstance.Class;
	 //   		GetMesh()->SetAnimInstanceClass(animClass->GetClass());
	 //   	}
	 // }
	 // else
	 // {
	 // 	GetMesh()->SetAnimClass(animClass->GetBlueprintClass());
	 // }
	 //
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMonPremierTPCPPCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMonPremierTPCPPCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMonPremierTPCPPCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMonPremierTPCPPCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMonPremierTPCPPCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMonPremierTPCPPCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMonPremierTPCPPCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AMonPremierTPCPPCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Shoot", IE_Pressed, this, &AMonPremierTPCPPCharacter::Shoot);
	PlayerInputComponent->BindAction("Pickup", IE_Pressed, this, &AMonPremierTPCPPCharacter::Pickup);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMonPremierTPCPPCharacter::GetCrouch);
	
	
}

void AMonPremierTPCPPCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	FVector shootPointLocation = GetCapsuleComponent()->GetComponentLocation();
	shootPointLocation.X = GetCapsuleComponent()->GetComponentLocation().X + 175;

	ShootPointMesh->SetWorldLocation(shootPointLocation);

	FVector pickupPointLocation = GetCapsuleComponent()->GetComponentLocation();
	pickupPointLocation.X = GetCapsuleComponent()->GetComponentLocation().X + 200;

	PickupPointMesh->SetWorldLocation(pickupPointLocation);
}

void AMonPremierTPCPPCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateDeathTimer(DeltaTime);
	
	//StaticMesh->OnComponentHit.AddDynamic(this, &AJumPadActor::OnHit);
}

void AMonPremierTPCPPCharacter::GetCrouch()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Player pressed the crouch button"));

	if(isCrouching)
	{
		isCrouching = false;
		GetCharacterMovement()->UnCrouch();
	}
	
	else
	{
		isCrouching = true;
		GetCharacterMovement()->Crouch();
	}
}

void AMonPremierTPCPPCharacter::Shoot()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Shoot key has been pressed"));

	// FVector Location(20.0f, 20.0f, 400.0f);
	FRotator Rotation = ShootPointMesh->GetComponentRotation();
	FActorSpawnParameters SpawnInfo;

	FVector Location = ShootPointMesh->GetComponentLocation();
	APaintballBullet* newBullet = GetWorld()->SpawnActor<APaintballBullet>(Location, Rotation, SpawnInfo);
}

void AMonPremierTPCPPCharacter::UpdateDeathTimer(float deltaTime) 
{
	if (!AMonPremierTPCPPCharacter::isDead) 
	{
		return;
	}

	if (AMonPremierTPCPPCharacter::internDeathTimer > 0) 
	{
		internDeathTimer -= deltaTime;
	}

	else 
	{
		if (!hasRespawned) 
		{
			AMonPremierTPCPPCharacter::Respawn();
			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Purple, TEXT("Player is respawning!"));
			AMonPremierTPCPPCharacter::hasRespawned = true;
		}
	}
}

void AMonPremierTPCPPCharacter::Respawn() 
{
	AController* controller = GetController();
	controller->UnPossess();

	FVector Location(20.0f, 20.0f, 400.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;

	ACharacter* newPlayer = GetWorld()->SpawnActor<ACharacter>(GetWorld()->GetAuthGameMode()->DefaultPawnClass, Location, Rotation,SpawnInfo);
	
	if(newPlayer != nullptr)
	{
		controller->Possess(newPlayer);	
	}

	// TSubclassOf<AActor> player;
	// ConstructorHelpers::FClassFinder<AActor> BPFinder(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Player_BP.Player_BP_C'"));
	//
	// if(BPFinder.Succeeded())
	// {
	// 	player = BPFinder.Class;
	// 	AActor* newPlayer = GetWorld()->SpawnActor<AActor>(BPFinder.Class,Location, Rotation,SpawnInfo);
	// 	ACharacter* newPlayerCharacter = Cast<ACharacter>(newPlayer);
	// 	controller->Possess(newPlayerCharacter);
	// }
	
	// APawn* newPlayer = GetWorld()->SpawnActor<APawn>(Location, Rotation, SpawnInfo);
	//
	// static ConstructorHelpers::FObjectFinder<UBlueprint> ItemBlueprint(TEXT("Blueprint'/Game/ThirdPersonCPP/Blueprints/Player_BP.Player_BP_C'"));
	//
	// if(ItemBlueprint.Succeeded())
	// {
	// 	
	// }
	// else
	// {
	// 	
	// }
	
	// newPlayer = GetWorld()->GetAuthGameMode()->DefaultPawnClass;
	// ACharacter* newPlayer = GetWorld()->SpawnActor<AMonPremierTPCPPCharacter>(Location, Rotation, SpawnInfo);
	// controller->Possess(newPlayer);
}

void AMonPremierTPCPPCharacter::Pickup()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Orange, TEXT("Player pressed the pickup button"));

	if(!actorInHand)
	{
		Hold();
	}
	else
	{
		Release();
	}
	
}

void AMonPremierTPCPPCharacter::Hold()
{
	FHitResult OutHit;
	FVector Start = GetCapsuleComponent()->GetComponentLocation();

	FVector ForwardVector = GetCapsuleComponent()->GetForwardVector();
	FVector End = ((ForwardVector * 1000.f) + Start);
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 2,0,10);

	if(GetWorld()->LineTraceSingleByChannel(OutHit, Start, End, ECC_Visibility, CollisionParams)) 
	{
		if(OutHit.bBlockingHit)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, FString::Printf(TEXT("Player hitted: %s"), *OutHit.GetActor()->GetName()));
			// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Purple, FString::Printf(TEXT("Hit impact point: %s"), *OutHit.ImpactPoint.ToString()));
			// GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString::Printf(TEXT("Hit normal point: %s"), *OutHit.ImpactNormal.ToString()));

			AActor* touchedActor = Cast<AActor>(OutHit.Actor);
			if(touchedActor != nullptr)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Silver, FString::Printf(TEXT("An actor is now in hand ")));
				actorInHand = touchedActor;

				//SUREMENT A VIRER ATTENTION ROMOR DU FUTUR
				OutHit.Component->SetSimulatePhysics(false);

				FAttachmentTransformRules rules(EAttachmentRule::KeepWorld, true);
				actorInHand->AttachToActor(this, rules, "PickupPoint");
				actorInHand->SetActorLocation(PickupPointMesh->GetComponentLocation());
				hasSomethingInHand = true;
			}
		}
	}
}

void AMonPremierTPCPPCharacter::Release()
{
	FDetachmentTransformRules rules(FDetachmentTransformRules::KeepWorldTransform);
	actorInHand->DetachFromActor(rules);

	UPrimitiveComponent* actorPrimitive = Cast<UPrimitiveComponent>(actorInHand->GetRootComponent());

	if(actorPrimitive != nullptr)
	{
		actorPrimitive->SetSimulatePhysics(true);
	}
	
	actorInHand = nullptr;
	hasSomethingInHand = false;
}

void AMonPremierTPCPPCharacter::GetDamage(float healthToRemove)
{
	AMonPremierTPCPPCharacter::playerHealth -= healthToRemove;

	AMonPremierTPCPPCharacter::CheckLifeState();
}

void AMonPremierTPCPPCharacter::DoRagdoll()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetCapsuleComponent()->SetCollisionProfileName("NoCollision");
	GetCharacterMovement()->DisableMovement();
}

void AMonPremierTPCPPCharacter::Die()
{
	AMonPremierTPCPPCharacter::isDead = true;
	AMonPremierTPCPPCharacter::internDeathTimer = AMonPremierTPCPPCharacter::timeBeforeRespawn;

	if(hasSomethingInHand)
	{
		Release();
	}
}

void AMonPremierTPCPPCharacter::CheckLifeState()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Player health : %f"), AMonPremierTPCPPCharacter::playerHealth));

	if (AMonPremierTPCPPCharacter::playerHealth <= 0) 
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("Player died !"));
		
		DoRagdoll();
		Die();
	}
}


void AMonPremierTPCPPCharacter::OnResetVR()
{
	// If MonPremierTPCPP is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in MonPremierTPCPP.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AMonPremierTPCPPCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AMonPremierTPCPPCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AMonPremierTPCPPCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMonPremierTPCPPCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AMonPremierTPCPPCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMonPremierTPCPPCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
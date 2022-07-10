// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "Gun.h"
#include "Components/CapsuleComponent.h"
#include "SimpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ShooterPlayerController.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	PlayerInputComponent->BindAxis(TEXT("MoveForward"),this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(TEXT("MoveRight"),this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAxis(TEXT("LookUp"),this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(TEXT("LookRight"),this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(TEXT("LookUpRate"),this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(TEXT("LookRightRate"),this, &AShooterCharacter::LookRightRate);
	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Jump);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Pressed, this, &AShooterCharacter::FirePress);
	PlayerInputComponent->BindAction(TEXT("PullTrigger"), EInputEvent::IE_Released, this, &AShooterCharacter::FireReleased);
	PlayerInputComponent->BindAction(TEXT("Pause"), EInputEvent::IE_Pressed, this, &AShooterCharacter::PauseGame).bExecuteWhenPaused = true;
	PlayerInputComponent->BindAction(TEXT("ReloadAmmo"), EInputEvent::IE_Pressed, this, &AShooterCharacter::ReloadAmmo);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Pressed, this, &AShooterCharacter::Run);
	PlayerInputComponent->BindAction(TEXT("Run"), EInputEvent::IE_Released, this, &AShooterCharacter::StopRun);

}

float AShooterCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser)
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    DamageToApply = FMath::Min(Health, DamageToApply);
	Health -= DamageToApply;
	
    if(IsDeath())
	{
		ASimpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASimpleShooterGameModeBase>();
		if(GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
	}
    return DamageToApply;
}

bool AShooterCharacter::IsDeath() const
{
    return Health <=0;
}

float AShooterCharacter::GetHealthPercent() const
{
	return Health / MaxHealth;
}

void AShooterCharacter::MoveForward(float AxisValue)
{
    AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue)
{
    AddMovementInput(GetActorRightVector() * AxisValue);
}

void AShooterCharacter::LookUpRate(float AxisValue)
{
    AddControllerPitchInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::LookRightRate(float AxisValue)
{
    AddControllerYawInput(AxisValue * RotationRate * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Jump()
{
    Super::Jump();
}

void AShooterCharacter::Fire()
{
	if(Gun->GetAmmo())
	{
	    Gun->PullTrigger();
	    StartAutoFire();   
	    UE_LOG(LogTemp, Warning, TEXT("Fire"));	
	}
	if(FireAnim != nullptr)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	    AnimInstance->UAnimInstance::Montage_Play(FireAnim);
	    AnimInstance->Montage_JumpToSection(FName("Fire"));
	}
	
}

void AShooterCharacter::StartAutoFire()
{
	GetWorldTimerManager().SetTimer(AutoFireTimer, this, &AShooterCharacter::FireReset, Gun->AutoFireRate, true);
	UE_LOG(LogTemp, Warning, TEXT("StartAutoFire()"));
}

void AShooterCharacter::FireReset()
{
	if(Gun->IsAutoFire && IsFirePressed)
	{
		Gun->PullTrigger();
		UE_LOG(LogTemp, Warning, TEXT("FireReset()"));
		if(FireAnim != nullptr)
	    {
		    UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	        AnimInstance->UAnimInstance::Montage_Play(FireAnim);
	        AnimInstance->Montage_JumpToSection(FName("Fire"));
	    }
	}
}

void AShooterCharacter::PauseGame()
{
	AShooterPlayerController* ShooterPlayerController = Cast<AShooterPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if(ShooterPlayerController)
	{
        ShooterPlayerController->PauseGame(); 
	}
}

void AShooterCharacter::ReloadAmmo()
{
	Gun->Reload();
}

void AShooterCharacter::FirePress()
{
	IsFirePressed = true;
	Fire();
	PlayerMovement->MaxWalkSpeed = 300;
	UE_LOG(LogTemp, Warning, TEXT("FirePress"));
}
void AShooterCharacter::FireReleased()
{
    IsFirePressed = false;
	UE_LOG(LogTemp, Warning, TEXT("FireReleased"));
}

void AShooterCharacter::Run()
{
	PlayerMovement->MaxWalkSpeed = 600;
}

void AShooterCharacter::StopRun()
{
   	PlayerMovement->MaxWalkSpeed = 300;
}
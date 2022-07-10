// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(Root);
	Ammo = MaxAmmo;


}

void AGun::PullTrigger()
{
    if(bReload) return;
	FHitResult HitPoint;
	FVector ShotDirection;

    bool bSuccess = GunTrace(HitPoint, ShotDirection);

    if(bSuccess && GetAmmo())
	{
		DecreaseAmmo();  
		UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, TEXT("MuzzleFlashSocket"));
	    UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, TEXT("MuzzleFlashSocket"));
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitPoint.Location, ShotDirection.Rotation());
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, HitPoint.Location, ShotDirection.Rotation());
			
		AActor* HitActor = HitPoint.GetActor();
		if(HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, HitPoint, ShotDirection, nullptr);
			AController* OwnerController = GetOwnerController();
            HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
	else
	{
		UGameplayStatics::SpawnSoundAttached(AmmoEmptySound, Mesh, TEXT("MuzzleFlashSocket"));
	}
	
}

void AGun::Reload()
{
	if(Ammo < MaxAmmo && !bReload)
	{
		bReload = true;
		UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, TEXT("MuzzleFlashSocket"));
	    FTimerHandle ReloadTimer;
		GetWorldTimerManager().SetTimer(ReloadTimer, this, &AGun::ReloadAmmo, 1);
	}
}

void AGun::ReloadAmmo()
{
	Ammo = MaxAmmo;
	bReload = false;
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& HitPoint, FVector& ShotDirection)
{
    AController* OwnerController = GetOwnerController();
	
	if( OwnerController == nullptr) 
	{
        return false;
	}  

	FVector Location;
	FRotator Rotation;
	OwnerController->GetPlayerViewPoint(Location, Rotation);
	ShotDirection = -Rotation.Vector();

	FVector End = Location + Rotation.Vector() * MaxRange;
    
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetOwner());
	return GetWorld()->LineTraceSingleByChannel(HitPoint, Location, End, ECollisionChannel::ECC_GameTraceChannel1, Params);
}

AController* AGun::GetOwnerController() const
{
	APawn *OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr)
		return nullptr;
	return OwnerPawn->GetController();
}

void AGun::DecreaseAmmo()
{
	Ammo--;
}




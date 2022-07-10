// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();

	void PullTrigger();
	void Reload();
	

    UFUNCTION(BlueprintPure)
	float GetAmmoCount()const{ return Ammo;}

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	bool IsAutoFire;

	UPROPERTY(EditAnywhere)
	float AutoFireRate = 0.8;

	bool GetAmmo() { return Ammo > 0;}


private:

    UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;
    
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	float MaxRange = 1000;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* ImpactSound;

	UPROPERTY(EditAnywhere)
	USoundBase* AmmoEmptySound;

	UPROPERTY(EditAnywhere)
	USoundBase* ReloadSound;

    

	UPROPERTY(EditAnywhere)
	float Damage = 10;

	UPROPERTY()
	float Ammo;

	UPROPERTY(EditAnywhere)
	float MaxAmmo = 30;

	
    bool bReload;
	

	

	

	bool GunTrace(FHitResult& HitPoint, FVector& ShotDirection);

	AController* GetOwnerController() const;

	void ReloadAmmo();

	void DecreaseAmmo();
	

	

	

};

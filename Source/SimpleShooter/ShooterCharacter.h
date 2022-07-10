// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	UFUNCTION(BlueprintPure)
	bool IsDeath() const;


	UFUNCTION(BlueprintPure)
	float GetHealthPercent() const;

	void Fire();
	void StartAutoFire();
	void FireReset();
	void PauseGame();
	void ReloadAmmo();
	void FirePress();
	void FireReleased();
	void Run();
	void StopRun();

	UPROPERTY(BlueprintReadOnly)
	class AGun* Gun;

private:

    void MoveForward(float AxisValue);
	void MoveRight(float AxisValue);
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	
	
	UPROPERTY(EditAnywhere)
	float RotationRate = 10;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;
    UPROPERTY(VisibleAnywhere)
	float Health;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* FireAnim;


	virtual void Jump() override;
	

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AGun> GunClass;

	class UCharacterMovementComponent* PlayerMovement = GetCharacterMovement();

	

    UPROPERTY()
	bool IsFirePressed;

    UPROPERTY()
	FTimerHandle AutoFireTimer;

	
};

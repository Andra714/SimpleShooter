// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ShooterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AShooterPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	
	virtual void GameHasEnded(class AActor* EndGameFocus = nullptr, bool bIsWinner = false) override;
	UFUNCTION(BlueprintCallable)
	void PauseGame();
	void DisplayPauseMenu();
	void RemovePauseMenu();

	UPROPERTY()
	bool bPause = false;
	UPROPERTY()
	UUserWidget* PauseMenu;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:

    UPROPERTY(EditAnywhere)
	TSubclassOf<class UUserWidget> LoseScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WinScreenClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> HUDClass;
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> WPauseMenu;
	
	
    UPROPERTY(EditAnywhere)
	float RestartDelay = 5;

	FTimerHandle RestartTimer;
    
	UPROPERTY()
	UUserWidget *HUD;
	
};

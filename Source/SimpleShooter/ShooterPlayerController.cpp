// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterPlayerController.h"
#include "TimerManager.h"
#include "Blueprint/UserWidget.h"

// Called when the game starts or when spawned
void AShooterPlayerController::BeginPlay()
{
	Super::BeginPlay();
	
	HUD = CreateWidget(this,  HUDClass);
    if(HUD != nullptr)
        {
            HUD->AddToViewport();
        }
    PauseMenu = CreateWidget(this, WPauseMenu);
    if(PauseMenu != nullptr)
        {
            PauseMenu->AddToViewport();
            PauseMenu->SetVisibility(ESlateVisibility::Hidden);
        }
}

void AShooterPlayerController::GameHasEnded(class AActor* EndGameFocus , bool bIsWinner)
{
    Super::GameHasEnded( EndGameFocus, bIsWinner);
    UE_LOG(LogTemp, Warning, TEXT("We have finished!"));
    
    HUD->RemoveFromViewport();
    if(bIsWinner)
    {
        UUserWidget* WinScreen = CreateWidget(this,  WinScreenClass);
        if(WinScreen != nullptr)
        {
            WinScreen->AddToViewport();
        }
    }
    else
    {
        UUserWidget* LoseScreen = CreateWidget(this, LoseScreenClass);
        if(LoseScreen != nullptr)
        {
            LoseScreen->AddToViewport();
        }
    }
    
    GetWorldTimerManager().SetTimer(RestartTimer, this, &APlayerController::RestartLevel, RestartDelay);
    
}

void AShooterPlayerController::PauseGame()
{
    
    if(PauseMenu)
    {
        if(!bPause)
        {
            bPause = true;
            SetPause(true);
            UE_LOG(LogTemp, Warning, TEXT("Pause true"));
            DisplayPauseMenu();
        }
        else
        {
            bPause = false;
            SetPause(false);
            UE_LOG(LogTemp, Warning, TEXT("Pause false"));
            RemovePauseMenu();
        }
    }
}

void AShooterPlayerController::DisplayPauseMenu()
{
    PauseMenu->SetVisibility(ESlateVisibility::Visible);
    bShowMouseCursor = bPause;
}
void AShooterPlayerController::RemovePauseMenu()
{
    PauseMenu->SetVisibility(ESlateVisibility::Hidden);
    bShowMouseCursor = bPause;
}
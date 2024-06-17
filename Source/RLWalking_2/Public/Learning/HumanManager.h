// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HumanManager.generated.h"

class ULearningAgentsInteractor;
class ULearningAgentsTrainer;
class UHumanLearningAgentsTrainer;
class ULearningAgentsPolicy;
class ULearningAgentsCritic;
class ULearningAgentsManager;

UCLASS()
class RLWALKING_2_API AHumanManager : public AActor
{
    GENERATED_BODY()

public:
    AHumanManager();

    virtual void Tick(float DeltaTime) override;

protected:
    virtual void BeginPlay() override;

protected:
    void AddTickPrequisiteForAgents();
    void CreateInteractor();
    void CreatePolicy();
    void CreateCritic();
    void CreateTrainer();

protected:
    UPROPERTY()
    ULearningAgentsManager* LearningAgentsManager;

protected:
    ULearningAgentsInteractor* Interactor;
    ULearningAgentsCritic* Critic;
    ULearningAgentsPolicy* Policy;
    ULearningAgentsTrainer* Trainer;
};
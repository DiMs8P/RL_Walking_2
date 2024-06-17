// Fill out your copyright notice in the Description page of Project Settings.


#include "Learning/HumanManager.h"

#include "LearningAgentsCritic.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsManager.h"
#include "LearningAgentsPolicy.h"
#include "Kismet/GameplayStatics.h"
#include "LearningAgentsTraining/Public/LearningAgentsTrainer.h"


AHumanManager::AHumanManager()
{
    PrimaryActorTick.bCanEverTick = true;

    LearningAgentsManager = CreateDefaultSubobject<ULearningAgentsManager>(TEXT("LearningAgentsManager"));
}

void AHumanManager::BeginPlay()
{
    Super::BeginPlay();

    AddTickPrequisiteForAgents();
    CreateInteractor();
    CreatePolicy();
    CreateCritic();
    CreateTrainer();
    
}

void AHumanManager::AddTickPrequisiteForAgents()
{
    TArray<AActor*> OutResult;
    UGameplayStatics::GetAllActorsOfClass(this, AActor::StaticClass(), OutResult);

    for (auto Result : OutResult)
    {
        Result->AddTickPrerequisiteActor(this);
    }
}

void AHumanManager::CreateInteractor()
{
    Interactor = ULearningAgentsInteractor::MakeInteractor(LearningAgentsManager, ULearningAgentsInteractor::StaticClass(), FName("HumanInteractor"));
}

void AHumanManager::CreatePolicy()
{
    FLearningAgentsPolicySettings LearningAgentsPolicySettings;
    LearningAgentsPolicySettings.HiddenLayerNum = 10;
    LearningAgentsPolicySettings.HiddenLayerSize = 512;
    LearningAgentsPolicySettings.bUseMemory = true;
    LearningAgentsPolicySettings.MemoryStateSize = 64;
    LearningAgentsPolicySettings.InitialEncodedActionScale = 0.1;
    LearningAgentsPolicySettings.ActivationFunction = ELearningAgentsActivationFunction::ELU;
    
    Policy = ULearningAgentsPolicy::MakePolicy(LearningAgentsManager, Interactor, ULearningAgentsPolicy::StaticClass(), FName("HumanPolicy"), LearningAgentsPolicySettings);
}

void AHumanManager::CreateCritic()
{
    FLearningAgentsCriticSettings LearningAgentsCriticSettings;
    LearningAgentsCriticSettings.HiddenLayerNum = 7;
    LearningAgentsCriticSettings.HiddenLayerSize = 512;
    LearningAgentsCriticSettings.ActivationFunction = ELearningAgentsActivationFunction::ELU;
    
    Critic = ULearningAgentsCritic::MakeCritic(LearningAgentsManager, Interactor, Policy, ULearningAgentsCritic::StaticClass(), FName("HumanCritic"), LearningAgentsCriticSettings);
}

void AHumanManager::CreateTrainer()
{
    FLearningAgentsTrainerSettings LearningAgentsTrainerSettings;
    LearningAgentsTrainerSettings.MaxEpisodeStepNum = 512;
    LearningAgentsTrainerSettings.MaximumRecordedEpisodesPerIteration = 1000;
    LearningAgentsTrainerSettings.MaximumRecordedStepsPerIteration = 10000;
    LearningAgentsTrainerSettings.TrainerCommunicationTimeout = 10.0f;
    
    Trainer = ULearningAgentsTrainer::MakeTrainer(LearningAgentsManager, Interactor, Policy, Critic, FName("HumanTrainer"), LearningAgentsTrainerSettings);
}

void AHumanManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}


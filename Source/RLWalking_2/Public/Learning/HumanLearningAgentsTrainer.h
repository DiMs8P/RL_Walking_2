// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LearningAgentsTraining/Public/LearningAgentsTrainer.h"
#include "HumanLearningAgentsTrainer.generated.h"

/**
 * 
 */
UCLASS()
class RLWALKING_2_API UHumanLearningAgentsTrainer : public ULearningAgentsTrainer
{
    GENERATED_BODY()

public:
    virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;
    virtual void GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;
    virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};

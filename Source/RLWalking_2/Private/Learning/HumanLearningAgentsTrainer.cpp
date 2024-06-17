// Fill out your copyright notice in the Description page of Project Settings.


#include "Learning/HumanLearningAgentsTrainer.h"

void UHumanLearningAgentsTrainer::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
    OutReward =
        GetAgent(AgentId)->GetTimeReward() +
        GetAgent(AgentId)->GetDistanceReward() +
        GetAgent(AgentId)->GetHeightReward() +
        GetAgent(AgentId)->GetOrientationReward();
    
    Super::GatherAgentReward_Implementation(OutReward, AgentId);
}

void UHumanLearningAgentsTrainer::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId)
{
    const ELearningAgentsCompletion TimeComplition = GetAgent(AgentId)->GetTimeComplition();
    const ELearningAgentsCompletion RewardComplition = GetAgent(AgentId)->GetRewardComplition();
    const ELearningAgentsCompletion GetGroundComplition = GetAgent(AgentId)->GetGetGroundComplition();

    OutCompletion = ELearningAgentsCompletion::Running;
    if (TimeComplition == ELearningAgentsCompletion::Termination || RewardComplition == ELearningAgentsCompletion::Termination || GetGroundComplition == ELearningAgentsCompletion::Termination)
    {
        OutCompletion = ELearningAgentsCompletion::Termination;
    }
    
    Super::GatherAgentCompletion_Implementation(OutCompletion, AgentId);
}

void UHumanLearningAgentsTrainer::ResetAgentEpisode_Implementation(const int32 AgentId)
{
    GetAgent(AgentId)->ResetState();
    Super::ResetAgentEpisode_Implementation(AgentId);
}
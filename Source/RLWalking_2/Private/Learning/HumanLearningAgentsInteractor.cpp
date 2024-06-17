// Fill out your copyright notice in the Description page of Project Settings.


#include "RLWalking_2/Public/Learning/HumanLearningAgentsInteractor.h"

void UHumanLearningAgentsInteractor::SpecifyAgentObservation_Implementation(
    FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
    const FLearningAgentsObservationSchemaElement LocationObservation = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, FName("LocationObservation"));
    const FLearningAgentsObservationSchemaElement RotationObservation = ULearningAgentsObservations::SpecifyRotationObservation(InObservationSchema, FName("RotationObservation"));

    const FLearningAgentsObservationSchemaElement RelativeJointsLocationObservationData = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema, FName("LocationObservation"));
    const FLearningAgentsObservationSchemaElement RelativeJointsLocationObservation = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, RelativeJointsLocationObservationData, 13, FName("RotationObservation"));

    const FLearningAgentsObservationSchemaElement RelativeJointsRotationObservationData = ULearningAgentsObservations::SpecifyRotationObservation(InObservationSchema, FName("RotationObservation"));
    const FLearningAgentsObservationSchemaElement RelativeJointsRotationObservation = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, RelativeJointsRotationObservationData, 13, FName("RotationObservation"));

    const FLearningAgentsObservationSchemaElement RelativeJointsVelocityObservationData = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, FName("VelocityObservation"));
    const FLearningAgentsObservationSchemaElement RelativeJointsAngularVelocityObservation = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, RelativeJointsVelocityObservationData, 14, FName("StaticArrayObservation"));
    const FLearningAgentsObservationSchemaElement RelativeJointsLinearVelocityObservation = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, RelativeJointsVelocityObservationData, 14, FName("StaticArrayObservation"));

    const FLearningAgentsObservationSchemaElement RelativeJointsHeightObservationData = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema, FName("VelocityObservation"));
    const FLearningAgentsObservationSchemaElement RelativeJointsHeightObservation = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, RelativeJointsHeightObservationData, 14, FName("RotationObservation"));
    
    TMap<FName, FLearningAgentsObservationSchemaElement> Map;
    Map.Add(FName("Goal Location Observation"), LocationObservation);
    Map.Add(FName("Global Actor Rotation"), RotationObservation);
    Map.Add(FName("Relative Joints Location"), RelativeJointsLocationObservation);
    Map.Add(FName("Relative Joints Rotation"), RelativeJointsRotationObservation);
    Map.Add(FName("Relative Joints Angular Velocity"), RelativeJointsAngularVelocityObservation);
    Map.Add(FName("Relative Joints Linear Velocity"), RelativeJointsLinearVelocityObservation);
    Map.Add(FName("Joints Height"), RelativeJointsHeightObservation);
    OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, Map);
    Super::SpecifyAgentObservation_Implementation(OutObservationSchemaElement, InObservationSchema);
}

void UHumanLearningAgentsInteractor::GatherAgentObservation_Implementation(
    FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject,
    const int32 AgentId)
{
    TMap<FName, FLearningAgentsObservationSchemaElement> Map;
    Map.Add(FName("Goal Location Observation"), GetAgent(AgentId)->GetObservations().GoalLocationObservation());
    Map.Add(FName("Global Actor Rotation"), GetAgent(AgentId)->GetObservations().GlobalActorRotation());
    Map.Add(FName("Relative Joints Location"), GetAgent(AgentId)->GetObservations().RelativeJointsLocation());
    Map.Add(FName("Relative Joints Rotation"), GetAgent(AgentId)->GetObservations().RelativeJointsRotation());
    Map.Add(FName("Relative Joints Angular Velocity"), GetAgent(AgentId)->GetObservations().RelativeJointsAngularVelocity());
    Map.Add(FName("Relative Joints Linear Velocity"), GetAgent(AgentId)->GetObservations().RelativeJointsLinearVelocity());
    Map.Add(FName("Joints Height"), GetAgent(AgentId)->GetObservations().JointsHeight());
    OutObservationObjectElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationObject, Map);
    Super::GatherAgentObservation_Implementation(OutObservationObjectElement, InObservationObject, AgentId);
}

void UHumanLearningAgentsInteractor::SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement,
    ULearningAgentsActionSchema* InActionSchema)
{
    const FLearningAgentsActionSchemaElement TargetAngleAction = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, FName("TargetAngle"));
    const FLearningAgentsActionSchemaElement TargetAnglesAction = ULearningAgentsActions::SpecifyStaticArrayAction(InActionSchema, TargetAngleAction, 33, FName("TargetAngles"));
    OutActionSchemaElement = TargetAnglesAction;
    Super::SpecifyAgentAction_Implementation(OutActionSchemaElement, InActionSchema);
}

void UHumanLearningAgentsInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject,
    const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId)
{
    TArray<FLearningAgentsActionObjectElement> OutActions;
    ULearningAgentsActions::GetStaticArrayAction(OutActions, InActionObject, InActionObjectElement, FName("TargetAngles"));

    TArray<float> NeuralNetworkResult;
    for (const auto& OutAction : OutActions)
    {
        float ActionValue;
        ULearningAgentsActions::GetFloatAction(ActionValue, InActionObject, OutAction);
        NeuralNetworkResult.Add(ActionValue);
    }

    GetAgent(AgentId)->SetAngularOrientationTargets(NeuralNetworkResult);
    Super::PerformAgentAction_Implementation(InActionObject, InActionObjectElement, AgentId);
}
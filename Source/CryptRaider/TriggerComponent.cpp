// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    AActor* AcceptableActor{GetAcceptableActor()};
    if(AcceptableActor)
    {
        UPrimitiveComponent* Component{Cast<UPrimitiveComponent>(AcceptableActor->GetRootComponent())};
        if(Component)
        {
            Component->SetSimulatePhysics(false);
        }
        Component->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }
}

void UTriggerComponent::SetMover(UMover* NewMover)
{
    Mover = NewMover;
}

AActor *UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> Actors;
    GetOverlappingActors(Actors);
    for(int32 it{0}; it < Actors.Num(); ++it)
    {
        if(Actors[it]->ActorHasTag(AcceptableActorTag) && !Actors[it]->ActorHasTag("Grabbed"))
        {
            return Actors[it];
        }
    }
    return nullptr;
}

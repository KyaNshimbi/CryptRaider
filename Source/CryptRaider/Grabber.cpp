// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UPhysicsHandleComponent* PhysicsHandle{GetPhysicsHandle()};
	if(!PhysicsHandle)
	{
		return;
	}
	
	FVector TargetLocation{GetComponentLocation() + GetForwardVector() * HoldDistance};
	PhysicsHandle->SetTargetLocationAndRotation(TargetLocation, GetComponentRotation());
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Display, TEXT("Released Grabber"));
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* PhysicsHandle{GetPhysicsHandle()};
	if(!PhysicsHandle)
	{
		return;
	}
	FVector Start{GetComponentLocation()};
	FVector End{Start + GetForwardVector() * MaxGrabDistance};
	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	
	FCollisionShape Sphere{FCollisionShape::MakeSphere(GrabRadius)};
	FHitResult HitResult;
	bool HasHit {GetWorld()->SweepSingleByChannel(
		HitResult, 
		Start, End, 
		FQuat::Identity,
		ECC_GameTraceChannel2,
		Sphere
	)};
	if(HasHit)
	{
		PhysicsHandle->GrabComponentAtLocationWithRotation(
			HitResult.GetComponent(),
			NAME_None,
			HitResult.ImpactPoint,
			GetComponentRotation()
		);
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* PhysicsHandle{GetOwner()->FindComponentByClass<UPhysicsHandleComponent>()};
	if(!PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber requires a UPhysicsHandleComponent"));
	}
	return PhysicsHandle;
}
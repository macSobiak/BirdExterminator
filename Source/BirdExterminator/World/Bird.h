// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Bird.generated.h"

class UCollisionPredictor;
class ABirdFlock;
enum class EColliderType : uint8;

enum EBirdState
{
	FreeFlight,
	CollisionDetected,
	RecoveryAfterHit,
	Follow,
	OutOfBounds
};

UCLASS()
class BIRDEXTERMINATOR_API ABird : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABird();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RegisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);
	void UnregisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);

	void Initialize(ABirdFlock *BirdFlock, const int &PlaceInFlockRef, FVector3f &PlayableAreaRef);
	
	UPROPERTY(EditAnywhere)
	float InitialVelocity = 100;
	
private:
	FRotator TurnSpeedRotator = FRotator(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	TArray<EColliderType> ActiveColliders;
	TArray<EColliderType> CollidersToIgnore;

	UPROPERTY()
	ABirdFlock *BirdFlockToFollow;
	
	EBirdState BirdState = FreeFlight;

	UPROPERTY(EditAnywhere)
	float TurnSpeed = 10;

	UPROPERTY(EditAnywhere)
	float HitCooldown = 2;
	float CurrentCooldown = 0;
	
	int PlaceInFlock = 0;
	
	FVector3f PlayableArea;
};

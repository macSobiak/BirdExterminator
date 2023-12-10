// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Bird.generated.h"

class UCollisionPredictor;
class ABirdFlock;
class BirdBehavior;
class ABirdsController;

enum class EColliderType : uint8;


DECLARE_EVENT_OneParam(ABird, FOnBirdDestroyed, ABird*);

UCLASS()
class BIRDEXTERMINATOR_API ABird : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABird();
	virtual ~ABird() override;

protected:
	virtual void Destroyed() override;
private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void RecoverFromHit();
	void AdjustAppearance(UMaterial* MaterialToSet, const ECollisionChannel &ChannelToSet, const ECollisionResponse &ResponseToBirdPrey);
	void SetAppearance(UMaterial* MaterialToSet);
	void SetCollision(const ECollisionChannel &ChannelToSet, const ECollisionResponse &ResponseToBirdPrey);
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RegisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);
	void UnregisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);

	void InitializeCommonObjects(ABirdsController* BirdsControllerInstance);
	void InitializeAsPrey(ABirdFlock* BirdFlock, const int& PlaceInFlockRef, const FVector3f& PlayableAreaRef);
	void TransformToPrey(const FVector3f& PlayableAreaRef);
	void InitializeAsPredator(const FVector3f& PlayableAreaRef);
	
	FOnBirdDestroyed OnBirdDestroyedEvent;
		
	UPROPERTY()
	ABirdsController* BirdsController;
	
private:
	FRotator TurnSpeedRotator = FRotator(0, 0, 0);

	UPROPERTY()
	UStaticMeshComponent* MeshComponent;

	TArray<EColliderType> ActiveColliders;
	TArray<EColliderType> CollidersToIgnore;
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInst;
	
	BirdBehavior* BirdBehaviorDefinition = nullptr;
	
	UPROPERTY(EditAnywhere)
	float HitCooldown = 2;
	float CurrentCooldown = 0;
	
	bool IsOnHitCooldown = false;
	bool IsInitialized = false;
	
public:
	bool IsDestructable = false;
	
};
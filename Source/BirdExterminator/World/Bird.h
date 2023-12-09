// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Bird.generated.h"

class UCollisionPredictor;
class ABirdFlock;
class BirdBehavior;

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
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void RecoverFromHit();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RegisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);
	void UnregisterModifier(const FRotator &VectorOffset, UCollisionPredictor *CollisionPredictor);

	void InitializeBase(UMaterial* MaterialToSet, const ECollisionChannel &ChannelToSet, const ECollisionResponse &ResponseToBirdPrey);
	void InitializeAsPrey(ABirdFlock* BirdFlock, const int& PlaceInFlockRef, const FVector3f& PlayableAreaRef);
	void InitializeAsPredator(const FVector3f& PlayableAreaRef);
	
	void SetAppearance(UMaterial* MaterialToSet);
	void SetCollision(const ECollisionChannel &ChannelToSet, const ECollisionResponse &ResponseToBirdPrey);
	BirdBehavior* BirdBehaviorDefinition = nullptr;

	FOnBirdDestroyed OnBirdDestroyedEvent;
private:
	
	FRotator TurnSpeedRotator = FRotator(0, 0, 0);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComponent;

	TArray<EColliderType> ActiveColliders;
	TArray<EColliderType> CollidersToIgnore;

	UPROPERTY()
	UMaterial* StoredMaterialPrey = nullptr;
	UPROPERTY()
	UMaterial* StoredMaterialPredator = nullptr;
	
	
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterialInst;

	UPROPERTY(EditAnywhere)
	float HitCooldown = 2;
	
	float CurrentCooldown = 0;
	bool IsOnHitCooldown = false;
	bool IsInitialized = false;
public:
	bool IsDestructable = false;
	
};
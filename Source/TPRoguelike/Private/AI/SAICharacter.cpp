// Made by Jan Puto 2022 :D

#include "AI/SAICharacter.h"
#include "Perception/PawnSensingComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "AIController.h"
#include "Components/SAttributeComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UI/SWorldUserWidget.h"
#include "UI/SPlayerSpottedWidget.h"
#include "Components/CapsuleComponent.h"
#include "Components/SActionComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "FunctionLibrary/LogsFunctionLibrary.h"


ASAICharacter::ASAICharacter()
{
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp");

	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>(TEXT("AttributeComponent"));

	ActionComponent = CreateDefaultSubobject<USActionComponent>(TEXT("ActionComponent"));

	HealingParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HealingParticleComponent"));
	HealingParticleComp->SetupAttachment(RootComponent);
	HealingParticleComp->bAutoActivate = false;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);
}

void ASAICharacter::Tick(float DeltaTime)
{
	if (bIsHealingOverTime)
	{
		TimeSinceLastHealing += DeltaTime;

		if (TimeSinceLastHealing >= HealingInterval)
		{
			Heal(HealthPerSecond);
			TimeSinceLastHealing = 0.f;
		}
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);

	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	SetTargetActor(Pawn);
	
	//DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.f, true);
}

bool ASAICharacter::SetTargetActor(AActor* TargetActor)
{
	if (AttributeComponent->IsAlive())
	{
		// If this ai ware hit by another ai it will start shooting this ai (its kinda fun)
		if (TargetActor != this)
		{
			TObjectPtr<AAIController> AIController = Cast<AAIController>(GetController());
			if (AIController)
			{
				TObjectPtr<UBlackboardComponent> BBCComp = AIController->GetBlackboardComponent();

				TObjectPtr<AActor> CurrentTargetActor = Cast<AActor>(BBCComp->GetValueAsObject(BBV_TargetActor));
				if (TargetActor != CurrentTargetActor)
				{
					MulticastSpawnPlayerSpottedWidget();
				}

				BBCComp->SetValueAsObject(BBV_TargetActor, TargetActor);

				return true;
			}
		}
	}

	return false;
}


void ASAICharacter::MulticastSpawnPlayerSpottedWidget_Implementation()
{
	// If AI spots new target actor it will show PlayerSpotted Widget
	if (PlayerSpottedWidgetInstance == nullptr && ensure(PlayerSpottedWidgetClass))
	{
		PlayerSpottedWidgetInstance = CreateWidget<USPlayerSpottedWidget>(GetWorld(), PlayerSpottedWidgetClass);
	}

	if (PlayerSpottedWidgetInstance && !PlayerSpottedWidgetInstance->IsInViewport())
	{
		FString ActionMsg = FString::Printf(TEXT("Adding Widget"));
		ULogsFunctionLibrary::LogOnScreen_IsClientServer(GetWorld(), ActionMsg, FColor::Red, 2.0f);

		PlayerSpottedWidgetInstance->AttachedActor = this;
		PlayerSpottedWidgetInstance->AddToViewport();
		PlayerSpottedWidgetInstance->PlayIconShakeAnimation();

		if (!GetWorldTimerManager().IsTimerActive(PlayerSpottedTimer))
		{
			FTimerDelegate RemovePlayerSpottedDelegate;
			RemovePlayerSpottedDelegate.BindUFunction(this, "RemovePlayerSpottedWidget");
			GetWorldTimerManager().SetTimer(PlayerSpottedTimer, RemovePlayerSpottedDelegate, PlayerSpottedWidgetLifeTime, false);
		}
	}
}

void ASAICharacter::RemovePlayerSpottedWidget()
{
	FString ActionMsg = FString::Printf(TEXT("Removing Widget"));
	ULogsFunctionLibrary::LogOnScreen_IsClientServer(GetWorld(), ActionMsg, FColor::Red, 2.0f);
	PlayerSpottedWidgetInstance->RemoveFromViewport();
}

////////////////////////////////////////////////////
/// Health + Death
void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float DeltaHealth)
{
	if (HasAuthority())
	{
		SetTargetActor(InstigatorActor);
	}

	if (DeltaHealth < 0.0f)
	{
		GetMesh()->SetScalarParameterValueOnMaterials(TimeToHitParamName, GetWorld()->GetTimeSeconds());

		if (HealthBarWidgetInstance == nullptr && ensure(HealthBarWidgetClass))
		{
			HealthBarWidgetInstance = CreateWidget<USWorldUserWidget>(GetWorld(), HealthBarWidgetClass);
			if (HealthBarWidgetInstance)
			{
				HealthBarWidgetInstance->AttachedActor = this;
				HealthBarWidgetInstance->AddToViewport();
			}
		}
	}

	if (DeltaHealth < 0 && NewHealth <= 0.f)
	{
		OnDeath();
	}
}

void ASAICharacter::OnDeath()
{
	if (HasAuthority())
	{
		// Stoping Logic
		TObjectPtr<AAIController> AIController = Cast<AAIController>(GetController());
		if (ensure(AIController))
		{
			AIController->GetBrainComponent()->StopLogic("Killed");
		}
	}

	// Set Ragdoll
	GetMesh()->SetAllBodiesSimulatePhysics(true);
	GetMesh()->SetCollisionProfileName("Ragdoll");

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCharacterMovement()->DisableMovement();

	if (bIsHealingOverTime)
	{
		StopHealingOverTime();
	}

	HealthBarWidgetInstance->RemoveFromViewport();
	PlayerSpottedWidgetInstance->RemoveFromViewport();
	RemovePlayerSpottedWidget();

	// Destroy Actor in 10s
	SetLifeSpan(10.f);
}

void ASAICharacter::Heal(float HealingAmount)
{
	// For now waiting is implemented in Behaviur Tree
	if (HealingAmount > 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Healed for %f HP"), *this->GetName(), HealingAmount);
		AttributeComponent->ApplyHealthChange(this, HealingAmount);
	}
}

void ASAICharacter::StartHealingOverTime(float _HealthPerSecond, float _SecondsOfHealing)
{
	UE_LOG(LogTemp, Warning, TEXT("%s Started Healing"), *this->GetName());

	SecondsOfHealing = _SecondsOfHealing;
	HealthPerSecond = _HealthPerSecond;
	bIsHealingOverTime = true;

	HealingParticleComp->Activate(true);

	// Heal a little at a start
	Heal(HealthPerSecond);
}

void ASAICharacter::StopHealingOverTime()
{
	UE_LOG(LogTemp, Warning, TEXT("%s Stopped Healing"), *this->GetName());

	SecondsOfHealing = 0;
	HealthPerSecond = 0;
	bIsHealingOverTime = false;

	HealingParticleComp->Deactivate();
}

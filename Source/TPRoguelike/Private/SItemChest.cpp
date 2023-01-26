// Made by Jan Puto 2022 :D


#include "SItemChest.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Net/UnrealNetwork.h"

ASItemChest::ASItemChest()
{
	PrimaryActorTick.bCanEverTick = true;

	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseMesh"));
	RootComponent = BaseMesh;
	BaseMesh->SetSimulatePhysics(true);

	ParticleComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	ParticleComp->SetupAttachment(RootComponent);
	ParticleComp->SetAutoActivate(false);
	
	LidMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LidMesh"));
	LidMesh->SetupAttachment(BaseMesh);

	TargetLidPitch = 110.f;

	//SetReplicates(true);
	bReplicates = true;
	SetReplicateMovement(true);
	BaseMesh->SetIsReplicated(true);
}


void ASItemChest::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASItemChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASItemChest::Interact_Implementation(APawn* InstigatorPawn)
{
	// Opening Lid animation in Blueprint
	//ParticleComp->Activate(true);

	bIsOpened = !bIsOpened;

	// It will be called automaticly on clients but if we want to call it on the server we need to do this manualy here
	OnRep_bIsOpened();
}


void ASItemChest::OnRep_bIsOpened()
{
	ParticleComp->Activate(true);
	OpenCloseLid(bIsOpened);
}


bool ASItemChest::CanInteract_Implementation(APawn* InstigatorPawn)
{
	return true;
}


bool ASItemChest::IsEnabled_Implementation()
{
	return true;
}


void ASItemChest::OnActorLoaded_Implementation()
{
	//OnRep_bIsOpened();

	float LidRotation = bIsOpened ? OpenLidRotation : 0.0f;

	LidMesh->SetRelativeRotation(FRotator(LidRotation, 0.f, 0.f));
}


void ASItemChest::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASItemChest, bIsOpened);
}
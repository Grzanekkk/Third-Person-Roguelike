// Made by Jan Puto 2022 :D


#include "Characters/STargetDummy.h"
#include "Components/SAttributeComponent.h"

// Sets default values
ASTargetDummy::ASTargetDummy()
{
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");
	RootComponent = MeshComponent;

	AttributeComp = CreateDefaultSubobject<USAttributeComponent>("AttribComponent");
	AttributeComp->OnHealthChanged.AddDynamic(this, &ASTargetDummy::OnHealthChanged);
}

void ASTargetDummy::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth,
	float DeltaHealth)
{
	if(DeltaHealth < 0)
	{
		MeshComponent->SetScalarParameterValueOnMaterials(FName("TimeToHit"), GetWorld()->GetTimeSeconds());
	}
}
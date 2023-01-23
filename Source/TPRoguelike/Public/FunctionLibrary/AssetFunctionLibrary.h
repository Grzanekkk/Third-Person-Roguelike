// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AssetFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TPROGUELIKE_API UAssetFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

	template<class T>
	static UClass* LoadClassSynchronousIfNeeded(TSoftClassPtr<T> ClassToCheck)
	{
		UClass* ClassVerified = nullptr;

		if (ClassToCheck.IsValid())
		{
			ClassVerified = ClassToCheck.Get();
		}
		else
		{
			ClassVerified = ClassToCheck.LoadSynchronous();
		}

		return ClassVerified;
	}
};

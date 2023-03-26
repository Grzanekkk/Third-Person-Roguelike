// Made by Jan Puto 2023 :D

#pragma once

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "GameplayTagContainer.h"
#include "RogueInputComponent.generated.h"

class URogueInputConfig;
/**
 * 
 */
UCLASS()
class TPROGUELIKE_API URogueInputComponent : public UEnhancedInputComponent
{
	GENERATED_BODY()
	
public:
	void SetMappingContext(UInputMappingContext* MappingContext, int32 Priority);

	template<class UserClass, typename FuncType>
	void BindNativeAction(const URogueInputConfig* InputConfig, const FGameplayTag& InputTag, ETriggerEvent TriggerEvent, UserClass* Object, FuncType Func, bool bLogIfNotFound);

	template<class UserClass, typename PressedFuncType, typename ReleasedFuncType>
	void BindAbilityActions(const URogueInputConfig* InputConfig, UserClass* Object, PressedFuncType PressedFunc, ReleasedFuncType ReleasedFunc, TArray<uint32>& BindHandles);


};

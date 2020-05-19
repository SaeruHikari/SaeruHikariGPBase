#pragma once
#include "Components/InputComponent.h"
#include "NestedNamespace.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FNameSpaceTreeNode
{
	GENERATED_BODY()

		UPROPERTY(BlueprintReadWrite, EditAnywhere)
		FName name;

	inline bool operator==(const FNameSpaceTreeNode& res) const
	{
		return this->name == res.name;
	}
	inline bool operator==(const FName res) const
	{
		return this->name == res;
	}

	TArray<FNameSpaceTreeNode> sons;
	bool bReached = false;
};

UCLASS(BlueprintType, Blueprintable)
class UNamespaceTree : public UObject
{
	GENERATED_BODY()

		UFUNCTION(BlueprintCallable)
		void Initialize(FName spliter);

	UFUNCTION(BlueprintCallable)
		void MergeSequence(FString sequence);

	UFUNCTION(BlueprintPure)
		const TArray<FNameSpaceTreeNode> GetRoots();

	UFUNCTION(BlueprintPure)
		bool Has(FString sequence);



protected:
	UPROPERTY()
		FName spliter = FName(".");

	TArray<FNameSpaceTreeNode> roots;
};

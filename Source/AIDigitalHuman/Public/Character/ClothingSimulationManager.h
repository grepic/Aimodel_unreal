// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Core/AIDigitalHumanTypes.h"
#include "ClothingSimulationManager.generated.h"

/**
 * Clothing item types
 */
UENUM(BlueprintType)
enum class EClothingType : uint8
{
    Top             UMETA(DisplayName = "Top/Shirt"),
    Bottom          UMETA(DisplayName = "Bottom/Pants/Skirt"),
    Dress           UMETA(DisplayName = "Full Dress"),
    Underwear_Top   UMETA(DisplayName = "Bra/Lingerie Top"),
    Underwear_Bottom UMETA(DisplayName = "Panties/Lingerie Bottom"),
    Sleepwear       UMETA(DisplayName = "Sleepwear/Robe"),
    Swimwear        UMETA(DisplayName = "Swimwear/Bikini"),
    Accessories     UMETA(DisplayName = "Accessories"),
    Footwear        UMETA(DisplayName = "Footwear")
};

/**
 * Clothing state
 */
UENUM(BlueprintType)
enum class EClothingState : uint8
{
    On              UMETA(DisplayName = "Fully On"),
    Adjusted        UMETA(DisplayName = "Adjusted/Shifted"),
    PartiallyOff    UMETA(DisplayName = "Partially Removed"),
    Off             UMETA(DisplayName = "Fully Removed")
};

/**
 * Single clothing item configuration
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FClothingItem
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    FString ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    EClothingType Type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    TSoftObjectPtr<USkeletalMesh> Mesh;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    TSoftObjectPtr<UMaterialInterface> Material;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    EClothingState CurrentState = EClothingState::On;

    /** Animation for putting on */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    TSoftObjectPtr<UAnimMontage> PutOnAnimation;

    /** Animation for removing */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    TSoftObjectPtr<UAnimMontage> RemoveAnimation;

    /** Cloth physics asset */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    TSoftObjectPtr<UClothingAssetBase> ClothPhysicsAsset;

    /** Can be removed by user request */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    bool bCanBeRemoved = true;

    /** Required tier to request removal */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    EUserTier RequiredTierForRemoval = EUserTier::Premium;

    /** Order in outfit (lower = removed first) */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Clothing")
    int32 LayerOrder = 0;
};

/**
 * Complete outfit preset
 */
USTRUCT(BlueprintType)
struct AIDIGITALHUMAN_API FOutfitPreset
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
    FString OutfitName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
    TArray<FClothingItem> Items;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Outfit")
    EUserTier RequiredTierToUnlock = EUserTier::Free;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClothingChanged, const FClothingItem&, Item, EClothingState, NewState);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOutfitChanged, const FOutfitPreset&, NewOutfit);

/**
 * Manages clothing, cloth physics simulation, and outfit changes
 */
UCLASS(BlueprintType, Blueprintable)
class AIDIGITALHUMAN_API UClothingSimulationManager : public UObject
{
    GENERATED_BODY()

public:
    UClothingSimulationManager();

    /** Initialize clothing system */
    UFUNCTION(BlueprintCallable, Category = "Clothing")
    void Initialize(class AAIDigitalHumanCharacter* OwnerCharacter);

    // ==================== Outfit Management ====================

    /** Apply complete outfit */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Outfit")
    void ApplyOutfit(const FOutfitPreset& Outfit);

    /** Get current outfit */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Outfit")
    FOutfitPreset GetCurrentOutfit() const { return CurrentOutfit; }

    /** Register outfit preset */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Outfit")
    void RegisterOutfit(const FOutfitPreset& Outfit);

    /** Get all registered outfits */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Outfit")
    TArray<FOutfitPreset> GetAvailableOutfits(EUserTier UserTier) const;

    // ==================== Individual Items ====================

    /** Add clothing item */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    void AddClothingItem(const FClothingItem& Item);

    /** Remove clothing item by name */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    void RemoveClothingItem(const FString& ItemName, bool bPlayAnimation = true);

    /** Adjust clothing item state */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    void SetClothingState(const FString& ItemName, EClothingState NewState);

    /** Get all current clothing items */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    TArray<FClothingItem> GetCurrentClothing() const { return CurrentClothing; }

    /** Check if specific item is worn */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    bool IsWearing(const FString& ItemName) const;

    /** Check if specific type is worn */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Items")
    bool IsWearingType(EClothingType Type) const;

    // ==================== Cloth Physics ====================

    /** Enable/disable cloth simulation */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Physics")
    void SetClothSimulationEnabled(bool bEnabled);

    /** Set wind effect on clothing */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Physics")
    void SetWindEffect(FVector WindDirection, float WindStrength);

    /** Set gravity scale for cloth */
    UFUNCTION(BlueprintCallable, Category = "Clothing|Physics")
    void SetClothGravityScale(float Scale);

    // Events
    UPROPERTY(BlueprintAssignable, Category = "Clothing|Events")
    FOnClothingChanged OnClothingChanged;

    UPROPERTY(BlueprintAssignable, Category = "Clothing|Events")
    FOnOutfitChanged OnOutfitChanged;

protected:
    /** Spawn clothing mesh component */
    void SpawnClothingMesh(const FClothingItem& Item);

    /** Remove clothing mesh component */
    void DestroyClothingMesh(const FString& ItemName);

    /** Update cloth physics settings */
    void UpdateClothPhysics();

private:
    UPROPERTY()
    class AAIDigitalHumanCharacter* OwningCharacter;

    UPROPERTY()
    TArray<FClothingItem> CurrentClothing;

    UPROPERTY()
    FOutfitPreset CurrentOutfit;

    UPROPERTY()
    TArray<FOutfitPreset> RegisteredOutfits;

    UPROPERTY()
    TMap<FString, USkeletalMeshComponent*> ClothingMeshComponents;

    bool bClothSimulationEnabled = true;
    FVector CurrentWindDirection;
    float CurrentWindStrength = 0.0f;
    float ClothGravityScale = 1.0f;
};

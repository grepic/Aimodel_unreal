// Copyright 2024 AI Digital Human Project. All Rights Reserved.

#include "Character/ClothingSimulationManager.h"
#include "Character/AIDigitalHumanCharacter.h"
#include "AIDigitalHumanModule.h"
#include "Components/SkeletalMeshComponent.h"
#include "ClothingSimulation/ClothingSimulationInteractor.h"
#include "Engine/World.h"

UClothingSimulationManager::UClothingSimulationManager()
{
}

void UClothingSimulationManager::Initialize(AAIDigitalHumanCharacter* OwnerCharacter)
{
    OwningCharacter = OwnerCharacter;

    if (!OwningCharacter)
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("ClothingSimulationManager: No owner character provided"));
        return;
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Clothing Simulation Manager initialized"));
}

void UClothingSimulationManager::ApplyOutfit(const FOutfitPreset& Outfit)
{
    // Remove all current clothing
    TArray<FString> ItemsToRemove;
    for (const FClothingItem& Item : CurrentClothing)
    {
        ItemsToRemove.Add(Item.ItemName);
    }
    for (const FString& ItemName : ItemsToRemove)
    {
        RemoveClothingItem(ItemName, false);
    }

    // Apply new outfit items
    for (const FClothingItem& Item : Outfit.Items)
    {
        AddClothingItem(Item);
    }

    CurrentOutfit = Outfit;
    OnOutfitChanged.Broadcast(CurrentOutfit);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Applied outfit: %s"), *Outfit.OutfitName);
}

void UClothingSimulationManager::RegisterOutfit(const FOutfitPreset& Outfit)
{
    // Check if already registered
    for (const FOutfitPreset& Existing : RegisteredOutfits)
    {
        if (Existing.OutfitName == Outfit.OutfitName)
        {
            UE_LOG(LogAIDigitalHuman, Warning, TEXT("Outfit already registered: %s"), *Outfit.OutfitName);
            return;
        }
    }

    RegisteredOutfits.Add(Outfit);
    UE_LOG(LogAIDigitalHuman, Log, TEXT("Registered outfit: %s"), *Outfit.OutfitName);
}

TArray<FOutfitPreset> UClothingSimulationManager::GetAvailableOutfits(EUserTier UserTier) const
{
    TArray<FOutfitPreset> Available;

    for (const FOutfitPreset& Outfit : RegisteredOutfits)
    {
        if (static_cast<uint8>(UserTier) >= static_cast<uint8>(Outfit.RequiredTierToUnlock))
        {
            Available.Add(Outfit);
        }
    }

    return Available;
}

void UClothingSimulationManager::AddClothingItem(const FClothingItem& Item)
{
    // Check if already wearing
    if (IsWearing(Item.ItemName))
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Already wearing: %s"), *Item.ItemName);
        return;
    }

    // Add to current clothing
    FClothingItem NewItem = Item;
    NewItem.CurrentState = EClothingState::On;
    CurrentClothing.Add(NewItem);

    // Spawn mesh
    SpawnClothingMesh(NewItem);

    OnClothingChanged.Broadcast(NewItem, EClothingState::On);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Added clothing: %s"), *Item.ItemName);
}

void UClothingSimulationManager::RemoveClothingItem(const FString& ItemName, bool bPlayAnimation)
{
    int32 ItemIndex = INDEX_NONE;
    FClothingItem RemovedItem;

    // Find item
    for (int32 i = 0; i < CurrentClothing.Num(); i++)
    {
        if (CurrentClothing[i].ItemName == ItemName)
        {
            ItemIndex = i;
            RemovedItem = CurrentClothing[i];
            break;
        }
    }

    if (ItemIndex == INDEX_NONE)
    {
        UE_LOG(LogAIDigitalHuman, Warning, TEXT("Clothing item not found: %s"), *ItemName);
        return;
    }

    // Play remove animation if requested
    if (bPlayAnimation && RemovedItem.RemoveAnimation.IsValid() && OwningCharacter)
    {
        if (UAnimMontage* Montage = RemovedItem.RemoveAnimation.LoadSynchronous())
        {
            OwningCharacter->PlayAnimMontage(Montage);
        }
    }

    // Update state
    RemovedItem.CurrentState = EClothingState::Off;

    // Remove mesh
    DestroyClothingMesh(ItemName);

    // Remove from list
    CurrentClothing.RemoveAt(ItemIndex);

    OnClothingChanged.Broadcast(RemovedItem, EClothingState::Off);

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Removed clothing: %s"), *ItemName);
}

void UClothingSimulationManager::SetClothingState(const FString& ItemName, EClothingState NewState)
{
    for (FClothingItem& Item : CurrentClothing)
    {
        if (Item.ItemName == ItemName)
        {
            EClothingState OldState = Item.CurrentState;
            Item.CurrentState = NewState;

            // Handle state change
            if (NewState == EClothingState::Off && OldState != EClothingState::Off)
            {
                RemoveClothingItem(ItemName, true);
            }
            else
            {
                // Partial states - adjust mesh visibility/position
                if (USkeletalMeshComponent** MeshComp = ClothingMeshComponents.Find(ItemName))
                {
                    switch (NewState)
                    {
                    case EClothingState::Adjusted:
                        // Apply morph target or bone adjustment for "shifted" look
                        break;
                    case EClothingState::PartiallyOff:
                        // Apply partial removal morph
                        break;
                    default:
                        break;
                    }
                }

                OnClothingChanged.Broadcast(Item, NewState);
            }

            return;
        }
    }
}

bool UClothingSimulationManager::IsWearing(const FString& ItemName) const
{
    for (const FClothingItem& Item : CurrentClothing)
    {
        if (Item.ItemName == ItemName && Item.CurrentState != EClothingState::Off)
        {
            return true;
        }
    }
    return false;
}

bool UClothingSimulationManager::IsWearingType(EClothingType Type) const
{
    for (const FClothingItem& Item : CurrentClothing)
    {
        if (Item.Type == Type && Item.CurrentState != EClothingState::Off)
        {
            return true;
        }
    }
    return false;
}

void UClothingSimulationManager::SetClothSimulationEnabled(bool bEnabled)
{
    bClothSimulationEnabled = bEnabled;

    for (auto& Pair : ClothingMeshComponents)
    {
        if (Pair.Value)
        {
            Pair.Value->SetEnableGravity(bEnabled);
        }
    }

    UE_LOG(LogAIDigitalHuman, Log, TEXT("Cloth simulation %s"), bEnabled ? TEXT("enabled") : TEXT("disabled"));
}

void UClothingSimulationManager::SetWindEffect(FVector WindDirection, float WindStrength)
{
    CurrentWindDirection = WindDirection.GetSafeNormal();
    CurrentWindStrength = WindStrength;

    UpdateClothPhysics();
}

void UClothingSimulationManager::SetClothGravityScale(float Scale)
{
    ClothGravityScale = FMath::Clamp(Scale, 0.0f, 2.0f);
    UpdateClothPhysics();
}

void UClothingSimulationManager::SpawnClothingMesh(const FClothingItem& Item)
{
    if (!OwningCharacter || !Item.Mesh.IsValid())
    {
        return;
    }

    // Load mesh
    USkeletalMesh* Mesh = Item.Mesh.LoadSynchronous();
    if (!Mesh)
    {
        UE_LOG(LogAIDigitalHuman, Error, TEXT("Failed to load clothing mesh for: %s"), *Item.ItemName);
        return;
    }

    // Create component
    USkeletalMeshComponent* NewComp = NewObject<USkeletalMeshComponent>(OwningCharacter);
    NewComp->SetSkeletalMesh(Mesh);
    NewComp->AttachToComponent(OwningCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
    NewComp->SetLeaderPoseComponent(OwningCharacter->GetMesh());
    NewComp->RegisterComponent();

    // Apply material
    if (Item.Material.IsValid())
    {
        NewComp->SetMaterial(0, Item.Material.LoadSynchronous());
    }

    // Enable cloth physics if available
    if (Item.ClothPhysicsAsset.IsValid())
    {
        // Cloth simulation would be set up here
    }

    ClothingMeshComponents.Add(Item.ItemName, NewComp);
}

void UClothingSimulationManager::DestroyClothingMesh(const FString& ItemName)
{
    if (USkeletalMeshComponent** MeshComp = ClothingMeshComponents.Find(ItemName))
    {
        if (*MeshComp)
        {
            (*MeshComp)->DestroyComponent();
        }
        ClothingMeshComponents.Remove(ItemName);
    }
}

void UClothingSimulationManager::UpdateClothPhysics()
{
    for (auto& Pair : ClothingMeshComponents)
    {
        if (Pair.Value)
        {
            // Apply wind and gravity settings
            // This would use the cloth simulation interactor
        }
    }
}

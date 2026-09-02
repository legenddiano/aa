#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TacticalStrikeCharacter.generated.h"

UCLASS()
class TACTICALSTRIKE_API ATacticalStrikeCharacter : public ACharacter
{
    GENERATED_BODY()
public:
    ATacticalStrikeCharacter();
    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") int32 MagazineSize = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") int32 AmmoInMagazine = 30;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") int32 ReserveAmmo = 90;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") float Damage = 34.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon") float FireRate = 0.105f;

protected:
    void MoveForward(float Value);
    void MoveRight(float Value);
    void Fire();
    void Reload();
    void StartSprint();
    void StopSprint();
    void StartCrouch();
    void StopCrouch();

private:
    float NextFireTime = 0.0f;
    bool bSprinting = false;
};

#include "TacticalStrikeCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

ATacticalStrikeCharacter::ATacticalStrikeCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCharacterMovement()->MaxWalkSpeed = 310.0f;
    GetCharacterMovement()->MaxWalkSpeedCrouched = 150.0f;
}

void ATacticalStrikeCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
}

void ATacticalStrikeCharacter::SetupPlayerInputComponent(UInputComponent* Input)
{
    Super::SetupPlayerInputComponent(Input);
    Input->BindAxis("MoveForward", this, &ATacticalStrikeCharacter::MoveForward);
    Input->BindAxis("MoveRight", this, &ATacticalStrikeCharacter::MoveRight);
    Input->BindAxis("Turn", this, &APawn::AddControllerYawInput);
    Input->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
    Input->BindAction("Fire", IE_Pressed, this, &ATacticalStrikeCharacter::Fire);
    Input->BindAction("Reload", IE_Pressed, this, &ATacticalStrikeCharacter::Reload);
    Input->BindAction("Sprint", IE_Pressed, this, &ATacticalStrikeCharacter::StartSprint);
    Input->BindAction("Sprint", IE_Released, this, &ATacticalStrikeCharacter::StopSprint);
    Input->BindAction("Crouch", IE_Pressed, this, &ATacticalStrikeCharacter::StartCrouch);
    Input->BindAction("Crouch", IE_Released, this, &ATacticalStrikeCharacter::StopCrouch);
}

void ATacticalStrikeCharacter::MoveForward(float V) { AddMovementInput(GetActorForwardVector(), V); }
void ATacticalStrikeCharacter::MoveRight(float V) { AddMovementInput(GetActorRightVector(), V); }

void ATacticalStrikeCharacter::StartSprint() { bSprinting = true; GetCharacterMovement()->MaxWalkSpeed = 470.0f; }
void ATacticalStrikeCharacter::StopSprint() { bSprinting = false; GetCharacterMovement()->MaxWalkSpeed = 310.0f; }
void ATacticalStrikeCharacter::StartCrouch() { Crouch(); }
void ATacticalStrikeCharacter::StopCrouch() { UnCrouch(); }

void ATacticalStrikeCharacter::Reload()
{
    const int32 Needed = MagazineSize - AmmoInMagazine;
    const int32 Loaded = FMath::Min(Needed, ReserveAmmo);
    AmmoInMagazine += Loaded;
    ReserveAmmo -= Loaded;
}

void ATacticalStrikeCharacter::Fire()
{
    if (bSprinting || AmmoInMagazine <= 0 || GetWorld()->GetTimeSeconds() < NextFireTime) return;
    NextFireTime = GetWorld()->GetTimeSeconds() + FireRate;
    --AmmoInMagazine;

    FVector Start;
    FRotator Rot;
    GetController()->GetPlayerViewPoint(Start, Rot);
    const FVector End = Start + Rot.Vector() * 10000.0f;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(RifleTrace), true, this);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params) && Hit.GetActor())
    {
        UGameplayStatics::ApplyPointDamage(Hit.GetActor(), Damage, Rot.Vector(), Hit, GetController(), this, nullptr);
    }
    DrawDebugLine(GetWorld(), Start, Hit.bBlockingHit ? Hit.ImpactPoint : End, FColor::White, false, 0.08f, 0, 0.7f);
}

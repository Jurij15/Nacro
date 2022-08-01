#pragma once

#include "Globals.hpp"
#define NPOS std::string::npos

namespace Player
{
	//We call this in frontend to get the characterparts currently on our player. This sets the variables in Globals, so we can apply them ingame.
	inline void GrabCharacterParts()
	{
		for (int i = 0; i < UObject::GetGlobalObjects().Num(); ++i)
		{
			auto Objects = UObject::GetGlobalObjects().GetByIndex(i);

			if (Objects != nullptr)
			{
				if (Objects->GetFullName().find("CustomCharacterPart") != NPOS && Objects->GetFullName().find("ATH") != NPOS)
				{
					if (Objects->GetFullName().find("Head") != NPOS)
						Globals::charPartHead = static_cast<UCustomCharacterPart*>(Objects);
					else
						Globals::charPartBody = static_cast<UCustomCharacterPart*>(Objects);
				}

				if (Globals::charPartHead && Globals::charPartBody)
					break;
			}
		}
	}

	//Spawn a new Athena pawn and set our pawn to it
	inline void SpawnPlayer()
	{
		Globals::AthenaController->CheatManager->Summon(L"PlayerPawn_Athena_C");
		SDK::TArray<AActor*> outActors;
		Globals::GameplayStatics->STATIC_GetAllActorsOfClass(Globals::GEngine->GameViewport->World, SDK::APlayerPawn_Athena_C::StaticClass(), &outActors);
		Globals::AthenaPawn = static_cast<AFortPlayerPawnAthena*>(outActors[0]);
	}

	//Set our player's CharacterParts
	inline void ChooseParts(UCustomCharacterPart* InHead, UCustomCharacterPart* InBody)
	{
		Globals::AthenaPawn->ServerChoosePart(EFortCustomPartType::Head, InHead);
		Globals::AthenaPawn->ServerChoosePart(EFortCustomPartType::Body, InBody);
	}

	//Show CharacterParts on our player
	inline void ShowParts()
	{
		Globals::AthenaPlayerState->OnRep_CharacterParts();
	}

	//Sets team index, allowing for markers and health in top left
	inline void SetTeamIndex(EFortTeam Team)
	{
		Globals::AthenaPlayerState->TeamIndex = Team;
		Globals::AthenaPlayerState->OnRep_TeamIndex();
	}

	//Equip given item definition
	inline void Equip(UFortWeaponItemDefinition* Item, FGuid Guid)
	{
		Globals::AthenaPawn->EquipWeaponDefinition(Item, Guid);

		if (Globals::bInstantReload)
		{
			Globals::WeaponReloadMontage = Globals::AthenaPawn->CurrentWeapon->WeaponReloadMontage;
			Globals::ReloadAnimation = Globals::AthenaPawn->CurrentWeapon->ReloadAnimation;

			Globals::AthenaPawn->CurrentWeapon->WeaponReloadMontage = nullptr;
			Globals::AthenaPawn->CurrentWeapon->ReloadAnimation = nullptr;
		}
	}

	DWORD UpdatePawn(LPVOID lpParam)
	{
		while (true)
		{
			//quite possibly the most dumbass shit ive ever had to put in an if
			/*if (GetAsyncKeyState(VK_SHIFT) & 0x8000 && GetAsyncKeyState(0x57) & 0x8000 && !Globals::AthenaController->IsInAircraft() && !Globals::AthenaPawn->IsSkydiving() && Globals::AthenaPawn->CurrentWeapon && !Globals::AthenaPawn->CurrentWeapon->bIsReloadingWeapon && !Globals::AthenaPawn->CurrentWeapon->bIsTargeting)
				Globals::AthenaPawn->CurrentMovementStyle = EFortMovementStyle::Sprinting;
			else
				Globals::AthenaPawn->CurrentMovementStyle = EFortMovementStyle::Running;

			if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !Globals::AthenaController->IsInAircraft() && !Globals::AthenaPawn->IsSkydiving())
			{
				//We use this variable to avoid jump/glider spam.
				if (!Globals::bHasJumped)
				{
					Globals::bHasJumped = true;
					if (Globals::AthenaPawn->bIsCrouched)
					{
						Globals::AthenaPawn->UnCrouch(true);
					}
					else
					{
						if (!Globals::bInfiniteJump)
						{
							if (!Globals::AthenaPawn->IsJumpProvidingForce())
							{
								Globals::AthenaPawn->Jump();
							}
						}
						else
						{
							Globals::AthenaPawn->Jump();
						}
					}
				}
			}
			else if (GetAsyncKeyState(VK_SPACE) & 0x8000 && !Globals::AthenaController->IsInAircraft() && Globals::AthenaPawn->IsSkydiving() && !Globals::AthenaPawn->IsParachuteForcedOpen())
			{
				if (!Globals::bHasJumped)
				{
					Globals::bHasJumped = true;
					if (!Globals::AthenaPawn->IsParachuteOpen())
						Globals::AthenaPawn->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, 2U); //gliding
					else
						Globals::AthenaPawn->CharacterMovement->SetMovementMode(EMovementMode::MOVE_Custom, 3U); //skydiving
				}
			}
			else
				if (Globals::bHasJumped)
					Globals::bHasJumped = false;

			//When aiming, the player should use a different movementstyle. Without this, it plays regular running animations, which just looks wrong.
			if (Globals::AthenaPawn->CurrentWeapon && Globals::AthenaPawn->CurrentWeapon->bIsTargeting)
				Globals::AthenaPawn->CurrentMovementStyle = EFortMovementStyle::Walking;*/

			if (Globals::bIsInLobby)
				break;

			Sleep(1000 / 30);
		}

		ExitThread(0);
	}
};
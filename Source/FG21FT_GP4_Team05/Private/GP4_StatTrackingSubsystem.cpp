#include "GP4_StatTrackingSubsystem.h"

void UGP4_StatTrackingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGP4_StatTrackingSubsystem::SetUpPlayersData(const int Id)
{
	if (PlayerDataList[Id].bIsAssigned)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3,
				FColor::Yellow,
				FString::Printf(TEXT("This player was already added")));
		}
		return;
	}
	PlayerDataList[Id].bIsAssigned = true;
	PlayerDataList[Id].PlayerID = Id;
	PlayerDataList[Id].Name.Append(" ");
	PlayerDataList[Id].Name.Append(FString::FromInt(PlayerDataList[Id].PlayerID + 1));
	ConnectedPlayersAmount++;
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			5,
			FColor::Yellow,
			FString::Printf(TEXT("Connected players: %d"),ConnectedPlayersAmount));
	}
}

void UGP4_StatTrackingSubsystem::SetPlayerChoice(const int PlayerId, const int CharacterChoice)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	PlayerDataList[PlayerId].CharacterChoice = CharacterChoice;
}

FPlayerData& UGP4_StatTrackingSubsystem::GetPlayerData(const int PlayerId)
{
	if (PlayerId < 0 || PlayerId >= Max_Player_Amount )
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3,
				FColor::Yellow,
				FString::Printf(TEXT("Index out of bounds.")));
		}
	}
	return PlayerDataList[PlayerId];
}

void UGP4_StatTrackingSubsystem::UpdatePlayerRoundScore(const int PlayerId,int Score)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	if (PlayerDataList[PlayerId].bIsAssigned)
	{
		PlayerDataList[PlayerId].CurrentRoundScore += Score;
	}
}

void UGP4_StatTrackingSubsystem::AddAllPlayersRoundScoreToRound()
{
	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		PlayerDataList[i].RoundsScore.Add(PlayerDataList[i].CurrentRoundScore);
	}
}

void UGP4_StatTrackingSubsystem::ResetAllPlayersCurrentRoundScore()
{
	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		PlayerDataList[i].CurrentRoundScore = 0;
	}
}

void UGP4_StatTrackingSubsystem::UpdatePlayersTotalScore()
{
	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		PlayerDataList[i].PreviousTotalScore = PlayerDataList[i].TotalScore;
		int TotalScore = 0;
		for (int m = 0; m < PlayerDataList[i].RoundsScore.Num(); ++m)
		{
			TotalScore += PlayerDataList[i].RoundsScore[m];
		}
		PlayerDataList[i].TotalScore = TotalScore;
		
		if (GEngine)
        	{
        		GEngine->AddOnScreenDebugMessage(
        			-1,
        			10,
        			FColor::Yellow,
        			FString::Printf(TEXT("total score: %d"),PlayerDataList[i].TotalScore));
        	}
	}
}

void UGP4_StatTrackingSubsystem::UpdateScoreInOtherPlayersGoal(const int PlayerId, const int GoalId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	if (PlayerDataList[GoalId].bIsAssigned)
	{
		PlayerDataList[PlayerId].ScoredOnOtherGoal += 1;
	}
}

void UGP4_StatTrackingSubsystem::UpdateTimesBumpedOtherPlayers(const int PlayerId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	PlayerDataList[PlayerId].TimesBumpedOtherPlayers += 1;
}

void UGP4_StatTrackingSubsystem::UpdateTimesCordBroke(const int PlayerId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	PlayerDataList[PlayerId].TimesCordBroke += 1;
}

int UGP4_StatTrackingSubsystem::GetWinningPlayerId()
{
	int WinnerId = 0;

	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		if (PlayerDataList[i].TotalScore > PlayerDataList[WinnerId].TotalScore)
		{
			WinnerId = PlayerDataList[i].PlayerID;
		}
	}
	return WinnerId;
}

void UGP4_StatTrackingSubsystem::UpdatePatientsKilled(const int PlayerId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	PlayerDataList[PlayerId].PatientsKilled += 1;
}

void UGP4_StatTrackingSubsystem::UpdateHealthyPatientsRescued(const int PlayerId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	PlayerDataList[PlayerId].HealthyPatientsRescued += 1;
}

void UGP4_StatTrackingSubsystem::ResetStatTracking()
{
	for (int i = 0; i < Max_Player_Amount; ++i)
	{
		PlayerDataList[i].bIsAssigned = false;
		PlayerDataList[i].Name = "Player";
		PlayerDataList[i].PlayerID = -1;
		PlayerDataList[i].CharacterChoice = 0;
		PlayerDataList[i].TotalScore = 0;
		PlayerDataList[i].CurrentRoundScore = 0;
		PlayerDataList[i].RoundsScore.Empty();
		PlayerDataList[i].ScoredOnOtherGoal = 0;
		PlayerDataList[i].TimesBumpedOtherPlayers = 0;
		PlayerDataList[i].TimesCordBroke = 0;
		PlayerDataList[i].PatientsKilled = 0;
		PlayerDataList[i].HealthyPatientsRescued = 0;
		PlayerDataList[i].PreviousTotalScore = 0;
		PlayerDataList[i].PatientsStolen = 0;
		PlayerDataList[i].TimeHoldingPatients = 0;
		PlayerDataList[i].BonusMvps.Empty();
		PlayerDataList[i].DeductionMvps.Empty();
	}
	ConnectedPlayersAmount = 0;
}

int UGP4_StatTrackingSubsystem::GetPlayerWithHighestRoundScore(const int Round)
{
	int Id = 0;
	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		if (PlayerDataList[i].RoundsScore[Round] > PlayerDataList[Id].RoundsScore[Round])
		{
			Id = PlayerDataList[i].PlayerID;
		}
	}
	return Id;
}

void UGP4_StatTrackingSubsystem::UpdatePatientsStolen(const int PlayerId)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	
	if (PlayerDataList[PlayerId].bIsAssigned)
	{
		PlayerDataList[PlayerId].PatientsStolen++;
	}
}

void UGP4_StatTrackingSubsystem::UpdateTimeHoldingPatient(const int PlayerId, int Time)
{
	if (CheckIfIndexIsOutOfBounds(PlayerId))
	{
		return;
	}
	
	if (PlayerDataList[PlayerId].bIsAssigned)
	{
		PlayerDataList[PlayerId].TimeHoldingPatients += Time;
	}
}

void UGP4_StatTrackingSubsystem::SetMvps()
{
	int Altruist = 0;
	int Saboteur = 0;
	int Reckless = 0;
	int Saint = 0;
	int Thief = 0;
	int AttachmentIssues = 0;
	
	for (int i = 0; i < ConnectedPlayersAmount; ++i)
	{
		if (PlayerDataList[i].ScoredOnOtherGoal > PlayerDataList[Altruist].ScoredOnOtherGoal )
		{
			Altruist = PlayerDataList[i].PlayerID;
		}

		if (PlayerDataList[i].TimesBumpedOtherPlayers > PlayerDataList[Saboteur].TimesBumpedOtherPlayers )
		{
			Saboteur = PlayerDataList[i].PlayerID;
		}

		if (PlayerDataList[i].PatientsKilled > PlayerDataList[Reckless].PatientsKilled )
		{
			Reckless = PlayerDataList[i].PlayerID;
		}

		if (PlayerDataList[i].HealthyPatientsRescued > PlayerDataList[Saint].HealthyPatientsRescued )
		{
			Saint = PlayerDataList[i].PlayerID;
		}

		if (PlayerDataList[i].PatientsStolen > PlayerDataList[Thief].PatientsStolen )
		{
			Thief = PlayerDataList[i].PlayerID;
		}

		if (PlayerDataList[i].TimeHoldingPatients > PlayerDataList[AttachmentIssues].TimeHoldingPatients )
		{
			AttachmentIssues = PlayerDataList[i].PlayerID;
		}
	}

	PlayerDataList[Altruist].DeductionMvps.Add(EMVPS::EMVPS_Altruist);
	PlayerDataList[Saboteur].BonusMvps.Add(EMVPS::EMVPS_Saboteur);
	PlayerDataList[Reckless].DeductionMvps.Add(EMVPS::EMVPS_Reckless);
	PlayerDataList[Saint].BonusMvps.Add(EMVPS::EMVPS_Saint);
	PlayerDataList[Thief].BonusMvps.Add(EMVPS::EMVPS_Thief);
	PlayerDataList[AttachmentIssues].BonusMvps.Add(EMVPS::EMVPS_AttachmentIssues);
	//
	// MvpLookUp.Add(EMVPS::EMVPS_Altruist, Altruist);
	// MvpLookUp.Add(EMVPS::EMVPS_Saboteur, Saboteur);
	// MvpLookUp.Add(EMVPS::EMVPS_Reckless, Reckless);
	// MvpLookUp.Add(EMVPS::EMVPS_Saint, Saint);
	// MvpLookUp.Add(EMVPS::EMVPS_Thief, Thief);
	// MvpLookUp.Add(EMVPS::EMVPS_AttachmentIssues, AttachmentIssues);

	
}

int UGP4_StatTrackingSubsystem::GetMvpStat(const int PlayerId, EMVPS Mvp)
{
	switch (Mvp)
	{
	case EMVPS::EMVPS_Altruist:
		return PlayerDataList[PlayerId].ScoredOnOtherGoal;

	case EMVPS::EMVPS_Saboteur:
		return PlayerDataList[PlayerId].TimesBumpedOtherPlayers;

	case EMVPS::EMVPS_Reckless:
		return PlayerDataList[PlayerId].PatientsKilled;

	case EMVPS::EMVPS_Saint:
		return PlayerDataList[PlayerId].HealthyPatientsRescued;

	case EMVPS::EMVPS_Thief:
		return PlayerDataList[PlayerId].PatientsStolen;

	case EMVPS::EMVPS_AttachmentIssues:
		return PlayerDataList[PlayerId].TimeHoldingPatients;
		
	default:
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3,
				FColor::Yellow,
				FString::Printf(TEXT("No case for enum in stat tracking subsystem.")));
		};
		return 0;
	}
}

bool UGP4_StatTrackingSubsystem::CheckIfIndexIsOutOfBounds(const int Index)
{
	if (Index < 0 || Index >= Max_Player_Amount )
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				3,
				FColor::Yellow,
				FString::Printf(TEXT("Index out of bounds.")));
		}
		return true;
	}
	return false;
}

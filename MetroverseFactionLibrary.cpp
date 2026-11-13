#include "MetroverseFactionLibrary.h"
#include "MetroversePlayerState.h"

FString UMetroverseFactionLibrary::GetFactionDisplayName(EMetroverseFaction Faction)
{
    switch (Faction)
    {
        case EMetroverseFaction::Government: return TEXT("Government");
        case EMetroverseFaction::Police: return TEXT("Metro Police Department");
        case EMetroverseFaction::Hospital: return TEXT("Metro Medical Services");
        case EMetroverseFaction::NewsNetwork: return TEXT("Metro News Network");
        case EMetroverseFaction::FIB: return TEXT("Federal Investigation Bureau");
        case EMetroverseFaction::Administration: return TEXT("Server Administration");
        default: return TEXT("Unaffiliated");
    }
}

FString UMetroverseFactionLibrary::GetFactionDescription(EMetroverseFaction Faction)
{
    switch (Faction)
    {
        case EMetroverseFaction::Government: return TEXT("Public services, city policy, civic events and government administration.");
        case EMetroverseFaction::Police: return TEXT("Public safety, patrol roleplay, traffic assistance and lawful investigations.");
        case EMetroverseFaction::Hospital: return TEXT("Emergency response, patient care, medical roleplay and health education.");
        case EMetroverseFaction::NewsNetwork: return TEXT("News reporting, interviews, public announcements and city media coverage.");
        case EMetroverseFaction::FIB: return TEXT("Federal-level investigations, evidence handling and inter-agency roleplay.");
        case EMetroverseFaction::Administration: return TEXT("Server operations, player support, moderation and community safety.");
        default: return TEXT("Explore the city and build your roleplay career before joining an organisation.");
    }
}

FString UMetroverseFactionLibrary::GetFactionHeadquarters(EMetroverseFaction Faction)
{
    switch (Faction)
    {
        case EMetroverseFaction::Government: return TEXT("Civic Centre, Metro City");
        case EMetroverseFaction::Police: return TEXT("Central Police Station, Metro City");
        case EMetroverseFaction::Hospital: return TEXT("Metro General Hospital, Metro City");
        case EMetroverseFaction::NewsNetwork: return TEXT("Broadcast House, Metro City");
        case EMetroverseFaction::FIB: return TEXT("Federal Bureau Building, Metro City");
        case EMetroverseFaction::Administration: return TEXT("Administration Centre, Metro City");
        default: return TEXT("No headquarters assigned");
    }
}

int32 UMetroverseFactionLibrary::GetFactionMaximumRank(EMetroverseFaction Faction)
{
    return Faction == EMetroverseFaction::None ? 0 : 10;
}

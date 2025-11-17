// Copyright University of Inland Norway

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include <vector>
#include "SimulationController.generated.h"


// Struct for the Unreal DataTable
USTRUCT(BlueprintType)
struct FPopulationDensityEffect : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PopulationDensity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NormalPopulationDensity;
};



UCLASS()
class ZOMBIEAPOCALYPSE_API ASimulationController : public AActor
{
	GENERATED_BODY()
	
public:	
	ASimulationController();

	// Runs one simulation step each Tick 
	virtual void Tick(float DeltaTime) override;

	// Function to read data from Unreal DataTable into the graphPts vector
	void ReadDataFromTableToVectors();

	// Holds the amount of people having this remaining days left to become a Zombie
	struct ConveyorBatch
	{
		float amountOfPeople;      // people currently in conveyor (bitten)
		float remainingDays;       // days left before conversion
	};

	// ----- TIME SPECS -----
	const int STARTTIME {0};        // Simulation StartTime
	const int STOPTIME  {120};      // By this time all simulations should be finished
	const float DT      {1.0f};      // DeltaTime for the simulation, 1 == 1 day steps

	// ----- CONSTANTS / INITIALIZATION -----
	const float days_to_become_infected_from_bite {15.f};   // Days
	const float Bitten_capacity {1000.f};                   // Capacity inside conveyor == people at start
	const float patient_zero {1.0f};                        // Zombies
	const float CONVERSION_FROM_PEOPLE_TO_ZOMBIES {1.f};    // Conversion constant, always 1
	const float normal_number_of_bites {1.f};               // People/zombie/day
	const float land_area {1000.f};                         // In m2
	const float normal_population_density {0.1f};           // people/m2
	
	void write_row(int t, std::ofstream &csv);
	float graph_lookup(float xIn);
	float conveyor_content();


	// Unreal Lookup table for population density effect
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Variables")
	class UDataTable* PopulationDensityEffectTable{ nullptr };

	// How long each time step is in Unreal, in seconds
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Variables")
	float SimulationStepTime{ 1.0f };	

	// Turn on/off debug printing to the Output Log
	UPROPERTY(EditAnywhere, Category = "Simulation Variables")
	bool bShouldDebug{ false };


	// Stocks (initial)

	//Susceptible (People) - choose a number that has a clean sqrt!
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Simulation Variables")
	int32 Susceptible{ 100 };
	// Zombies = patient_zero
	int32 Zombies{ 1 };		
	// Just to check if we are correctly updating stocks - used in SimulationHUD
	int32 Bitten{ 0 };          // s Bitten -> in BittenArraySize

	// Conveyor content is the Bitten people incubating
	// Will have one ConveyorBatch of each remainingDays
	// So max days_to_become_infected_from_bite different ConveyorBatch'es
	std::vector<ConveyorBatch> conveyor;            // accepts multiple batches
	

	// Time accumulator for simulation steps, used in Tick function
	float AccumulatedTime{ 0.f };

	// Number of time steps completed - to keep track and compare to Stella
	int TimeStepsFinished{ 0 };



protected:
	virtual void BeginPlay() override;

};

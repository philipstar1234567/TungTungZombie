// Copyright University of Inland Norway

#include "SimulationController.h"
#include <iostream>
#include  <cmath>
#include <algorithm>
#include <fstream>
#include <vector>
#include <iomanip>

ASimulationController::ASimulationController()
{
    PrimaryActorTick.bCanEverTick = true;
}

struct ConveyorBatch
{
    float amountOfPeople;      // people currently in conveyor (bitten)
    float remainingDays;       // days left before conversion
};


// GRAPH points: population_density_effect_on_zombie_bites
// Values read in from Unreal DataTable for more flexibility
std::vector<std::pair<float, float>> graphPts
    = {
    {0.000f, 0.014f}, {0.200f, 0.041f}, {0.400f, 0.101f}, {0.600f, 0.189f}, {0.800f, 0.433f},
    {1.000f, 1.000f}, {1.200f, 1.217f}, {1.400f, 1.282f}, {1.600f, 1.300f}, {1.800f, 1.300f},
    {2.000f, 1.300f}
    };


std::vector<ConveyorBatch> conveyor;   

float ASimulationController::graph_lookup(float xIn)
{
    // Piecewise linear interpolation with endpoint clamping

    // Special cases, xIn outside table range:
    if (xIn <= graphPts.front().first)
        return graphPts.front().second;
    if (xIn >= graphPts.back().first)
        return graphPts.back().second;

    for (size_t i = 1; i < graphPts.size(); ++i)
    {
        if (xIn <= graphPts[i].first)
        {
            // Linear interpolation between points in graph:
            float x0 = graphPts[i-1].first, x1 = graphPts[i].first;     //the x value below and above xIn
            float y0 = graphPts[i-1].second, y1 = graphPts[i].second;   //the y value below and above xIn
            float t = (xIn - x0) / (x1 - x0);   //distance between x0 and x1
            return y0 + t*(y1 - y0);            //return the interpolated y value
        }
    }
    return graphPts.back().second; // fallback
}

// Sums up all bitten
float ASimulationController::conveyor_content()
{
    float sum = 0.0;
    for (ConveyorBatch &batch : conveyor)
        sum += batch.amountOfPeople;
    return sum;
}

void ASimulationController::BeginPlay()
{
    Super::BeginPlay();

    // Checking if the DataTable is assigned
    if (!PopulationDensityEffectTable)
    {
          UE_LOG(LogTemp, Error, TEXT("PopulationDensityEffectTable is not assigned!"));
    }
    else
    {
       // Table found, read data into vector
       ReadDataFromTableToVectors();
    }
}



void ASimulationController::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    AccumulatedTime += DeltaTime;

    // If Unreal timestep is reached
    if (AccumulatedTime >= SimulationStepTime)
    {
        
        
        AccumulatedTime = 0.f; // Reset accumulator
        UE_LOG(LogTemp, Log, TEXT("SimulationStep"));
<<<<<<< Updated upstream
=======

        //std::cout << "\n***** Day: " << t << " *****\n";
        // --- Calculate auxiliaries (using values at current time t) ---
        Bitten = conveyor_content(); // current conveyor content (people)
        float non_zombie_population = Bitten + Susceptible; // People
        if (non_zombie_population != 0.f)
        {
            TimeStepsFinished++;
        }
        
        float population_density = non_zombie_population / land_area; // people/m2

        float x = population_density / normal_population_density; // dimensionless
        // std::cout << "population_density / normal_population_density: " << x << "\n";

        float population_density_effect_on_zombie_bites = graph_lookup(x);
        // std::cout << "population_density_effect_on_zombie_bites: " << population_density_effect_on_zombie_bites << "\n";

        float number_of_bites_per_zombie_per_day =
            normal_number_of_bites * population_density_effect_on_zombie_bites; // People/zombie/day

        // ROUND as in Stella (nearest int, half away from zero) -> std::round
        float total_bitten_per_day =
            std::round(Zombies * number_of_bites_per_zombie_per_day); // People/day

        // Avoid division by zero by MAX(non_zombie_population, 1)
        float denom = std::max(non_zombie_population, 1.f); //GET KRIS TO LOOK AT THIS
        float number_of_bites_from_total_zombies_on_susceptible =
            std::round((Susceptible / denom) * total_bitten_per_day); // People/day

        // This is the actual number of bites all zombies should do this time step
        // So THIS is the number to use inside of Unreal
        std::cout << "Number of Susceptible to bite this timestep: "
                  << number_of_bites_from_total_zombies_on_susceptible << "\n";

        // getting_bitten UNIFLOW, but enforce NON-NEGATIVE Susceptible
        float getting_bitten = number_of_bites_from_total_zombies_on_susceptible;
        // Truncate to not exceed available Susceptible this step (Euler non-negative safeguard)
        getting_bitten = std::min(getting_bitten, floor(Susceptible)); // ensure we don't drive below 0 //ASK GROUP ABOUT MIN, AI GOOF?

        // --- Conveyor mechanics for Bitten ---
        // 1) Progress existing batches and compute raw outflow (people exiting conveyor)

        for (ConveyorBatch &b : conveyor)
        {
            b.remainingDays -= DT;  // update remainingDays
            // std::cout << "ConveyorBatch amount of people:" << b.amountOfPeople << " remaining days: "
                      // << b.remainingDays << "\n";
        }
        // collect finished batches -> remainingDays == 0
        std::vector<ConveyorBatch> next_conveyor;
        float raw_outflow_people {0.f};
        next_conveyor.reserve(conveyor.size());
        for (ConveyorBatch &b : conveyor)
        {
            if (b.remainingDays <= 0.0)
                raw_outflow_people += b.amountOfPeople;
            else
                next_conveyor.push_back(b);

        }
        conveyor.swap(next_conveyor);

        // 2) Capacity check for new inflow (people)
        float current_content = conveyor_content();
        float free_cap = std::max(0.f, Bitten_capacity - current_content); //GET HLP HERE
        float inflow_people = std::max(0.f, std::min(getting_bitten, free_cap)); //AND HERE

        if (inflow_people > 0.f)
            conveyor.push_back(ConveyorBatch{inflow_people, days_to_become_infected_from_bite});

        // 3) Conveyor outflow converted to zombies
        // becoming_infected == becoming Zombies this timestep
        float becoming_infected = raw_outflow_people * CONVERSION_FROM_PEOPLE_TO_ZOMBIES; // Zombies/day

        // --- STOCK UPDATES (Euler) ---
        // Susceptible(t+1) = S(t) - getting_bitten
        Susceptible = std::max(0.f, Susceptible - getting_bitten); //WHY MULTIPLY BY DT HERE?

        // Zombies(t+1) = Z(t) + becoming_infecting
        Zombies = std::max(0.f, Zombies + becoming_infected); //WHY MULTIPLY BY DT HERE?

        // Bitten stock is already handled implicitly by conveyor vector
        // (content increased by inflow_people, decreased by raw_outflow_people)
        Bitten = conveyor_content();

        // --- (Optional) derived integer populations, not used further ---
        // float Susceptible_Population = floor(Susceptible);
        // float Zombie_Population = floor(Zombies);+

        std::cout << "Susceptible: " << Susceptible << ", Bitten: " << Bitten << ", Zombies: " << Zombies << "\n";

        // Write row for t+DT
        //write_row(t + (int)DT, csvFile);
>>>>>>> Stashed changes
    }

<<<<<<< Updated upstream
    // ----- SIMULATION LOOP -----
    for (int t = STARTTIME; t < STOPTIME; t += (int)DT)
    {
        std::cout << "\n***** Day: " << t << " *****\n";
        // --- Calculate auxiliaries (using values at current time t) ---
        Bitten = conveyor_content(); // current conveyor content (people)
        float non_zombie_population = Bitten + Susceptible; // People
        float population_density = non_zombie_population / land_area; // people/m2

        float x = population_density / normal_population_density; // dimensionless
        // std::cout << "population_density / normal_population_density: " << x << "\n";

        float population_density_effect_on_zombie_bites = graph_lookup(x);
        // std::cout << "population_density_effect_on_zombie_bites: " << population_density_effect_on_zombie_bites << "\n";

        float number_of_bites_per_zombie_per_day =
            normal_number_of_bites * population_density_effect_on_zombie_bites; // People/zombie/day

        // ROUND as in Stella (nearest int, half away from zero) -> std::round
        float total_bitten_per_day =
            std::round(Zombies * number_of_bites_per_zombie_per_day); // People/day

        // Avoid division by zero by MAX(non_zombie_population, 1)
        float denom = std::max(non_zombie_population, 1.f);
        float number_of_bites_from_total_zombies_on_susceptible =
            std::round((Susceptible / denom) * total_bitten_per_day); // People/day

        // This is the actual number of bites all zombies should do this time step
        // So THIS is the number to use inside of Unreal
        std::cout << "Number of Susceptible to bite this timestep: "
                  << number_of_bites_from_total_zombies_on_susceptible << "\n";

        // getting_bitten UNIFLOW, but enforce NON-NEGATIVE Susceptible
        float getting_bitten = number_of_bites_from_total_zombies_on_susceptible;
        // Truncate to not exceed available Susceptible this step (Euler non-negative safeguard)
        getting_bitten = std::min(getting_bitten, (float)floor(Susceptible)); // ensure we don't drive below 0

        // --- Conveyor mechanics for Bitten ---
        // 1) Progress existing batches and compute raw outflow (people exiting conveyor)
        for (ConveyorBatch &b : conveyor)
        {
            b.remainingDays -= DT;  // update remainingDays
            // std::cout << "ConveyorBatch amount of people:" << b.amountOfPeople << " remaining days: "
                      // << b.remainingDays << "\n";
        }
        // collect finished batches -> remainingDays == 0
        std::vector<ConveyorBatch> next_conveyor;
        float raw_outflow_people {0.f};
        next_conveyor.reserve(conveyor.size());
        for (ConveyorBatch &b : conveyor)
        {
            if (b.remainingDays <= 0.0)
                raw_outflow_people += b.amountOfPeople;
            else
                next_conveyor.push_back(b);

        }
        conveyor.swap(next_conveyor);

        // 2) Capacity check for new inflow (people)
        float current_content = conveyor_content();
        float free_cap = std::max(0.f, Bitten_capacity - current_content);
        float inflow_people = std::max(0.f, std::min(getting_bitten, free_cap));

        if (inflow_people > 0.f)
            conveyor.push_back(ConveyorBatch{inflow_people, days_to_become_infected_from_bite});

        // 3) Conveyor outflow converted to zombies
        // becoming_infected == becoming Zombies this timestep
        float becoming_infected = raw_outflow_people * CONVERSION_FROM_PEOPLE_TO_ZOMBIES; // Zombies/day

        // --- STOCK UPDATES (Euler) ---
        // Susceptible(t+1) = S(t) - getting_bitten
        Susceptible = std::max(0.f, Susceptible - getting_bitten * DT);

        // Zombies(t+1) = Z(t) + becoming_infecting
        Zombies = std::max(0.f, Zombies + becoming_infected * DT);

        // Bitten stock is already handled implicitly by conveyor vector
        // (content increased by inflow_people, decreased by raw_outflow_people)
        Bitten = conveyor_content();

        // --- (Optional) derived integer populations, not used further ---
        // float Susceptible_Population = floor(Susceptible);
        // float Zombie_Population = floor(Zombies);

        std::cout << "Susceptible: " << Susceptible << ", Bitten: " << Bitten << ", Zombies: " << Zombies << "\n";

        // Write row for t+DT
        //write_row(t + (int)DT, csvFile);
    }
}

=======
>>>>>>> Stashed changes
// Function to read data from Unreal DataTable into the graphPts vector
void ASimulationController::ReadDataFromTableToVectors()
{
    if (bShouldDebug)
       UE_LOG(LogTemp, Log, TEXT("Read Data From Table To Vectors"))

    TArray<FName> rowNames = PopulationDensityEffectTable->GetRowNames();

    for (int i{0}; i < rowNames.Num(); i++)
    {
       if (bShouldDebug)
          UE_LOG(LogTemp, Log, TEXT("Reading table row index: %d"), i)

       FPopulationDensityEffect* rowData = PopulationDensityEffectTable->FindRow<FPopulationDensityEffect>(rowNames[i], TEXT(""));
       if (rowData)
       {
           graphPts.emplace_back(rowData->PopulationDensity, rowData->NormalPopulationDensity);

           if (bShouldDebug)
           {
               std::pair<float, float> lastPair = graphPts.back();
               UE_LOG(LogTemp, Warning, TEXT("Reading table row: %d, pair: (%f, %f)"), i, lastPair.first, lastPair.second);
           }
       }
    }
}

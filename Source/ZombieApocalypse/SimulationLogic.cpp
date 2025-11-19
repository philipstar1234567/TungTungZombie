// Copyright University of Inland Norway

#include "SimulationLogic.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>


SimulationLogic::~SimulationLogic()
{
}

SimulationLogic::SimulationLogic()
{
    // Holds the amount of people having this remaining days left to become a Zombie
    struct ConveyorBatch
    {
        float amountOfPeople;      // people currently in conveyor (bitten)
        float remainingDays;       // days left before conversion
    };


    // ***** Global declared variables for simplicity in this test *****

    // ----- TIME SPECS -----
    const int STARTTIME {0};        // Simulation StartTime
    const int STOPTIME  {120};      // By this time all simulations should be finished
    const float DT      {1.f};      // DeltaTime for the simulation, 1 == 1 day steps

    // ----- CONSTANTS / INITIALIZATION -----
    const float days_to_become_infected_from_bite {15.f};   // Days
    const float Bitten_capacity {1000.f};                    // Capacity inside conveyor == people at start
    const float patient_zero {1.0f};                        // Zombies
    const float CONVERSION_FROM_PEOPLE_TO_ZOMBIES {1.f};    // Conversion constant, always 1
    const float normal_number_of_bites {1.f};               // People/zombie/day
    const float land_area {1000.f};                         // In m2
    const float normal_population_density {0.1f};           // people/m2

    // Stocks (initial)
    float Susceptible {100.f};        // People
    float Bitten{0.f};
    float Zombies = patient_zero;     // Zombies == 1.f at start

    // Conveyor content is the Bitten people incubating
    // Will have one ConveyorBatch of each remainingDays
    // So max days_to_become_infected_from_bite different ConveyorBatch'es
    std::vector<ConveyorBatch> conveyor;            // accepts multiple batches

    // GRAPH points: population_density_effect_on_zombie_bites
    // x = population_density / normal_population_density
    const std::vector<std::pair<float,float>> graphPts ={
        {0.000f, 0.014f}, {0.200f, 0.041f}, {0.400f, 0.101f}, {0.600f, 0.189f}, {0.800f, 0.433f},
        {1.000f, 1.000f}, {1.200f, 1.217f}, {1.400f, 1.282f}, {1.600f, 1.300f}, {1.800f, 1.300f},
        {2.000f, 1.300f}
    };

    /*
    // Forward declarations -> functions after int main()
    float graph_lookup(float xIn);
    float conveyor_content();
    void write_row(int t, std::ofstream &csv);
    */
}

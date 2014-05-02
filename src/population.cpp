
#include "population.hpp"

namespace neurocid {

PopulationLayout make_default_population_layout() {
	return {
		20, // size_
		//Ship Layout
		{
		    //Projectile Layout
			{
				1,    // max_speed_
				10000, // max_travel_
				10     // radius_
			},
			false, // isDummy_
			true,// canShoot_
			true,// canRotate_
			true,// canMove_
			false,// disableProjectileFitness_

			50.0,// radius_
			1.0, // max_speed_
			1.0, // max_rotation_
			20000.0, // max_fuel_
			20000.0, // start_fuel_
			1.0, // fuel_rate_
			0.7, // hardness_

			20,// max_ammo_
			0, // start_ammo_
			5, // max_cooldown_
			3, // max_damage_
			1, // crashes_per_damage_
			6,  // num_perf_desc_
			"fitness/amir" // fitnessFunction;
		},
		//BrainLayout
		{
			4,  // metaInputs
			3,  // metaLayers
			4,  // meta neurons per hidden layer

			4,  // numEyes
			20, // numVectorsPerEye_;
			4,  // numEyeLayers_;
			5, // numEyeNeuronsPerHidden_;

			5,  // outputs
			3,  // layers
			5,  // neurons per hidden layer
			4   // numBrains
		},
		//FailityLayout
		{
			300,
			6000,
			500
		}
	};
}

}

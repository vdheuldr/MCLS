#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
/********************************************************************************/
/********************************************************************************/
/*  Function to advance the interface                                           */
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* The update of the level-set field to the new time-level exists of a number   */
/* of steps.                                                                    */
/* 1- First the level-set field is advected                                     */
/* 2- Second the level-set field is re-initialized                              */
/* 3- The level-set field is made mass-conserving by adapting it to the         */
/* convected volume of fluid field. 						*/
/* In the original implementation of Sander, this function included the         */
/* computation of the curvature and surface tension, but this has now been      */
/* moved to a separate part that concerns the coupling between the interface    */
/* dynamics and the Navier-Stokes solution algorithm. 				*/
/********************************************************************************/
      void level_set_update_main_control
      (
      Array3<double> level_set_new, 			// level set field at new time level
						// mass conserving
      Array3<double> level_set_old, 			// level set field at old time level
						// mass conserving
      Array3<double> volume_of_fluid, 		// volume of fluid field
      Array3<double> u_1_velocity_new, 		// velocity field at new time level x1 direction
      Array3<double> u_2_velocity_new, 		// velocity field at new time level x2 direction
      Array3<double> u_3_velocity_new,		// velocity field at new time level x3 direction
      int number_primary_cells_i,		// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,		// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k,		// number of primary (pressure) cells in x3 direction
      double actual_time_step_level_set,	// time step used for level-set advection
						// computed from all stability restrictions and 
						// possibly subscycling
      double mesh_width_x1,			// grid spacing in x1 direction (uniform)
      double mesh_width_x2,			// grid spacing in x2 direction (uniform)
      double mesh_width_x3,			// grid spacing in x3 direction (uniform)
      int apply_mass_distribution_algorithm,    // =1:apply the mass redistribution algorithm
						// to avoid numerical vapour
      int apply_mass_conservation_correction,    // =1:apply the mass conservation correction
						// to the level-set field
      double volume_of_fluid_tolerance,		// tolerance in volume of fluid field
						// for admissable values
      double lower_bound_derivatives,		// lower bound for the first partial derivatives
						// to consider it a limiting case of vanishing
						// partial derivatives
      int number_vof_2_level_set_iterations,	// number of OUTER iterations in the conversion 
						// from volume of fluid to level-set
      int number_iterations_ridder,		// maximum number of iterations allowed in the
						// nonlinear root finding algorithm
						// this is the number of INNER iterations
      double vof_2_level_set_tolerance,		// tolerance in the conversion from volume
						// of fluid value to level-set value
      double cfl_number_reinitialization,	// courant-friedrichs-lewy number for the reinitialization
						// equation
      int maximum_reinitialization_steps,	// maximum number of time steps in the reinitialization
						// algorithm
      double tolerance_reinitialization		// stop the reinitialization when the infinite norm of
						// the pseudo time derivative has fallen below this
						// tolerance value
    
	
      )
      {
	/* function definitions */
      void advect_level_set(				// advect the level-set field
             Array3<double> level_set_old, 		
             Array3<double> level_set_new, 		
             Array3<double> u_1_velocity_new, 	
             Array3<double> u_2_velocity_new, 	
             Array3<double> u_3_velocity_new,	
             int number_primary_cells_i,	
             int number_primary_cells_j,	
             int number_primary_cells_k,	
             double actual_time_step_level_set,
             double mesh_width_x1,		
             double mesh_width_x2,		
             double mesh_width_x3		
      );
      void reinitialize_level_set(		// reinitialize the level-set field
              Array3<double> level_set_star, 		
              int number_primary_cells_i,		
              int number_primary_cells_j,		
              int number_primary_cells_k,		
              double mesh_width_x1,			
              double mesh_width_x2,			
              double mesh_width_x3,			
              double cfl_number_reinitialization,	
              int maximum_reinitialization_steps,	
              double tolerance_reinitialization		
      );
      void make_level_set_mass_conserving		// make level-set field mass-conserving
      (
            Array3<double> level_set_star, 		
            Array3<double> level_set_new, 			
            Array3<double> level_set_old, 			
            Array3<double> volume_of_fluid, 		
            Array3<double> u_1_velocity_new, 		
            Array3<double> u_2_velocity_new, 		
            Array3<double> u_3_velocity_new,		
            int number_primary_cells_i,		
            int number_primary_cells_j,		
            int number_primary_cells_k,		
            double actual_time_step_level_set,	
            double mesh_width_x1,			
            double mesh_width_x2,			
            double mesh_width_x3,			
            int apply_mass_distribution_algorithm,    
            int apply_mass_conservation_correction,   
            double volume_of_fluid_tolerance,		
            double lower_bound_derivatives,		
            int number_vof_2_level_set_iterations,	
            int number_iterations_ridder,		
            double vof_2_level_set_tolerance		
      );
       Array3<double> level_set_star; 		// level set field at new time level
						// after convection and reinitialization
						// not mass conserving
      
      /* allocate memory for the level-set field */
      
      
      /* advect the level-set field */
      
      advect_level_set( level_set_old, level_set_star, 
			 u_1_velocity_new, u_2_velocity_new, u_3_velocity_new, 
			  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k, 
			    actual_time_step_level_set, mesh_width_x1, mesh_width_x2, mesh_width_x3);
    
      
      /* apply re-initialization to the level-set field */
      
      reinitialize_level_set(level_set_star, number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
				    mesh_width_x1, mesh_width_x2, mesh_width_x3,	
				      cfl_number_reinitialization, maximum_reinitialization_steps, 
					  tolerance_reinitialization);
             
      
      /* apply correction to level-set field to make it mass-conserving */
      
       
      
      make_level_set_mass_conserving( level_set_star, level_set_new, level_set_old, 
					volume_of_fluid, 		
					  u_1_velocity_new, u_2_velocity_new, u_3_velocity_new,		
					    number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,		
					      actual_time_step_level_set,	
						mesh_width_x1, mesh_width_x2, mesh_width_x3,			
						  apply_mass_distribution_algorithm, 
						    apply_mass_conservation_correction,   
						      volume_of_fluid_tolerance,
							lower_bound_derivatives,		
							  number_vof_2_level_set_iterations,	 
							    number_iterations_ridder,		
							      vof_2_level_set_tolerance);

      
      }
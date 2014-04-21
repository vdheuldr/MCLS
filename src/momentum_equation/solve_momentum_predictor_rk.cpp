#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
      
/********************************************************************************/
/* Function for momentum prediction in all directions				*/
/*  										*/
/*  Programmer	: Coen Hennipman       						*/
/*  Date	: 06-03-2014    						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/********************************************************************************/

 EXPORT void solve_momentum_predictor_rk(
      Array3<double> u_1_velocity_star, 		// velocity field at star time level x1 direction
      Array3<double> u_2_velocity_star, 		// velocity field at star time level x2 direction
      Array3<double> u_3_velocity_star,			// velocity field at star time level x3 direction
      Array3<double> u_1_velocity_old, 			// velocity field at old time level x1 direction
      Array3<double> u_2_velocity_old, 			// velocity field at old time level x2 direction
      Array3<double> u_3_velocity_old,			// velocity field at old time level x3 direction
      Array3<double> scaled_density_u1,			// scaled density for the controlvolumes
      Array3<double> scaled_density_u2,			// scaled density for the controlvolumes
      Array3<double> scaled_density_u3,			// scaled density for the controlvolumes      
      
      Array3<double> momentum_source_term_u_1,		// source term of the momentum equation in x1 direction
					       		// defined on all u1 points (including boundaries)
      Array3<double> momentum_source_term_u_2,		// source term of the momentum equation in x2 direction
					        	// defined on all u1 points (including boundaries)
      Array3<double> momentum_source_term_u_3,		// source term of the momentum equation in x3 direction
					        	// defined on all u1 points (including boundaries)
      Array3<double> level_set, 			// level-set field
      double actual_time_step_navier_stokes,	        // time step used for level-set advection
							// computed from all stability restrictions and 
							// possibly subscycling
      int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
      double mesh_width_x1,				// grid spacing in x1 direction (uniform)
      double mesh_width_x2,				// grid spacing in x2 direction (uniform)
      double mesh_width_x3,				// grid spacing in x3 direction (uniform)
      double smoothing_distance_factor,			// the smoothing distance is smoothing_distance_factor
      double rho_plus_over_rho_minus,
      double rho_minus_over_mu_minus,			// this was the 'Reynolds' number
							// in the original implementation of Sander
      double mu_plus_over_mu_minus,			// ratio of the viscosities of both phases
      int source_terms_in_momentum_predictor    	// =1, the source terms are applied in the momentum predictor
					        	// equation
					        	// =0, the source terms are applied in the momentum corrector
					        	// equation  
       )

  {







// u* = add_3_arrays 



// convection diffusion u(n-1)

// u* = add_3_arrays

// convection diffusion u*

// u** = add_4_arrays

// convection diffusion u**

// u*** = add_4_arrays 



  }      










      	
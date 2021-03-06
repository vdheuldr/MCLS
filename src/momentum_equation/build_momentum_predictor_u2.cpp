#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>

/********************************************************************************/
/*  Function to build the whole system of linear equations for the velocity     */
/*  u2                 								*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* In the current implementation the generation of the matrix, right-hand-side  */
/* and the application of the boundary conditions are completely separated.     */
/* In this function all the required tasks are collected:                       */
/* - Build matrix without boundary conditions                                   */
/* - Build rhside without boundary conditions                                   */
/* - Apply boundary conditions to the right hand side, using the matrixi        */
/* - Apply boundary conditions to the matrix (matrix folding)                   */ 
/********************************************************************************/
EXPORT void build_momentum_predictor_u2(
      Array2<double> momentum_matrix_u2,			// momentum matrix velocity x2 direction
      Array1<double> momentum_rhside_u2,			// momentum rhside velocity x2 direction
      Array3<double> level_set, 				// level-set field
      Array3<double> momentum_source_term_u_2, 		// complete source term for the momentum equation
							// in x2 direction=(-p,2+ g_2 +F2)
      Array3<double> scaled_density_u2,                 // scaled density for the controlvolumes
                                                   // of the momentum equation in x2 direction
      Array3<double> u_1_velocity_old, 			// velocity field at old time level x1 direction
      Array3<double> u_2_velocity_old, 			// velocity field at old time level x2 direction
      Array3<double> u_3_velocity_old,			// velocity field at old time level x3 direction
      int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
      double actual_time_step_navier_stokes,	// time step used for level-set advection
							// computed from all stability restrictions and 
							// possibly subscycling
      double mesh_width_x1,				// grid spacing in x1 direction (uniform)
      double mesh_width_x2,				// grid spacing in x2 direction (uniform)
      double mesh_width_x3,				// grid spacing in x3 direction (uniform)
      double rho_plus_over_rho_minus,		// ratio of the densities of the two phases
      double smoothing_distance_factor,		// the smoothing distance is smoothing_distance_factor
							// times the smallest mesh width
      double rho_minus_over_mu_minus,		// this was the 'Reynolds' number
							// in the original implementation of Sander
      double mu_plus_over_mu_minus,			// ratio of the viscosities of both phases
      boundary_face boundary_faces[6],		// array with all the information
							// for the boundary conditions 
      int number_matrix_connections			// number of connections in momentum matrix				       
	 )


      {
    /* build momentum equation matrix for velocity u1 */
    /* without considering the boundary conditions */

      build_momentum_matrix_u2( momentum_matrix_u2, level_set, scaled_density_u2,
				 number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
				  actual_time_step_navier_stokes,		
				    mesh_width_x1, mesh_width_x2, mesh_width_x3,				
				      rho_plus_over_rho_minus, rho_minus_over_mu_minus, mu_plus_over_mu_minus,			
					smoothing_distance_factor);

    /* build momentum equation rhside for velocity u1 */
    /* without considering the boundary conditions */

      build_momentum_rhs_u2( momentum_rhside_u2, level_set, scaled_density_u2,				
			      momentum_source_term_u_2, 		 
				u_1_velocity_old, u_2_velocity_old, u_3_velocity_old,			 
				  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
				    actual_time_step_navier_stokes,		 
				      mesh_width_x1, mesh_width_x2,mesh_width_x3,				 
					rho_plus_over_rho_minus,			 
					  smoothing_distance_factor,			 
					    rho_minus_over_mu_minus,			 
					      mu_plus_over_mu_minus);

    /* apply boundary conditions to the momentum equation rhside for velocity u1  */
    /* this has to be done first, because in that case the matrix can be utilised */

      fold_momentum_rhside_u2(boundary_faces,	
				momentum_rhside_u2, momentum_matrix_u2,			
				  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			 
				    mesh_width_x1, mesh_width_x2, mesh_width_x3);

    /* apply boundary conditions to the momentum equation matrix for velocity u1 */
 

      fold_momentum_matrix_u2(boundary_faces,			
				momentum_matrix_u2,		
				  number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
				    number_matrix_connections);




      }




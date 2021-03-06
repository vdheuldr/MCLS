#include "../headers/array.h"
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>

/********************************************************************************/
/*  Function to initialize the interface                                        */
/*  											*/
/*  Programmer	: Duncan van der Heul       						*/
/*  Date	: 10-03-2013       							*/
/*  Update	:        								*/
/********************************************************************************/
/* Notes										*/
/* A number of cases are predefined for the exact computation of the level-set  */
/* field and the corresponding volume of fluid field                            */
/* The cases include:                                                           */
/* - an ellipsoid, defined by its center and the three principle axes           */
/* - a symmetric capillary wave with elevation in the x3 direction              */
/********************************************************************************/
EXPORT void initialize_interface(
	  geometry flow_type,				// the kind of initial condition that has to be applied
	  bubble *the_bubbles,			// array with definitions of the bubbles
	  int number_of_bubbles,			// number of bubbles in the initial condition
	  surface *the_free_surfaces,			// array with definitions of the free surfaces
	  int number_of_free_surfaces, 		// number of bubbles in the domain (<10)
	  int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	  int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	  int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
	  double mesh_width_x1,			// grid spacing in x1 direction (uniform)
	  double mesh_width_x2,			// grid spacing in x2 direction (uniform)
	  double mesh_width_x3,			// grid spacing in x3 direction (uniform)
	  double lower_bound_derivatives,		// lower bound for the first partial derivatives
							// to consider it a limiting case of vanishing
							// partial derivatives
	  Array3<double> level_set,			// level-set field
	  Array3<double> volume_of_fluid,			// volume of fluid field
	  Array3<double> curvature,			// interface curvature
	  Array3<double> unsmoothed_curvature,		// interface curvature without smoothing
      	  int apply_curvature_smoothing,		// =1, apply curvature smoothing
							// =0, use unsmoothed curvature
         int number_curvature_smoothing_steps,	// number of iterations applied in the
							// curvature smoothing algorithm
         int apply_curvature_smoothing_filter, 	// =1, apply curvature smoothing filter
	    						// =0, do not apply curvature smoothing filter		
	  double smoothing_distance_factor,		// the heaviside function is smoothed over
							// an interval of width 2*smoothing_distance
	  int debugging_mode				// =1, program is run in 
      							// debugging mode with additional output and checks
      							// =0, progam is run in production mode
						
	  )
 {
     /* intialize the level-set field */
     
       initialize_level_set( flow_type, the_bubbles, number_of_bubbles, 
			      the_free_surfaces, number_of_free_surfaces, 		
				number_primary_cells_i,	number_primary_cells_j, number_primary_cells_k,	
				  mesh_width_x1, mesh_width_x2,	mesh_width_x3, level_set);			
      
	
     /* intialize the volume of fluid field */
     
       initialize_volume_of_fluid( level_set, volume_of_fluid, 
				      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,
					lower_bound_derivatives);
      
  	
 }
    
     
     
     
     
     
     
     
     
     
     

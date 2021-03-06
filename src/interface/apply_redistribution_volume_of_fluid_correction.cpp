#include "../headers/array.h"
#include<algorithm>
#include<iostream>
/********************************************************************************/
/*  Function to redistribute the volume of fluid correction.			*/
/*  										       */
/*  Programmer	: Duncan van der Heul       					       */
/*  Date	: 10-03-2013       						       */
/*  Update	:        							       */
/********************************************************************************/
/* Notes									       */
/* When cells contain numerical vapour (non interface cells with intermediate   */
/* volume of fluid values) or the volume of fluid is outside the allowed        */
/* interval, the correct value is set and the error is moved to cells that      */
/* allow their volume of fluid value to be raised or lowered.                   */
/* The current function convects the error towards the interface, because the   */
/* interface cells are the only ones that can be modified without bringing the  */
/* volume of fluid value outside the allowed interval [0.1].                    */
/********************************************************************************/

EXPORT void apply_redistribution_volume_of_fluid_correction(
         Array3<double> level_set,                                  // level-set field
         Array3<double> volume_of_fluid,                            // volume of fluid field
         Array3<double> volume_of_fluid_correction,                 // correction to volume of fluid field
         double mesh_width_x1,                                 // grid spacing in x1 direction (uniform)
         double mesh_width_x2,                                 // grid spacing in x2 direction (uniform)
         double mesh_width_x3,                                 // grid spacing in x3 direction (uniform)
         int number_primary_cells_i,                           // number of primary (pressure) cells in x1 direction
         int number_primary_cells_j,                           // number of primary (pressure) cells in x2 direction
         int number_primary_cells_k,                           // number of primary (pressure) cells in x3 direction
         int maximum_number_mass_redistribution_iterations,    // number of iterations allowed to make
                                                               // the volume of fluid field valid
                                                               // these are the sweeps on the vof error
         double vof_2_level_set_tolerance,                     // tolerance in the conversion from volume
                                                               // of fluid value to level-set value
         double volume_of_fluid_tolerance,                     // tolerance in volume of fluid field
                                                               // for admissable values
         double time_step_mass_redistribution                  // time step for the mass redistribution
                                                               // algorithm
         )
 {
      Array3<double> time_derivative_volume_of_fluid_correction; // time-derivative of the volume of fluid
							    // correction
      Array3<double> velocity_mass_redistribution_x1;	    // convection velocity in mass
							    // redistribution algorithm x1 
							    // component
      Array3<double> velocity_mass_redistribution_x2;	    // convection velocity in mass
							    // redistribution algorithm x1 
							    // component
      Array3<double> velocity_mass_redistribution_x3;	    // convection velocity in mass
							    // redistribution algorithm x1 
							    // component
      double max_time_derivative_vof_correction;
      double cfl_number_mass_redistribution=0.1;
      double one_over_dx1	=    			// 1/(grid spacing in x1 direction)
		    1.0/(mesh_width_x1);
      double one_over_dx2	=    			// 1/(grid spacing in x2 direction)
		    1.0/(mesh_width_x2);
      double one_over_dx3	=    			// 1/(grid spacing in x3 direction)
		    1.0/(mesh_width_x3);
      double flux_mass_redistribution_x1_left_;	// left  hand flux mass redistribution, x1 direction
      double flux_mass_redistribution_x1_right;	// right hand flux mass redistribution, x1 direction
      double flux_mass_redistribution_x2_left_;	// left  hand flux mass redistribution, x2 direction
      double flux_mass_redistribution_x2_right;	// right hand flux mass redistribution, x2 direction
      double flux_mass_redistribution_x3_left_;	// left  hand flux mass redistribution, x3 direction
      double flux_mass_redistribution_x3_right;	// right hand flux mass redistribution, x3 direction
      double current_level_set_value;
      double current_volume_of_fluid_value;
      double mass_redistribution_tolerance=	// when the infinite norm of time-derivative of the volume of fluid
	  0.01*tolerance_level_set_conversion;  // correction falls below this tolerance the mass
						// redistribution algorithm is terminated.
     
      int number_cells_vof_out_of_bounds;	// number of control volumes where the volume of fluid
						// function is OUTSIDE the interval [0,1]
      int number_cells_numerical_vapor;		// number of control volumes where the volume of fluid
						// function is INSIDE the interval [0,1]
						// while the cell has 6 neighbours with the same sign:
						// the cell is NOT an interface cell
      int number_cells_invalid_volume_of_fluid; // sum of number of vapour cells and number of cells
						// with the volume of fluid outside [0,1];
      int iteration_number;			// iteration number for mass redistribution
						// algorithm
      int i_index, j_index, k_index;  		// local variables for loop indexing

	
      /* allocate memory for the redistribution_velocity */
      
      velocity_mass_redistribution_x1.create(number_primary_cells_i+1, number_primary_cells_j+2,
						   number_primary_cells_k+2);
      velocity_mass_redistribution_x2.create(number_primary_cells_i+2, number_primary_cells_j+1,
						   number_primary_cells_k+2);
      velocity_mass_redistribution_x3.create(number_primary_cells_i+2, number_primary_cells_j+2,
						   number_primary_cells_k+1);
      
      /* allocate memory for the time derivative of the volume of fluid correction*/
      
      
      time_derivative_volume_of_fluid_correction.create(number_primary_cells_i+2,
                                                                      number_primary_cells_j+2,
						                                 number_primary_cells_k+2);



      /* compute the time-step for the mass redistribution algorithm */
      
      time_step_mass_redistribution=cfl_number_mass_redistribution/(one_over_dx1+one_over_dx2+one_over_dx3);

      /* take care of the two-dimensional cases for time step computation*/
      
      if(number_primary_cells_i==1)
      {
	  time_step_mass_redistribution=cfl_number_mass_redistribution/
					  (one_over_dx2+one_over_dx3);
      }
      if(number_primary_cells_i==1)
      {
	  time_step_mass_redistribution=cfl_number_mass_redistribution/
					  (one_over_dx1+one_over_dx3);
      }
      if(number_primary_cells_i==1)
      {
      time_step_mass_redistribution=cfl_number_mass_redistribution/
					  (one_over_dx1+one_over_dx2);
      }
     
      /* compute velocity for the mass redistribution velocity */
      
      set_constant_vector(number_primary_cells_i+1*number_primary_cells_j*number_primary_cells_k,
				**velocity_mass_redistribution_x1,0.0);
      set_constant_vector(number_primary_cells_i*number_primary_cells_j+1*number_primary_cells_k,
				**velocity_mass_redistribution_x1,0.0);
      set_constant_vector(number_primary_cells_i*number_primary_cells_j*number_primary_cells_k+1,
				**velocity_mass_redistribution_x1,0.0);

      for(i_index=0;i_index<number_primary_cells_i+1;i_index++)
      {
	  for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
	  {
	      for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
	      {
		  velocity_mass_redistribution_x1[i_index][j_index][k_index]=
		    compute_redistribution_velocity( level_set[i_index][j_index][k_index],
							level_set[i_index+1][j_index][k_index],
							  mesh_width_x1);
	      }  
  
	  }  
     
      } 

      for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
      {
	  for(j_index=0;j_index<number_primary_cells_j+1;j_index++)
	  {
	      for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
	      {
		  velocity_mass_redistribution_x2[i_index][j_index][k_index]=
		    compute_redistribution_velocity( level_set[i_index][j_index][k_index],
							level_set[i_index][j_index+1][k_index],
							  mesh_width_x2);
	      }  
  
	  }  
     
      } 
     
      for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
      {
	  for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
	  {
	      for(k_index=0;k_index<number_primary_cells_k+1;k_index++)
	      {
		  velocity_mass_redistribution_x3[i_index][j_index][k_index]=
		    compute_redistribution_velocity( level_set[i_index][j_index][k_index],
							level_set[i_index][j_index][k_index+1],
							  mesh_width_x3);
	      }  
  
	  }  
     
      } 

      
      /* set parameters to correctly enter the correction loop */
      iteration_number=1;
      max_time_derivative_vof_correction=10.0;
      number_cells_invalid_volume_of_fluid=10;
      
      while(iteration_number<maximum_number_mass_redistribution_iterations &&
				  number_cells_vof_out_of_bounds>0 && 
				  max_time_derivative_vof_correction > mass_redistribution_tolerance)
      {
       
	  /* reset the counters and criteria */
	  
	  max_time_derivative_vof_correction=10.0;
	  number_cells_invalid_volume_of_fluid=0;
	  number_cells_numerical_vapor=0;
	  number_cells_vof_out_of_bounds=0;

	  /* compute the time derivative of the volume of fluid correction */
	  /* for all cells */
	  
	  for(i_index=1;i_index<number_primary_cells_i;i_index++)
	  {
	      for(j_index=1;j_index<number_primary_cells_j;j_index++)
	      {
		  for(k_index=1;k_index<number_primary_cells_k;k_index++)
		  {
		    
		    /* compute all fluxes for the mass redistribution equation for this cell */
		    flux_mass_redistribution_x1_left_=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x1[i_index-1][j_index  ][k_index  ],
				volume_of_fluid_correction[i_index-1][j_index][k_index],volume_of_fluid_correction[i_index][j_index][k_index]);
		    flux_mass_redistribution_x1_right=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x1[i_index  ][j_index][k_index],
				volume_of_fluid_correction[i_index][j_index][k_index],volume_of_fluid_correction[i_index+1][j_index][k_index]);
		    flux_mass_redistribution_x2_left_=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x2[i_index  ][j_index-1][k_index  ],
				volume_of_fluid_correction[i_index][j_index-1][k_index],volume_of_fluid_correction[i_index][j_index][k_index]);
		    flux_mass_redistribution_x2_right=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x2[i_index][j_index][k_index],
				volume_of_fluid_correction[i_index][j_index][k_index],volume_of_fluid_correction[i_index][j_index+1][k_index]);
		    flux_mass_redistribution_x3_left_=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x3[i_index  ][j_index  ][k_index-1],
				volume_of_fluid_correction[i_index][j_index][k_index-1],volume_of_fluid_correction[i_index][j_index][k_index]);
		    flux_mass_redistribution_x3_right=
			  upwind_flux_mass_redistribution(
			      velocity_mass_redistribution_x3[i_index][j_index][k_index],
				volume_of_fluid_correction[i_index][j_index][k_index],volume_of_fluid_correction[i_index][j_index][k_index+1]);
			  
		/* compute the complete time-derivative of the volume of fluid correction */

		    time_derivative_volume_of_fluid_correction[i_index][j_index][k_index]= -1.0*(
		        one_over_dx1*(flux_mass_redistribution_x1_left_-flux_mass_redistribution_x1_right)+
			one_over_dx2*(flux_mass_redistribution_x2_left_-flux_mass_redistribution_x2_right)+
			one_over_dx3*(flux_mass_redistribution_x3_left_-flux_mass_redistribution_x3_right));

		/* update the maximum value of this time derivative */

		    max_time_derivative_vof_correction=std::max(
			    time_derivative_volume_of_fluid_correction[i_index][j_index][k_index],
				  max_time_derivative_vof_correction);
		  }  
  
	      }  
     
	  } 


	
	/* tentatively update the value of the volume of fluid with the correction 		   */
	/* check if with this correction the number of cells with an invalid volume of fluid value */
	/* is reduced to zero. This includes both the cells with vapour as well as the cells that  */
	/* have a volume of fluid value outside the interval [0,1] */
	
	  for(i_index=1;i_index<number_primary_cells_i;i_index++)
	  {
	      for(j_index=1;j_index<number_primary_cells_j;j_index++)
	      {
		  for(k_index=1;k_index<number_primary_cells_k;k_index++)
		  {
		      /* update the volume of fluid correction */
		      
		      volume_of_fluid_correction[i_index][j_index][k_index]=
			    time_step_mass_redistribution*
			    time_derivative_volume_of_fluid_correction[i_index][j_index][k_index];
			    
		      current_level_set_value=level_set[i_index][j_index][k_index];
		      current_volume_of_fluid_value=volume_of_fluid[i_index][j_index][k_index];
		      /* check if the cell is a vapour cell, when this update is applied */
		    if(
			/* is the cell surrounded by neighbours with the same sign ?*/
		      (
		      (level_set[i_index-1][j_index  ][k_index  ]*current_level_set_value>0)&&
		      (level_set[i_index+1][j_index  ][k_index  ]*current_level_set_value>0)&&
		      (level_set[i_index  ][j_index-1][k_index  ]*current_level_set_value>0)&&
		      (level_set[i_index  ][j_index+1][k_index  ]*current_level_set_value>0)&&
		      (level_set[i_index  ][j_index  ][k_index+1]*current_level_set_value>0)&&
		      (level_set[i_index  ][j_index  ][k_index-1]*current_level_set_value>0))
			    &&
			/* is the volume of fluid at the same time in the OPEN interval <0,1>? */
		      (current_volume_of_fluid_value>1.0-volume_of_fluid_tolerance ||
			  current_volume_of_fluid_value<0.0+volume_of_fluid_tolerance)
		    )
		    {
		    /* then the cell is a vapour cell, and the volume_of_fluid_correction is not correct yet */
		    /* additional iterations are required */
		    /* update the number of vapour cells */
		    
			  number_cells_numerical_vapor++;
		    }
		    if( (current_volume_of_fluid_value> 1.0+volume_of_fluid_tolerance)||
			    (current_volume_of_fluid_value < 0.0-volume_of_fluid_tolerance))
		    {
		      /* the volume of fluid value is out of bounds */
		      /* additional iterations are required */
		      /* update the number of out of bounds cells */
			  
			  number_cells_vof_out_of_bounds++;
		    }  
		      
		    
		  } 
  
	      }  
     
	  } 
	  number_cells_invalid_volume_of_fluid=number_cells_numerical_vapor+number_cells_vof_out_of_bounds;
      }
     
      /* check if the mass redistribution algorithm produced a satisfactory solution */
      /* after the number of allowed iterations has been performed 		     */
      
      if(iteration_number > maximum_number_mass_redistribution_iterations &&
				  number_cells_vof_out_of_bounds>0 )
      {
	/* the number of iterations exceeds the allowed number of iterations */
	/* and there are still cells with incorrect values 		     */
	  std::cerr<< "Mass redistribution algorithm did not produce a correct \n" <<
		      "volume of fluid field";

	  exit(1);  
      }
      else
      {
	
	  for(i_index=1;i_index<number_primary_cells_i;i_index++)
	  {
	      for(j_index=1;j_index<number_primary_cells_j;j_index++)
	      {
		  for(k_index=1;k_index<number_primary_cells_k;k_index++)
		  {
		      /* update the volume of fluid correction */
		      
		      volume_of_fluid[i_index][j_index][k_index]+=
			      volume_of_fluid_correction[i_index][j_index][k_index];

		    
		  } 
  
	      }  
     
	  } 
      }
      
      /* deallocate memory for the redistribution_velocity */
      
      velocity_mass_redistribution_x1.destroy();
      velocity_mass_redistribution_x2.destroy();
      velocity_mass_redistribution_x3.destroy();
      
      /* deallocate memory for the time derivative of the volume of fluid correction*/
      
      
      time_derivative_volume_of_fluid_correction.destroy();
  }     
  

#include "../headers/array.h"

#include<cstdlib>
#include<iostream>
#include<algorithm>
/********************************************************************************/
/********************************************************************************/
/*  Function to compute the volume of fluid for u1 controlvolumes 		*/
/*  method. 										*/
/*  											*/
/*  Programmer	: Duncan van der Heul       						*/
/*  Date	: 10-03-2013       							*/
/*  Update	:        								*/
/********************************************************************************/
/* Notes										*/
/********************************************************************************/

EXPORT void compute_vof_at_u1_points(
	Array3<double> level_set, 				// level set field 
							// mass conserving
	Array3<double> d_level_set_d_x1,			// first partial derivative of
							// the level-set field wrt x1
							// second order central approximation
	Array3<double> d_level_set_d_x2,			// first partial derivative of 
							// the level-set field wrt x2
							// second order central approximation
	Array3<double> d_level_set_d_x3,			// first partial derivative of
 							// the level-set field wrt x3
							// second order central approximation
	Array3<double> volume_of_fluid_u1,			// volume of fluid value for the controlvolumes
							// of the momentum equation in x1 direction
	int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
	double lower_bound_derivatives    		// lower bound for the first partial derivatives
							// to consider it a limiting case of vanishing
							// partial derivatives
     )
{
	double scaled_level_set;
	double scaled_volume_donating_region;
	
       int i_index, j_index, k_index;  		// local variables for loop indexing
	

 /* compute the vof of the control volume of the u1 velocity */
	  
	for( i_index=0;i_index<number_primary_cells_i+1;i_index++){
	    for(j_index=1;j_index<number_primary_cells_j+1;j_index++){
		for(k_index=1;k_index<number_primary_cells_k+1;k_index++){
			volume_of_fluid_u1[i_index][j_index][k_index]=0;
			if(i_index>0)
			{
				scaled_level_set=
				level_set[i_index][j_index][k_index]+0.25*
			  	d_level_set_d_x1[i_index][j_index][k_index];
				if(!level_set_2_vof( scaled_level_set,
				       d_level_set_d_x1[i_index][j_index][k_index]*0.5,
					   d_level_set_d_x2[i_index][j_index][k_index],
					      d_level_set_d_x3[i_index][j_index][k_index],
					    scaled_volume_donating_region,
					      lower_bound_derivatives))
                                {
				        volume_of_fluid_u1[i_index][j_index][k_index]=
			  	                0.5*scaled_volume_donating_region;
                                }
                                else
                                {
                                        std::cerr<< "function evaluation level_set_2_vof\n";
                                        std::cerr<< "failed in compute_vof_at_u1_points \n";
                                        std::cerr<< "line 57 \n";
                                        exit(1);
                                }
			}
			if(i_index<number_primary_cells_i)
			{
				scaled_level_set=
				level_set[i_index+1][j_index][k_index]-
				0.25*d_level_set_d_x1[i_index+1][j_index][k_index];
				if(!level_set_2_vof( scaled_level_set,
				       d_level_set_d_x1[i_index+1][j_index][k_index]*0.5,
					   d_level_set_d_x2[i_index+1][j_index][k_index],
					      d_level_set_d_x3[i_index+1][j_index][k_index],
					    scaled_volume_donating_region,
					    lower_bound_derivatives))
                                {
				        volume_of_fluid_u1[i_index][j_index][k_index]+=
			  	                0.5*scaled_volume_donating_region;
                                }
                                else
                                {
                                        std::cerr<< "function evaluation level_set_2_vof\n";
                                        std::cerr<< "failed in compute_vof_at_u1_points \n";
                                        std::cerr<< "line 80 \n";
                                        exit(1);
                                }
			}
// 			  std::cerr<<"vof u1 "<<volume_of_fluid_u1[i_index][j_index][k_index]<<"\n";
		}
	    }
	}
	
	
}


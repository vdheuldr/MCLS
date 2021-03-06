
#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
class coordinate
{
public:
  double x1,x2,x3;
  coordinate(double xx1=0, double xx2=0, double xx3=0){x1=xx1;x2=xx2;x3=xx3;}
};
class vector
{
public:
  double u1,u2,u3;
  vector(double uu1=0, double uu2=0, double uu3=0){u1=uu1;u2=uu2;u3=uu3;}
};
/********************************************************************************/
/*  Function to do the time-stepping sequence from start to end                 */
/*  											*/
/*  Programmer	: Duncan van der Heul       						*/
/*  Date	: 10-03-2013       							*/
/*  Update	:        								*/
/********************************************************************************/
/* Notes:										*/
/*  The magnitude, centroid and velocity of the volume enclosed by the 		*/
/*  interface are computed and collected and written to file			*/
/********************************************************************************/
void analyse_interface_properties(
      	double ***volume_of_fluid, 			// volume of fluid field
	double ***level_set,				// level set field
      	double ***u_1_velocity_new, 			// velocity field at new time level x1 direction
      	double ***u_2_velocity_new, 			// velocity field at new time level x2 direction
      	double ***u_3_velocity_new,			// velocity field at new time level x3 direction
      	int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
      	int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
      	int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
      	double mesh_width_x1,				// grid spacing in x1 direction (uniform)
      	double mesh_width_x2,				// grid spacing in x2 direction (uniform)
      	double mesh_width_x3,				// grid spacing in x3 direction (uniform)
	double actual_time,				// actual time in the simulation for which the solution
	double lower_bound_derivatives    		// lower bound for the first partial derivatives
							// to consider it a limiting case of vanishing
							// partial derivatives
     )
{
	void compute_vof_at_u1_points(	// compute volume of fluid for u1 control volumes
		double ***level_set, 				
		double ***d_level_set_d_x1,			
		double ***d_level_set_d_x2,			
		double ***d_level_set_d_x3,			
		double ***volume_of_fluid_u1,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k,			
		double lower_bound_derivatives    		
     		);
 	void compute_vof_at_u2_points(	// compute volume of fluid for u2 control volumes
		double ***level_set, 				
		double ***d_level_set_d_x1,			
		double ***d_level_set_d_x2,			
		double ***d_level_set_d_x3,			
		double ***volume_of_fluid_u2,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k,			
		double lower_bound_derivatives    		
     		);
	void compute_vof_at_u3_points(	// compute volume of fluid for u3 control volumes
		double ***level_set, 				
		double ***d_level_set_d_x1,			
		double ***d_level_set_d_x2,			
		double ***d_level_set_d_x3,			
		double ***volume_of_fluid_u3,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k,			
		double lower_bound_derivatives    		
     		);
     double ***double_Matrix2(                         			// allocate memory for a three-
	       int number_primary_cells_i,		                   	// dimensional array of doubles
		int number_primary_cells_j, 	
		int number_primary_cells_k
		);
      void   free_double_Matrix2( 						// deallocate memory for a three
		double ***doubleMatrix2, 					// dimensional array of doubles
		int number_primary_cells_i,	
		int number_primary_cells_j
		);
      void 	compute_level_set_gradient(					// compute gradient of level-set field
		double ***level_set_star, 
		double ***d_level_set_d_x1, 
		double ***d_level_set_d_x2, 
		double ***d_level_set_d_x3,
		int number_primary_cells_i, 
		int number_primary_cells_j, 
		int number_primary_cells_k
		 );
	
	coordinate volume_centroid;  			// centroid of the volum enclosed by the interface 
	vector velocity_centroid;			// velocity of the volume enclosed by the interface 
	vector velocity_centroid_vof;			// velocity of the volume enclosed by the interface 
							// computed using vof weighting*/
	double enclosed_volume=0;			// magnitude of the volume enclosed by the interface
	double x1_coordinate_cell_center;		// x1 coordinate of primary cell center
	double x2_coordinate_cell_center;		// x2 coordinate of primary cell center
	double x3_coordinate_cell_center;		// x3 coordinate of primary cell center
	double ***d_level_set_d_x1;			// first partial derivative of
							// the level-set field wrt x1
							// second order central approximation
	double ***d_level_set_d_x2;			// first partial derivative of 
							// the level-set field wrt x2
							// second order central approximation
	double ***d_level_set_d_x3;			// first partial derivative of
 							// the level-set field wrt x3
							// second order central approximation
	double ***volume_of_fluid_u1;			// volume of fluid value for the controlvolumes
							// of the momentum equation in x1 direction
	double ***volume_of_fluid_u2;			// volume of fluid value for the controlvolumes
							// of the momentum equation in x1 direction
	double ***volume_of_fluid_u3;			// volume of fluid value for the controlvolumes
							// of the momentum equation in x1 direction
	double LS_BBi0, LS_BBi1;			// the LS-value of the two compared cells 
	double LS_RTk0, LS_RTk1;			// the LS-value of the two compared cells 
	int i_indexBB0, i_indexBB1;			// index of the cell in which the sign of the LS changes
	int k_indexRT0, k_indexRT1;			// index of the cell in which the sign of the LS changes
						
	
       static ofstream interface_details; 	/* output stream for details of the interface */
      	int i_index, j_index, k_index;  		// local variables for loop indexing
	
	/* compute centroid of the enclosed volume */
	/* and compute the velocity of the enclosed volume */
	/* by weighting the coordinates of the cell center and the velocity at the  */
	/* cell center by the volume of fluid 					   */
	
	
	for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
  	{
      		x1_coordinate_cell_center=(i_index-0.5)*mesh_width_x1;
		
		for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
      		{
     			x2_coordinate_cell_center=(j_index-0.5)*mesh_width_x2;
			
	  		for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
	  		{
     				x3_coordinate_cell_center=(k_index-0.5)*mesh_width_x3;
				
				enclosed_volume+=(1-volume_of_fluid[i_index][j_index][k_index]);
				volume_centroid.x1+= (1-volume_of_fluid[i_index][j_index][k_index])*
							x1_coordinate_cell_center;
				volume_centroid.x2+= (1-volume_of_fluid[i_index][j_index][k_index])*
							x2_coordinate_cell_center;
				volume_centroid.x3+= (1-volume_of_fluid[i_index][j_index][k_index])*
							x3_coordinate_cell_center;
				velocity_centroid.u1+=(1-volume_of_fluid[i_index][j_index][k_index])*
							0.5*(u_1_velocity_new[i_index-1][j_index][k_index]+
								u_1_velocity_new[i_index][j_index][k_index]);
				velocity_centroid.u2+=(1-volume_of_fluid[i_index][j_index-1][k_index])*
							0.5*(u_2_velocity_new[i_index][j_index][k_index]+
								u_2_velocity_new[i_index-1][j_index][k_index]);
				velocity_centroid.u3+=(1-volume_of_fluid[i_index][j_index][k_index])*
							0.5*(u_3_velocity_new[i_index][j_index][k_index-1]+
								u_3_velocity_new[i_index][j_index][k_index]);
  
	  		}  
  
      		}  
     
  	} 
  	
  	/* scale the sum of all components and coordinates by the enclosed volume */
  	
  	volume_centroid.x1=volume_centroid.x1/enclosed_volume;
 	volume_centroid.x2=volume_centroid.x2/enclosed_volume;
 	volume_centroid.x3=volume_centroid.x3/enclosed_volume;
	
	velocity_centroid.u1=velocity_centroid.u1/enclosed_volume;
	velocity_centroid.u2=velocity_centroid.u2/enclosed_volume;
	velocity_centroid.u3=velocity_centroid.u3/enclosed_volume;
		
	
	/* the new approach to obtain the centroid velocities */
	
	/* allocate memory for the volume of fluid fields at the velocity points */
	
	volume_of_fluid_u1=double_Matrix2(number_primary_cells_i+1, number_primary_cells_j+2,
					      number_primary_cells_k+2);
	volume_of_fluid_u2=double_Matrix2(number_primary_cells_i+2, number_primary_cells_j+1,
					      number_primary_cells_k+2);
	volume_of_fluid_u3=double_Matrix2(number_primary_cells_i+2, number_primary_cells_j+2,
					      number_primary_cells_k+1);
	
	
	
	/* allocate memory for the derivatives of the level-set field */
	
	
	d_level_set_d_x1=double_Matrix2(number_primary_cells_i+2, number_primary_cells_j+2,
				    number_primary_cells_k+2);
	d_level_set_d_x2=double_Matrix2(number_primary_cells_i+2, number_primary_cells_j+2,
				    number_primary_cells_k+2);
	d_level_set_d_x3=double_Matrix2(number_primary_cells_i+2, number_primary_cells_j+2,
				    number_primary_cells_k+2);
	
	/* compute the gradient of the old level-set field, necessary for the level-set/vof conversion */
	compute_level_set_gradient(level_set, d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,
				    number_primary_cells_i, number_primary_cells_j, number_primary_cells_k					  
				      );
	
	/* compute volume of fluid values at the u1 points */

	compute_vof_at_u1_points( level_set, 	
					d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u1,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute volume of fluid values at the u2 points */

	compute_vof_at_u2_points( level_set,	
					d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u2,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute volume of fluid values at the u3 points */
	

	compute_vof_at_u3_points( level_set, 	
					d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u3,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute the velocity at the centroid using local vof values */
	
	for(i_index=0;i_index<number_primary_cells_i+1;i_index++)
  	{		
		for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
      		{			
	  		for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
	  		{
				if(i_index==0 ||i_index==number_primary_cells_i)
				{
 				velocity_centroid_vof.u1+=(0.5-volume_of_fluid_u1[i_index][j_index][k_index])*
 							u_1_velocity_new[i_index][j_index][k_index];
				}
				else
				{
				velocity_centroid_vof.u1+=(1.0-volume_of_fluid_u1[i_index][j_index][k_index])*
							u_1_velocity_new[i_index][j_index][k_index];
				}
	  		}  
  
      		}  
     
  	} 
	
	for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
  	{		
		for(j_index=0;j_index<number_primary_cells_j+1;j_index++)
      		{			
	  		for(k_index=1;k_index<number_primary_cells_k+1;k_index++)
	  		{
				if(j_index==0 ||j_index==number_primary_cells_j)
				{
					velocity_centroid_vof.u2+=(0.5-volume_of_fluid_u2[i_index][j_index][k_index])*
								u_2_velocity_new[i_index][j_index][k_index];
				}
				else
				{
					velocity_centroid_vof.u2+=(1.0-volume_of_fluid_u2[i_index][j_index][k_index])*
								u_2_velocity_new[i_index][j_index][k_index];
				}
	  		}  
  
      		}  
     
  	} 
	
	for(i_index=1;i_index<number_primary_cells_i+1;i_index++)
  	{		
		for(j_index=1;j_index<number_primary_cells_j+1;j_index++)
      		{			
	  		for(k_index=0;k_index<number_primary_cells_k+1;k_index++)
	  		{
				if(k_index==0 || k_index==number_primary_cells_k)
				{
					velocity_centroid_vof.u3+=(0.5-volume_of_fluid_u3[i_index][j_index][k_index])*
								u_3_velocity_new[i_index][j_index][k_index];
				}
				else
				{
					velocity_centroid_vof.u3+=(1.0-volume_of_fluid_u3[i_index][j_index][k_index])*
								u_3_velocity_new[i_index][j_index][k_index];
				}
	  		}  
  
      		}  
     
  	} 
	
	cerr<<"velocity_centroid_vof.u1 "<<velocity_centroid_vof.u1<<"\n";
	cerr<<"velocity_centroid_vof.u2 "<<velocity_centroid_vof.u2<<"\n";
	cerr<<"velocity_centroid_vof.u3 "<<velocity_centroid_vof.u3<<"\n";
	cerr<<"enclosed_volume	   "<<enclosed_volume<<"\n";
	
	velocity_centroid_vof.u1=velocity_centroid_vof.u1/enclosed_volume;
	velocity_centroid_vof.u2=velocity_centroid_vof.u2/enclosed_volume;
	velocity_centroid_vof.u3=velocity_centroid_vof.u3/enclosed_volume;

// 	cerr<<"velocity_centroid_vof.u1 "<<velocity_centroid_vof.u1<<"\n";
// 	cerr<<"velocity_centroid_vof.u2 "<<velocity_centroid_vof.u2<<"\n";
// 	cerr<<"velocity_centroid_vof.u3 "<<velocity_centroid_vof.u3<<"\n";
		
	enclosed_volume=enclosed_volume*mesh_width_x1*mesh_width_x2*mesh_width_x3;

// 	interface position details
	LS_BBi1 = level_set[number_primary_cells_i][2][number_primary_cells_k+1];
	for(i_index=number_primary_cells_i;i_index>0;i_index--)
  	{		
		LS_BBi0 = LS_BBi1;
		LS_BBi1 = level_set[i_index][2][number_primary_cells_k+1];
		if (LS_BBi0*LS_BBi1<0)
		{
		  i_indexBB0 = i_index+1;
		  i_indexBB1 = i_index;
		  break;
		}
  	}	
	
	
	
	
	
	
	
	
	
	/* handle the file to which the data have to be written */
	
	/* if the file is not open, open it and write the header */
	
	if(!interface_details.is_open())
	{
		cout<<"writing headers\n";
		interface_details.open("interface_details.csv");
		
		/* check if the file is open indeed */
		
		if(!interface_details.is_open())
		{
			cout<< " file to write details of the interface \n";
			cout<< " can not be opened. In function         \n";
			cout<< " analyse_interface_properties line 115  \n";
			exit(1);
		}
		
		/* write the header for the file if it is opened for the first time */
		interface_details<<"time,enclosed_volume,centroid_x1,centroid_x2,centroid_x3,";
		interface_details<<"centroid_u1,centroid_u2,centroid_u3,";
		interface_details<<"centroid_vof_u1,centroid_vof_u2,centroid_vof_u3,";
		interface_details<<"i_indexBB0,LS_BBi0,i_indexBB1,LS_BBi1,";
		interface_details<<"centroid_vof_u1,centroid_vof_u2,centroid_vof_u3\n";
	}
	
	/* write interface details for this time step */
	
	
	
	interface_details<<actual_time<<","<<enclosed_volume<<","<<volume_centroid.x1<<","<<volume_centroid.x2<<","<<volume_centroid.x3<<",";
	interface_details<<velocity_centroid.u1<<","<<velocity_centroid.u2<<","<<velocity_centroid.u3<<",";
	interface_details<<velocity_centroid_vof.u1<<","<<velocity_centroid_vof.u2<<","<<velocity_centroid_vof.u3<<",";
	interface_details<<i_indexBB0<<","<<LS_BBi0<<","<<i_indexBB1<<","<<LS_BBi1<<",";
	interface_details<<velocity_centroid_vof.u1<<","<<velocity_centroid_vof.u2<<","<<velocity_centroid_vof.u3<<"\n"<<flush;
	
	/* deallocate memory for the derivatives of the level-set field and auxiliary vof fields*/

	free_double_Matrix2(volume_of_fluid_u1, number_primary_cells_i+1, number_primary_cells_j+2);
	free_double_Matrix2(volume_of_fluid_u2, number_primary_cells_i+2, number_primary_cells_j+1);
	free_double_Matrix2(volume_of_fluid_u3, number_primary_cells_i+2, number_primary_cells_j+2);
	free_double_Matrix2(d_level_set_d_x1, number_primary_cells_i+2, number_primary_cells_j+2);
	free_double_Matrix2(d_level_set_d_x2, number_primary_cells_i+2, number_primary_cells_j+2);
	free_double_Matrix2(d_level_set_d_x3, number_primary_cells_i+2, number_primary_cells_j+2);
	
}
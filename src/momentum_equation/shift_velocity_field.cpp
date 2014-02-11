#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
/********************************************************************************/
/********************************************************************************/
/*  Function to shift the velocity field from the new to the old time-level     */
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/*  										*/
/*  										*/
/*  										*/
/********************************************************************************/

    void shift_velocity_field(
      double ***u_1_velocity_old, 	     	// velocity field at old time level x1 direction
      double ***u_2_velocity_old, 	     	// velocity field at old time level x2 direction
      double ***u_3_velocity_old,	     	// velocity field at old time level x3 direction
      double ***u_1_velocity_new, 	     	// velocity field at new time level x1 direction
      double ***u_2_velocity_new, 	     	// velocity field at new time level x2 direction
      double ***u_3_velocity_new,	     	// velocity field at new time level x3 direction
      double ***u_1_velocity_star,	     	// velocity field at star time level x1 direction
      double ***u_2_velocity_star,	     	// velocity field at star time level x2 direction
      double ***u_3_velocity_star,	     	// velocity field at star time level x3 direction
      int number_primary_cells_i,		// number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,		// number of primary (pressure) cells in x2 direction
      int number_primary_cells_k		// number of primary (pressure) cells in x3 direction
	 )		      
      {
	/* function definitions */
      void copy_general_field( 			// copy 'general' field from source to target
	    double ***source_field, 		
	    double ***target_field,		
	    int start_index_i,			
	    int final_index_i,			
	    int start_index_j,			
	    int final_index_j,			
	    int start_index_k,			
	    int final_index_k			
	   );
      int i,j,k;
 
      /* shift the velocity fields */
      /* new becomes old */
      /* new becomes star */
      
//       copy_general_field(u_1_velocity_new, u_1_velocity_old,
//                        0, number_primary_cells_i,
//                          0, number_primary_cells_j+1,
//                            0, number_primary_cells_k+1);
// 
//       copy_general_field(u_1_velocity_star, u_1_velocity_new,
//                        0, number_primary_cells_i,
//                          0, number_primary_cells_j+1,
//                            0, number_primary_cells_k+1);
// 
//       copy_general_field(u_2_velocity_new, u_2_velocity_old,
//                        0, number_primary_cells_i+1,
//                          0, number_primary_cells_j,
//                            0, number_primary_cells_k+1);
// 
//       copy_general_field(u_2_velocity_star, u_2_velocity_new,
//                        0, number_primary_cells_i+1,
//                          0, number_primary_cells_j,
//                            0, number_primary_cells_k+1);
// 
//       copy_general_field(u_3_velocity_new, u_3_velocity_old,
//                        0, number_primary_cells_i+1,
//                          0, number_primary_cells_j+1,
//                            0, number_primary_cells_k);
// 
//       copy_general_field(u_3_velocity_star, u_3_velocity_new,
//                        0, number_primary_cells_i+1,
//                          0, number_primary_cells_j+1,
//                            0, number_primary_cells_k);


      copy_general_field(u_1_velocity_star, u_1_velocity_new,
                       0, number_primary_cells_i,
                         0, number_primary_cells_j+1,
                           0, number_primary_cells_k+1);
      
      copy_general_field(u_1_velocity_new, u_1_velocity_old,
                       0, number_primary_cells_i,
                         0, number_primary_cells_j+1,
                           0, number_primary_cells_k+1);

      copy_general_field(u_2_velocity_star, u_2_velocity_new,
                       0, number_primary_cells_i+1,
                         0, number_primary_cells_j,
                           0, number_primary_cells_k+1);
      
      copy_general_field(u_2_velocity_new, u_2_velocity_old,
                       0, number_primary_cells_i+1,
                         0, number_primary_cells_j,
                           0, number_primary_cells_k+1);


      copy_general_field(u_3_velocity_star, u_3_velocity_new,
                       0, number_primary_cells_i+1,
                         0, number_primary_cells_j+1,
                           0, number_primary_cells_k);
      
      copy_general_field(u_3_velocity_new, u_3_velocity_old,
                       0, number_primary_cells_i+1,
                         0, number_primary_cells_j+1,
                           0, number_primary_cells_k);

      }
	      
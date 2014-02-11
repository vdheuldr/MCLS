/********************************************************************************/
/********************************************************************************/
/*  Function to interpolate velocity u3 to the cell center of primary cell      */
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/*  										*/
/*  										*/
/*  										*/
/*  										*/
/********************************************************************************/
 void interpolate_velocity_u3_vertex(
	  double ***u_3_velocity_new, 			// velocity field at new time level x1 direction
	  double ***u_3_velocity_vertex,		// velocity in cell vertex, x3 component
	  int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	  int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	  int number_primary_cells_k			// number of primary (pressure) cells in x3 direction
			    )
  {
	int i_index, j_index, k_index; 			// local variables for loop indexing
	    
	    for(i_index=1;i_index<number_primary_cells_i+2;i_index++)
	    {
		for(j_index=1;j_index<number_primary_cells_j+2;j_index++)
		{
		    for(k_index=0;k_index<number_primary_cells_k+1;k_index++)
		    {
		      u_3_velocity_vertex[i_index-1][j_index-1][k_index]=0.25*(
			      u_3_velocity_new[i_index-1][j_index  ][k_index  ] +
			      u_3_velocity_new[i_index-1][j_index-1][k_index  ] +
			      u_3_velocity_new[i_index  ][j_index  ][k_index  ] +
			      u_3_velocity_new[i_index  ][j_index-1][k_index  ] 
				      );
		    }
		}
	    }
  }
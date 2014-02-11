/********************************************************************************/
/********************************************************************************/
/*  Function to compute the density at the controlvolumes of the velocity    	*/
/*  											*/
/*  Programmer	: Duncan van der Heul       						*/
/*  Date	: 10-03-2013       							*/
/*  Update	:        								*/
/********************************************************************************/
/* Notes										*/
/********************************************************************************/

	void compute_density_u_controlvolumes
	(
	double ***level_set, 				// level set field 
							// mass conserving
	double ***scaled_density_u1,			// scaled density for the controlvolumes
							// of the momentum equation in x1 direction
	double ***scaled_density_u2,			// scaled density for the controlvolumes
							// of the momentum equation in x2 direction
	double ***scaled_density_u3,			// scaled density for the controlvolumes
							// of the momentum equation in x3 direction
	int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
       double rho_plus_over_rho_minus,		// ratio of the densities of the two phases
	double lower_bound_derivatives    		// lower bound for the first partial derivatives
							// to consider it a limiting case of vanishing
							// partial derivatives
		
	)
	{
	void compute_vof_at_u1_points(		// compute volume of fluid for u1 control volumes
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
 	void compute_vof_at_u2_points(		// compute volume of fluid for u2 control volumes
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
	void compute_vof_at_u3_points(		// compute volume of fluid for u3 control volumes
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
     double ***double_Matrix2(                        			// allocate memory for a three-
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
	void compute_scaled_density_u1(					// compute scaled density at u1 control volumes
		double ***scaled_density_u1,			
		double ***volume_of_fluid_u1,		
      		double rho_plus_over_rho_minus,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k			
		);
	void compute_scaled_density_u2(					// compute scaled density at u2 control volumes
		double ***scaled_density_u2,			
		double ***volume_of_fluid_u2,		
      		double rho_plus_over_rho_minus,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k			
		);
 	void compute_scaled_density_u3(					// compute scaled density at u3 control volumes
		double ***scaled_density_u3,			
		double ***volume_of_fluid_u3,		
      		double rho_plus_over_rho_minus,		
		int number_primary_cells_i,			
		int number_primary_cells_j,			
		int number_primary_cells_k			
		);
     
      
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

	compute_vof_at_u1_points( level_set, d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u1,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute volume of fluid values at the u2 points */

	compute_vof_at_u2_points( level_set, d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u2,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute volume of fluid values at the u3 points */

	compute_vof_at_u3_points( level_set, d_level_set_d_x1, d_level_set_d_x2, d_level_set_d_x3,			
					   volume_of_fluid_u3,		
					      number_primary_cells_i, number_primary_cells_j, number_primary_cells_k,			
						  lower_bound_derivatives);	
	
	/* compute scaled density at the u1 points */

	compute_scaled_density_u1( scaled_density_u1, volume_of_fluid_u1,		
      					rho_plus_over_rho_minus,		
					   number_primary_cells_i, number_primary_cells_j, number_primary_cells_k);
	
	/* compute scaled density at the u2 points */

	compute_scaled_density_u2( scaled_density_u2, volume_of_fluid_u2,		
      					rho_plus_over_rho_minus,		
					   number_primary_cells_i, number_primary_cells_j, number_primary_cells_k);
	
	/* compute scaled density at the u3 points */

	compute_scaled_density_u3( scaled_density_u3, volume_of_fluid_u3,		
      					rho_plus_over_rho_minus,		
					   number_primary_cells_i, number_primary_cells_j, number_primary_cells_k);

       /* deallocate memory for the volume of fluid fields at the velocity points */

       free_double_Matrix2(volume_of_fluid_u1, number_primary_cells_i+1, number_primary_cells_j+2);
       free_double_Matrix2(volume_of_fluid_u2, number_primary_cells_i+2, number_primary_cells_j+1);
       free_double_Matrix2(volume_of_fluid_u3, number_primary_cells_i+2, number_primary_cells_j+2);



       /* deallocate memory for the derivatives of the level-set field */


       free_double_Matrix2(d_level_set_d_x1, number_primary_cells_i+2, number_primary_cells_j+2);
       free_double_Matrix2(d_level_set_d_x2, number_primary_cells_i+2, number_primary_cells_j+2);
       free_double_Matrix2(d_level_set_d_x3, number_primary_cells_i+2, number_primary_cells_j+2);

}
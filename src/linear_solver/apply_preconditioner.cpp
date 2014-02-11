
/********************************************************************************/
/********************************************************************************/
/*  Function to apply a precomputed incomplete choleski preconditioner          */
/*  method. 									*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* The incomplete choleski preconditioner is applied. It is based on a          */
/* splitting of the original matrix and is therefore not explicitly computed    */
/********************************************************************************/
     void apply_preconditioner(
	int i_dimension,    	// number of unknowns in the system in i-direction
	int j_dimension, 	// number of unknowns in the system in i-direction
	int k_dimension, 	// number of unknowns in the system in i-direction
	double **matrix_A,			// matrix under consideration
	double *preconditioner_matrix_M, 	// preconditioner matrix
	double *rhside_vector, 			// right hand side of preconditioner system
	double *solution_vector_x		// solution of the preconditioner system	
	)

	/* function definitions */
	{
	double *double_Vector( 			// allocate a vector of doubles with length
	      int length_vector			// length_vector
	      ); 	
							
	void free_double_Vector(		// deallocate vector of doubles
	      double *vector				
	      );	
 	
	
	
	int number_dof_in_slice= 		// number of degrees of freedom in one slice of the domain
		i_dimension*j_dimension; 	// as considered in the matrix
			   
	int dimension_system=			// dimension of the linear system 				
	  i_dimension*j_dimension*k_dimension;
	int row_index;	   			// index to indicate the row of the matrix 
	double *temporary_storage_vector;  	// vector to store intermediate results
	
	
	
	temporary_storage_vector=double_Vector(dimension_system);
	
	/* first part */
	
	temporary_storage_vector[0]=rhside_vector[0]/preconditioner_matrix_M[0];
	
	for(row_index=1;row_index<i_dimension;row_index++){
	    temporary_storage_vector[row_index]=
		  (rhside_vector[row_index]-matrix_A[1][row_index-1]*
		      temporary_storage_vector[row_index-1])/preconditioner_matrix_M[row_index];
	}
	
	for(row_index=i_dimension;row_index<number_dof_in_slice;row_index++){
	    temporary_storage_vector[row_index]=
		  (rhside_vector[row_index]-matrix_A[1][row_index-1]*
		      temporary_storage_vector[row_index-1] -
		      matrix_A[2][row_index-i_dimension]*
		      temporary_storage_vector[row_index-i_dimension] 
					)/preconditioner_matrix_M[row_index];
	}
	for(row_index=number_dof_in_slice;row_index<dimension_system;row_index++){
	    temporary_storage_vector[row_index]=
		  (rhside_vector[row_index]-matrix_A[1][row_index-1]*
		      temporary_storage_vector[row_index-1] -
		      matrix_A[2][row_index-i_dimension]*
		      temporary_storage_vector[row_index-i_dimension] -
		      matrix_A[3][row_index-number_dof_in_slice]*
			temporary_storage_vector[row_index-number_dof_in_slice]
					)/preconditioner_matrix_M[row_index];
	}
	
	/* second part */
	
	solution_vector_x[dimension_system-1]=
			  temporary_storage_vector[dimension_system-1];
	
	for(row_index=dimension_system-2;
		      row_index>dimension_system-i_dimension-1;row_index--){
	    solution_vector_x[row_index]=
		   temporary_storage_vector[row_index]-(matrix_A[1][row_index]*
		      solution_vector_x[row_index+1])/preconditioner_matrix_M[row_index];
	}
	
	for(row_index=dimension_system-i_dimension-1;
		      row_index>dimension_system-number_dof_in_slice-1;row_index--){
	    solution_vector_x[row_index]=
		   temporary_storage_vector[row_index]-(matrix_A[1][row_index]*
		      solution_vector_x[row_index+1] +
		      matrix_A[2][row_index]*
		      solution_vector_x[row_index+i_dimension] 
					)/preconditioner_matrix_M[row_index];
	}
	for(row_index=dimension_system-number_dof_in_slice-1;row_index>-1;row_index--){
	    solution_vector_x[row_index]=
		  temporary_storage_vector[row_index]-(matrix_A[1][row_index]*
		      solution_vector_x[row_index+1] +
		      matrix_A[2][row_index]*
		      solution_vector_x[row_index+i_dimension] +
		      matrix_A[3][row_index]*
			solution_vector_x[row_index+number_dof_in_slice]
					)/preconditioner_matrix_M[row_index];
	}
	
	/* deallocate memory for temporary storage vector */
// 		for(row_index=1;row_index<i_dimension;row_index++){
// 		    solution_vector_x[row_index]=rhside_vector[row_index];
// 		}

	free_double_Vector(temporary_storage_vector);
	
}

      
#include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
enum variable{velocity_u1, velocity_u2, velocity_u3, level_set, pressure};
enum boundary_conditions_type{dirichlet, neumann, periodic};
enum boundary_conditions_rule{constant, function};
enum cell_centerings{cell_centered, vertex_centered};


class boundary_variable
{
public:
  variable variable_name;
  boundary_conditions_type boundary_condition_type;
  boundary_conditions_rule boundary_condition_rule;
  cell_centerings cell_centering;
  double boundary_condition_value;
  boundary_variable(variable varname, boundary_conditions_type bound_type,
				     boundary_conditions_rule bound_rule,
				     cell_centerings  cell_cent,
					double bound_value );
  boundary_variable(variable varname);
};

class boundary_face
{
public:
    boundary_variable boundary_variables[5];
    boundary_face(void);
   
};
        
/********************************************************************************/
/*  Function to apply the boundary conditions to a given velocity field         */
/*               						                */
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* This function replaces the function 'bound' from Sander's original           */
/* implementation. It sets the value of the velocity components on the          */
/* boundary and the value of the velocity components in the virtual cells.      */
/*                                                                              */
/********************************************************************************/
    
  void apply_boundary_conditions_velocity(
	  boundary_face boundary_faces[6],		// array with all the information
							// for the boundary conditions 
	  double ***u_1_velocity, 			// velocity field x1 direction
	  double ***u_2_velocity, 			// velocity field x2 direction
	  double ***u_3_velocity, 			// velocity field x3 direction
	  double mesh_width_x1,				// grid spacing in x1 direction (uniform)
	  double mesh_width_x2,				// grid spacing in x2 direction (uniform)
	  double mesh_width_x3,				// grid spacing in x3 direction (uniform)
	  int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	  int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	  int number_primary_cells_k			// number of primary (pressure) cells in x3 direction
     )
  {
	/* function definitions */
     void apply_boundary_conditions_velocity_u1(        // apply boundary conditions to 
	  boundary_face boundary_faces[6],		// velocity in u1 direction
	  double ***u_1_velocity, 			
	  double mesh_width_x1,				
	  double mesh_width_x2,				
	  double mesh_width_x3,				
	  int number_primary_cells_i,			
	  int number_primary_cells_j,			
	  int number_primary_cells_k			
     );
      void apply_boundary_conditions_velocity_u2(	// apply boundary conditions to
	  boundary_face boundary_faces[6],		// velocity in u2 direction
	  double ***u_2_velocity, 			
	  double mesh_width_x1,				
	  double mesh_width_x2,				
	  double mesh_width_x3,				
	  int number_primary_cells_i,			
	  int number_primary_cells_j,			
	  int number_primary_cells_k			
     );
      void apply_boundary_conditions_velocity_u3(	// apply boundary conditions to
	  boundary_face boundary_faces[6],		// velocity in u3 direction
	  double ***u_3_velocity, 			
	  double mesh_width_x1,				
	  double mesh_width_x2,				
	  double mesh_width_x3,				
	  int number_primary_cells_i,			
	  int number_primary_cells_j,			
	  int number_primary_cells_k			
     );
      

     apply_boundary_conditions_velocity_u1(boundary_faces,		
					      u_1_velocity, 			
						  mesh_width_x1, mesh_width_x2, mesh_width_x3, 
						    number_primary_cells_i, 
						      number_primary_cells_j,			
							number_primary_cells_k);

     
     apply_boundary_conditions_velocity_u2(boundary_faces,		
					      u_2_velocity, 			
						  mesh_width_x1, mesh_width_x2, mesh_width_x3, 
						    number_primary_cells_i, 
						      number_primary_cells_j,			
							number_primary_cells_k);

     apply_boundary_conditions_velocity_u3(boundary_faces,		
					      u_3_velocity, 			
						  mesh_width_x1, mesh_width_x2, mesh_width_x3, 
						    number_primary_cells_i, 
						      number_primary_cells_j,			
							number_primary_cells_k);

 
  }






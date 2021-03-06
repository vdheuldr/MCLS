#include "../headers/array.h"
       #include<cstdlib>
#include<iostream>
#include<algorithm>
#include<math.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;

/********************************************************************************/
/*  Function to solve the tentative velocity field u star, x2 components        */
/*                                                                              */
/*                                                                              */
/*  Programmer       : Duncan van der Heul                                      */
/*  Date      : 10-03-2013                                                      */
/*  Update    :                                                                 */
/********************************************************************************/
/* Notes                                                                        */
/*                                                                              */
/*                                                                              */
/*                                                                              */
/*                                                                              */
/********************************************************************************/

EXPORT void initialize_pressure(
      Array3<double> pressure,                        // pressure
      double mesh_width_x1,                      // grid spacing in x1 direction (uniform)
      double mesh_width_x2,                      // grid spacing in x2 direction (uniform)
      double mesh_width_x3,                      // grid spacing in x3 direction (uniform)
      int number_primary_cells_i,                // number of primary (pressure) cells in x1 direction
      int number_primary_cells_j,                // number of primary (pressure) cells in x2 direction
      int number_primary_cells_k                 // number of primary (pressure) cells in x3 direction
           )
      {
       double x3_coordinate_cell_center;         // x3 coordinate of primary cell center
       double gravity=-10.0;
       int i_index, j_index, k_index;            // local variables for loop indexing
        
       for(i_index=0;i_index<number_primary_cells_i+2;i_index++)
       {
              for(j_index=0;j_index<number_primary_cells_j+2;j_index++)
              {
                     for(k_index=0;k_index<number_primary_cells_k+2;k_index++)
                     {
                            x3_coordinate_cell_center=mesh_width_x3*k_index;
                            pressure[i_index][j_index][k_index]=x3_coordinate_cell_center*gravity;


                     }
              }
       }

      }

      

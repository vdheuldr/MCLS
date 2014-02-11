#include <iostream>
#include <stdlib.h> 
#include <stdio.h> 
#include <math.h>
#include <string>
#include <sstream>
#include <fstream>
using namespace std;
class restart_parameters
{
public:
      int start_from_restart_file;		
      int write_solution_to_restart_file;
      string name_restart_file_to_write;
      string name_restart_file_to_read;
      restart_parameters(void);
};
/********************************************************************************/
/*  Function to read a restart file containing the main unknowns                */
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* The restart file is a binary file with the complete solution. 		*/
/* The solution is read in the following order					*/
/* 										*/
/*	  level_set								*/	
/*	  volume_of_fluid 							*/		
/*	  u_1_velocity 								*/		
/*	  u_2_velocity 								*/		
/*	  u_3_velocity								*/		
/*	  pressure								*/		
/********************************************************************************/
     void read_restart_file(
	  double &time_of_restart,			// time for which restart file is available
	  double ***level_set, 				// level set field at new time level
							// mass conserving
	  double ***volume_of_fluid, 			// volume of fluid field
	  double ***u_1_velocity, 			// velocity field x1 direction
	  double ***u_2_velocity, 			// velocity field x2 direction
	  double ***u_3_velocity,			// velocity field x3 direction
	  double ***pressure,				// pressure field
	  int number_primary_cells_i,			// number of primary (pressure) cells in x1 direction
	  int number_primary_cells_j,			// number of primary (pressure) cells in x2 direction
	  int number_primary_cells_k,			// number of primary (pressure) cells in x3 direction
	  restart_parameters my_restart_parameters	// all parameters for reading/writing restart files
	   )
     {     
     
     
     
	    ifstream InputFile;				// the stream is set to the output file
	    string InputFile_name;			// the name of the output file
	    int final_index_i;				// final index first dimension 
	    int final_index_j;				// final index second dimension 
	    int final_index_k;				// final index third dimension 
	    int i_index, j_index;  			// local variables for loop indexing
	    double gnoef;
     

	  
	  /* initialize the output file for writing */
	  
	  InputFile_name=my_restart_parameters.name_restart_file_to_read;
	  InputFile.open(InputFile_name.c_str(), ios::in | ios:: binary);

	  /* read the time at which the restart file is available */
	  
	  InputFile.read((char *) &time_of_restart, sizeof(time_of_restart));
	  
	  cerr<<"time of restart is "<< time_of_restart<<" \n";
	
	  /*read the level-set field */
	  
  	  final_index_i=number_primary_cells_i+1;
 	  final_index_j=number_primary_cells_j+1;
 	  final_index_k=number_primary_cells_k+1;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) level_set[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }
	  
	  /* write the volume of fluid field */
	  
  	  final_index_i=number_primary_cells_i+1;
 	  final_index_j=number_primary_cells_j+1;
 	  final_index_k=number_primary_cells_k+1;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) volume_of_fluid[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }

          /* write the velocity field in 1 - direction */
	  
  	  final_index_i=number_primary_cells_i;
 	  final_index_j=number_primary_cells_j+1;
 	  final_index_k=number_primary_cells_k+1;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) u_1_velocity[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }
          
	  /* write the velocity field in 2 - direction */
	  
  	  final_index_i=number_primary_cells_i+1;
 	  final_index_j=number_primary_cells_j;
 	  final_index_k=number_primary_cells_k+1;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) u_2_velocity[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }
	  /* write the velocity field in 3 - direction */
	  
  	  final_index_i=number_primary_cells_i+1;
 	  final_index_j=number_primary_cells_j+1;
 	  final_index_k=number_primary_cells_k;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) u_3_velocity[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }
          
	  /* write the pressure field */
	  
  	  final_index_i=number_primary_cells_i+1;
 	  final_index_j=number_primary_cells_j+1;
 	  final_index_k=number_primary_cells_k+1;
 	  
          for(i_index=0;i_index<final_index_i+1;i_index++)
          {
		for(j_index=0;j_index<final_index_j+1;j_index++)
           	{

		  InputFile.read((char *) pressure[i_index][j_index],
					  sizeof(double) * (final_index_k+1));

           	}
          }
          

	  InputFile.close();
     }
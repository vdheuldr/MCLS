
/********************************************************************************/
/********************************************************************************/
/*  Function to compute the flux for the advection equation for the level-set   */
/*  field.                       						*/
/*  										*/
/*  Programmer	: Duncan van der Heul       					*/
/*  Date	: 10-03-2013       						*/
/*  Update	:        							*/
/********************************************************************************/
/* Notes									*/
/* A first order upwind scheme is used for the convection equation              */
/********************************************************************************/


EXPORT double compute_level_set_flux( 		
    double cell_face_velocity,				// normal velocity at the face
    double left_phi, 					// left hand value of level-set
    double right_phi)					// right hand value of level-set
  {
    double flux;					// flux for level-set advection
							// equation
      if(cell_face_velocity>0.0)
      {
	  flux=left_phi*cell_face_velocity;
	  return flux;
      }
      else	
      {
	  if(cell_face_velocity<0.0)
	  {
	      flux=right_phi*cell_face_velocity;
	      return flux;
	  }
	  else
	  {
	      flux=0.5*(left_phi+right_phi)*cell_face_velocity;
	      return flux;
	  }
	    
      }
      
  }



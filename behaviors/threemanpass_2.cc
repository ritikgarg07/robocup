#include <iostream>
#include <cmath>

int main()
{
	float distance = 5;         // distance for hitting the ball
    float angle = 10;           // (IN DEGREES)
    float angle_rad = angle*M_PI/180;


    VecPosition threemanpass_position[50];
    threemanpass_position[0] = VecPosition(0,0,0);
    
    threemanpass_position[1].setX(threemanpass_position[0].getX() + distance*cos(angle_rad));
    threemanpass_position[1].setY(threemanpass_position[0].getY() + distance*sin(angle_rad));
    threemanpass_position[1].setZ(0);
    for (int ii = 2; ii < 50; ii++)
    {
        threemanpass_position[ii].setX(threemanpass_position[ii - 1].getX() + distance*cos(2*angle_rad));
        threemanpass_position[ii].setY(threemanpass_position[ii-1].getY() + (pow(-1,(ii-1)%2))*distance*sin(2*angle_rad))

        threemanpass_position[ii].setZ(0);            
    }
	/*
	*
	*
	*
	*/

	if (worldModel->getUNum() == PLAYER_CENTRE)
   	{
   		if (PLAYER_CENTRE == playerClosestToBall)
   		{
   			return kickBall(KICK_FORWARD,threemanpass_position[counter+1]);
  		}
  		else 
  		{
  			return goToTarget(threemanpass_position[]);
  		}
   	}

   	if (worldModel->getUNum() == PLAYER_LEFT)
   	{
   		if (PLAYER_CENTRE == playerClosestToBall)
   		{
   			return kickBall(KICK_FORWARD,);
  		}
  		else 
  		{
  			return goToTarget();
  		}
   	}

   	if (worldModel->getUNum() == PLAYER_RIGHT)
   	{
   		if (PLAYER_CENTRE == playerClosestToBall)
   		{
   			return kickBall(KICK_FORWARD,);
  		}
  		else 
  		{
  			return goToTarget();
  		}
   	}
   	else return SKILL_STAND;






/*
	float distance = 5;         // distance for hitting the ball
    float angle = 30;           // (IN DEGREES)
    float angle_rad = angle*M_PI/180;


    VecPosition threemanpass_position[50];
    threemanpass_position[0] = VecPosition(0,0,0);
    
    threemanpass_position[1].setX(threemanpass_position[0].getX() + distance*cos(angle_rad));
    threemanpass_position[1].setY(threemanpass_position[0].getY() + distance*sin(angle_rad));
    threemanpass_position[1].setZ(0);
    for (int ii = 2; ii < 50; ii++)
    {
        threemanpass_position[ii].setX(threemanpass_position[ii - 1].getX() + distance*cos(2*angle_rad));
        threemanpass_position[ii].setY(threemanpass_position[ii-1].getY() + (pow(-1,(ii-1)%2))*distance*sin(2*angle_rad));
        threemanpass_position[ii].setZ(0);            
    }
    if (worldModel->getUNum() == PLAYER_LEFT)
    {
        if (me.getDistanceTo(threemanpass_position[dummy_variable]) < .5)
        {
            dummy_variable++;
        }      
        return goToTarget(threemanpass_position[dummy_variable]);
    }
    else return SKILL_STAND;
*/


   	// TODO - MAKE SEPARATE ARRAYS FOR EACH PLAYER?

#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include <cmath>


extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */


VecPosition get_centre(VecPosition A, VecPosition B, VecPosition C)     // A,B,C being points on circle
{
    VecPosition centre;
    centre.setX( (((pow(A.getX(),2) + pow(A.getY(),2))*(B.getY() - C.getY())) + ((pow(B.getX(),2) + pow(B.getY(),2))*(C.getY() - A.getY())) + ((pow(C.getX(),2) + pow(C.getY(),2))*(A.getY() - B.getY())))/(2*(((A.getX())*(B.getY() - C.getY()))- ((A.getY())*(B.getX()-C.getX())) +  B.getX()*C.getY() - C.getX()*B.getY())));
    
    centre.setY( (( pow(A.getX(),2) + pow(A.getY(),2) )*(C.getX() - B.getX()) + (pow(B.getX(),2) + pow(B.getY(),2))*(A.getX() - C.getX())  +   (pow(C.getX(),2) + pow(C.getY(),2))*(B.getX() - A.getX())) /(2*(((A.getX())*(B.getY() - C.getY()))- ((A.getY())*(B.getX()-C.getX())) +  B.getX()*C.getY() - C.getX()*B.getY())));
    
    centre.setZ(0);

    return centre;
}

float get_radius(VecPosition centre, VecPosition A)
{
    return centre.getDistanceTo(A);
}

void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    //beamX = -8 + worldModel->getUNum();
    //beamY = 0;
    beamX = -14;
    if (worldModel->getUNum() == 1) //left
    {
        beamY = 5;    
    } 
    if (worldModel->getUNum() == 2) //right
    {
        beamY = -5;    
    }
    if (worldModel->getUNum() == 3) //centre
    {
        beamY = 0;    
    }
    beamAngle = 0;
}


SkillType NaoBehavior::selectSkill() {
    // My position and angle
    //cout << worldModel->getUNum() << ": " << worldModel->getMyPosition() << ",\t" << worldModel->getMyAngDeg() << "\n";

    // Position of the ball
    //cout << worldModel->getBall() << "\n";

    // Example usage of the roboviz drawing system and RVSender in rvdraw.cc.
    // Agents draw the position of where they think the ball is
    // Also see example in naobahevior.cc for drawing agent position and
    // orientation.
    /*
    worldModel->getRVSender()->clear(); // erases drawings from previous cycle
    worldModel->getRVSender()->drawPoint("ball", ball.getX(), ball.getY(), 10.0f, RVSender::MAGENTA);
    */

    // ### Demo Behaviors ###

    // Walk in different directions
    //return goToTargetRelative(VecPosition(1,0,0), 0); // Forward
    //return goToTargetRelative(VecPosition(-1,0,0), 0); // Backward
    //return goToTargetRelative(VecPosition(0,1,0), 0); // Left
    //return goToTargetRelative(VecPosition(0,-1,0), 0); // Right
    //return goToTargetRelative(VecPosition(1,1,0), 0); // Diagonal
    //return goToTargetRelative(VecPosition(0,1,0), 90); // Turn counter-clockwise
    //return goToTargetRelative(VecPosition(0,-1,0), -90); // Turn clockwise
    //return goToTargetRelative(VecPosition(1,0,0), 15); // Circle

    // Walk to the ball
    // return goToTarget(ball);

    // Turn in place to face ball
    /*double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }*/

    // Walk to ball while always facing forward
    // return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());

    // Dribble ball toward opponent's goal
    // return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // Kick ball toward opponent's goal
    // return kickBall(KICK_FORWARD, VecPosition(0, 0, 0)); // Basic kick
    // return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // Just stand in place
    // return SKILL_STAND;

    // Demo behavior where players form a rotating circle and kick the ball
    // back and forth
    // return demoKickingCircle();
    return threemanpass();
    // return testing();
}

SkillType NaoBehavior::testing()
{ 


    enum Players
    {
        buffer,             // 0
        PLAYER_LEFT,        // 1
        PLAYER_RIGHT,       // 2
        PLAYER_CENTRE       // 3
    };

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int jj = WO_TEAMMATE1; jj < WO_TEAMMATE1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        int playerNum = jj - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( jj );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    /***********************************/


    double distance = 5;         // distance for hitting the ball
    float angle = 20;           // (IN DEGREES)
    float angle_rad = angle*M_PI/180;

    /**************determining positions***********/
    VecPosition threemanpass_position[25];
    threemanpass_position[0] = VecPosition(0,0,0);
    
    static int counter_centre = 0;
    static int counter_left = 1;
    static int counter_right = 2;


    threemanpass_position[1].setX(threemanpass_position[0].getX() + distance*cos(M_PI/4));
    threemanpass_position[1].setY(threemanpass_position[0].getY() + distance*sin(M_PI/4));
    threemanpass_position[1].setZ(0);
    for (int ii = 2; ii < 25; ii++)
    {
        threemanpass_position[ii].setX(threemanpass_position[ii - 1].getX() + distance*sin(angle_rad));
        threemanpass_position[ii].setY(threemanpass_position[ii-1].getY() + (pow(-1,(ii-1)%2))*distance*cos(angle_rad));
        threemanpass_position[ii].setZ(0);            
    }        
    /**************************************************/

    /***************checking whose turn it is to kick the ball************/

    int player_to_kick = 0;
    for (int ii = 0; ii < 25; ii++)
    {
        if(threemanpass_position[ii].getDistanceTo(ball) <= distance/2)
        {
            switch(ii%3)
            {
                case 0: player_to_kick = PLAYER_CENTRE;
                        break;
                case 1: player_to_kick = PLAYER_LEFT;
                        break;
                case 2: player_to_kick = PLAYER_RIGHT;
                        break;
                default: player_to_kick = 0;
            }
        }
    }
    /***********************************************************************/


    if (worldModel->getUNum() == PLAYER_CENTRE)
    {
        static int kicked_centre = 0;
        if(player_to_kick == PLAYER_CENTRE)
        {    
            kicked_centre = 1;            
            return kickBall(KICK_FORWARD,threemanpass_position[counter_centre + 1]);
        }
        else
        {
            if (kicked_centre == 1)         
            {
                kicked_centre = 0;
                counter_centre = counter_centre + 3;
                time_initial = worldModel->getTime();
            }

            if (counter_centre != 0)
            { 
                double rotateRate = 6;
                VecPosition centre = get_centre(threemanpass_position[counter_centre - 3],threemanpass_position[counter_centre + 1 - 3],threemanpass_position[counter_centre]);
                static double time_initial = worldModel->getTime();
                VecPosition go_to = centre + ((threemanpass_position[counter_centre - 3] - centre)*1.2).rotateAboutZ((worldModel->getTime() - time_initial)*(pow(-1,counter_centre+1))*rotateRate);
                
                if (me.getDistanceTo(threemanpass_position[counter_centre]) < 1)
                {
                    return goToTarget(threemanpass_position[counter_centre]);
                }                
                else 
                {
                    return goToTarget(go_to);
                }
            }
            else return goToTarget(collisionAvoidance(true, false, true, 1, .5, threemanpass_position[0], true));
        }
    }
    else if (worldModel->getUNum() == PLAYER_LEFT)
    {
        return goToTarget(threemanpass_position[1]);
    }
    else return SKILL_STAND;


}

SkillType NaoBehavior::threemanpass() 
{

    enum Players
    {
        buffer,             // 0
        PLAYER_LEFT,        // 1
        PLAYER_RIGHT,       // 2
        PLAYER_CENTRE       // 3
    };

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int jj = WO_TEAMMATE1; jj < WO_TEAMMATE1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        int playerNum = jj - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( jj );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }
    /***********************************/


    double distance = 6;         // distance for hitting the ball
    float angle = 20;           // (IN DEGREES)
    float angle_rad = angle*M_PI/180;
    double rotateRate = 6;

    /**************determining positions***********/
    VecPosition threemanpass_position[25];
    threemanpass_position[0] = VecPosition(0,0,0);
    
    static int counter_centre = 0;
    static int counter_left = 1;
    static int counter_right = 2;


    threemanpass_position[1].setX(threemanpass_position[0].getX() + distance*cos(M_PI/4));
    threemanpass_position[1].setY(threemanpass_position[0].getY() + distance*sin(M_PI/4));
    threemanpass_position[1].setZ(0);
    for (int ii = 2; ii < 25; ii++)
    {
        threemanpass_position[ii].setX(threemanpass_position[ii - 1].getX() + distance*sin(angle_rad));
        threemanpass_position[ii].setY(threemanpass_position[ii-1].getY() + (pow(-1,(ii-1)%2))*distance*cos(angle_rad));
        threemanpass_position[ii].setZ(0);            
    }        
    /**************************************************/

    /***************checking whose turn it is to kick the ball************/

    int player_to_kick = 0;
    double shortest_distance = 100;
    int index_of_pos = 0;
    for (int ii = 0; ii < 25; ii++)
    {
        if(threemanpass_position[ii].getDistanceTo(ball) <= shortest_distance)
        {
            shortest_distance = threemanpass_position[ii].getDistanceTo(ball);
            index_of_pos = ii;
        }
    }
    switch(index_of_pos%3)
        {
            case 0: player_to_kick = PLAYER_CENTRE;
                    break;
            case 1: player_to_kick = PLAYER_LEFT;
                    break;
            case 2: player_to_kick = PLAYER_RIGHT;
                    break;
            default: player_to_kick = 0;
        }
    /***********************************************************************/


    if (worldModel->getUNum() == PLAYER_CENTRE)
    {
        static int kicked_centre = 0;
        if(player_to_kick == PLAYER_CENTRE)
        {    
            kicked_centre = 1;            
            return kickBall(KICK_FORWARD,threemanpass_position[counter_centre + 1]);
        }
        else
        {
            if (kicked_centre == 1)         
            {
                kicked_centre = 0;
                VecPosition go_to = threemanpass_position[counter_centre + 1] + VecPosition(0.5*pow(-1,counter_centre),0,0);
                counter_centre = counter_centre + 3;               
            }

            if (counter_centre != 0)
            { 
                if (me.getDistanceTo(go_to) < 0.5)
                {
                    return goToTarget(collisionAvoidance(true, false, false, 1, .5,threemanpass_position[counter_centre], true));
                }                
                else 
                {
                    return goToTarget(go_to);
                }
            }
            else return goToTarget(threemanpass_position[0]);
        }
    }

    else if (worldModel->getUNum() == PLAYER_LEFT)
    {
        static int kicked_left = 0;
        if(player_to_kick == PLAYER_LEFT)
        {
            kicked_left = 1;          
            return kickBall(KICK_FORWARD,threemanpass_position[counter_left + 1]);
        }
        else
        {   if (kicked_left == 1)
            {
                kicked_left = 0;
                VecPosition go_to = threemanpass_position[counter_left + 1] + VecPosition(0.5*pow(-1,counter_left),0,0);
                counter_left = counter_left + 3;                
            }

            if (counter_left != 1)
            {
                if (me.getDistanceTo(go_to) < 0.5)
                {                    
                    return goToTarget(collisionAvoidance(true, false,false, 1,.5, threemanpass_position[counter_left], true));
                }
                else 
                {
                    return goToTarget(go_to);
                }
            }
            else return goToTarget(threemanpass_position[1]);
        }        
    }

    else if (worldModel->getUNum() == PLAYER_RIGHT)
    {
        static int kicked_right = 0;
        if(player_to_kick == PLAYER_RIGHT)
        {
            kicked_right = 1;            
            return kickBall(KICK_FORWARD,threemanpass_position[counter_right + 1]);
        }
        else
        {
            if (kicked_right == 1)
            {
                kicked_right = 0;
                VecPosition go_to = threemanpass_position[counter_right + 1] + VecPosition(0.5*pow(-1,counter_right),0,0);
                counter_right = counter_right + 3;
            }

            if (counter_right != 2)
            {
                if (me.getDistanceTo(go_to) < 0.5)
                {
                    return goToTarget(collisionAvoidance(true, false,false, 1,.5, threemanpass_position[counter_right], true));
                }
                else 
                {
                    return goToTarget(threemanpass_position[counter_right]);
                }
            }
            else return(goToTarget(threemanpass_position[2]));
        }    
    }
    else return SKILL_STAND;
}


/*
 * Demo behavior where players form a rotating circle and kick the ball
 * back and forth
 */

SkillType NaoBehavior::demoKickingCircle() 
{
    // Parameters for circle
    VecPosition center = VecPosition(-HALF_FIELD_X/2.0, 0, 0);
    double circleRadius = 5.0;
    double rotateRate = 2.5;

    // Find closest player to ball
    int playerClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; ++i) 
    {
        VecPosition temp;
        int playerNum = i - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) {
            // This is us
            temp = worldModel->getMyPosition();
        } else {
            WorldObject* teammate = worldModel->getWorldObject( i );
            if (teammate->validPosition) {
                temp = teammate->pos;
            } else {
                continue;
            }
        }
        temp.setZ(0);

        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) {
            playerClosestToBall = playerNum;
            closestDistanceToBall = distanceToBall;
        }
    }

    if (playerClosestToBall == worldModel-> getUNum()) 
    {
        // Have closest player kick the ball toward the center
        return kickBall(KICK_FORWARD, center);
    } 
    else 
    {
        // Move to circle position around center and face the center
        VecPosition localCenter = worldModel->g2l(center);

        SIM::AngDeg localCenterAngle = atan2Deg(localCenter.getY(), localCenter.getX());
        // Our desired target position on the circle
        // Compute target based on uniform number, !rotate rate!, and time
        VecPosition target = center + VecPosition(circleRadius,0,0).rotateAboutZ(360.0/(NUM_AGENTS-1)*(worldModel->getUNum()-(worldModel->getUNum() > playerClosestToBall ? 1 : 0)) + worldModel->getTime()*rotateRate);

        // Adjust target to not be too close to teammates or the ball
        target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);


        if (me.getDistanceTo(target) < .25 && abs(localCenterAngle) <= 10) 
        {
            // Close enough to desired position and orientation so just stand
            return SKILL_STAND;
        } 
        else if (me.getDistanceTo(target) < .5) 
        {
            // Close to desired position so start turning to face center
            return goToTargetRelative(worldModel->g2l(target), localCenterAngle);
        }
        else 
        {
            // Move toward target location
            return goToTarget(target);
        }
    }
}

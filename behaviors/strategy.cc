#include "naobehavior.h"
// #include "scram.h"
#include "../rvdraw/rvdraw.h"
#include <cmath>
#include "scram.h"

enum Players
    {
        // directions defined facing opposition's goal
        buffer,             
        LEFT_FORWARD = WO_TEAMMATE1,       
        RIGHT_FORWARD = WO_TEAMMATE2,     
        CENTRE_FORWARD = WO_TEAMMATE3,     
        LEFT_MID = WO_TEAMMATE4,            
        RIGHT_MID = WO_TEAMMATE5,           
        CENTRE_MID = WO_TEAMMATE6,          
        RIGHT_DEF = WO_TEAMMATE7,           
        RIGHT_C_DEF = WO_TEAMMATE8,         
        LEFT_DEF = WO_TEAMMATE9,            
        LEFT_C_DEF = WO_TEAMMATE10,         
        GOALKEEPER = WO_TEAMMATE11,          
    };

VecPosition defaultpos = VecPosition(0,0,0);
extern int agentBodyType;

/*
 * Real game beaming.
 * Filling params x y angle
 */

void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    switch(worldModel->getUNum())
    {
        case LEFT_FORWARD:
            beamX = -1;
            beamY = 5;
            break;
        case RIGHT_FORWARD:
            beamX = -1;
            beamY = -5;
            break;
        case CENTRE_FORWARD:
            beamX = -1;
            beamY = 0;
            break;
        case LEFT_MID:
            beamX = -5;
            beamY = 5;
            break;
        case RIGHT_MID:
            beamX = -5;
            beamY = -5;
            break;
        case CENTRE_MID:
            beamX = -5;
            beamY = 0;
            break;
        case LEFT_DEF:
            beamX = -10;
            beamY = 8;
            break;
        case LEFT_C_DEF:
            beamX = -10;
            beamY = 4;
            break;
        case RIGHT_DEF:
            beamX = -10;
            beamY = -8;
            break;
        case RIGHT_C_DEF:
            beamX = -10;
            beamY = -4;
            break;
        case GOALKEEPER:
            beamX = -14;
            beamY = 0;
            break;
        default: 
            beamX = 10;
            beamY = 10;
            break;
    }
    beamAngle = 0;

}

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
    // Vecposition A == Vecposition of a point on the CIRCLE
    return centre.getDistanceTo(A);
}


SkillType NaoBehavior::selectSkill() 
{
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
    // return threemanpass();
    // return testing();
    // return football();
    /*if(worldModel->getUNum() == LEFT_FORWARD)
    {
        std::cout << LEFT_FORWARD << " " << WO_TEAMMATE1 << " ";
        std::cout << worldModel->getUNum() << "\n";
    }*/
    //return testing();
    if ((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT))
        return kickoff();
    else if ((worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_RIGHT))
    {
        return stay();
    }
    
    else if((worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        return kickin();
    }

    /*
    else if((worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel-)getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_RIGHT)
        return kickin_opp();
    */

    else if(worldModel->getBall().getX() > 2) // change 2?
        return attackplay();
    else return defenseplay();
}

SkillType NaoBehavior::testing()
{ 
    return SKILL_STAND;
}

SkillType NaoBehavior::stay()
{
    return SKILL_STAND;
}

SkillType NaoBehavior::kickoff()
{
    if (worldModel->getUNum() == CENTRE_FORWARD)
        return kickBall(KICK_FORWARD, VecPosition(-5,6,0)); //change initial kick position
    else SKILL_STAND;
}

SkillType NaoBehavior::attackplay()
{
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
    if(worldModel->getUNum() == playerClosestToBall)
    {
        // on-ball player
        if ((me.getDistanceTo(VecPosition(15,0,0))) < 4.5)     // see 4.5?
            return kickBall(KICK_FORWARD,VecPosition(15,0,0));
        else 
            return kickBall(KICK_DRIBBLE, VecPosition(15,0,0)); //or passing?
        /*    
            calcpos();
            return kickball(KICK_FORWARD,targpos[?????])
        */     
    }
    else 
    {
        return moveToOff();          // determine way to get position 
        //return SKILL_STAND;                     // temporary    
    }
    /*
    switch(worldModel->getUNum())
    {
        case LEFT_FORWARD:
            defaultpos = VecPosition(8,5,0);
            break;
        case RIGHT_FORWARD:
            defaultpos = VecPosition(8,-5,0);
            break;
        case CENTRE_FORWARD:
            defaultpos = VecPosition(7,0,0);
            break;
        case LEFT_MID:
            defaultpos = VecPosition(1,5,0);
            break;
        case RIGHT_MID:
            defaultpos = VecPosition(1,-5,0);
            break;
        case CENTRE_MID:
            defaultpos = VecPosition(0,0,0);
            break;
        case LEFT_DEF:
            defaultpos = VecPosition(-7,6,0);
            break;
        case LEFT_C_DEF:
            defaultpos = VecPosition(-5,4,0);
            break;
        case RIGHT_DEF:
            defaultpos = VecPosition(-7,-6,0);
            break;
        case RIGHT_C_DEF:
            defaultpos = VecPosition(-5,-4,0);
            break;
        case GOALKEEPER:
            defaultpos = VecPosition(-14,0,0);
            break;
        default: 
            defaultpos = VecPosition(0,0,0);
            break;
    }
    */
}

SkillType NaoBehavior::kickin()
{
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
    if (worldModel->getUNum() == playerClosestToBall)
    {
        return kickBall(KICK_FORWARD, VecPosition(15,0,0)); //change vecposition to position of teammmate
    }  
    return moveToOff();         // change to go to position
}

SkillType NaoBehavior::kickin_opp()
{
    // return SKILL_STAND;
    return moveToOff(); //position of opponents (to block)
}

SkillType NaoBehavior::defenseplay()
{ 
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
    if(worldModel->getUNum() == playerClosestToBall)
    {
        // on-ball player
        // goToTarget(ball);
        return kickBall(KICK_FORWARD, VecPosition(15,0,0));
    }
    else return moveToOff();
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
            static VecPosition go_to = (0,0,0);
            if (kicked_centre == 1)         
            {
                kicked_centre = 0;
                go_to = threemanpass_position[counter_centre + 1] + VecPosition(0,0.5*pow(-1,counter_centre),0);
                counter_centre = counter_centre + 3;               
            }

            if (counter_centre != 0)
            { 
                static int reached_go_to = 0;
                if (me.getDistanceTo(go_to) < 1)
                {   
                    reached_go_to = 1;
                }   
                if (reached_go_to == 1)
                {             
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,threemanpass_position[counter_centre], true));
                }
                else 
                {
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,go_to, true));
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
        {               
            static VecPosition go_to = (0,0,0);
            if (kicked_left == 1)
            {
                kicked_left = 0;
                go_to = threemanpass_position[counter_left + 1] + VecPosition(0,0.5*pow(-1,counter_left),0);
                counter_left = counter_left + 3;                
            }

            if (counter_left != 1)
            {                
                static int reached_go_to = 0;
                if (me.getDistanceTo(go_to) < 1)
                {   
                    reached_go_to = 1;
                }   
                if (reached_go_to == 1)
                {             
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,threemanpass_position[counter_left], true));
                }
                else 
                {
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,go_to, true));
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
            static VecPosition go_to = (0,0,0);
            if (kicked_right == 1)
            {
                kicked_right = 0;
                go_to = threemanpass_position[counter_right + 1] + VecPosition(0,0.5*pow(-1,counter_right),0);
                counter_right = counter_right + 3;
            }

            if (counter_right != 2)
            {                
                static int reached_go_to = 0;
                if (me.getDistanceTo(go_to) < 1)
                {   
                    reached_go_to = 1;
                }   
                if (reached_go_to == 1)
                {             
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,threemanpass_position[counter_right], true));
                }
                else 
                {
                    return goToTarget(collisionAvoidance(true, false, true, 1, .5,go_to, true));
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

VecPosition targpos[11];

void NaoBehavior::calcpos()
{
    for (int ii = 0;ii <11;++ii)
    {
        targpos[ii] = ball;
    }
}
SkillType NaoBehavior::moveToOff()
{
   //make an array to store the targets
   //these will be decided based on the ball and the boundaries
   //init target array

   // change code here to set the positions
   /*targpos[0]= ball;
   double GKx = 10;
   VecPosition goalCentre = Vecposition(-15, 0, 0);
   Vecposition temp = (ball +(GKx*goalCentre))/(1+GKx);
   targpos[1] = temp;// GK i want this to move in a circle but be in the middle of the ball and the centre of the goal
   dobule CBx = 8;
   Vecposition temp = (ball +(CBx*goalCentre))/(1+CBx);
   targpos[2] = temp; //defender centre back, same as GK but further away from the goal
   targpos[3] = ; //defender cntre right
   targpos[4] = ; //defender centre left
   targpos[5] = ; //mid back lefft
   targpos[6] = ; //mid back right
   targpos[7] = ; //midright
   targpos[8] = ; //mid left
   targpos[9] = ; //forward left
   targpos[10] = ; //foward right
*/
    for (int ii = 0;ii <11;ii++)
    {
        targpos[ii] = ball;
    }
   //modify the code to target the two(or three?) "most dangerous" then change targets of the bots nearest to the dangerous positions to the dangerous positons
   //for(int i = WO_OPPONENT1; i < WO_OPPONENT1+)
   /********************************************************/
   VecPosition teampos[11];
   for(int i = WO_TEAMMATE1; i < WO_TEAMMATE1+NUM_AGENTS; i++){
       WorldObject* teammate = worldModel->getWorldObject( i );
       teampos[i-WO_TEAMMATE1] = teammate->pos;
   }
   //one position is on ball
   Test t;
   for(int i = 0; i < NUM_AGENTS; ++i)
   {
       t.starts.push_back(std::make_pair(teampos[i].getX(), teampos[i].getY()));
       t.targets.push_back(std::make_pair(targpos[i].getX(), targpos[i].getY()));
   }

   std::vector<Edge> ansVector = SOLVER(t);

   int selfindex = (worldModel->getUNum()-1);

   if(worldModel->getMyPosition().getDistanceTo(targpos[ansVector[selfindex].second.second]) < 0.50){
       return SKILL_STAND;
   }
   /********************************************************/
   VecPosition target = targpos[ansVector[selfindex].second.second];
   target = collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
   return goToTarget(target);

}

//should I have a different function for moveToDeff?
//how to work on passes?
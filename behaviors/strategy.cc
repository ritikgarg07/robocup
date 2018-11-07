#include "naobehavior.h"
// #include "scram.h"
#include "../rvdraw/rvdraw.h"
#include <cmath>

extern int agentBodyType;



enum Players
    {
        // directions defined facing opposition's goal            
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

enum Playstyle
    {   
        ATTACK_PLAY = 0,
        DEFENSE_PLAY = 1,
        GENERAL = 2,
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
            beamX = -.1;
            beamY = 6;
            break;
        case RIGHT_FORWARD:
            beamX = -.1;
            beamY = -6;
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
            beamX = -14.5;
            beamY = 0.85;
            break;
        case LEFT_C_DEF:
            beamX = -10;
            beamY = 4;
            break;
        case RIGHT_DEF:
            beamX = -14.5;
            beamY = -0.85;
            break;
        case RIGHT_C_DEF:
            beamX = -10;
            beamY = -4;
            break;
        case GOALKEEPER:
            beamX = -14.5;
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

int NaoBehavior::onballplayer()
{
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
    return playerClosestToBall; 
}

int NaoBehavior::opponentattacker()           // returns closest opponent to our goal
{
    // Find closest player to our goal
    int opponentClosestToGoal = -1;
    double closestDistanceToGoal = 10000;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        WorldObject* opponent = worldModel->getWorldObject( jj );
        if (opponent->validPosition) 
        {
            temp = opponent->pos;
        }
        else 
        {
            continue;
        }
    
        double distanceToGoal = temp.getDistanceTo(VecPosition(-15,0,0));
        if (distanceToGoal < closestDistanceToGoal) 
        {
            opponentClosestToGoal = jj;
            closestDistanceToGoal = distanceToGoal;
        }
    }
    /***********************************/
    return opponentClosestToGoal;

}



int NaoBehavior::opponentgk()           // returns closest opponent to where we want to score a goal
{
    // Find closest player to opponent goal
    int opponentClosestToGoal = -1;
    double closestDistanceToGoal = 10000;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        WorldObject* opponent = worldModel->getWorldObject( jj );
        if (opponent->validPosition) 
        {
            temp = opponent->pos;
        }
        else 
        {
            continue;
        }
    
        double distanceToGoal = temp.getDistanceTo(VecPosition(15,0,0));
        if (distanceToGoal < closestDistanceToGoal) 
        {
            opponentClosestToGoal = jj;
            closestDistanceToGoal = distanceToGoal;
        }
    }
    /***********************************/
    return opponentClosestToGoal;

}

int NaoBehavior::opponentball()           // returns closest opponent to the ball
{
    // Find closest player to opponent goal
    int opponentClosestToBall = -1;
    double closestDistanceToBall = 10000;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        WorldObject* opponent = worldModel->getWorldObject( jj );
        if (opponent->validPosition) 
        {
            temp = opponent->pos;
        }
        else 
        {
            continue;
        }
    
        double distanceToBall = temp.getDistanceTo(ball);
        if (distanceToBall < closestDistanceToBall) 
        {
            opponentClosestToBall = jj;
            closestDistanceToBall = distanceToBall;
        }
    }
    /***********************************/
    return opponentClosestToBall;
}


VecPosition NaoBehavior::getposition(int player_number)
{
    WorldObject* player = worldModel->getWorldObject(player_number);
    VecPosition temp = player->pos;
    return temp;
}

int NaoBehavior::opponentcount(VecPosition a, double distance)
{
    int opponent_counter = 0;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        WorldObject* opponent = worldModel->getWorldObject( jj );
        if (opponent->validPosition) 
        {
            temp = opponent->pos;
        }
        else 
        {
            continue;
        } 
        double distanceToplayer = temp.getDistanceTo(a);
        if (distanceToplayer < distance) 
        {
            opponent_counter++;    
        }
    }    
    return opponent_counter;    
}
bool NaoBehavior::posession()    // player closest distanceis 0.4 , closest opponent is 0.8 and closest player is not fallen
{
    int playerClosestToBall = onballplayer();
    double closestDistance = ball.getDistanceTo(getposition(playerClosestToBall));

    int opponent = opponentball();
    double closestDistanceOpp = ball.getDistanceTo(getposition(opponent));

    if(closestDistance <= 0.8 && closestDistanceOpp >= 0.8) // && !(worldModel->getFallenTeammate(playerClosestToBall))
    {
        return true;
    }
    else return false;
}

double NaoBehavior::modulus(VecPosition a)
{
    if (a.getDistanceTo(VecPosition(0,0,0)) == 0)
    {
        return 1;           // better error handling?
    }
    return VecPosition(0,0,0).getDistanceTo(a);
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

    // return kickBall(KICK_DRIBBLE, VecPosition(HALF_FIELD_X, 0, 0));

    // return kickBall(KICK_FORWARD, VecPosition(0, 0, 0)); // Basic kick
    // return kickBall(KICK_IK, VecPosition(HALF_FIELD_X, 0, 0)); // IK kick

    // return SKILL_STAND;
    
    // return demoKickingCircle();
    // return threemanpass();
    // return testing();
    static double startTime = worldModel->getTime();    
    if(((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)))
    {
        startTime = worldModel->getTime();
    }
    if (((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)) || worldModel->getTime()-startTime < 10)
    {
            return kickoff(worldModel->getTime() - startTime);
    }
    else if ((worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_RIGHT))
    {
        if(worldModel->getUNum() == LEFT_DEF || worldModel->getUNum() == RIGHT_DEF)
        {
            return moveToOff(GENERAL);
        }
        else return stay();
    }
    
    else if((worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {

        return kickin();
    }

    else if((worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_RIGHT))
        return kickin_opp();
    else if((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == GOALKEEPER)
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }
        else if(worldModel->getUNum() == CENTRE_MID)
        {
            return goToTarget(VecPosition(0,0,0));
        }
         else return moveToOff(GENERAL);               // needs to be changed so that players don't move to the ball and kick it
    }
    else if((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == LEFT_FORWARD)
        {
            return kickBall(KICK_LONG, VecPosition(12,0,0));          // change vecposition to someplace else?
        }
        else if(worldModel->getUNum() == CENTRE_FORWARD)
            return goToTarget(VecPosition(13,0,0));
        else return moveToOff(GENERAL);
    }
    else if((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_RIGHT) || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_LEFT))
    {
        return defenseplay();
    }
    else if((worldModel->getPlayMode() == PM_FREE_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_FREE_KICK_RIGHT &&worldModel->getSide() == SIDE_RIGHT))
    {
        return attackplay();
    }
    else if((worldModel->getPlayMode() == PM_FREE_KICK_LEFT && worldModel->getSide() == SIDE_RIGHT) || (worldModel->getPlayMode() == PM_FREE_KICK_RIGHT && worldModel->getSide() == SIDE_LEFT))
    {
        return defenseplay();
    }
    else if(ball.getDistanceTo(VecPosition(-15,0,0)) < 9)   // change 5
    {
        return defenseplay();
    }
    return attackplay();
}

SkillType NaoBehavior::testing()
{ 
    return SKILL_STAND;
}

SkillType NaoBehavior::stay()
{
    return SKILL_STAND;
}

SkillType NaoBehavior::kickoff(double time)
{

    if(worldModel->getUNum() == CENTRE_FORWARD)
    {
        //kick to left forward or right ehhhh
        VecPosition temp = getposition(LEFT_FORWARD);
        temp = temp + VecPosition(1,0,0);
        return kickBall(KICK_FORWARD, temp);
    }
    if(worldModel->getUNum() == LEFT_FORWARD && (time>1))           
    {
        return kickBall(KICK_LONG, VecPosition(15,0.7,0));
    }
    return SKILL_STAND;
}

SkillType NaoBehavior::attackplay()
{

    int playerClosestToBall = onballplayer();
    
    /******calculates 'offset' based on gk position************/
    int opponentClosestToGoal = opponentgk();    
    double offset = 0;
    if(getposition(opponentClosestToGoal).getY() > 0)
    {
        offset = -1;
    }
    else
    {
        offset = 1;
    }
    /**********************************************************/

    /**********sends the farther of left/right forward as a 'winger'*************/
    static int winger = -1;
    if(ball.getDistanceTo(getposition(RIGHT_FORWARD)) < ball.getDistanceTo(getposition(LEFT_FORWARD)))
    {
        int winger = LEFT_FORWARD;
    }
    else winger = RIGHT_FORWARD;
    if(worldModel->getUNum() == winger && playerClosestToBall != winger)
    {
        if(winger == RIGHT_FORWARD)
            return goToTarget(VecPosition(9,-1,0));               //change vecposition
        else return goToTarget(VecPosition(9,1,0));
    }
    /*******************************************************************************/

    int opponent_counter_winger = opponentcount(getposition(winger),2);
    int opponent_counter_onball = opponentcount(getposition(playerClosestToBall),1);

    if(worldModel->getUNum() == playerClosestToBall)
    {
        // distance to goal less than 2 => use kick_ik to kick to goal
        if(ball.getDistanceTo(VecPosition(15,0,0)) < 2)
        {
            return kickBall(KICK_IK,VecPosition(16,0,0));
        }
        
        // distance to goal more than 2 less than 4 => use short kick to kick to goal
        else if ((ball.getDistanceTo(VecPosition(15,0,0))) < 4)    
        {
            VecPosition shoot_goal = VecPosition(16,0,0);
            shoot_goal.setY(offset);
            shoot_goal.setZ(0);
            if(opponent_counter_onball < 3)
            {
                return kickBall(KICK_FORWARD,shoot_goal);
            }
            else return kickBall(KICK_IK,shoot_goal);
        }

        // distance to goal more than 3 but less than 7 => use long kick to kick to goal
        else if ((ball.getDistanceTo(VecPosition(15,0,0))) < 10)    
        {
            VecPosition shoot_goal = VecPosition(16,0,0);
            shoot_goal.setY(offset);
            shoot_goal.setZ(0);
            if(opponent_counter_onball == 0)
            {
                return kickBall(KICK_LONG,shoot_goal);
            }
            else return kickBall(KICK_DRIBBLE,shoot_goal);
        }

        // passing to wingers
        else if(opponent_counter_winger < 2 && (ball.getDistanceTo(VecPosition(15,0,0)) > 7) && opponent_counter_winger > 1)
        {
            VecPosition winger_pos = VecPosition(9,1,0);
            if(winger == RIGHT_FORWARD)
                winger_pos.setY(-1);


            if (me.getDistanceTo(VecPosition(9,0,0)) < 2)
            {
                return kickBall(KICK_IK,winger_pos);
            }
            else if (me.getDistanceTo(VecPosition(9,0,0)) < 5)
            {
                return kickBall(KICK_FORWARD,winger_pos);
            }
            else return kickBall(KICK_LONG,winger_pos);
        }    

        // dribble to goal     
        else return kickBall(KICK_DRIBBLE, VecPosition(16,0,0)); 
    }
    else 
    { 
        return moveToOff(ATTACK_PLAY);    
    }
}

SkillType NaoBehavior::kickin()
{
    
    if (worldModel->getUNum() == onballplayer())
    {
        return kickBall(KICK_FORWARD, VecPosition(16,0,0)); //change vecposition to position of teammmate
    }  
    return moveToOff(GENERAL);    
}

SkillType NaoBehavior::kickin_opp()
{
    return moveToOff(GENERAL);
}

SkillType NaoBehavior::defenseplay()
{ 
    int playerClosestToBall = onballplayer();
    
    if(worldModel->getUNum() == playerClosestToBall)
    {
        if(opponentcount(ball,2) < 2 && ball.getDistanceTo(VecPosition(-15,0,0)) > 5)
        {
            return kickBall(KICK_LONG, VecPosition(15,0,0));
        }
        else return kickBall(KICK_DRIBBLE, VecPosition(16,0,0));
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 9 && worldModel->getUNum() == LEFT_DEF)
    {
        return kickBall(KICK_DRIBBLE,VecPosition(15,0,0));
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 7 && worldModel->getUNum() == RIGHT_DEF)
    {
        return kickBall(KICK_DRIBBLE,VecPosition(15,0,0));
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 5 && (worldModel->getUNum() == GOALKEEPER) && opponentcount(VecPosition(-15,0,0),2) < 2)
    {
        if(opponentcount(ball, 2) == 0)
        {
            return kickBall(KICK_LONG, VecPosition(15,0,0));
        }
        else if (opponentcount(ball, 1) > 0) 
        {
            return kickBall(KICK_IK,VecPosition(15,0,0));           // change this to a free space
        }
        else return kickBall(KICK_DRIBBLE,VecPosition(15,0,0));
    }
    else return moveToOff(DEFENSE_PLAY);
}

VecPosition targpos[11];

SkillType NaoBehavior::moveToOff(int Playstyle)
{
    int playerClosestToBall = onballplayer();
    VecPosition target = VecPosition(0,0,0);
    targpos[0] = VecPosition(-14.5,0,0);
    targpos[1] = VecPosition(-14.5,0.85,0);
    targpos[2] = VecPosition(-14.5,-0.85,0);
    
    if(worldModel->getUNum() == LEFT_C_DEF)
    {
        targpos[3] = getposition(opponentattacker());                 // change this to attacker closest to our goal
        target = targpos[3];
        if(me.getDistanceTo(ball) > 2)
        {
            target = collisionAvoidance(false /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
        }
    }
    if (worldModel->getUNum() == GOALKEEPER)
    {   
        target = targpos[0];
    }
    else if(worldModel->getUNum() == LEFT_DEF)
    {
        target = targpos[1];
    }
    else if(worldModel->getUNum() == RIGHT_DEF)
    {
        target = targpos[2];
    }
    else if(worldModel->getUNum() != LEFT_C_DEF)
    {
        target = ball;
        if(posession() == true)
        {
            if(Playstyle == ATTACK_PLAY)
            {
                if(worldModel->getUNum() != playerClosestToBall)
                {
                    //check angle of rotation
                    VecPosition target_dir = (VecPosition(15,0,0) - ball)*(1/modulus(VecPosition(15,0,0) - ball));
                    target  = ball + ((target_dir).rotateAboutZ(5*worldModel->getUNum()));                    
                    target = collisionAvoidance(false /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
                }
                else 
                {
                    target = ball;
                    target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
                }
            }
            else if(Playstyle == DEFENSE_PLAY)
            {
                if(worldModel->getUNum() != playerClosestToBall)
                {

                    VecPosition target_dir = (VecPosition(15,0,0) - ball)*(1.0/(modulus(VecPosition(15,0,0) - ball)));
                    target  = ball + ((target_dir).rotateAboutZ(5*worldModel->getUNum()));
                    target = collisionAvoidance(false /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
                }
                else 
                {
                    target = ball;
                    target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
                }
            }
            else 
            {
                target = ball;
                target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
            }
        }
        else
        { 
            target = target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
        }
    }
    return goToTarget(target);
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



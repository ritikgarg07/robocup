// directions defined facing opposition's goal            

#include <cmath>
#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
#include "functions.h"
#include <sstream>
#include <string>
// #include "scram.h"
int a = 0;

extern int agentBodyType;

enum Players
    {
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

extern int agentBodyType;

void NaoBehavior::beam( double& beamX, double& beamY, double& beamAngle ) {
    switch(worldModel->getUNum())
    {
        case LEFT_FORWARD:
            beamX = -1;
            beamY = 6;
            break;
        case RIGHT_FORWARD:
            beamX = -1;
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
    /*
    double distance, angle;
    getTargetDistanceAndAngle(ball, distance, angle);
    if (abs(angle) > 10) {
      return goToTargetRelative(VecPosition(), angle);
    } else {
      return SKILL_STAND;
    }
    */

    // Walk to ball while always facing forward
    // return goToTargetRelative(worldModel->g2l(ball), -worldModel->getMyAngDeg());
    return testing();
    
    static double startTime = worldModel->getTime();    
    if(((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)))
    {
        startTime = worldModel->getTime();
    }
    if (((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)) || worldModel->getTime()-startTime < 10)
    {
            return kickoff(worldModel->getTime() - startTime);
    }
    if ((worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_RIGHT))
    {
        if(worldModel->getUNum() == LEFT_FORWARD && ball.getY() > 1)
        {
            //std::cout << ball.getY() << " ";
            return goToTarget(VecPosition(-0.5,ball.getY(),0));
        }
        else if(worldModel->getUNum() == RIGHT_FORWARD && ball.getY() < -1)
        {
            return goToTarget(VecPosition(-0.5,ball.getY(),0));
        }
        else if(worldModel->getUNum() == CENTRE_FORWARD)
        {
            return goToTarget(VecPosition(-3,0,0));
        }
        else return SKILL_STAND;
    }
    if((worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {

        return kickin();
    }

    if((worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_RIGHT))
        return kickin_opp();
    if((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_RIGHT) || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_LEFT))
    {
        if(worldModel->getUNum() == CENTRE_MID)
        {
            goToTarget(VecPosition(0,0,0));
        }
        if(worldModel->getUNum() == LEFT_FORWARD)
        {
            goToTarget(VecPosition(13,0,0));
        }
        else return SKILL_STAND;
    }
    if((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == GOALKEEPER && me.getDistanceTo(VecPosition(-15,0,0)) < 3)
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }
        if(worldModel->getUNum() == GOALKEEPER)
        {
            return SKILL_STAND;
        }
        if(worldModel->getUNum() == RIGHT_DEF && me.getDistanceTo(VecPosition(-15,0,0)) < 3 && (ball.getDistanceTo(getposition(GOALKEEPER)) >= 3))
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }
        if(worldModel->getUNum() == RIGHT_DEF)
            return SKILL_STAND;
        if(worldModel->getUNum() == LEFT_DEF  && (ball.getDistanceTo(getposition(GOALKEEPER)) >= 3) && (ball.getDistanceTo(getposition(RIGHT_DEF)) >= 3))
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }        
        if(worldModel->getUNum() == CENTRE_MID)
        {
            return goToTarget(VecPosition(0,0,0));
        }
        if(worldModel->getUNum() != RIGHT_DEF || worldModel->getUNum() != LEFT_DEF)
        {
            return stay();               // needs to so that players don't move to the ball and kick it
        }
        else return moveToOff();
    }
    
    if((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == LEFT_FORWARD)
        {
            return kickBall(KICK_LONG, VecPosition(12,0,0));          // change vecposition to someplace else?
        }
        else if(worldModel->getUNum() == CENTRE_FORWARD)
            return goToTarget(VecPosition(13,0,0));
        else return moveToOff();
    }

    if((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_RIGHT) || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_LEFT))
    {
        return defenseplay();
    }
    
    if((worldModel->getPlayMode() == PM_FREE_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_FREE_KICK_RIGHT &&worldModel->getSide() == SIDE_RIGHT))
    {
        return attackplay();
    }
    if((worldModel->getPlayMode() == PM_FREE_KICK_LEFT && worldModel->getSide() == SIDE_RIGHT) || (worldModel->getPlayMode() == PM_FREE_KICK_RIGHT && worldModel->getSide() == SIDE_LEFT))
    {
        return defenseplay();
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 15)   // change 5
    {
        return defenseplay();
    }
    return attackplay();
}

SkillType NaoBehavior::testing()
{   
    if (worldModel->getUNum() == CENTRE_FORWARD)
    {	
    	SIM::AngDeg orientationme = worldModel->getMyAngDeg();
    	std::cout << orientationme << "\n";
    	if(worldModel->getMyAngDeg() > 89 && worldModel->getMyAngDeg() < 91){
    		return SKILL_STAND;
    	}
    	return goToTargetRelative(VecPosition(0,1,0), 90);
    }
    else return SKILL_STAND;
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
        VecPosition temp = getposition(RIGHT_FORWARD);
        temp = temp + VecPosition(1,0,0);
        return kickBall(KICK_FORWARD, temp);
    }

    if(worldModel->getUNum() == RIGHT_FORWARD && (time>1))           
    {
        return kickBall(KICK_LONG, VecPosition(15,-0.7,0));
    }
    return SKILL_STAND;
}

SkillType NaoBehavior::kickin()
{
    if (worldModel->getUNum() == ourClosest(ball))
    {
        return kickBall(KICK_LONG, VecPosition(16,0,0)); //change vecposition to position of teammmate
    }  
    return moveToOff();    
}

SkillType NaoBehavior::kickin_opp()
{
    return moveToOff();
}

SkillType NaoBehavior::attackplay()
{
    int playerClosestToBall = ourClosest(ball);
    
    /******calculates 'offset' based on gk position************/
    int opponentClosestToGoal = oppClosest(VecPosition(15,0,0));    
    double offset = 0;
    if(getposition(opponentClosestToGoal).getY() > 0)
    {
        offset = -0.7;
    }
    else
    {
        offset = 0.7;
    }
    /**********************************************************/


    
    if(worldModel->getUNum() == playerClosestToBall)
    {
        VecPosition shoot_goal = VecPosition(16,0,0);
        shoot_goal.setY(offset);
        shoot_goal.setZ(0);
            
        // distance to goal less than 2 => use kick_ik to kick to goal
        if(ball.getDistanceTo(VecPosition(15,0,0)) < 2)
        {
            return kickBall(KICK_IK,shoot_goal);
        }
        
        // distance to goal more than 2 less than 4 => use short kick to kick to goal
        else if ((ball.getDistanceTo(VecPosition(15,0,0))) < 4)    
        {
            static int temp_opp_c = opponentcount(ball,2);
            if(worldModel->getLastSkill() != KICK_FORWARD)
            {
                temp_opp_c = opponentcount(ball,3);
            }
            if(temp_opp_c == 0)
            {
                return kickBall(KICK_FORWARD,shoot_goal);
            }
            else return kickBall(KICK_IK,shoot_goal);
        }

        // distance to goal more than 4 but less than 13 => use long kick to kick to goal
        else if ((ball.getDistanceTo(VecPosition(15,0,0))) < 13)    
        {
            static int temp_opp_c = opponentcount(ball,1.5);
            if(worldModel->getLastSkill() != KICK_LONG)
            {
                temp_opp_c = opponentcount(ball,1.5);
            }
            static int temp_opp_r = opponentcount(getposition(RIGHT_FORWARD),1.5);
            if(worldModel->getLastSkill() != worldModel->getPreviousLastSkill())
            {
                temp_opp_r = opponentcount(getposition(RIGHT_FORWARD),1.5);
            }

            static int temp_opp_l = opponentcount(getposition(LEFT_FORWARD),1.5);
            if(worldModel->getLastSkill() != worldModel->getPreviousLastSkill())
            {
                temp_opp_l = opponentcount(getposition(LEFT_FORWARD),1.5);
            }
            
            static int temp_opp_me = opponentcount(worldModel->getMyPosition(),2);
            if(worldModel->getLastSkill() != worldModel->getPreviousLastSkill())
            {
                temp_opp_me = opponentcount(worldModel->getMyPosition(),2);
            }

            if(temp_opp_c == 0)
            {
                return kickBall(KICK_LONG,shoot_goal);
            }

            //passing to wingers && choosing which winger to pass

            if(temp_opp_r == 0 && worldModel->getUNum() != RIGHT_FORWARD && worldModel->getUNum() != LEFT_FORWARD)
            {   
                if(me.getDistanceTo(getposition(RIGHT_FORWARD)) < 5)
                    return kickBall(KICK_IK,getposition(RIGHT_FORWARD) + VecPosition(0.5,0.5,0));
                else if(temp_opp_me == 0 && me.getDistanceTo(getposition(RIGHT_FORWARD)) < 5)
                    return kickBall(KICK_FORWARD,getposition(RIGHT_FORWARD)+ VecPosition(0.5,0.5,0));
                else if(temp_opp_me == 0 && me.getDistanceTo(getposition(RIGHT_FORWARD)) < 15)
                    return kickBall(KICK_LONG,getposition(RIGHT_FORWARD)+ VecPosition(0.5,0.5,0));
                else return kickBall(KICK_DRIBBLE,APF(shoot_goal));
            }
            if(temp_opp_l== 0 && worldModel->getUNum() != RIGHT_FORWARD && worldModel->getUNum() != LEFT_FORWARD)
            {   
                if(me.getDistanceTo(getposition(LEFT_FORWARD)) < 5)
                    return kickBall(KICK_IK,getposition(LEFT_FORWARD) + VecPosition(0.5,0.5,0));
                else if(temp_opp_me == 0 && me.getDistanceTo(getposition(LEFT_FORWARD)) < 5)
                    return kickBall(KICK_FORWARD,getposition(LEFT_FORWARD) + VecPosition(0.5,0.5,0));
                else if(temp_opp_me == 0 && me.getDistanceTo(getposition(LEFT_FORWARD)) < 15)
                    return kickBall(KICK_LONG,getposition(LEFT_FORWARD) + VecPosition(0.5,0.5,0));
                else return kickBall(KICK_DRIBBLE,APF(shoot_goal));
            }
            /*
            if(posession() == false)
            {
                //std::cout << "not a problem with apf";
                return kickBall(KICK_DRIBBLE,shoot_goal);
            }*/
            return kickBall(KICK_DRIBBLE,APF(shoot_goal));
        }

        // dribble to goal
        /*
        if(posession() == false)
        {

            //std::cout << "not a problem with apf";
            return kickBall(KICK_DRIBBLE,shoot_goal);
        }     */
        return kickBall(KICK_DRIBBLE, APF(shoot_goal)); 
    }
    
    /**********sends left & right forwards as 'wingers'*************/
    static int winger_left = LEFT_FORWARD;
    static int winger_right = RIGHT_FORWARD;
    double distance, angle;
    if(worldModel->getUNum() == winger_left)
    {
        if(me.getDistanceTo(VecPosition(9,1,0)) < 0.5)
        {
            getTargetDistanceAndAngle(VecPosition(15,0,0), distance, angle);
            if (abs(angle) > 10) 
            {
                return goToTargetRelative(VecPosition(), angle);
            } 
            else 
            {
            return SKILL_STAND;
            }
        }
        return goToTarget(VecPosition(9,1,0));
    }
    else if(worldModel->getUNum() == winger_right)
    {
        if(me.getDistanceTo(VecPosition(8,-1,0)) < 0.5)
        {
            getTargetDistanceAndAngle(VecPosition(15,0,0), distance, angle);
            if (abs(angle) > 10) 
            {
                return goToTargetRelative(VecPosition(), angle);
            } 
            else 
            {
            return SKILL_STAND;
            }
        }
        return goToTarget(VecPosition(8,-1,0));
    }
    /***************************************************************/

    else 
    { 
        return moveToOff();    
    }
}


SkillType NaoBehavior::defenseplay()
{ 

    int playerClosestToBall = ourClosest(ball);
    
    if(worldModel->getUNum() == playerClosestToBall && worldModel->getUNum() != GOALKEEPER)
    {
        static int temp_opp_c = opponentcount(ball,2);
        if(worldModel->getLastSkill() != KICK_LONG)
        {
            temp_opp_c = opponentcount(ball,2);
        }
        
        static int temp_opp_b = opponentcount(ball,1);
        if(worldModel->getLastSkill() != KICK_IK)
        {
            temp_opp_b = opponentcount(ball,1.5);
        }

        if(temp_opp_c == 0)
        {
            return kickBall(KICK_LONG, APF(VecPosition(15,0,0)));
        }

        if(temp_opp_b > 0)
        {
            return kickBall(KICK_IK,APF(VecPosition(16,0,0)));
        }
        /*if(posession() == false)
        {
            //std::cout << "not a problem with apf";
            return kickBall(KICK_DRIBBLE,VecPosition(16,0,0));
        }*/
        return kickBall(KICK_DRIBBLE, APF(VecPosition(16,0,0)));
    }

    if((worldModel->getUNum() == GOALKEEPER && (ball.getDistanceTo(VecPosition(-15,0,0))) < 4))
    {
        VecPosition temp = VecPosition(-15,0,0) + (ball - VecPosition(-15,0,0))*(1/modulus(VecPosition(-15,0,0) - ball));
        if(me.getDistanceTo(temp) < 0.5)
        {
            static int dive_start_right = worldModel->getTime();
            if(worldModel->getLastSkill() != SKILL_DIVE_RIGHT)
                dive_start_right = worldModel->getTime();
            if(worldModel->getTime() > dive_start_right + 8)
                return SKILL_STAND;
            return SKILL_DIVE_RIGHT;    
        }
        else return goToTarget(temp);        
    }
    if((worldModel->getUNum() == GOALKEEPER && (ball.getDistanceTo(VecPosition(-15,0,0))) < 4))
    {
        VecPosition temp = VecPosition(-14,0.85,0) + (ball - VecPosition(-15,0,0))*(1/modulus(VecPosition(-15,0,0) - ball));
        if(me.getDistanceTo(temp) < 1)
        {
            static int dive_start_right_2 = worldModel->getTime();
            if(worldModel->getLastSkill() != SKILL_DIVE_RIGHT)
                dive_start_right_2 = worldModel->getTime();
            if(worldModel->getTime() > dive_start_right_2 + 8)
                return SKILL_STAND;
            return SKILL_DIVE_RIGHT;    
        }
        else return goToTarget(temp); 
    }

    if(worldModel->getUNum() == RIGHT_DEF && ball.getDistanceTo(VecPosition(-15,0,0)) < 4)
    {
        VecPosition temp = VecPosition(-13.5,-0.85,0) + (ball - VecPosition(-15,0,0))*(1/modulus(VecPosition(-15,0,0) - ball));
        if(me.getDistanceTo(temp) < 1)
        {
            static int dive_start_left = worldModel->getTime();
            if(worldModel->getLastSkill() != SKILL_DIVE_LEFT)
              dive_start_left = worldModel->getTime();
            if(worldModel->getTime() > dive_start_left + 8)
                return SKILL_STAND;
            return SKILL_DIVE_LEFT;
        }
        else return goToTarget(temp);
    }
    
    if(worldModel->getUNum() == GOALKEEPER && playerClosestToBall == GOALKEEPER)
    {
        return kickBall(KICK_IK,APF(VecPosition(15,0,0)));
    }
    if(worldModel->getUNum() == GOALKEEPER)
    {
        if(ballgoal().getY() < -0.8 || ballgoal().getY() > 0.8)
        {
            return moveToOff();
        }
        else return goToTarget(ballgoal());
    }

    if(worldModel->getUNum() == LEFT_DEF)
    {
        VecPosition temp_target = ballgoal();
        temp_target.setX(-14);
        if(temp_target.getY() < -1.2 || temp_target.getY() > 1.2)
        {
            return moveToOff();
        }
        else return goToTarget(temp_target);
    }
    if(worldModel->getUNum() == RIGHT_DEF)
    {
        VecPosition temp_target = ballgoal();
        temp_target.setX(-13.5);
        if(temp_target.getY() < -1.2 || temp_target.getY() > 1.2)
        {
            return moveToOff();
        }
        else return goToTarget(temp_target);
    }
    /**********sends left & right forwards as 'wingers'*************/
    static int winger_left = LEFT_FORWARD;
    static int winger_right = RIGHT_FORWARD;
    double distance, angle;
    if(worldModel->getUNum() == winger_left)
    {
        if(me.getDistanceTo(VecPosition(9,1,0)) < 0.5)
        {
            getTargetDistanceAndAngle(VecPosition(15,0,0), distance, angle);
            if (abs(angle) > 10) 
            {
                return goToTargetRelative(VecPosition(), angle);
            } 
            else 
            {
            return SKILL_STAND;
            }
        }
        return goToTarget(VecPosition(9,1,0));
    }
    if(worldModel->getUNum() == winger_right)
    {
        if(me.getDistanceTo(VecPosition(8,-1,0)) < 0.5)
        {
            getTargetDistanceAndAngle(VecPosition(15,0,0), distance, angle);
            if (abs(angle) > 10) 
            {
                return goToTargetRelative(VecPosition(), angle);
            } 
            else 
            {
            return SKILL_STAND;
            }
        }
        return goToTarget(VecPosition(8,-1,0));
    }
    /***************************************************************/
    else return moveToOff();
}

VecPosition targpos[11];

SkillType NaoBehavior::moveToOff()
{
    int playerClosestToBall = ourClosest(ball);
    VecPosition target = VecPosition(0,0,0);
    targpos[0] = VecPosition(-14.5,0,0);        //gk
    targpos[1] = VecPosition(-14,0.85,0);       // left
    targpos[2] = VecPosition(-13.5,-0.85,0);    // right
  //  
    // if(worldModel->getUNum() == LEFT_C_DEF)
    // {
    //     target = getposition(oppClosest(VecPosition(15,0,0)));     
    //     if(target.getDistanceTo(VecPosition(-15,0,0)) > 10)
    //     {
    //         return goToTarget(VecPosition(-10,0,0));
    //     }
    //     if(me.getDistanceTo(ball) > 2)
    //     {
    //         target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    //     }
    //     else 
    //     {
    //         target = ball + (worldModel->getWorldObject(WO_BALL)->absVel)*(1/modulus(worldModel->getWorldObject(WO_BALL)->absVel));
    //         target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    //     }
    // }
    
    // if(worldModel->getUNum() == RIGHT_C_DEF)
    // {
    //     target = getposition(oppClosest(getposition(oppClosest(VecPosition(15,0,0)))));
    //     if(target.getDistanceTo(VecPosition(-15,0,0)) > 10)
    //     {
    //         return goToTarget(VecPosition(-8,0,0));
    //     }
    //     if(me.getDistanceTo(ball) > 2)
    //     {
    //         target = collisionAvoidance(true /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    //     }
    //     else 
    //     {
    //         target = ball + (worldModel->getWorldObject(WO_BALL)->absVel)*(1/modulus(worldModel->getWorldObject(WO_BALL)->absVel));
    //         target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    //     }   
    // }

    // if (worldModel->getUNum() == GOALKEEPER)
    // {   
    //     target = targpos[0];
    //     target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    // }
    // else if(worldModel->getUNum() == LEFT_DEF)
    // {
    //     target = targpos[1];
    //     target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    // }
    // else if(worldModel->getUNum() == RIGHT_DEF)
    // {
    //     target = targpos[2];
    //     target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
    // }
    // else if(worldModel->getUNum() != LEFT_C_DEF && worldModel->getUNum() != RIGHT_C_DEF)
    // {
    //     target = ball;
    //     if(worldModel->getUNum() != playerClosestToBall)
    //         {
    //             if(ball.getDistanceTo(getposition(oppClosest(ball))) < ball.getDistanceTo(getposition(ourClosest(ball))))
    //             {
    //                 target = ball + (worldModel->getWorldObject(WO_BALL)->absVel)*(1/modulus(worldModel->getWorldObject(WO_BALL)->absVel));
    //             }
    //             else
    //             {
    //                 VecPosition target_dir = (VecPosition(15,0,0) - ball)*(1/modulus(VecPosition(15,0,0) - ball));
    //                 target  = ball + ((target_dir).rotateAboutZ(15 + 5*worldModel->getUNum()));
    //                 target = collisionAvoidance(true /*teammate*/, false/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);                    
    //             }
    //         }
    //     else 
    //     {
    //         target = ball + (worldModel->getWorldObject(WO_BALL)->absVel)*(1/modulus(worldModel->getWorldObject(WO_BALL)->absVel));
    //     }
    // }
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
        if (worldModel->getUNum() == playerNum) 
        {
            // This is us
            temp = worldModel->getMyPosition();
        } 
        else 
        {
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

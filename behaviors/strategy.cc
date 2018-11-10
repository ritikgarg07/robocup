#include <cmath>
#include "naobehavior.h"
#include "../rvdraw/rvdraw.h"
// #include "scram.h"

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

int NaoBehavior::ourClosest(VecPosition a)
{
    int playerClosest = -1;
    double closestDistance = 10000;
    for(int jj = WO_TEAMMATE1; jj < WO_TEAMMATE1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        int playerNum = jj - WO_TEAMMATE1 + 1;
        if (worldModel->getUNum() == playerNum) 
        {
            temp = worldModel->getMyPosition();
        }
        else 
        {
            WorldObject* teammate = worldModel->getWorldObject( jj );
            if (teammate->validPosition) 
            {
                temp = teammate->pos;
            } 
            else 
            {
                continue;
            }
        }
        temp.setZ(0);

        double distance = temp.getDistanceTo(a);
        if (distance < closestDistance) 
        {
            playerClosest = playerNum;
            closestDistance = distance;
        }
    }   
    return playerClosest; 
}

int NaoBehavior::oppClosest(VecPosition a)
{
    int playerClosest = -1;
    double closestDistance = 10000;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1+NUM_AGENTS; ++jj) 
    {
        VecPosition temp;
        int playerNum = jj - WO_OPPONENT1 + 1;

        WorldObject* teammate = worldModel->getWorldObject( jj );
        if (teammate->validPosition) 
        {
            temp = teammate->pos;
        } 
        else 
        {
            continue;
        }
        temp.setZ(0);

        double distance = temp.getDistanceTo(a);
        if (distance < closestDistance) 
        {
            playerClosest = playerNum;
            closestDistance = distance;
        }
    }   
    return playerClosest; 
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
    int playerClosestToBall = ourClosest(ball);
    double closestDistance = ball.getDistanceTo(getposition(playerClosestToBall));

    int opponent = oppClosest(ball);
    double closestDistanceOpp = ball.getDistanceTo(getposition(opponent));

    if(closestDistance <= 0.6 && closestDistanceOpp >= 1 && !(worldModel->getFallenTeammate(playerClosestToBall)))
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

VecPosition NaoBehavior::ballgoal()
{
    VecPosition goal_ball = (0,0,0);
    WorldObject *ball = worldModel->getWorldObject(WO_BALL);
    double pos_x = ball->pos.getX();
    double pos_y = ball->pos.getY();

    double vel_x = ball->absVel.getX();
    double vel_y = ball->absVel.getY();
    double time_for_goal;

    if(vel_x < 0.1)
    {
        time_for_goal = 0;
    }

    else time_for_goal = (-15 - pos_x)/vel_x;
    goal_ball.setX(-14.5);
    goal_ball.setY(pos_y + (vel_y*time_for_goal));
    goal_ball.setZ(0);

    cout << "pos_x" << pos_x << "\n";
    cout << "pos_y" << pos_y << "\n";
    cout << "vel_x" << vel_x << "\n";
    cout << "vel_y" << vel_y << "\n";
    cout << "time: " << time_for_goal << "\n";
    cout << goal_ball << "\n";
    if(goal_ball.getY() > 1.3)
    {
        goal_ball.setY(1.3);
    }
    else if(goal_ball.getY() < -1.3)
    {
        goal_ball.setY(1.3);
    }
    return goal_ball;
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
    // return testing();
    
    static double startTime = worldModel->getTime();    
    if(((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)))
    {
        startTime = worldModel->getTime();
    }
    if (((worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_RIGHT)) || worldModel->getTime()-startTime < 5)
    {
            return kickoff(worldModel->getTime() - startTime);
    }
    else if ((worldModel->getPlayMode() == PM_KICK_OFF_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_OFF_LEFT && worldModel->getSide() == SIDE_RIGHT))
    {
        if(worldModel->getUNum() == LEFT_FORWARD && ball.getY() > 1)
        {
            return goToTarget(VecPosition(-0.5,ball.getY(),0));
        }
        else if(worldModel->getUNum() == RIGHT_FORWARD && ball.getY() < -1)
        {
            return goToTarget(VecPosition(-0.5,ball.getY(),0));
        }
        else if(worldModel->getUNum() == CENTRE_FORWARD)
        {
            return goToTarget(VecPosition(-2,0,0));
        }
        return SKILL_STAND;
    }
    else if((worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {

        return kickin();
    }

    else if((worldModel->getPlayMode() == PM_KICK_IN_RIGHT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_KICK_IN_LEFT && worldModel->getSide() == SIDE_RIGHT))
        return kickin_opp();
    else if((worldModel->getPlayMode() == PM_GOAL_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_GOAL_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == GOALKEEPER && me.getDistanceTo(VecPosition(-15,0,0)) < 2)
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }
        else if(worldModel->getUNum() == RIGHT_DEF && me.getDistanceTo(VecPosition(-15,0,0)) < 2)
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }
        else if(worldModel->getUNum() == LEFT_DEF)
        {
            return kickBall(KICK_LONG, VecPosition(0,0,0));
        }        
        else if(worldModel->getUNum() == CENTRE_MID)
        {
            return goToTarget(VecPosition(0,0,0));
        }
        else if(worldModel->getUNum() != RIGHT_DEF || worldModel->getUNum() != LEFT_DEF)
        {
            return stay();               // needs to be changed so that players don't move to the ball and kick it
        }
        else return moveToOff();
    }
    else if((worldModel->getPlayMode() == PM_CORNER_KICK_LEFT && worldModel->getSide() == SIDE_LEFT) || (worldModel->getPlayMode() == PM_CORNER_KICK_RIGHT && worldModel->getSide() == SIDE_RIGHT))
    {
        if (worldModel->getUNum() == LEFT_FORWARD)
        {
            return kickBall(KICK_LONG, VecPosition(12,0,0));          // change vecposition to someplace else?
        }
        else if(worldModel->getUNum() == CENTRE_FORWARD)
            return goToTarget(VecPosition(13,0,0));
        else return moveToOff();
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
    else if(ball.getDistanceTo(VecPosition(-15,0,0)) < 15)   // change 5
    {
        return defenseplay();
    }
    return attackplay();
}

SkillType NaoBehavior::testing()
{ 
    if(worldModel->getUNum() == GOALKEEPER)
    {
        return goToTarget(ballgoal());
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
            if(opponentcount(getposition(playerClosestToBall),2) < 1)
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
            if(opponentcount(getposition(playerClosestToBall),1) == 0)
            {
                return kickBall(KICK_LONG,shoot_goal);
            }

            //passing to wingers
            else if(opponentcount(getposition(winger),2) < 2 && (ball.getDistanceTo(VecPosition(15,0,0)) > 7) && opponentcount(getposition(playerClosestToBall),2) > 1)

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
            else return kickBall(KICK_DRIBBLE,APF(shoot_goal));
        }

        // dribble to goal     
        else return kickBall(KICK_DRIBBLE, APF(VecPosition(16,0,0))); 
    }
    else 
    { 
        return moveToOff();    
    }
}


SkillType NaoBehavior::defenseplay()
{ 
    int playerClosestToBall = ourClosest(ball);
    
    if(worldModel->getUNum() == playerClosestToBall)
    {
        if(opponentcount(ball,2) < 2)
        {
            return kickBall(KICK_LONG, VecPosition(15,0,0));
        }
        else if(opponentcount(ball,1) > 0)
        {
            return kickBall(KICK_IK,VecPosition(16,0,0));
        }
        else return kickBall(KICK_DRIBBLE, VecPosition(16,0,0));
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 9 && worldModel->getUNum() == LEFT_DEF)
    {
        return kickBall(KICK_IK,VecPosition(15,0,0));
    }
    if(ball.getDistanceTo(VecPosition(-15,0,0)) < 7 && worldModel->getUNum() == RIGHT_DEF)
    {
        return kickBall(KICK_IK,VecPosition(15,0,0));
    }
    /*if(ball.getDistanceTo(VecPosition(-15,0,0)) < 5 && (worldModel->getUNum() == GOALKEEPER) && opponentcount(VecPosition(-15,0,0),2) < 2)
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
    }*/
    if(worldModel->getUNum() == GOALKEEPER)
    {
        goToTarget(ballgoal());
    }
    else return moveToOff();
}

VecPosition targpos[11];

SkillType NaoBehavior::moveToOff()
{
    int playerClosestToBall = ourClosest(ball);
    VecPosition target = VecPosition(0,0,0);
    targpos[0] = VecPosition(-14.5,0,0);
    targpos[1] = VecPosition(-14.5,0.85,0);
    targpos[2] = VecPosition(-14.5,-0.85,0);
    
    if(worldModel->getUNum() == LEFT_C_DEF)
    {
        targpos[3] = getposition(oppClosest(VecPosition(15,0,0)));                 
        target = targpos[3];
        if(target.getDistanceTo(VecPosition(-15,0,0)) > 10)
        {
            return goToTarget(VecPosition(-10,0,0));
        }
        if(me.getDistanceTo(ball) > 2)
        {
            target = collisionAvoidance(false /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
        }
        else target = ball;
    }
    if(worldModel->getUNum() == RIGHT_C_DEF)
    {
        target = getposition(oppClosest(getposition(oppClosest(VecPosition(15,0,0)))));
        if(target.getDistanceTo(VecPosition(-15,0,0)) > 10)
        {
            return goToTarget(VecPosition(-8,0,0));
        }
        if(me.getDistanceTo(ball) > 2)
        {
            target = collisionAvoidance(false /*teammate*/, true/*opponent*/, false/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/, target, true/*keepDistance*/);
        }
        else target = ball;   
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
    else if(worldModel->getUNum() != LEFT_C_DEF && worldModel->getUNum() != RIGHT_C_DEF)
    {
        target = ball;
        if(worldModel->getUNum() != playerClosestToBall)
            {
                if(ball.getDistanceTo(oppClosest(ball)) < ball.getDistanceTo(ourClosest(ball)))
                {
                    target = ball;
                }
                else
                {
                    VecPosition target_dir = (VecPosition(15,0,0) - ball)*(1/modulus(VecPosition(15,0,0) - ball));
                    target  = ball + ((target_dir).rotateAboutZ(15 + 5*worldModel->getUNum()));                    
                }
            }
        else 
        {
            target = ball;
        }
    }
    return goToTarget(target);
}

VecPosition NaoBehavior::APF(VecPosition target)
{
    double katt = 5;                                // placeholder value
    double krep = 10;
    double influence = 5;
    
    // VecPosition obstacle = VecPosition(0,0,0);
    
    VecPosition Fatt = VecPosition(0,0,0);
    VecPosition Frep = VecPosition(0,0,0);
    VecPosition Frep2 = VecPosition(0,0,0);


    if(me.getDistanceTo(target) > katt)
    {
        Fatt = (worldModel->getMyPosition() - target)*(-1*katt);
    }
    else Fatt = (worldModel->getMyPosition() - target)*((1/modulus(worldModel->getMyPosition() - target))*(-8*katt/me.getDistanceTo(target)));
    VecPosition Fnet = Fatt;
    for(int jj = WO_OPPONENT1; jj < WO_OPPONENT1 + NUM_AGENTS; ++jj)
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
        double distanceToplayer = me.getDistanceTo(temp);
        if (distanceToplayer < influence) 
        {
            Frep2 = (worldModel->getMyPosition() - temp)*(krep/(me.getDistanceTo(temp)))*(1/distanceToplayer - 1/influence);
            Frep.setX((worldModel->getMyPosition() - temp).getY()*(-1));
            Frep.setY((worldModel->getMyPosition() - temp).getX());
            if(Frep.getX() < 0)             // WORKS ONLY IF TARGET IS GOAL! (VERY SPECIFIC) TODO:IMPROVE
            {       
                Frep.setX(Frep.getX()*(-1));
                Frep.setY(Frep.getY()*(-1));
            }
            Frep = Frep*(krep/me.getDistanceTo(temp));
            Fnet += (Frep + Frep2);
        }
        else 
            continue;
    }    
    VecPosition moveto = worldModel->getMyPosition() + Fnet;
    return moveto;
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



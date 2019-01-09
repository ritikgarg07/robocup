#ifndef FUNCTIONS_H
#define FUNCTIONS_H

//returns our closest player to a position
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
            if (teammate->validPosition && worldModel->getFallenTeammate(jj) == 0)  
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

//return closest opponent to a position
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

//returns position of a given player
VecPosition NaoBehavior::getposition(int player_number)
{
    WorldObject* player = worldModel->getWorldObject(player_number);
    VecPosition temp = player->pos;
    return temp;
}


//returns the number of opponents in a circle of radius distance, about a distance d

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


bool NaoBehavior::posession()    // player closest distance is 0.4 , closest opponent is 0.8 and closest player is not fallen
{
    int playerClosestToBall = ourClosest(ball);
    double closestDistance = ball.getDistanceTo(getposition(playerClosestToBall));

    int opponent = oppClosest(ball);
    double closestDistanceOpp = ball.getDistanceTo(getposition(opponent));

    if(closestDistance <= 0.5 && closestDistanceOpp >= 1)
    {
        return true;
    }
    else return false;
}

//gives the modulus of a given function
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

    return goal_ball;
}


VecPosition NaoBehavior::APF(VecPosition target)
{
    double katt = 5;                                // placeholder value
    double krep = 10;
    double influence = 5;
    
    VecPosition Fatt = VecPosition(0,0,0);
    VecPosition Fatt2 = VecPosition(0,0,0);
    VecPosition Frep = VecPosition(0,0,0);
    VecPosition Frep2 = VecPosition(0,0,0);


    if(me.getDistanceTo(target) > katt)
    {
        Fatt = (worldModel->getMyPosition() - target)*(-1*katt);
    }
    else Fatt = (worldModel->getMyPosition() - target)*((1/modulus(worldModel->getMyPosition() - target))*(-8*katt/me.getDistanceTo(target)));
    Fatt2 = VecPosition(0.4,0,0).rotateAboutZ(worldModel->getMyAngDeg());
    // change vecposition's x to change strength
    VecPosition Fnet = Fatt + Fatt2;


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
            // repulsive force
            Frep2 = (worldModel->getMyPosition() - temp)*((krep/2)/(me.getDistanceTo(temp)))*(1/distanceToplayer - 1/influence);
            
            // Frep => circular force
            Frep.setX((worldModel->getMyPosition() - temp).getY()*(-1));
            Frep.setY((worldModel->getMyPosition() - temp).getX());
            if(Frep.getX() < 0)             // WORKS ONLY IF TARGET IS GOAL! (VERY SPECIFIC) TODO:IMPROVE
            {       
                Frep.setX(Frep.getX()*(-1));
                Frep.setY(Frep.getY()*(-1));
            }
            Frep = Frep*(krep/me.getDistanceTo(temp));
            Fnet += (Frep + Frep2);
            // basic avoidance to local minima
        }
        else 
            continue;
    }    
    if (modulus(Fnet) < 0.4)
    {
   		Fnet = VecPosition(1,0,0);
   	}   
    VecPosition moveto = worldModel->getMyPosition() + Fnet;
    return moveto;
}


double NaoBehavior::cost_cal(VecPosition location)
{
    double cost = 0;

    enum KICK
    {
        k_IK = 0,
        k_FORWARD = 1,
        k_LONG = 2,
    };

    double time_long = 2.0;     // time taken to execute the kicks                             
    double time_short = 3.5;
    double time_ik = 0.8;
    double distance_location = ball.getDistanceTo(location); 

    double rotation_time = 3.9/360;                     // per degree
    SIM::AngDeg orient_closest_opp;                 // orientation of the closest opp to the kick position
    SIM::AngDeg orient_closest_our;
    SIM::AngDeg orient_closest_opp_ball;         //orientation of closest bot to our present position
    orient_closest_opp = (location - getposition(oppClosest(location))).getTheta() - (worldModel->getWorldObject(oppClosest(location))->orien);
    orient_closest_our = (location - getposition(ourClosest(location))).getTheta() - (worldModel->getWorldObject(ourClosest(location))->orien);
    orient_closest_opp_ball = (location - getposition(oppClosest(ball))).getTheta() - (worldModel->getWorldObject(oppClosest(ball))->orien);
    double opp_speed;

    /*************All the variables used for evaluating********************************/
    double time;
    double ourplayers;          // number of our players within a certain distance of the target pos of ball 
    double oppplayers = opponentcount(location,2);    // 2 is arbitrary
    double dist_us_target = getposition(ourClosest(location)).getDistanceTo(location);
     // closest teammate's distance to approx target location    
    double dist_opp_target = getposition(oppClosest(location)).getDistanceTo(location);                         
    double dist_our_goal = VecPosition(-15,0,0).getDistanceTo(location);                            
    // distance of target location from our goal (where opp scores)
    double distance_opp_goal = VecPosition(15,0,0).getDistanceTo(location);                       
    
    // include later
    double speed_us;
    double speed_opp;                               // mod of speed of opp
    double distance_us_present = ball.getDistanceTo(ourClosest(ball));                             
    double distance_opp_present = ball.getDistanceTo(oppClosest(ball));                    
    double time_opp = dist_opp_target/opp_speed + orient_closest_opp*rotation_time;
    /**************************************************************/


    int kick_type = 0;
    if(distance_location >= 9.5)
    {
        kick_type = k_LONG;
        time = time_long;
    }        
    else if(distance_location > 2.5 && distance_location < 9.5)
    {
        kick_type = k_FORWARD;
        time = time_short;
    }
    else if(distance_location <= 2.5)
    {
        kick_type = k_IK;
        time = time_ik;
    }

    // add appropriate scaling factors
    cost = time + ((distance_opp_goal - dist_our_goal)/10) + oppplayers + dist_opp_target - dist_us_target;

    return cost;
}



VecPosition NaoBehavior::kicklocation()
{
    VecPosition centre = ball;
    VecPosition rotater = VecPosition(1,0,0);
    VecPosition fav = VecPosition(0,0,0);

    double distance_long = 14;
    double distance_short = 5;
    double distance_ik = 1.6;


    VecPosition location[24];
    
    // defining positions on field which will be tested
    for(int ii =  0; ii < 8; ii++)
    {
        location[ii] = centre + (rotater*distance_long).rotateAboutZ((360/8)*ii);
    }

    for(int ii =  8; ii < 16; ii++)
    {
        location[ii] = centre + (rotater*distance_short).rotateAboutZ((360/8)*(ii-8));
    }

    for(int ii =  16; ii < 24; ii++)
    {
        location[ii] = centre + (rotater*distance_ik).rotateAboutZ((360/8)*(ii-16));
    }
    //
    double cost = 100000;
    double least_cost = 0;
    for(int jj = 0; jj < 24; jj++)
    {
        cost = cost_cal(location[jj]);
        if(cost < least_cost)
        {
            least_cost = cost;
            fav = location[jj];
        }
    }
    return fav;
}



#endif

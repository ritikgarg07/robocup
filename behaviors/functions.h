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
        }
        else 
            continue;
    }    
    VecPosition moveto = worldModel->getMyPosition() + Fnet;
    return moveto;
}


#endif

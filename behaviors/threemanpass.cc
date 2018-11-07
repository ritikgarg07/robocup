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

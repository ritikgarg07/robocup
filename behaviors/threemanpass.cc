SkillType NaoBehavior::threemanpass(int counter) 
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


    VecPosition kick_next = VecPosition(0,0,0);
    VecPosition move_next = VecPosition(0,0,0);

    if (worldModel->getUNum() == PLAYER_CENTRE)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD, VecPosition((3,-3,0)));
                        }
                    else
                        {
                            //return goToTarget(VecPosition(8,-2,0));    
                            return goToTarget(collisionAvoidance(true /*teammate*/, false/*opponent*/, true/*ball*/, 1/*proximity thresh*/, .5/*collision thresh*/,VecPosition(8,-2,0), true/*keepDistance*/));
                        }
                    break;       
        }
    }


    else if (worldModel->getUNum() == PLAYER_RIGHT)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD,VecPosition((6,2,0)));
                        }
                    else
                        {
                            return goToTarget(VecPosition(3,-3,0));    
                        }
                        break;
        }
    }

    else if (worldModel->getUNum() == PLAYER_LEFT)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD, VecPosition(8,-2,0));
                        }
                    else
                        {
                            return goToTarget(VecPosition(6,2,0));    
                        }           
                    break;           
        }
    }

    else 
    {
        // default case
        return SKILL_STAND;
    }
}


/*
    if (worldModel->getUNum() == PLAYER_CENTRE)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD, VecPosition((3,-3,0)));
                        }
                    else
                        {
                            return goToTarget(VecPosition(8,-2,0));    
                        }
                    break;
            default: return goToTarget(VecPosition(-5,5,0));    
        }
    }


    else if (worldModel->getUNum() == PLAYER_RIGHT)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD,VecPosition((6,2,0)));
                        }
                    else
                        {
                            return goToTarget(VecPosition(3,-3,0));    
                        }
                    break;
            default: return goToTarget(VecPosition(5,5,0));
        }
    }

    else if (worldModel->getUNum() == PLAYER_LEFT)
    {
        switch(counter)
        {
            case 0: if(worldModel->getUNum() == playerClosestToBall)
                        {
                            return kickBall(KICK_FORWARD, VecPosition(8,-2,0));
                        }
                    else
                        {
                            return goToTarget(VecPosition(6,2,0));    
                        }           
                    break;
            case 1: return SKILL_STAND;  
            default: return goToTarget(VecPosition(10,0,0));
        }
    }

    else 
    {
        // default case
        return SKILL_STAND;
    }
    */
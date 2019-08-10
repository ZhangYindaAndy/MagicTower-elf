#ifndef TMONSTER_H
#define TMONSTER_H

#include"string"

using namespace std;

class TMonster
{
    public:
        string name;
        int hp,atk,def,money,star,level,max_level;
        TMonster() {}
        virtual ~TMonster() {}
        void Change(int ATK,int DEF,int scarecrow,int huge)
        {
            if (def>=ATK) cost=1<<30;
            else
            if (DEF>=atk) cost=0;
            else
            {
                int rATK=(huge&&ATK>atk)?ATK*2:ATK;
                int t=(hp-1)/(rATK-def)-scarecrow;
                t=t<0?0:t;
                cost=t*(atk-DEF);
            }
        }
        int Cost()
        {
            return cost;
        }
    protected:
    private:
        int cost;
};

#endif // TMONSTER_H

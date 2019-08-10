#ifndef MAGICTOWER_H
#define MAGICTOWER_H

#include"string"
#include"cstdio"
#include"iostream"
#include"fstream"
#include"conio.h"
#include"windows.h"
#include"cstdlib"
#include"TMonster.h"
#include"cstring"

#define EASY 0.7
#define NORMAL 0.85
#define HARD 1
#define KEY_UP    0xE048
#define KEY_DOWN  0xE050
#define KEY_LEFT  0xE04B
#define KEY_RIGHT 0xE04D
#define KEY_ESC   0x001B
#define KEY_1 '1'
#define KEY_2 '2'
#define KEY_3 '3'
#define KEY_4 '4'
#define KEY_5 '5'
#define KEY_6 '6'
#define KEY_7 '7'
#define KEY_8 '8'
#define KEY_9 '9'
#define KEY_ENTER 0x0D
//

//

using namespace std;

struct TMap
{
    int n,m,h;
    int map[100][20][20];
    int up_x[100];
    int up_y[100];
    int down_x[100];
    int down_y[100];
};

struct TCharacter
{
    int x,y,h;
    int hp,atk,def,money;
    int yellow,blue,red;
    int highest,level,star;
};


class Magictower
{
    public:
        bool boss;
        Magictower(string&);
        virtual ~Magictower();
        bool waitmessage();
        void print();
        int getkey();
    protected:
    private:
        bool handbook,lift,kettle,scarecrow,huge,eye,oldkey[100],magicbook,feather;
        int exchange,levelup,times,earthquark,exchangekey,godbless,pickaxe;
        TMap Map;
        TCharacter Warrior;
        TMonster Monsters[100];
        int num_monsters;
        bool wall(int,int,int);
        int fight(int);
        void get(int);
        bool door(int,int,int);
        void up_or_down(int&,int&,int&);
        int block(int);
        void update();
};

#endif // MAGICTOWER_H

#include "Magictower.h"
#include "fstream"

Magictower::Magictower(string& save_name)
{
    ifstream imap,imonsters,icharacter;
    string smap="save\\"+save_name+"\\map.txt";
    imap.open((char*)smap.data(),ios::in);
    string smonsters="save\\"+save_name+"\\monsters.txt";
    imonsters.open((char*)smonsters.data(),ios::in);
    string scharacter="save\\"+save_name+"\\character.txt";
    icharacter.open((char*)scharacter.data(),ios::in);
    imap>>Map.h>>Map.n>>Map.m;
    for (int k=1;k<=Map.h;++k)
    {
        for (int i=0;i<Map.n;++i)
            for (int j=0;j<Map.m;++j)
                imap>>Map.map[k][i][j];
        imap>>Map.down_x[k]>>Map.down_y[k]>>Map.up_x[k]>>Map.up_y[k];
    }
    imonsters>>num_monsters;
    for (int i=0;i<num_monsters;++i)
        imonsters>>Monsters[i].name>>Monsters[i].atk>>Monsters[i].def>>Monsters[i].hp>>Monsters[i].money>>Monsters[i].level>>Monsters[i].star>>Monsters[i].max_level;
    icharacter>>Warrior.atk>>Warrior.def>>Warrior.hp>>Warrior.level>>Warrior.h>>Warrior.x>>Warrior.y;
    Warrior.star=Warrior.yellow=Warrior.blue=Warrior.red=Warrior.money=0;
    Warrior.highest=1;
    boss=true;
    handbook=lift=kettle=scarecrow=huge=eye=magicbook=feather=false;
    levelup=exchange=earthquark=exchangekey=pickaxe=0;
    times=1;
    godbless=-1;
    for (int i=0;i<=Map.h;++i) oldkey[i]=false;
    update();
}

Magictower::~Magictower() {}

int Magictower::getkey()
{
    int nkey=getch(),nk=0;
    if(nkey>=128||nkey==0)nk=getch();
    return nk>0?nkey*256+nk:nkey;
}

bool Magictower::waitmessage()
{
    int nkey=getkey();
    if (nkey==KEY_ESC)
    {
        system("cls");
        printf("\n是否要结束游戏？\n按回车结束游戏，按ESC返回游戏\n");
        int nkey;
        do
        {
            nkey=getkey();
        }
        while (nkey!=KEY_ENTER&&nkey!=KEY_ESC);
        if (nkey==KEY_ENTER)
            return false;
        else
            return true;
    }
    else
    {
        int x=Warrior.x,y=Warrior.y,h=Warrior.h;
        bool tool_used=false;
        switch (nkey)
        {
        case KEY_UP:
            x--;
            break;
        case KEY_DOWN:
            x++;
            break;
        case KEY_LEFT:
            y--;
            break;
        case KEY_RIGHT:
            y++;
            break;
        case KEY_1:
            tool_used=true;
            if (handbook)
            {
                bool visit[100];
                memset(visit,false,sizeof(visit));
                for (int i=0;i<Map.n;++i)
                    for (int j=0;j<Map.m;++j)
                        if (Map.map[Warrior.h][i][j]>=100)
                            visit[Map.map[Warrior.h][i][j]-100]=true;
                system("cls");
                for (int i=0;i<num_monsters;++i)
                    if (visit[i])
                    {
                        printf("%12s  生命值：%6d    击杀获得金币：%d\n",(char*)Monsters[i].name.data(),Monsters[i].hp,Monsters[i].money*(1+kettle));
                        if (fight(i)==1<<30)
                            printf("              攻击力：%6d    击杀损失生命：%s\n",Monsters[i].atk,"????");
                        else
                            printf("              攻击力：%6d    击杀损失生命：%d\n",Monsters[i].atk,fight(i));
                        printf("              防御力：%6d    击杀获得经验：%d\n\n",Monsters[i].def,Monsters[i].star);
                    }
                printf("\n请按ESC返回：\n");
                int t;
                do
                {
                    t=getkey();
                }
                while (t!=KEY_ESC);
                system("cls");
                print();
            }
            break;
        case KEY_2:
            tool_used=true;
            printf("\n正在进行跳跃~~~\n\n请按ESC返回");
            if (lift)
            {
                int t;
                do
                {
                    t=getkey();
                    if (t==KEY_UP&&Warrior.h<Warrior.highest)
                    {
                        Warrior.h++;
                        Warrior.x=Map.down_x[Warrior.h];
                        Warrior.y=Map.down_y[Warrior.h];
                    }
                    if (t==KEY_DOWN&&Warrior.h>1)
                    {
                        Warrior.h--;
                        Warrior.x=Map.up_x[Warrior.h];
                        Warrior.y=Map.up_y[Warrior.h];
                    }
                    system("cls");
                    print();
                    printf("\n正在进行跳跃~~~\n\n请按ESC返回");
                }
                while (t!=KEY_ESC);
            }
            break;
        case KEY_3:
            if (exchange>0)
            {
                exchange--;
                tool_used=true;
                swap(Warrior.atk,Warrior.def);
                update();
            }
            break;
        case KEY_4:
            if (levelup>0)
            {
                levelup--;
                tool_used=true;
                Warrior.level++;
                Warrior.atk+=100;
                Warrior.def+=100;
                update();
            }
            break;
        case KEY_5:
            if (earthquark>0)
            {
                earthquark--;
                tool_used=true;
                for (int i=0;i<Map.n;++i)
                    for (int j=0;j<Map.m;++j)
                        if (Map.map[Warrior.h][i][j]==1||Map.map[Warrior.h][i][j]==7)
                            Map.map[Warrior.h][i][j]=0;
            }
            break;
        case KEY_6:
            if (exchangekey>0)
            {
                tool_used=true;
                int t;
                do
                {
                    system("cls");
                    printf("千年秤：\n\n");
                    printf("1、用7把黄钥匙换3把蓝钥匙\n");
                    printf("2、用3把蓝钥匙换7把黄钥匙\n");
                    printf("3、用7把黄钥匙换1把红钥匙\n");
                    printf("4、用1把红钥匙换7把黄钥匙\n");
                    printf("5、用3把蓝钥匙换1把红钥匙\n");
                    printf("6、用1把红钥匙换3把蓝钥匙\n");
                    printf("\n剩余使用次数：%d\n",exchangekey);
                    printf("\n黄钥匙：%d\n蓝钥匙：%d\n红钥匙：%d\n",Warrior.yellow,Warrior.blue,Warrior.red);
                    printf("\n按ESC返回\n");
                    t=getkey();
                    switch(t)
                    {
                    case KEY_1:
                        if (Warrior.yellow>=7)
                        {
                            Warrior.yellow-=7;
                            Warrior.blue+=3;
                            exchangekey--;
                        }
                        break;
                    case KEY_2:
                        if (Warrior.blue>=3)
                        {
                            Warrior.blue-=3;
                            Warrior.yellow+=7;
                            exchangekey--;
                        }
                        break;
                    case KEY_3:
                        if (Warrior.yellow>=7)
                        {
                            Warrior.yellow-=7;
                            Warrior.red++;
                            exchangekey--;
                        }
                        break;
                    case KEY_4:
                        if (Warrior.red>0)
                        {
                            Warrior.red--;
                            Warrior.yellow+=7;
                            exchangekey--;
                        }
                        break;
                    case KEY_5:
                        if (Warrior.blue>=3)
                        {
                            Warrior.blue-=3;
                            Warrior.red++;
                            exchangekey--;
                        }
                        break;
                    case KEY_6:
                        if (Warrior.red>0)
                        {
                            Warrior.red--;
                            Warrior.blue+=3;
                            exchangekey--;
                        }
                        break;
                    }
                }
                while (t!=KEY_ESC);
                system("cls");
                print();
            }
            break;
        case KEY_7:
            tool_used=true;
            if (feather)
            {
                for (int i=-1;i<=1;++i)
                    for (int j=-1;j<=1;++j)
                    {
                        int x=Warrior.x+i;
                        int y=Warrior.y+j;
                        if (x>=0&&x<Map.n&&y>=0&&y<Map.m&&Map.map[Warrior.h][x][y]==2)
                            Map.map[Warrior.h][x][y]=0;
                    }
            }
            break;
        case KEY_8:
            tool_used=true;
            if (oldkey[h])
            {
                printf("\n请按方向键选择想要开启的与你相邻的门\n\n按ESC返回");
                int t;
                do
                {
                    t=getkey();
                    switch(t)
                    {
                    case KEY_UP:
                        if (x>0&&Map.map[h][x-1][y]>=3&&Map.map[h][x-1][y]<=5)
                        {
                            Map.map[h][x-1][y]=0;
                            oldkey[h]=false;
                        }
                        break;
                    case KEY_DOWN:
                        if (x<Map.n-1&&Map.map[h][x+1][y]>=3&&Map.map[h][x+1][y]<=5)
                        {
                            Map.map[h][x+1][y]=0;
                            oldkey[h]=false;
                        }
                        break;
                    case KEY_LEFT:
                        if (y>0&&Map.map[h][x][y-1]>=3&&Map.map[h][x][y-1]<=5)
                        {
                            Map.map[h][x][y-1]=0;
                            oldkey[h]=false;
                        }
                        break;
                    case KEY_RIGHT:
                        if (y<Map.m-1&&Map.map[h][x][y+1]>=3&&Map.map[h][x][y+1]<=5)
                        {
                            Map.map[h][x][y+1]=0;
                            oldkey[h]=false;
                        }
                        break;
                    }
                }
                while (t!=KEY_ESC&&oldkey[h]);
            }
            break;
        case KEY_9:
            tool_used=true;
            if (pickaxe)
            {
                printf("\n请按方向键选择想要破坏的与你相邻的墙\n\n按ESC返回");
                int t;
                do
                {
                    t=getkey();
                    switch(t)
                    {
                    case KEY_UP:
                        if (x>0&&(Map.map[h][x-1][y]==1||Map.map[h][x-1][y]==7))
                        {
                            Map.map[h][x-1][y]=0;
                            pickaxe--;
                        }
                        break;
                    case KEY_DOWN:
                        if (x<Map.n-1&&(Map.map[h][x+1][y]==1||Map.map[h][x+1][y]==7))
                        {
                            Map.map[h][x+1][y]=0;
                            pickaxe--;
                        }
                        break;
                    case KEY_LEFT:
                        if (y>0&&(Map.map[h][x][y-1]==1||Map.map[h][x][y-1]==7))
                        {
                            Map.map[h][x][y-1]=0;
                            pickaxe--;
                        }
                        break;
                    case KEY_RIGHT:
                        if (y<Map.m-1&&(Map.map[h][x][y+1]==1||Map.map[h][x][y+1]==7))
                        {
                            Map.map[h][x][y+1]=0;
                            pickaxe--;
                        }
                        break;
                    }
                    system("cls");
                    print();
                    printf("\n请按方向键选择想要破坏的与你相邻的墙\n\n按ESC返回");
                }
                while (t!=KEY_ESC&&pickaxe);
            }
            break;
        }
        if (!tool_used)
        {
            if (x<0||x>=Map.n||y<0||y>=Map.m||wall(h,x,y))
            {
                x=Warrior.x;
                y=Warrior.y;
                h=Warrior.h;
            }
            else
            {
                int t=Map.map[h][x][y];
                if (t==7)
                    Map.map[h][x][y]=0;
                else
                if (t==8||t==9)
                    up_or_down(h,x,y);
                else
                if ((t>=10&&t<=29)||(t>=50&&t<100))
                {
                    get(t);
                    update();
                    Map.map[h][x][y]=0;
                }
                else
                if (t>=30&&t<=32)
                {
                    get(t);
                    update();
                    x=Warrior.x;
                    y=Warrior.y;
                    h=Warrior.h;
                }
                else
                if (t>=100)
                {
                    if (godbless>=0)
                    {
                        godbless++;
                        Warrior.hp+=(godbless/5)*150;
                        godbless%=5;
                    }
                    Warrior.hp-=fight(t-100);
                    Warrior.star+=Monsters[t-100].star*(1+magicbook);
                    if (Warrior.star>=((Warrior.level/4)+1)*10&&Warrior.level<12)
                    {
                        Warrior.star-=((Warrior.level/4)+1)*10;
                        Warrior.level++;
                        Warrior.atk+=50;
                        Warrior.def+=50;
                        update();
                    }
                    Warrior.money+=Monsters[t-100].money*(1+(int)kettle);
                    Map.map[h][x][y]=0;
                }
            }
            //if (Map.map[Warrior.h][Warrior.x][Warrior.y]!=8&&Map.map[Warrior.h][Warrior.x][Warrior.y]!=9) Map.map[Warrior.h][Warrior.x][Warrior.y]=0;
            Warrior.x=x;
            Warrior.y=y;
            Warrior.h=h;
        }
    }
    return true;
}

void Magictower::print()
{
    printf("\n    Floor %d\n\n",Warrior.h);
    for (int i=0;i<Map.n;++i)
    {
        for (int j=0;j<Map.m;++j)
            if (i==Warrior.x&&j==Warrior.y)
                printf("%4s",(char*)"勇士");
            else
            if (Map.map[Warrior.h][i][j]==7)
                printf("%4d",eye?7:1);
            else
            if (Map.map[Warrior.h][i][j]==8)
                printf("%4s",(char*)"上楼");
            else
            if (Map.map[Warrior.h][i][j]==9)
                printf("%4s",(char*)"下楼");
            else
                printf("%4d",Map.map[Warrior.h][i][j]);
        printf("\n");
    }
    printf("\n勇士\n\n星级：%d\n星数：%d\n生命值：%d\n攻击力：%d\n防御力：%d\n金钱：%d\n\n",Warrior.level,Warrior.star,Warrior.hp,Warrior.atk,Warrior.def,Warrior.money);
    printf("拥有物品：\n");
    printf("黄钥匙：%d\n蓝钥匙：%d\n红钥匙：%d\n",Warrior.yellow,Warrior.blue,Warrior.red);
    if (handbook) printf("千年智慧轮（按1使用）\n");
    if (lift) printf("千年锡杖（按2使用）\n");
    if (exchange>0) printf("右手持盾左手持剑：%d（按3使用）\n",exchange);
    if (levelup>0) printf("等级上升！：%d（按4使用）\n",levelup);
    if (earthquark>0) printf("地震：%d（按5使用）\n",earthquark);
    if (exchangekey>0) printf("千年秤：%d（按6使用）\n",exchangekey);
    if (feather) printf("妖鸟的羽毛扫（按7使用）\n");
    if (oldkey[0]) printf("千年钥匙（按8使用）\n");
    if (pickaxe) printf("迷宫变换：%d（按9使用）\n",pickaxe);
    if (kettle) printf("贪欲之壶\n");
    if (magicbook) printf("秘术之书\n");
    if (scarecrow) printf("废铁稻草人\n");
    if (huge) printf("巨大化\n");
    if (eye) printf("千年眼\n");
    if (godbless>=0) printf("神的恩惠\n");
}

bool Magictower::wall(int h,int x,int y)
{
    if (Map.map[h][x][y]==1||Map.map[h][x][y]==2||Map.map[h][x][y]==6) return true;
    else if ((Map.map[h][x][y]>=100)&&fight(Map.map[h][x][y]-100)>=Warrior.hp) return true;
    else if ((Map.map[h][x][y]>=3)&&(Map.map[h][x][y]<=5)&&(!door(h,x,y))) return true;
    else return false;
}

int Magictower::fight(int code)
{
    return Monsters[code].Cost();
}

void Magictower::get(int code)
{
    int t;
    switch(code)
    {
    case 59:
        Warrior.atk+=100;
        break;
    case 60:
        Warrior.def+=100;
        break;
    case 61:
        Warrior.hp+=200+Warrior.level*100;
        break;
    case 13:
        Warrior.yellow++;
        break;
    case 14:
        Warrior.blue++;
        break;
    case 15:
        Warrior.red++;
        break;
    case 20:
        Warrior.atk+=200;
        break;
    case 21:
        Warrior.def+=200;
        break;
    case 22:
        Warrior.atk+=400;
        break;
    case 23:
        Warrior.def+=400;
        break;
    case 24:
        Warrior.atk+=600;
        break;
    case 25:
        Warrior.def+=600;
        break;
    case 26:
        Warrior.atk+=800;
        break;
    case 27:
        Warrior.def+=800;
        break;
    case 30:
        break;
    case 31:
        break;
    case 32:
        do
        {
            system("cls");
            printf("\n1、%d生命值\n",((Warrior.h-1)/10+1)*200);
            printf("2、%d攻击力\n",((Warrior.h-1)/10+1)*20);
            printf("3、%d防御力\n",((Warrior.h-1)/10+1)*20);
            printf("\n花费金钱：%d\n",times*20);
            printf("\n勇士\n生命值：%d\n攻击力：%d\n防御力：%d\n金钱：%d\n",Warrior.hp,Warrior.atk,Warrior.def,Warrior.money);
            printf("\n请按ESC返回\n");
            t=getkey();
            if (Warrior.money>=times*20)
            {
                switch (t)
                {
                case KEY_1:
                    Warrior.hp+=((Warrior.h-1)/10+1)*200;
                    Warrior.money-=times*20;
                    times++;
                    break;
                case KEY_2:
                    Warrior.atk+=((Warrior.h-1)/10+1)*20;
                    Warrior.money-=times*20;
                    times++;
                    break;
                case KEY_3:
                    Warrior.def+=((Warrior.h-1)/10+1)*20;
                    Warrior.money-=times*20;
                    times++;
                    break;
                }
            }
        }
        while (t!=KEY_ESC);
        break;
    case 33:
        break;
    case 62:
        kettle=true;
        printf("\n你获得了“贪欲之壶”\n");
        printf("打败精灵后，使你获得双倍的金钱\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 68:
        scarecrow=true;
        update();
        printf("\n你获得了“废铁稻草人”\n");
        printf("与精灵战斗时，能帮你抵消一次敌人的攻击\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 64:
        huge=true;
        update();
        printf("\n你获得了“巨大化”\n");
        printf("与精灵战斗时，若你的攻击力高于敌人，使你的攻击力翻倍\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 65:
        exchange++;
        printf("\n你获得了一个“右手持盾左手持剑”\n");
        printf("按‘3’键使用，使你的攻击力和防御力互换\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 56:
        handbook=true;
        printf("\n你获得了“千年智慧轮”\n");
        printf("按‘1’键使用，显示当前层的精灵信息,再按ESC返回地图\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 53:
        lift=true;
        printf("\n你获得了“千年锡杖”\n");
        printf("按‘2’键使用，然后按上下来进行楼层跳跃，再按ESC返回地图\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 66:
        levelup++;
        printf("\n你获得了一个“等级上升！”\n");
        printf("按‘4’键使用，使等级上升一级\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 52:
        eye=true;
        printf("\n你获得了“千年眼”\n");
        printf("你现在可以看到暗墙\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 54:
        Warrior.hp+=5000;
        break;
    case 55:
        Warrior.atk+=1000;
        Warrior.def+=1000;
        break;
    case 57:
        exchangekey=5;
        printf("\n你获得“千年秤”\n");
        printf("按‘6’键使用，将一种钥匙转换成另一种钥匙，可使用五次\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 58:
        for (int i=0;i<=Map.h;++i) oldkey[i]=true;
        printf("\n你获得了“千年钥匙”\n");
        printf("按‘8’键使用，可以开启相邻的一扇门，每层楼限用一次\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 63:
        magicbook=true;
        printf("\n你获得了“秘术之书”\n");
        printf("打败精灵后，使你获得双倍的星数\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 67:
        earthquark++;
        printf("\n你获得了一个“地震”\n");
        printf("按‘5’键使用，将所在楼层所有墙壁破坏\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 69:
        godbless=0;
        printf("\n你获得了“神的恩惠”\n");
        printf("每击败5个精灵，回复150点生命值\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 70:
        pickaxe=10;
        printf("\n你获得了“迷宫变换”\n");
        printf("按‘9’使用，使相邻的一格墙消失，可使用10次\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    case 71:
        feather=true;
        printf("\n你获得了“妖鸟的羽毛扫”\n");
        printf("按‘7’使用，使自己周围8格的树藤消失\n");
        printf("\n\n按ESC返回\n");
        do
        {
            t=getkey();
        }
        while (t!=KEY_ESC);
        break;
    }
}

bool Magictower::door(int h, int x, int y)
{
    if(Map.map[h][x][y] == 3 && Warrior.yellow)
    {
        Warrior.yellow--;
        Map.map[h][x][y]=0;
        return true;
    }
    else
    if(Map.map[h][x][y] == 4 && Warrior.blue)
    {
        Warrior.blue--;
        Map.map[h][x][y]=0;
        return true;
    }
    else
    if(Map.map[h][x][y] == 5 && Warrior.red)
    {
        Warrior.red--;
        Map.map[h][x][y]=0;
        return true;
    }
    return false;
}

void Magictower::up_or_down(int& h, int& x, int& y)
{
    if(Map.map[h][x][y] == 8)
    {
        h++;
        x = Map.down_x[h];
        y = Map.down_y[h];
        Warrior.highest=h>Warrior.highest?h:Warrior.highest;
    }
    else
    if(Map.map[h][x][y] == 9)
    {
        h--;
        x = Map.up_x[h];
        y = Map.up_y[h];
    }
}

int Magictower::block(int h)
{
    return ((h-1)/10)+1;
}

void Magictower::update()
{
    for (int i=0;i<num_monsters;i++)
        Monsters[i].Change(Warrior.atk,Warrior.def,scarecrow,huge);
}

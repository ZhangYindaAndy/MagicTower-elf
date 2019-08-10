#include"iostream"
#include"Magictower.h"
#include"string"

using namespace std;

string load;

int main()
{
    //

    //
    load="test";
    Magictower game(load);
    do
    {
        system("cls");
        game.print();
    }
    while (game.waitmessage()&&game.boss);
    if (!game.boss)
    {
        system("cls");
        printf("\n恭喜你！闯关成功！\n");
    }
    else
    {
        system("cls");
        printf("\n胜败乃兵家常事，大侠请重新来过。\n");
    }
    return 0;
}

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
        printf("\n��ϲ�㣡���سɹ���\n");
    }
    else
    {
        system("cls");
        printf("\nʤ���˱��ҳ��£�����������������\n");
    }
    return 0;
}

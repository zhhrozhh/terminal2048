#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#define ddd printf("%d\n",__LINE__)
struct Block{
    int num;
    struct Block*up;
    struct Block*down;
    struct Block*left;
    struct Block*right;
};
struct Block**gridGen(){
    struct Block**list = (struct Block**)malloc(16*sizeof(struct Block*));
    for(int i=0;i<16;i++){
        struct Block*nb = (struct Block*)malloc(sizeof(struct Block));
        nb->num=0;
        list[i]=nb;
        if(!(i+1)%4)
            nb->right=NULL;
        if(i<4)
            nb->up = NULL;
        else{
            nb->up = list[i-4];
            list[i-4]->down = nb;
        }
        if(i>11)
            nb->down = NULL;
        if((i+1)%4==1)
            nb->left = NULL;
        else{
            nb->left = list[i-1];
            list[i-1]->right = nb;
        }
    }
    return list;
}
void printGrid(struct Block**grid){
    printf("----------------------------------\n");
    for(int i=0;i<16;i++){
        printf("|");
        if(grid[i]->num)
            printf("%d\t",grid[i]->num);
        else printf("\t");
        if(((i+1)%4))
            printf("|");
        else printf("|\n----------------------------------\n");
    }
}
#define CONT(d) MOV##d
#define MOVup down
#define MOVdown up
#define MOVright left
#define MOVleft right

#define MOVE(dir) \
    void move##dir(struct Block*b){\
        if(!b->dir){\
            move##dir(b->CONT(dir));\
            return;\
        }\
        struct Block*itr;\
        for(itr=b;itr->dir&&!itr->dir->num;itr=itr->dir);\
        itr->num=b->num;\
        if(itr!=b)b->num=0;\
        if(itr->dir&&itr->dir->num==itr->num){\
            itr->dir->num+=itr->num;\
            itr->num=0;\
        }\
        if(b->CONT(dir))move##dir(b->CONT(dir));\
    }
MOVE(up)
MOVE(down)
MOVE(right)
MOVE(left)
void up(struct Block**grid){
    moveup(grid[0]);
    moveup(grid[1]);
    moveup(grid[2]);
    moveup(grid[3]);
}
void down(struct Block**grid){
    movedown(grid[15]);
    movedown(grid[14]);
    movedown(grid[13]);
    movedown(grid[12]);
}
void left(struct Block**grid){
    moveleft(grid[0]);
    moveleft(grid[4]);
    moveleft(grid[8]);
    moveleft(grid[12]);
}
void right(struct Block**grid){
    moveright(grid[3]);
    moveright(grid[7]);
    moveright(grid[11]);
    moveright(grid[15]);
}
void randGenN(struct Block**grid){
    size_t x=0;
    while(1){
        x = rand()%16;
        if(!grid[x]->num)
            break;
    }
    int y = rand()%2;
    grid[x]->num = y?2:4;
}

void clearScreen(){
    for(int i=0;i<=9;i++)
        printf("\033[1A");
    for(int i=0;i<=9;i++)
        printf("                                   \n");
    for(int i=0;i<=9;i++)
        printf("\033[1A");
}
char getChar(){
    system("/bin/stty raw");
    char res=getchar();
    system("/bin/stty cooked");
    return res;
}
int main(){
    struct Block**x = gridGen();
    randGenN(x);
    printf("\033[2J\033[1;1H");
    clearScreen();
    printGrid(x);
    char a=' ';
    while(a!='q'){
        a=getChar();
        if(a==27&& getChar()==91){
            a=getChar();
            switch(a){
                case 'A':
                    up(x);
                    break;
                case 'B':
                    down(x);
                    break;
                case 'C':
                    right(x);
                    break;
                case 'D':
                    left(x);
                    break;
                default:break;
            }
            randGenN(x);
        }
        clearScreen();
        printGrid(x);
    }
    return 0;
}




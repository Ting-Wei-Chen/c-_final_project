#include<iostream>
#include<windows.h>
#include<conio.h>
#include<time.h>
#include<stdlib.h>

using namespace std;
const int row_number=28;
const int col_number=21;

int initial[row_number][col_number]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,1},
    {1,0,0,0,2,2,2,2,0,4,0,2,2,2,2,0,0,0,0,0,1},
    {1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,1},
    {1,0,0,0,2,2,2,2,0,0,0,2,2,2,2,0,0,0,0,0,1},
    {1,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,2,2,2,4,2,2,2,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,2,2,2,2,2,0,0,0,0,0,0,0,0,1}
};//初始化block 位置;
int wall[row_number][col_number];

int score=0,level=1,ball_num=3;
int x_pos=22,y_pos=10;
int left_plate=9,right_plate=12;
int moved[2]={-1,1};
//基本資訊
/*
0:nothing
1:wall
2:block
3:plate
4:bomb
*/
void SetColor(int color = 7)
{
  HANDLE hConsole;
  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole,color);
}
//改變字顏色
void gotoxy(int xpos, int ypos)
{
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}
//清屏函數
void setup(){
    for(int i=0;i<row_number;++i){
        for(int j=0;j<col_number;++j){
            wall[i][j]=initial[i][j];
        }
    }

    score=0,level=1,ball_num=3;
    x_pos=22,y_pos=10;
    left_plate=9,right_plate=12;
    moved[0]=-1;
    moved[1]=1;

    for(int i=0;i<row_number;++i){
        for(int j=0;j<col_number;++j){
            if(i==0||j==0||i==row_number-1||j==col_number-1){
                wall[i][j]=1;
            }
        }
    }

    wall[23][9]=3;
    wall[23][10]=3;
    wall[23][11]=3;
    wall[23][12]=3;
}
//建立基本資訊
void move_plate(int dir){
    if(dir==1){
        wall[23][left_plate]=0;
        left_plate+=1;
        right_plate+=1;
        wall[23][right_plate]=3;
    }
    else if(dir==-1){
        left_plate-=1;
        wall[23][left_plate]=3;
        wall[23][right_plate]=0;
        right_plate-=1;
    }
}
//移動plate
bool checkBlockTooMuch(){
    for(int i=1;i<row_number-1;++i){
        if(wall[21][i]==2||wall[21][i]==4){
            return 1;
        }
    }
    return 0;
}
//確認blocks 是否到plate正上方
void draw(){
    gotoxy(0,0);
    SetColor(15);
    cout<<"ball:"<<ball_num<<"\t"<<"score:"<<score<<"\t"<<"level:"<<level<<endl;
    for(int i=0;i<row_number;++i){
        for(int j=0;j<col_number;++j){
            if(wall[i][j]==1){
                SetColor(2);
                cout<<"* ";
            }
            else if(wall[i][j]==2){
                SetColor(11);
                cout<<"+ ";
            }
            else if(wall[i][j]==3){
                SetColor(6);
                cout<<"--";
            }
            else if(wall[i][j]==4){
                SetColor(27);
                cout<<"+ ";
            }
            else if(i==x_pos&&j==y_pos){
                SetColor(12);
                cout<<"@ ";
            }
            else{
                SetColor();
                cout<<"  ";
            }
        }
        cout<<endl;
    }
}
//根據地圖畫畫面函數
void adjustpos(){
    x_pos=22;
    y_pos=(left_plate+right_plate-1)/2;
    moved[0]=-1;
    moved[1]=1;
    draw();
    char dir;
    while(1){
        if(kbhit()){
            dir=getch();
            if(dir=='w'){
                break;
            }
            else if(dir=='a'&&left_plate>1){
                y_pos-=1;
                move_plate(-1);
            }
            else if(dir=='d'&&right_plate<col_number-2){
                y_pos+=1;
                move_plate(1);
            }
            draw();
        }
    }
}
//調整plate發射位置
void moving(){
    if(wall[x_pos+moved[0]][y_pos+moved[1]]==1){
        if(y_pos<=2){
            moved[1]=1;
        }
        else if(y_pos>=col_number-2){
            moved[1]=-1;
        }

        if(x_pos<=2){
            moved[0]=1;
        }
    }

    if(wall[x_pos+moved[0]][y_pos+moved[1]]==2){
        score+=1;
        wall[x_pos+moved[0]][y_pos+moved[1]]=0;
        moved[0]*=-1;
    }
    if(wall[x_pos+moved[0]][y_pos+moved[1]]==3){
        adjustpos();
    }
    if(wall[x_pos+moved[0]][y_pos+moved[1]]==4){
        for(int i=x_pos+moved[0]-1;i<=x_pos+moved[0]+1;++i){
            for(int j=y_pos+moved[1]-1;j<y_pos+moved[1]+1;++j){
                if(wall[i][j]==2){
                    score+=1;
                    wall[i][j]=0;
                }
                else if(wall[i][j]==4){
                    score+=2;
                    wall[i][j]=0;
                }
            }
        }
        moved[0]*=-1;
    }

    x_pos+=moved[0];
    y_pos+=moved[1];
}
//移動plate
void addOneRow(){
    for(int i=1;i<=21;++i){
        for(int j=1;j<col_number-1;++j){
            wall[i][j]=wall[i+1][j];
        }
    }
    for(int i=1;i<col_number-1;++i){
        wall[1][i]=2;
    }

    wall[1][rand()%(row_number-2)+1]=0;
    wall[1][rand()%(row_number-2)+1]=4;
}
//level+1時，從最上方新增一列block
void game(){
    char dir;
    while(1){
        if(kbhit()){
            dir=getch();
            if(dir=='z'&&moved[0]==-1){
                break;
            }
            else if(dir=='a'&&left_plate>=2){
                move_plate(-1);
            }
            else if(dir=='d'&&right_plate<col_number-2){
                move_plate(1);
            }
        }
        if(checkBlockTooMuch()){
            break;
        }
        if(x_pos==row_number-2){
            ball_num-=1;
            break;
        }
        if(score>=level*15+20){
            level+=1;
            addOneRow();
            ball_num+=1;
            adjustpos();
        }
        moving();
        draw();
        continue;
    }

    
    if(ball_num>=1){
        adjustpos();
        game();
    }
}
//遊戲進行(球移動)
void ending(){
    system("cls");
    SetColor(10);
    cout<<"game over"<<endl;
    cout<<"press z to restart";
}
//畫結束畫面
int main(){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(handle, &CursorInfo);
    //移除游標
    while(1){
        setup();
        draw();
        
        adjustpos();
        game();
        ending();
        char re;
        re=getch();
        if(re=='z'){
            continue;
        }

        break;
    }
    
    system("pause");
    return 0;
}
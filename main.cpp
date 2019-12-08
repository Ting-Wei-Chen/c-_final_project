#include<iostream>
#include<windows.h>
#include<conio.h>

using namespace std;
const int row_number=28;
const int col_number=21;

int wall[row_number][col_number]={
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
    {1,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,1},
    {1,0,0,0,2,2,2,2,0,4,0,2,2,2,2,0,0,0,0,0,1},
    {1,0,0,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,1},
    {1,0,0,0,2,2,2,2,0,0,0,2,2,2,2,0,0,0,0,0,1},
    {1,0,0,0,0,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,1}
};

int score=0,level=1,ball_num=3;
int x_pos=22,y_pos=10;
int left_plate=9,right_plate=12;
int moved[2]={-1,1};
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
void gotoxy(int xpos, int ypos)
{
  COORD scrn;
  HANDLE hOuput = GetStdHandle(STD_OUTPUT_HANDLE);
  scrn.X = xpos; scrn.Y = ypos;
  SetConsoleCursorPosition(hOuput,scrn);
}

void setup(){
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
bool checkBlockTooMuch(){
    for(int i=1;i<row_number-1;++i){
        if(wall[21][i]==2||wall[21][i]==4){
            return 1;
        }
    }
    return 0;
}
void draw(){
    gotoxy(0,0);
    cout<<"ball:"<<ball_num<<"\t"<<"score:"<<score<<"\t"<<"level"<<level<<endl;
    for(int i=0;i<row_number;++i){
        for(int j=0;j<col_number;++j){
            if(wall[i][j]==1){
                SetColor(12);
                cout<<"* ";
            }
            else if(wall[i][j]==2){
                SetColor(7);
                cout<<"+ ";
            }
            else if(wall[i][j]==3){
                SetColor(14);
                cout<<"--";
            }
            else if(wall[i][j]==4){
                SetColor(46);
                cout<<"+ ";
            }
            else if(i==x_pos&&j==y_pos){
                SetColor(7);
                cout<<"@ ";
            }
            else{
                cout<<"  ";
            }
        }
        cout<<endl;
    }
}
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
            else if(dir=='d'&&right_plate<row_number){
                y_pos+=1;
                move_plate(1);
            }
            draw();
        }
    }
}
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
void game(){
    char dir;
    while(1){
        if(kbhit()){
            dir=getch();
            if(dir=='z'){
                break;
            }
            else if(dir=='a'&&left_plate>=2){
                move_plate(-1);
            }
            else if(dir=='d'&&right_plate<=row_number-2){
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
        moving();
        draw();
        Sleep(20);
        continue;
    }

    if(score>=35+level*15){
        level+=1;
        adjustpos();
        game();
    }
    else if(ball_num>=1){
        adjustpos();
        game();
    }
}

void ending(){
    system("cls");
    SetColor(10);
    cout<<"game over"<<endl;;
}
int main(){
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO CursorInfo;
    GetConsoleCursorInfo(handle, &CursorInfo);
    CursorInfo.bVisible = false;
    SetConsoleCursorInfo(handle, &CursorInfo);
    //移除游標
    setup();
    draw();
    
    adjustpos();
    game();
    ending();
    system("pause");
    return 0;
}
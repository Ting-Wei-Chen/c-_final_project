#include<iostream>
#include<windows.h>
#include<conio.h>

const int row_number=28;
const int col_number=21;

int wall[row_number][col_number];

int score=0,level=1;
int x_pos=22,y_pos=10;
int left_plate=9,right_plate=12;
/*
0:nothing
1:wall
2:block
3:plate
4:bomb
*/
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

int main(){
    system("pause");
    return 0;
}
#define LATCH 12
#define DATA 13
#define CLK 11
#define MAX 100
#define LEN 1
#define PHAI 2
#define XUONG 3
#define TRAI 4
int huong=TRAI;
int huongtb = 0;
int indexDan = 0;
unsigned char firstRowTb = 16;
unsigned char firstColTb = 5;
#include<avr/pgmspace.h>
struct Toado {
  int x;
  int y;
};

//----------------------------------------------------------------------------------------snakeGame----------------------------------------------
Toado ran[MAX];
Toado vtmoi;
int sodot=3, dieuChinh=0;

//Khoi tao moi
void KhoiTao(){
  ran[0].x=4;
  ran[1].x=5;
  ran[2].x=6;
  ran[0].y=1;
  ran[1].y=1;
  ran[2].y=1;
  vtmoi.x=6;
  vtmoi.y=6;
}

//Hien thi ran
void HienThiRan(){
  dieuChinh = 60 - sodot*5;
  if(dieuChinh < 8){
    dieuChinh = 10;
  }
  for(int j=0; j<dieuChinh;j++){
    printfpoint(vtmoi.y,vtmoi.x);
    for(int i=0;i<sodot;i++){
       printfpoint(ran[i].y,ran[i].x);
    }
    BatSuKien();
  }
}

//Di chuyen
Toado DiChuyen(int huong){
  for(int i=sodot-1;i>=1;i--){
    ran[i]=ran[i-1];
  }
  switch(huong){
    case LEN:
      ran[0].y--;
      break;
    case PHAI:
      ran[0].x++;
      break;
    case XUONG:
      ran[0].y++;
      break;
    case TRAI:
      ran[0].x--;
      break;
  }
  
  if(ran[0].x==17) {
    ran[0].x=1;
  }
  else if(ran[0].x==0) {
    ran[0].x=16;
  }
  else if(ran[0].y==17){
    ran[0].y=1;
  }
  else if(ran[0].y==0){
    ran[0].y=16;
  }
    return Toado {ran[sodot - 1].x,ran[sodot - 1].y};
}

//Bat su kien nhan nut
void BatSuKien(){
  if(digitalRead(3)==0){
    if(huong == PHAI) huong = PHAI;
    else huong=TRAI;
  }
  else if(digitalRead(2)==0){
    if(huong == TRAI) huong = TRAI;
    else huong=PHAI;
  }
  else if(digitalRead(5)==0){
    if(huong == LEN) huong = LEN;
    else huong=XUONG;
  }
  else if(digitalRead(4)==0){
    if(huong == XUONG) huong = XUONG;
    else huong=LEN;
  }
}

//Gieo moi random
void GieoMoi(){
   vtmoi.x=random(1,16);
   vtmoi.y=random(1,16);
}

//Tang do dai ran
void RanDaiRa(){
  Toado vtdotcuoiCu; 
  vtdotcuoiCu = DiChuyen(huong);
  sodot=sodot+1;
  ran[sodot - 1].x = vtdotcuoiCu.x;
  ran[sodot - 1].y =vtdotcuoiCu.y;
}

//Ham dieu kien ket thuc tro choi
boolean KtThua(){
  int i=0;
  for(i=2; i<sodot; i++){
    if ((ran[0].x == ran[i].x) && (ran[0].y == ran[i].y)){
      return true;
      break;
    }
  }
  return false;
}

//----------------------------------------------------------------------------------------snakeGame---------------------------------------------

// ---------------------------------------------------------------------------------------basic fuction-----------------------------------------

//Doc trang thai 1 bit
boolean BitState(int val, int pos){
  boolean xbit;
  signed int xchar=-1;
  if(pos<16){
    xchar=0B0000000000000001 & val>>pos;
  }
  xbit=xchar;
  return xbit;
}

//doc trang thai 16 bit
void ShiftOut16bit(int num){
  for(int i=0;i<16;i++){
    digitalWrite(DATA,BitState(num,i));
    digitalWrite(CLK,HIGH);
    digitalWrite(CLK,LOW);
  }
}

// ghi trang thai 16 bit vao thanh ghi dich
void ChangeData(unsigned int datascreen,unsigned int col){
  digitalWrite(LATCH,LOW);
//  ShiftOut16bit(col);
  ShiftOut16bit(~datascreen);
  ShiftOut16bit(col);
  digitalWrite(LATCH,HIGH);
}


//in 1 diem bat ki
void printfpoint(int inrow,int incol){
  unsigned int LEDrow[16]={128,64,32,16,
       8,4,2,1,
       32768,16384,8192,4096,
       2048,1024,512,256};
  unsigned int LEDcol[16]={128,64,32,16,
       8,4,2,1,
       32768,16384,8192,4096,
       2048,1024,512,256};
  ChangeData(LEDcol[inrow-1],LEDrow[incol-1]);
}

// ---------------------------------------------------------------------------------------basic fuction-----------------------------------------

// ---------------------------------------------------------------------------------------planeGame--------------------------------------------
unsigned char tg = 1;
unsigned char ran1 = random(12)+3;
unsigned char ran2 = random(12)+3;
unsigned char ban = 0;
Toado plane[5];
Toado dan[100];
Toado vtVc[2];
void setupPlane(unsigned char row, unsigned char col){ //diem nhon la diem bat dau 
  plane[1].x=col;
  plane[0].x=col-1;
  plane[2].x=col+1;
  plane[3].x=col;

  plane[0].y=row;
  plane[1].y=row;
  plane[2].y=row;
  plane[3].y=row-1;
}

//hien thi tau bay
void disPosPlane(unsigned char row, unsigned char ran1, unsigned char ran2){
  int i,j;
  for(j=0; j<=5; j++){
    randomVC(row,ran1);
//    randomVC(row,ran2);
    for(i=0; i<=3; i++){
      printfpoint(plane[i].y, plane[i].x);
    }
  }
}

//di chuyen tau bay
void movePlane(){
  if(digitalRead(3)==0){
     firstColTb--; //PHAI
  }
  else if(digitalRead(2)==0){
     firstColTb++; //TRAI
  }
  else if(digitalRead(5)==0){
    firstRowTb++; //LEN
  }
  else if(digitalRead(4)==0){
    firstRowTb--; //XUONG
  }
  if(firstColTb>15){
    firstColTb = 15;
  }
  if(firstColTb<2){
    firstColTb = 2;
  }
  if(firstRowTb>16){
    firstRowTb = 16;
  }
  if(firstRowTb<2){
    firstRowTb = 2;
  }
}

//ban dan
void shoot(){
  int i,j;
  if (dan[indexDan-1].y != plane[0].y && dan[indexDan-1].x != plane[1].x){
    dan[indexDan].y = plane[0].y;
    dan[indexDan].x = plane[1].x;
  }
  for(i = 0; i<= 2; i++){
    for(j = 0; j<= indexDan; j++){
      if(dan[j].y >= 1 && dan[j].y <= 16 && dan[j].x >= 1 && dan[j].x <= 16){
          printfpoint(dan[j].y, dan[j].x);
      }
    }
  }
  
  if(indexDan == 1000) indexDan = 0;
  indexDan++;
}

void randomVC(unsigned char row, unsigned char ran){
  int i,j;
  for(j=0; j<=3; j++){
      if(row>=1 && row <= 16 && ran >= 1 && ran <= 16){
        printfpoint(row,ran);
        printfpoint(row+1,ran);
        vtVc[0].y = row;
        vtVc[0].x = ran;
        vtVc[1].y = row+1;
        vtVc[1].x = ran;
      }
  }
}

// ---------------------------------------------------------------------------------------planeGame-----------------------------------------

//-------------------------------------------------------------------giao dien------------------------------------------------------------------
unsigned char tg_gach = 1;
unsigned char ran_vt_gach = random(15) + 1;
unsigned char ran_gach = random(3) + 1;
int Screen_v_row=0, Screen_v_col=0;
void Screen_v(int q, int i){
  printfpoint(1+q, 1+i);
  printfpoint(1+q, 1+i);
  printfpoint(1+q, 2+i);
  printfpoint(1+q, 3+i);
  printfpoint(1+q, 4+i);
  printfpoint(1+q, 5+i);
  printfpoint(1+q, 6+i);
  printfpoint(1+q, 7+i);
  printfpoint(1+q, 8+i);

  printfpoint(8+q, 1+i);
  printfpoint(8+q, 2+i);
  printfpoint(8+q, 3+i);
  printfpoint(8+q, 4+i);
  printfpoint(8+q, 5+i);
  printfpoint(8+q, 6+i);
  printfpoint(8+q, 7+i);
  printfpoint(8+q, 8+i);
  
  printfpoint(1+q, 1+i);
  printfpoint(2+q, 1+i);
  printfpoint(3+q, 1+i);
  printfpoint(4+q, 1+i);
  printfpoint(5+q, 1+i);
  printfpoint(6+q, 1+i);
  printfpoint(7+q, 1+i);
  printfpoint(8+q, 1+i);
  
  printfpoint(1+q, 8+i);
  printfpoint(2+q, 8+i);
  printfpoint(3+q, 8+i);
  printfpoint(4+q, 8+i);
  printfpoint(5+q, 8+i);
  printfpoint(6+q, 8+i);
  printfpoint(7+q, 8+i);
  printfpoint(8+q, 8+i);
}

void moveScreen(){
  if(digitalRead(2)==0){
     delay(50);
     Screen_v_col = Screen_v_col - 8; //PHAI
//     ran_vt_gach--;
     delay(50);
  }
  else if(digitalRead(3)==0){
     delay(50);
     Screen_v_col = Screen_v_col + 8; //TRAI
//     ran_vt_gach++;
     delay(50);
  }
  else if(digitalRead(4)==0){
    Screen_v_row = Screen_v_col + 8; //LEN
  }
  else if(digitalRead(5)==0){
    Screen_v_row = Screen_v_col - 8; //XUONG
  }
  else if(digitalRead(6)==0){
    if(Screen_v_row==0 && Screen_v_col==0){
      while(1){
        planeGame();
        if(digitalRead(1)==0) break;
      }
    } else if(Screen_v_row==0 && Screen_v_col==8){
       while(1){
        snakeGame();
        if(digitalRead(1)==0) break;
      }
    }
  }
  if (Screen_v_col > 8) Screen_v_col = 8;
  if (Screen_v_col < 0) Screen_v_col = 0;
  if (Screen_v_row > 8) Screen_v_row = 8;
  if (Screen_v_row < 0) Screen_v_row = 0;
}

//-------------------------------------------------------------------giao dien------------------------------------------------------------------

//-------------------------------------------------------------------xep gach-----------------------------------------------------
Toado gach[2];
void tao_gach(unsigned char gach, unsigned char row, unsigned char gach_random){
  switch(gach){
    case 1:{//gach vuong
      printfpoint(row,gach_random);
      printfpoint(row-1,gach_random);
      printfpoint(row,gach_random+1);
      printfpoint(row-1,gach_random+1);
      break;
    }
    case 2:{//gach 1 thanh
      printfpoint(row,gach_random);
      printfpoint(row+1,gach_random);
      printfpoint(row+2,gach_random);
      break;
    }
    case 3:{
      printfpoint(row,gach_random);
      printfpoint(row+1,gach_random);
      printfpoint(row+2,gach_random);
      printfpoint(row+2,gach_random+1);
      break;
    }
    case 4:{
      printfpoint(row,gach_random);
      printfpoint(row,gach_random+1);
      printfpoint(row+1,gach_random+1);
      printfpoint(row+1,gach_random+2);
      break;
    }
  }
}
//-------------------------------------------------------------------xep gach-----------------------------------------------------

void snakeGame(){
  if(KtThua()==true){
     while(1){
      printfpoint(8,8);
     }
  }
  HienThiRan();
  DiChuyen(huong);
  if(ran[0].x == vtmoi.x && ran[0].y == vtmoi.y){
    RanDaiRa();
    GieoMoi();
  }
  BatSuKien();

  printfpoint(5,5);
}

void planeGame(){
  int j=0, i;
  setupPlane(firstRowTb,firstColTb);
  disPosPlane(tg, ran1, ran2);
  tg++;
  if (tg == 11) {
    tg = 2;
    ran1 = random(12)+2;
  }
  movePlane();
  shoot();
  ban++;
  for(j = 1; j<= indexDan; j++){
    dan[j].y = dan[j].y - 1;
    if(((dan[j].y == vtVc[0].y) && (dan[j].x == vtVc[0].x)) || ((dan[j].y == vtVc[1].y) && (dan[j].x == vtVc[1].x))){
      ran1 = 100; 
      for(i = 0; i <= 1; i++){
            printfpoint(vtVc[0].y, 1);
            printfpoint(vtVc[0].y, 2);
            printfpoint(vtVc[0].y, 3);
            printfpoint(vtVc[0].y, 4);
            printfpoint(vtVc[0].y, 5);
            printfpoint(vtVc[0].y, 6);
            printfpoint(vtVc[0].y, 7);
            printfpoint(vtVc[0].y, 8);
            printfpoint(vtVc[0].y, 9);
            printfpoint(vtVc[0].y, 10);
            printfpoint(vtVc[0].y, 11);
            printfpoint(vtVc[0].y, 12);
            printfpoint(vtVc[0].y, 13);
            printfpoint(vtVc[0].y, 14);
            printfpoint(vtVc[0].y, 15);
            printfpoint(vtVc[0].y, 16);
       }
     }
  }
}
void setup() {
  pinMode(2,INPUT_PULLUP);
  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(1,INPUT_PULLUP);
  pinMode(LATCH,OUTPUT);
  pinMode(DATA,OUTPUT);
  pinMode(CLK,OUTPUT);
  KhoiTao();
}
// ban mb

void loop() {
//  int i;
//  for(i=0; i<50; i++){
//     tao_gach(ran_gach,tg_gach,ran_vt_gach);
//     moveScreen();
//  }
    Screen_v(Screen_v_row,Screen_v_col);
    moveScreen();

//  tg_gach++;
//  if (tg_gach == 16) {
//    tg_gach = 1;
//    ran_vt_gach = random(15) + 1;
//    ran_gach = random(5);
//  }
//  planeGame(); 
//  snakeGame();
}

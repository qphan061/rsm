#include <iostream>
using namespace std;
#include <winbgim.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#define 	MINW 		20
#define 	MINH 		20
#define 	MAXW 		800
#define 	MAXH 		600
#define 	MINX 		20
#define 	MINY 		20
#define 	MAXX 		800
#define 	MAXY 		600
#define 	BACKGROUND	15

struct Point {
	int x,y,x0,y0;
};
struct HighScore {
	int score;
	char name[30];
};

void initGame ();
bool checkPoint ();
void drawPoint (int x,int y,int r);
void moveSnake ();
void drawSnake ();
void drawFood ();
void drawGame ();
void classic();
void modern();
void mainLoop (void (*gloop)());
void run ();
void changeDirecton (int x);
void showHighScore();
void getHighScore();
void checkHighScore(int score);
void initScore();
bool isEmpty();
void showText(int x,int y,char *str);
void showTextBackground(int x,int y,char *str,int color);


int level;
bool endGame;
int snakeLength;
Point snake[200];
Point direction;
Point food;
const int DIRECTION	= 10;
HighScore  highscore[5];
char* score_str = new char[20];// Convert score to string
void initScore(){
		if (isEmpty()){
			for (int i = 0;i < 5;i++){
			strcpy(highscore[i].name,"");
			highscore[i].score = 0;
			}
			getHighScore();
		}else{
			char arr[20];
			int count = 0;
			FILE *f;
			f = fopen("highscore.txt", "r");
			for (int i = 0;i < 5;){
				while (!feof(f)){
					count++;
					fscanf(f, "%s", arr);
				   if (count%2 == 1){
				   		strcpy(highscore[i].name,arr);
						//printf ("%s\t",ch);
					}
					else{
						highscore[i].score = atoi(arr);
						i++;
						}
				   }
				}
			}
}

void showTextBackground(int x,int y,char *str,int color){
	int bk = getbkcolor();
	setbkcolor(color);
	outtextxy(x,y,str);
	delay(300);
	setbkcolor(bk);
}

void showText(int x,int y,char *str){
	int c = getcolor();
	int color = rand() % 16 + 1;
	setcolor(color);
	outtextxy(x+3,y,str);
	setcolor(c);
	delay(500);
}

bool isEmpty(){
	FILE *fp;
	long size;
	fp = fopen("highscore.txt","r");
	if(fp){
		fseek (fp, 0, SEEK_END);
        size = ftell(fp);
		fclose(fp);
	}
	return(size == 0);
}
bool checkPoint (){
	for (int i = 0;i < snakeLength;i++){
		if (food.x == snake[i].x && food.y == snake[i].y)
		return false;
	}
	return true;
}
void initGame(){
	setbkcolor (15);
	cleardevice ();
	setwindowtitle ("RAN DANG BO....");
	endGame = false;
	setfillstyle (1,5);
	bar (MINX,MINY,MAXX,MINY+5);
	bar (MAXX,MINY,MAXX-5,MAXY);
	bar (MAXX,MAXY,MINX,MAXY-5);
	bar (MINX,MINY,MINX+5,MAXY);
    setcolor (0);
    int bk = getbkcolor();
    setfillstyle (SOLID_FILL,0);
	int a[] = {15,350,30,350,30,345,50,355,30,365,30,360,15,360,15,350};
    fillpoly (8,a);
   

	snake[0].x = 60;snake[0].y = 50;
	snake[1].x = 50;snake[1].y = 50;
	snake[2].x = 40;snake[2].y = 50;
	snakeLength = 3;
	setfillstyle (1,BACKGROUND);
	bar (25,25,415,215);
	direction.x = 10;direction.y = 0; // Khoi tao di theo huong trai qua phai
	setcolor (2);
	srand ( time(NULL));
	//Init food
    do{
        food.x = (rand() % (39) + 3)*10;
    	food.y = (rand() % (19) + 3)*10;
	}while (checkPoint() == false);
}

void drawPoint (int x,int y,int r){
	circle (x,y,r);
	floodfill (x,y,getcolor());
}


// Snake is moving
void modern(){
	for (int i = 0;i < snakeLength;i++){
		if (i == 0){
			snake[0].x0 = snake[0].x;snake[0].y0 = snake[0].y;
			snake[0].x += direction.x;
			snake[0].y += direction.y;
		}
		else {
			snake[i].x0 = snake[i].x;snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;snake[i].y = snake[i-1].y0;
		}
		if (snake[0].x < MINX+5 || snake[0].y < MINY+5 || snake[0].x > MAXX-5 || snake[0].y > MAXY - 5 )
			endGame = true;
		if (i != 0 && snake[0].x == snake[i].x && snake[0].y == snake[i].y)	 endGame = true;
	}
	if (snake[0].x == food.x && snake[0].y == food.y){
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0;
		snakeLength++;
	
		//Random again food if sanke ate food
		srand ( time(NULL));
		do{
        	food.x = (rand() % (39) + 3)*10;
    		food.y = (rand() % (19) + 3)*10;
		}while (checkPoint() == false);
	}
}

void classic(){
    for (int i = 0; i < snakeLength; i++) {
        if (i == 0) {
            snake[0].x0 = snake[0].x;snake[0].y0 = snake[0].y;
			snake[0].x += direction.x;
			snake[0].y += direction.y;
        }else{
            snake[i].x0 = snake[i].x;snake[i].y0 = snake[i].y;
			snake[i].x = snake[i-1].x0;snake[i].y = snake[i-1].y0;
        }
         
        if (snake[i].x >= MAXX) snake[i].x = MINX + 10;
        if (snake[i].x <= MINX) snake[i].x = MAXX - 10;
        if (snake[i].y >= MAXY) snake[i].y = MINY + 10;
        if (snake[i].y <= MINY) snake[i].y = MAXY - 10;
         
        if (i != 0 && (snake[0].x == snake[i].x && snake[0].y == snake[i].y)) endGame = true;
    }
    if (snake[0].x == food.x && snake[0].y == food.y){
		snake[snakeLength].x = snake[snakeLength-1].x0;snake[snakeLength].y = snake[snakeLength-1].y0;
		snakeLength++;
	
		//Random again food if sanke ate food
		srand ( time(NULL));
        do{
        	food.x = (rand() % (39) + 3)*10;
    		food.y = (rand() % (19) + 3)*10;
		}while (checkPoint() == false);
	}
}

void drawSnake (){
	setfillstyle (1,10);
	int count = 0;
	for (int i = 0;count < snakeLength;i++){
			drawPoint(snake[i].x, snake[i].y,5);
			count++;
		}
	//delete point when snake move
	int x = getcolor();
	setfillstyle (1,BACKGROUND);
	drawPoint(snake[snakeLength-1].x0,snake[snakeLength-1].y0,5);
	setcolor (BACKGROUND);
	circle (snake[snakeLength-1].x0,snake[snakeLength-1].y0,5);
	setcolor(x);
	setfillstyle (1,5);
}

void drawFood (){
	int x = getcolor();
	int c = rand() % 14 + 1;
		setcolor(c);setfillstyle (SOLID_FILL, c);
		drawPoint(food.x, food.y,5);
	setcolor(x);setfillstyle (1,BACKGROUND);
}

//Ve toan bo giao dien game
void drawGame (){
	drawSnake();
	drawFood();
	int x = getcolor ();
	setcolor (4);
	sprintf(score_str,"%d",snakeLength*level*10-level*30);    //show score
    settextstyle(4,0,3);
    outtextxy(450,20,"SCORE:");
    setcolor(11);
    outtextxy(600,20,score_str);
	setcolor (x);
	settextstyle(6, 0, 5);
    setcolor(14);  // Chọn màu chữ, có thể thay đổi
    outtextxy(MAXX+200, MAXY/2, "1 2 1 2 A co len!!!!");
    

    setcolor(x);  // Khôi phục màu cũ
}


void changeDirecton (int x){
	switch(x){
                case 72: 
                    if (direction.y != DIRECTION) {
                    
            			direction.y = -DIRECTION; direction.x = 0;
        			}
                    break;
                case 80: 
                	if (direction.y != -DIRECTION) {
                	
            			direction.y = DIRECTION; direction.x = 0;
        			}
                   break;
                case 77:
                    if (direction.x != -DIRECTION) {
                    	
            			direction.x = DIRECTION; direction.y = 0;
        			}
                    break;
                case 75:
                    if (direction.x != DIRECTION) {
                    
            			direction.x = -DIRECTION; direction.y = 0;
        			}
                    break;
                case 27:
                	endGame = true;
					break;
            }
}

void mainLoop (void (*xxx)()){
	int x = getcolor();
	if (!kbhit()){
		xxx();
	}
	else{
		char ch;
        ch = getch();
		if (ch == -32) ch = getch();
		if ( ch == 32){
			setcolor(0);outtextxy (100,100,"TIEP TUC CHOI DI");setcolor(x);
			ch = getch();
			while (ch != 32){
				ch = getch();
				delay(0);
			}
			setcolor (BACKGROUND);
			outtextxy (100,100,"TIEP TUC CHOI DI");
			setcolor (x);
			delay(1000);
		}
        changeDirecton(ch);
        mainLoop (xxx);
    }
}

void showHighScore(){
	FILE *f;
	f = fopen("highscore.txt", "r");
	char ch[20];
	settextstyle (1,0,5);
	setcolor (15);
	outtextxy (150,50,"HIGH SCORE");
	settextstyle (1,0,4);
	int y = 150,count = 0;
	while (!feof(f)){
		if (count == 10) break;
		count++;
		fscanf(f, "%s", ch);
   		if (count%2 == 1){
   			setcolor (10);
   			outtextxy (180,y,ch);
   			y+=50;
		}else{
			setcolor (12);
			outtextxy (500,y-50,ch);	   	
		}
	}
	fclose(f);
}
void getHighScore (){
	FILE *f;
	f = fopen("highscore.txt", "w");
	for (int i = 0;i < 5;i++){
		fputs(highscore[i].name,f);
		fputs(" ",f);
		fprintf(f,"%d",highscore[i].score);
		fputs("\n",f);
	}
	fclose(f);
}
void checkHighScore (int _score){
	char _name[20]={""};
	for (int i = 0;i < 5;i++){
		if (_score > highscore[i].score){
			//to do sth
			settextstyle(1,0,3);
				for (int j = 0;j < 50;j++){
					if (j%2 == 0){
						setcolor (14);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}else{
						setcolor (9);
						if (i == 0)
						outtextxy(460,100,"BEST SCORE");
						else
						outtextxy(460,100,"HIGH SCORE");
						delay(100);
					}
				}
				settextstyle(1,0,2);
				setcolor (4);outtextxy(430,150,"TEN:");
				delay(100);
				char ch1;
				int x = 0;
				char str[2];
				str[1] = 0;
				while (ch1 != 13 && x < 10){
					do{
						ch1 = getch();
					}while (ch1 < 65 && ch1 >90 || ch1 < 97 && ch1 > 132);
					x++;
					str[0] = ch1;
					strcat(_name,str);
					outtextxy(540,150,_name);
				}
			for (int j = 4;j > i;j--){
				strcpy(highscore[j].name,highscore[j-1].name);
				highscore[j].score = highscore[j-1].score;
			}
			strcpy(highscore[i].name,_name);
			highscore[i].score = _score;
			break;
		}
	}
	getHighScore();
}
void run (){
	initwindow (800,600);
	initScore();
	int x = 0;
	char *s = new char[5];
	while (x <= 100){
		cleardevice();
		setcolor (14);settextstyle(4,0,5);outtextxy (100,200,"DANG LOAD...");
		sprintf (s,"%d",x);
		setcolor (4);
		outtextxy (550,200,strcat(s,"%"));
		if (x == 100)
			delay (500);
		delay (20);
		x++;
	}
	
	while (true){
	x1:
	int bk = getbkcolor();
	cleardevice();
	setbkcolor(0);
	cleardevice();
	clearmouseclick(WM_LBUTTONDOWN);
	
	while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 200 || mousey() > 465 || mousey() < 190 || mousex() > 580 ){
		settextstyle(1,0,6);
		setcolor (18);
		outtextxy(120,70,"GAME MENU");
		settextstyle(10,0,5);
		showText (220,200,"CHOI THOI");
		showText (220,270,"DIEM CAO");
		showText (220,340,"TAT GAME ON THI CUOI KI");
		showText (220,410,"THONG TIN");
	}
	clearmouseclick(WM_LBUTTONDOWN);
	if (mousey() <= 255){
		showTextBackground(220,200,"CHOI THOI",15);
		x2:
		setbkcolor(0);
		cleardevice();
		int nameFunction;
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN) || mousex() < 290 || mousex() > 520 || mousey() < 240 || mousey() > 400){
			settextstyle(1,0,6);setcolor(12);
			outtextxy (150,150,"CHE DO");
			setcolor(15);settextstyle(1,0,4);
			showTextBackground(250,250,"CO DIEN",0);
			showTextBackground (250,350,"HIEN DAI",0);
		}
		if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 290 && mousex() <= 520 && mousey() >= 240 && mousey() <= 400){
			if(mousey() <= 300) nameFunction = 1;
			else nameFunction = 2;
		}
		setbkcolor(0);
		cleardevice();
		setcolor(12);settextstyle(4,0,3);
		outtextxy(10,310,"CHON MUC :");
		settextstyle (4,0,6);
		setcolor (13);
		outtextxy (150,200,"MUC 1 - 5");
		setcolor(16);settextstyle(4,0,3);
		outtextxy(10,310,"CHON MUC :");
		int xx = 330;
char str1[2];

// Hiển thị các số 1, 2, 3, 4, 5 chỉ một lần duy nhất
for (int i = 1; i <= 5; i++) {
    sprintf(str1, "%d", i);
    settextstyle(4, 0, 4);  // Kiểu chữ và kích thước
    showTextBackground(xx, 300, str1, 0);
    xx += 100;
}

// Chờ một chút để người chơi có thể nhìn thấy các số, rồi chuyển sang phần tiếp theo
delay(2000);  // Chờ 2 giây (2000 ms) trước khi tiếp tục

// Tiến hành phần tiếp theo của chương trình (ví dụ chọn mức độ)
if (ismouseclick(WM_LBUTTONDOWN) && mousex() >= 300 && mousex() <= 790 && mousey() >= 280 && mousey() <= 350) {
    if (mousex() <= 390) level = 1;
    else if (mousex() <= 490) level = 2;
    else if (mousex() <= 590) level = 3;
    else if (mousex() <= 690) level = 4;
    else level = 5;
}

		initGame();
		while (!endGame){
			//Change delay folow level
		delay (250 - (level-1)*40);
		(nameFunction == 1) ? mainLoop(classic):mainLoop(modern);
		drawGame();
	}
	setcolor(15);  // White color for instructions
    settextstyle(4, 0, 2);  // Smaller font size for instructions
    outtextxy(10, 40, "Controls:");
    outtextxy(10, 60, "Arrow Keys: Move");
    outtextxy(10, 80, "ESC: Exit");
		
		settextstyle (1,0,4);
		setcolor (0);
		outtextxy (100,110,"NON THE!");
		setcolor (8);
		settextstyle (1,0,2);
		outtextxy (100,160,"DIEM : ");
		setcolor(10);
		outtextxy (300,160,score_str);
		//Here is check high score
		checkHighScore(snakeLength*level*10-level*30);
		setcolor(10);settextstyle(4,0,5);
		outtextxy (100,200,"THU PHAT NUA NHI ?");
		settextstyle(4,0,3);
		setcolor(1);
		clearmouseclick(WM_LBUTTONDOWN);
		while (!ismouseclick(WM_LBUTTONDOWN)){
			showTextBackground(150,270,"YES",12);showTextBackground(300,270,"NO",7);
		}
		if(ismouseclick(WM_LBUTTONDOWN)){
			if (mousex() >= 140 && mousex() <= 230 && mousey() >= 260 && mousey() <= 310){
				showTextBackground(150,270,"YES",7);
				delay(100);
				goto x2;
			}
			else if(mousex() >= 290 && mousex() <= 360 && mousey() >= 260 && mousey() <= 310){
				showTextBackground(300,270,"NO",12);
				delay(100);
				goto x1;
			}
        	}
    	}else if (mousey() <= 325){
			showTextBackground(220,270,"DIEM CAO",15);
			cleardevice();
			showHighScore();
			while (!ismouseclick(WM_LBUTTONDOWN)){
				delay(0);
			}
			goto x1;
		}else if(mousey() <= 395){
			showTextBackground (220,340,"TAT GAME ON THI CUOI KI",15);
			exit(0);
		}else{
			
		cleardevice();
		settextstyle (1,0,4);
		setcolor (15);
		int x = getcolor();
		while(!ismouseclick(WM_LBUTTONDOWN)){
			settextstyle(10,0,6);setcolor(15);
			outtextxy(260,20,"ABOUT");
			settextstyle(1,0,4);setcolor(9);
			outtextxy(20,100,"DAI HOC BACH KHOA HA NOI");
			settextstyle(7,0,5);
			showText(290,150,"DE TAI");
			settextstyle(4,0,2);
			showText(170,200,"TRO CHOI \"RAN SAN MOI\"");
			settextstyle(7,0,3);
			showText(200,270,"SINH VIEN THUC HIEN");
			settextstyle(4,0,2);
			showText(150,320,"PHAN NGUYEN DANG QUYEN-20224119/TRAN DUC MANH-20224055");
			settextstyle(3,0,3);
			showText(250,390,"GIANG VIEN HUONG DAN");
			settextstyle(4,0,3);
			showText(180,440,"HOANG QUANG HUY");
			settextstyle(4,0,3);
			showText(250,500,"Su dung ca phim mui ten de di chuyen");
			settextstyle(4,0,3);
			showText(250,570,"Phim ESC de lui lai, phim SPACE de tam dung game");
			settextstyle(4,0,3);
			
		}
		setcolor(x);
		settextstyle (1,0,2);
		setcolor (4);
		goto x1;
		}
	}
}
int main (){
	run();
	//Giai phong bo nho con tro;
	delete score_str;
	getch();
	closegraph ();
    return 0;
}

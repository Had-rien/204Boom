#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

/* Create a Struct Game for an easy access to the game variables in all functions. */
struct Game{
	int height; /* for the height of the grid */
	int width;  /* for the width of the grid */
	int lenghtMax; /* the maximum number of figures in one number of the game */
	int changed; /* state of the game : lost, efficient move... */
	int goal; /* winnig value of the game */
	int large; /* =1 for larger grid */
};

typedef int ** array;

/* @ requires h and w positie integers
 * @ assigns a 2d array of int
 * @ ensures the return of a pointer on the array
 */
array alloc(int h, int w){
  array mat;
  mat = (int**) malloc(h*sizeof(int*)); 
  int i;
  for(i=0;i<h;i=i+1){
    mat[i]=(int*)calloc(w,sizeof(int));
  }
  return mat;
}

/*--------BOMBS-------*/

/* @ requires a game and a array of int
 * @ assigns int i,j for the loops
 * @ ensures bombs values are decreased in order to exlose sooner
 */
void tictacBombs(struct Game game, array numbers){
	int i,j;
	for(i=0; i<game.height; i=i+1){
		for(j=0; j<game.width; j=j+1){
			if(numbers[i][j]<0){
				numbers[i][j]=numbers[i][j]-1;
			}
		}
	}
	return;
}

/* @ requires a game, a array of int, coordinate of the bomb
 * @ assigns new values to numbers
 * @ ensures + bombs destroy the surrending
 */
void explodePlus(struct Game game, array numbers, int i, int j){
	numbers[i][j]=0;
	if(j+1<game.width){	numbers[i][j+1]=0;}
	if(j-1>=0){	numbers[i][j-1]=0;}
	if(i+1<game.height){	numbers[i+1][j]=0;}
	if(i-1>=0){numbers[i-1][j]=0;}
	return;
}

/* @ requires a game, a array of int, coordinate of the bomb
 * @ assigns new values to numbers
 * @ ensures * bombs destroy the surrending
 */
void explodeEtoile(struct Game game, array numbers, int i, int j){
	numbers[i][j]=0;
	if(j+1<game.width){
		if(i+1<game.height){	numbers[i+1][j+1]=0;}
		if(i-1>=0){	numbers[i-1][j+1]=0;}
	}
	if(j-1>=0){	
		if(i+1<game.height){	numbers[i+1][j-1]=0;}
		if(i-1>=0){	numbers[i-1][j-1]=0;}
	}
	return;
}

/* @ requires a game, a array of int, coordinate of the bomb
 * @ assigns new values to numbers
 * @ ensures bombs explose when needed
 */
void explodeBombs(struct Game game, array numbers, int i, int j){
	if(numbers[i][j]==-7){
		explodePlus(game, numbers, i, j);
	}
	else if(numbers[i][j]==-14){
		explodeEtoile(game, numbers, i, j);
	}
	else if(numbers[i][j]==-21){
		explodePlus(game, numbers, i, j);
		explodeEtoile(game, numbers, i, j);
	}
	return;
}

/* @ requires a game, a array of int, coordinate of the bomb
 * @ assigns new values to numbers
 * @ ensures bombs explose when choosen
 */
void chosenBombs(struct Game * pgame, array numbers, int i, int j){
	if(numbers[i][j]>=0){
		printf("This is not a Bomb!\n");
		pgame->changed = 0;
		return;
	}
	else if(numbers[i][j]>=-7){
		explodePlus(*pgame, numbers, i, j);
	}
	else if(numbers[i][j]>=-14){
		explodeEtoile(*pgame, numbers, i, j);
	}
	else if(numbers[i][j]>=-21){
		explodePlus(*pgame, numbers, i, j);
		explodeEtoile(*pgame, numbers, i, j);
	}
	pgame->changed = 1;
	return;
}

/* @ requires a game and a array of int
 * @ assigns int i,j for the loops
 * @ ensures bombs explose when needed
 */
void parseBombs(struct Game game, array numbers){
	int i,j;
	for(i=0; i<game.height; i=i+1){
		for(j=0; j<game.width; j=j+1){
			explodeBombs(game, numbers, i, j);
		}
	}
	return;
}
/*----------DISPLAY-----------*/
/* @ requires number neddss to be a valid int
 * @ assigns int count to the number of figures of number
 * @ ensures number <= 9 
 */
int checkLenght(int number){
	int count;
	count =1;
	while(number>9){ 
		number = number/10;
		count = count+1;
	} /* if number is defined, number/10 must be inferior to 9 one time */
	return count;
}

/* @ requires Game.lenghtMax and number to be defined 
 * @ assigns an int i, print spaces to stdout
 * @ ensures (number of figures -1) to be printed in stdout
 */
void addSpace(struct Game game, int number){
	int i;
	if(number ==0){
		number = game.lenghtMax-1;
	}
	else{
		number = game.lenghtMax-checkLenght(number);
	}
	for(i=1; i<=number; i=i+1){
		printf(" ");
	}
	return;
}

/* @ requires game.* and numbers to be defined, numbers must be an array full of int
 * @ assigns int tmp,i,j , game.lenghtMax
 * @ ensures game.lenghtMax contains the numbers of figures of the biggest int in numbers
 */
int getLenghtMax(struct Game game, array numbers){
	int i,j;
	int tmp;
	game.lenghtMax = 1;
	for(i=0; i<game.width; i=i+1){
		for(j=0; j<game.height; j=j+1){
			tmp = checkLenght(numbers[j][i]);
			if(tmp > game.lenghtMax){
				game.lenghtMax = tmp;
			}
		}
		printf("\n");
	}
	return game.lenghtMax;
}

/* @ requires game.lenghtMax to be defined, and a positive int
 * @ assigns int i
 * @ ensures game.lenghtMax-1 hyphen are printed to stdout
 */
void addHyphen(struct Game game){
	int i;
	for(i=1; i<game.lenghtMax; i=i+1){
		printf("-");
	}
	return;
}

/* @ requires a letter representing a direction
 * @ assigns stdout
 * @ ensures the direction of the move is displayed
 */
void printDirections(char dir){
	if(dir=='d'){
		printf("%6s-->\n\n\n","");
	}
	else if(dir=='q'){
		printf("%6s<--\n\n\n","");
	}
	else if(dir=='s'){
		printf("%6s|\n%6sv\n\n","","");
	}
	else if(dir=='z'){
		printf("%6s^\n%6s|\n\n","","");
	}
	else{
		printf("\n\n\n");
	}
}

void printLine(struct Game game, int width){
	int i,j;
	printf("\n%5s","");
	for(i=1; i<width; i=i+1){
		printf("----");
		if(game.large==1){
			printf("--");
		}
		for(j=1;j<game.lenghtMax; j=j+1){
			printf("-");
		}
	}
	printf("----");
	if(game.large==1){
		printf("--");
	}
}

/* @ requires game.* and numbers to be defined, numbers must be an array full of int
 * @ assigns int i,k
 * @ ensures the display of a grid containing the values of numbers
 * Uses getLenghtMax addHyphen addSpace printDirections
 */
void grid(struct Game * pgame, int height, int width, array numbers, char dir, int BombI, int BombJ){
	int j;
	int i;
	pgame->lenghtMax = getLenghtMax(*pgame, numbers);
	system("clear");
	printf("\n"); 
	if(pgame->changed==0){
		printf(" You need to choose a valid move!\n\n");	
	}
	else{printf(" Your goal for this game : reach %d \n\n",pgame->goal);}
	printDirections(dir);
	pgame->changed=0;
	printf("    ");
	for(i=0; i<width; i=i+1){
		printf("|---");
		if(pgame->large==1){
				printf("%s","--");
		}
		addHyphen(*pgame);
	}
	for(i=0; i<height; i=i+1){
		if(pgame->large==1){
			printf("|\n    ");
			for(j=0; j<width; j=j+1){
				printf("|%5s","");
				addSpace(*pgame,0);
			}
		}
		printf("|\n%3d ",height-i);
		for(j=0; j<width; j=j+1){
			printf("|");
			if(pgame->large==1){
				printf("%1s","");
			}
			if(numbers[i][j]>0){
				addSpace(*pgame,numbers[i][j]);
				printf(" %d ",numbers[i][j]);
			}
			else if (numbers[i][j]==0){
				printf("   ");
				addSpace(*pgame,numbers[i][j]);
			}
			else if (numbers[i][j]>=-7)
			{
				addSpace(*pgame,1);
				if(i==BombI && j==BombJ){ printf("[");}
				else{printf(" ");}
				printf("%1c",'+');
				if(i==BombI && j==BombJ){ printf("]");}
				else{printf(" ");}
			}
			else if (numbers[i][j]>=-14)
			{
				addSpace(*pgame,1);
				if(i==BombI && j==BombJ){ printf("[");}
				else{printf(" ");}
				printf("%1c",'x');
				if(i==BombI && j==BombJ){ printf("]");}
				else{printf(" ");}
			}
			else if (numbers[i][j]>=-21)
			{
				addSpace(*pgame,1);
				if(i==BombI && j==BombJ){ printf("[");}
				else{printf(" ");}
				printf("%1c",'*');
				if(i==BombI && j==BombJ){ printf("]");}
				else{printf(" ");}
			}
			else{
				printf(" %1c ",'e');
				addSpace(*pgame,numbers[i][j]);
			}
			if(pgame->large==1){
				printf("%1s","");
			}
		}
		if(pgame->large==1){
			printf("|\n    ");
			for(j=0; j<width; j=j+1){
				printf("|%5s","");
				addSpace(*pgame,0);
			}
		}
		printf("|\n    ");
		for(j=0; j<width; j=j+1){
			printf("|---");
			if(pgame->large==1){
				printf("%s","--");
			}
			addHyphen(*pgame);
		}
	}
	printf("|\n     ");
	for(i=1; i<= pgame->width; i=i+1){
		addSpace(*pgame, 0);
		if(pgame->large==1){
			printf("%1s","");
		}
		printf("%2d  ",i);
		if(pgame->large==1){
			printf("%1s","");
		}
	}
	printLine(*pgame,pgame->width);
	printf("\n  ");
}

/*---------MOVE--------*/

/* @ requires pointers to the current case (number) and the next (benumber) and a Game
 * @ assigns new value to *number and *benumber
 * @ ensures the fusion of two bombs of the categories
 */
void moveBombe(struct Game * pgame, int * benumber, int * number){
	if(*benumber==0){
		pgame->changed=1;
		*benumber = *number;
		*number = 0;
	}
	else if(*benumber>0){return;}
	else if(*number >=-7 && *benumber >= -7){
			*benumber = -8;
			*number = 0;
			pgame->changed=1;
	}
	else if(*number >= -14 &&  *number <=-8 && *benumber >= -14 && *benumber <=-8){				
		*benumber = -15;
		*number = 0;
		pgame->changed=1;
	}
	return;
}

/* @ requires height and width to be defined, numbers an array full of int
 * @ assigns int to tnumbers[*][*]
 * @ ensures tnumbers is the transpose of numbers
 */
void transpose(int height, int width, array numbers, array tnumbers){
	int i,j;
	for(i=0; i<height; i=i+1){
		for(j=0; j<width; j=j+1){
			tnumbers[j][i]=numbers[i][j];
		}
	}
}

/* @ requires k the index of the spot in the direction on the move, pointers on the case to be changed
 * @ assigns pgame->changed *benumber an *number
 * @ ensures if we are in the right case, the fusion of two cases
 */
void moveFusion(struct Game *pgame, int k, int width, int * benumber, int * number){
	if(k<0){
		return;
	}
	else if (k>= width){
		return;
	}
	else if (*number<0){
		moveBombe(pgame, benumber, number);
		return;
	}
	else if(*benumber==0 || *number == *benumber){
		if(*benumber==0){
			pgame->changed=1;
			*benumber = *number;
			*number = 0;
		}
		else if(*number == *benumber){				
			*benumber = *number*2;
			*number = 0;
			pgame->changed=1;
		}
	}
	return;
}

/* @ requires game.* numbers to be defined, numbers full of int, dir a char in {z,q,s,d}
 * @ assigns int to numbers[x][x] , int i,j,k,l,tmp,height,width
 * @ ensures the int in nubers move in the dir selected
 */
void moveBegin(struct Game *pgame, array numbers, char dir){
	int i,j;
	int height,width;
	int tmp;
	height = pgame->height;
	width = pgame->width;
	/* if dir==z, then numbers is transposed, so width and height is inverted */
	if(dir=='z'){
		tmp = width;
		width = height;
		height = tmp;
	}
	int k;
	int l;
	for(i=0; i<height; i=i+1){
		l=0;
		for(j=0; j<width ;j=j+1){
			if(numbers[i][j] == 0){/*Simple translation is executed*/
				for(k=j; k<width-1; k=k+1){
					if(numbers[i][k+1]!=0){ pgame->changed=1;}
					numbers[i][k]=numbers[i][k+1];
					numbers[i][k+1]=0;
				}
				l=l+1;
				if(l>=width){break;}
				/* if we translated we might have still to move the new numbers[i][j] */
				j=j-1;
			}
			else{/* check the case after if it is empty or equal to [i][j] */
				k=j-1;	
				moveFusion(pgame,k, width, &numbers[i][k],&numbers[i][j]);
			}
		}

	}
}
/* @ requires game.* numbers to be defined, numbers full of int, dir a char in {z,q,s,d}
 * @ assigns int to numbers[x][x] , int i,j,k,l,tmp,height,width
 * @ ensures the int in nubers move in the dir selected
 */
void moveEnd(struct Game *pgame, array numbers, char dir){
	int i,j;
	int height,width;
	int tmp;
	height = pgame->height;
	width = pgame->width;
	/* if dir==s, then numbers is transposed, so width and height has to be inverted */
	if(dir=='s'){
		tmp = width;
		width = height;
		height = tmp;
	}
	int k=0;
	int l;
	for(i=0; i<height; i=i+1){
		l=0;
		for(j=width-1; j>=0 ;j=j-1){
			if(numbers[i][j] == 0){/*Simple translation is executed*/
				for(k=j; k>0; k=k-1){
					if(numbers[i][k-1]!=0){pgame->changed=1;}
					numbers[i][k]=numbers[i][k-1];
					numbers[i][k-1]=0;
				}
				l=l+1;
				if(l>=width){break;}
				/* if we translated we might have still to move the new numbers[i][j] */
				j=j+1;
			}
			else{ /* calling moveFusion to check the case after if it is empty or equal to [i][j] */
				k=j+1;	
				moveFusion(pgame,k,width,&numbers[i][k],&numbers[i][j]);
			}
		}
	}
}

/* @ requires game.* numbers to be defined, numbers full of int, dir a char in {z,q,s,d}
 * @ assigns int to numbers[x][x] , int i,j
 * @ ensures the int in nubers move in the dir selected by calling moveBegin and moveEnd
 */
void move(struct Game *pgame, array numbers, array tnumbers, char dir){
	switch(dir){
		case 'z':{ /*transpose numbers so we can use the same function as 'q'*/
			transpose(pgame->height,pgame->width,numbers,tnumbers);			
			moveBegin(pgame,tnumbers,dir);
			transpose(pgame->width,pgame->height,tnumbers,numbers);
			break;
		}
		case 'q':{
			moveBegin(pgame,numbers,dir);
			break;
		}
		case 's':{ /*transpose numbers so we can use the same function as 'd'*/
			transpose(pgame->height,pgame->width,numbers,tnumbers);
			moveEnd(pgame,tnumbers,dir);
			transpose(pgame->width,pgame->height,tnumbers,numbers);
			break;
		}
		case 'd':{
			moveEnd(pgame,numbers,dir);
			break;
		}
		default :{
			printf("Wrong direction selected\n");
			return;
		}
	}

}

/*----------SELECTION-------*/
/* requires
 * assigns char buffer[0]
 * ensures the return of the first char entered 
*/
char getChar(){
    char buffer[2]; /* 2 car 1 caractère PLUS le \0 des chaînes */
	system("/bin/stty -icanon");
    scanf("%1s", buffer);
	system("/bin/stty icanon");
    return tolower(buffer[0]);
}

/* requires
 * assigns char buffer[0] int temp
 * ensures the return of the first int entered 
*/
int getInt(){
	char buffer[6];
	int temp;
	system("/bin/stty -icanon");
    scanf("%s", buffer);
	system("/bin/stty icanon");
	temp =(int) strtol(buffer, NULL, 10);
    return temp;
}

/* @ requires a defined game
 * @ assigns int i,lenght
 * @ ensures enough spaces added to stdout
 */
void getSpaces(struct Game game){
	int i;
	int lenght = game.width - 7;
	if(game.large==1){lenght = game.width-5;}
	for(i=0; i<lenght; i=i+1){
		printf("%4s","");
		if(game.large==1){
			printf("%2s","");
		}
	}
	return;
}

/* requires pgame and numbers defined
 * assigns int h,w
 * ensures the chose of the bomb
*/
void selectBomb(struct Game *pgame, array numbers){
	int h, w;
	h=-1;
	w=-1;
	do{
		do{
			printf("\n");
			getSpaces(*pgame);
			printf("Enter a height <= %d %16s",pgame->height,"");
			h = pgame->height - getInt();
		}while( (h >= pgame->height) || (h < 0));
		do{
			printf("\n");
			getSpaces(*pgame);
			printf("Enter a width  <= %d %16s",pgame->width,"");
			w = getInt()-1;
		}while( (w >= pgame->width) || (w < 0));
		if(numbers[h][w]>=0){
			pgame->changed=0;
			return;
		}
		pgame->changed =1;
		grid(pgame,  pgame->height, pgame->width, numbers,'n',h,w);
		printf("\n\n");
		getSpaces(*pgame);
		printf("Press Y to explode [%d][%d] : %7s", pgame->height + h, w+1,"");
	}while( getChar() != 'y');
	chosenBombs(pgame, numbers, h, w);
	return;
}

/* @ requires game.* defined numbers a defined array full of int
 * @ assigns char dir
 * @ ensures if z,q,s,d is pressed, the arrays move in the desired direction by caling move
 */
void getMove(struct Game * pgame, array numbers, array tnumbers){
	char dir;
	do{
		do{
			printf("\n");
			getSpaces(*pgame);
			printf("Press  Z to move, B to pick a bomb  \n");
			getSpaces(*pgame);
			printf("      QSD                           ");
			dir = getChar();
		}while(dir!= 'z' && dir !='s' && dir !='q' && dir !='d' && dir!='b');
		pgame->changed =1;
		grid(pgame, pgame->height, pgame->width, numbers,dir,-1,-1);
		printf("\n\n");
		getSpaces(*pgame);
		printf("Confirm %c                           ",toupper(dir));
	}while(dir!=getChar());
	if(dir=='b'){
		selectBomb(pgame, numbers);
		return;
	}
	move(pgame, numbers, tnumbers, dir);
}

/*---------RAND---------*/
/* requires game.* defined, numbers array full of int
 * assigns int nb,i,j
 * ensures return nb = number of 0 in numbers
 */
int getNbEmpty(struct Game game, array numbers){
	int nb;
	int i,j;
	nb = 0;
	for(i=0; i<game.height; i=i+1){
		for(j=0; j<game.width; j=j+1){
			if(numbers[i][j]==0){
				nb=nb+1;
			}
		}
	}
	if(nb==0){
		return 0;
	}
	return nb;
}

/* @ requires a,b defined,  a<b
 * @ assigns 
 * @ ensures return of a random int in [a,b[
 */
int rand_a_b(int a, int b){
	srand(time(NULL)); /* Comment here for debug */
    return rand()%(b-a) +a;
}

/* @ requires game and numbers defined
 * @ assigns int i,j,max
 * @ ensures max is the value of the biggest int in numbers
 */
int getMax(struct Game game, array numbers){
	int i,j,max;
	max = 0;
	for(i=0; i<game.height; i=i+1){
		for(j=0; j<game.width; j=j+1){
			if(numbers[i][j]>max){
				max = numbers[i][j];
			}
		}
	}
	return max;
}

/* @ requires max to be the maximum element of numbers, *pnumber a 0 in numbers
 * @ assigns int rd (random selection on the new element)
 * @ ensures *pnumber is affected a random value
 */
void pickRandom(int max, int * pnumber){
	int rd;
	if(max > 256){
		rd = rand_a_b(0,21);
		if(rd <= 12){ *pnumber= 1;}
		else if(rd <= 17){ *pnumber = 2;}
		else if(rd <= 19){ *pnumber = -1; /*bombe +*/}
		else { *pnumber = -8;/*bonbe x*/}
	}
	else if (max>=64){
		rd = rand_a_b(0,11);
		if (rd <= 7){ *pnumber = 1;}
		else if (rd <= 9){	*pnumber = 2;}
		else { *pnumber = -1 ;/*bombe*/}
	}
	else{
		*pnumber = 1;
	}
	return;
}

/* requires game.*defined , numbers array full of int
 * assigns int nb,a,i,j  numbers[rand][rand]
 * ensures a randomly selected 0 in nupmbers is assigned 1
 */
void insertRandom(struct Game * pgame, array numbers){
	int nb,a,i,j;
	if(pgame->changed==0){
		return;
	}
	nb = getNbEmpty(*pgame,numbers);
	if(nb==0){
		pgame->changed=-1;
		return;
	}
	else if(nb>1){
		a = rand_a_b(1,nb);
	}
	else{ a=1;}
	for(i=0; i<pgame->height; i=i+1){
		for(j=0; j<pgame->width; j=j+1){
			if(numbers[i][j]==0){
				if(a>=2){
					a=a-1;
				}
				else{
					nb=getMax(*pgame,numbers);
					pickRandom(nb, &numbers[i][j]);
					return;
				}
			}
		}
	}
	return;
}

/*--------INIT---&&---STATUS------*/
/* @ requires *pgame to be defined
 * @ assigns game.height game.width stdout char temp
 * @ ensures game contain the desired values (default or not)
 */
void init(struct Game *pgame){
	char  temp;
	printf("\n %6s Welcome to Hadrien's 'deux-mille quarante-boom' \n\n","");
	printf("\n If you do not know what to do, restart the game with '-h' as parameter\n");
	do{
		printf("\nEnter your goal for this game : ");
		pgame->goal = getInt();
	}while(pgame->goal <= 0);
	printf("\nEnter Y if you wish to pick the size : ");
	temp = getChar();
	if(temp == 'y'){
		printf("\nEnter the height : ");
		pgame->height = getInt();
		printf("\nEnter the width  : ");
		pgame->width  = getInt();
	}
	else{ /* standard dimensions */
		pgame->height = 4;
		pgame->width = 5;
	}
	printf("\nEnter Y if you want a large grid : ");
	temp = getChar();
	if(temp =='y'){	pgame->large = 1;}
	else{ pgame->large = 0;}
	pgame->lenghtMax = 1;
	pgame->changed = 1;
}

/* @ requires game and numbers defined
 * @ assignes int i,j for the loop
 * @ ensures to return 0 if the goal is reached
 */
int statusGame(struct Game game, array numbers){
	int i,j;
	for(i=0; i<game.height; i=i+1){
		for(j=0; j<game.width; j=j+1){
			if(numbers[i][j]>=game.goal){
				return 0;
			}
		}
	}
	return 1;
}

/* @ requires height, width to be defined int, numbers to be an array, initialized or not
 * @ assigns int to numbers[x][y]
 * @ ensures numbers is an array full of 0
 */
void zero(int height, int width, array numbers){
	int i;
	int j;
	for(i=0;i<height;i=i+1){
		for(j=0;j<width;j=j+1){
			numbers[i][j] = 0;
		}
	}
}

/* @ requires name of a text file
 * @ assigns FILE *f and int c
 * @ ensures to print the text file
 */
static void displayFile(const char *file_name)
{
    FILE *f = fopen(file_name, "r");  
    if (f != NULL)
    {
        int c;

        while ((c = fgetc(f)) != EOF) 
        {
            putchar(c);                  
        }
        fclose(f);
    }
}

/* @ requires pointer to a game, numbers
 * @ assigns
 * @ ensures to tell the player he lost
 */
void lost(struct Game * pgame, array numbers){
	system("clear");
	printf("\n\n");
	grid(pgame, pgame->height, pgame->width, numbers,'n',-1,-1);
	printf("\n  Game Over !\n\n\n");
	return ;
}

/*---------------------------*/
/* requires 
 * assigns
 * ensures 
 */
int main(int argc, char const * argv[]){
	if(argc < 1){ return 1;}
	if(argc > 1){
		if(strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0){
			displayFile("readMe.txt");
			getChar();
		}
	}
	system("clear");
	struct Game game;
	struct Game * pgame;
	pgame = &game;
	init(pgame);
	array numbers = alloc(game.height,game.width);
	array tnumbers = alloc(game.width,game.height);
	zero(game.height, game.width, numbers);
	zero(game.width, game.height, tnumbers);

	insertRandom(pgame,numbers);
	grid(pgame, game.height, game.width, numbers,'n',-1,-1);
	/* Loop to ensures the game keeps going until the grid is full or the goal is reached*/
	while( statusGame(game,numbers)==1){
		getMove(pgame, numbers, tnumbers);
		parseBombs(game, numbers);
		tictacBombs(game, numbers);
		insertRandom(pgame, numbers);
		/* While the grid is full, the end is near ! */
		while(getNbEmpty(game, numbers)==0){
			system("clear");
			printf("\n\n");
			grid(pgame, game.height, game.width, numbers,'n',-1,-1);
			printf("\n%6sMight be your last move!\n","");
			getMove(pgame, numbers, tnumbers);
			parseBombs(game, numbers);
			tictacBombs(game, numbers);
			if( statusGame(game,numbers)==0){
				break;
			}
			if( getNbEmpty(game, numbers)==0 && pgame->changed==0){
				lost(pgame,numbers);
				free(numbers);
				free(tnumbers);
				return 0;
			}
			else{
				pgame->changed=1;
			}
			insertRandom(pgame,numbers);
		}
		grid(pgame, game.height, game.width, numbers,'n',-1,-1);
	}
	system("clear");
	printf("\n\n");
	pgame->changed=1;
	grid(pgame, game.height, game.width, numbers,'n',-1,-1);
	printf("\n  You WON !!!, you reached %d !!!\n\n\n",game.goal);
	free(numbers);
	free(tnumbers);
	return 0;
}

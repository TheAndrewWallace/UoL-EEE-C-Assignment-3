#include <stdio.h>
#include <stdlib.h>

#define ROW 5
#define COL 5

#define MARKER_1	'X'
#define MARKER_2	'O'
#define NO_MARKER	' '

#define NO_WINNER	1
#define WINNER_1	2
#define WINNER_2	3
#define DRAW		4

void create_board(char (*play_board)[COL]);									//Function creates board
int disp_menu1();															//Displays menu and gets input from user on what mode to play
void play_multiplayer(char (*play_board)[COL]);								//Function for when user selects multiplayer
void print_board(char (*play_board)[COL]);									//Function to print the board to the user
int check_board(char (*play_board)[COL], int i, int j, int opt2);			//Function checks board
void play_solo(char (*play_board)[COL], int opt2);							//Function used to pla against the computer
int disp_menu2();															//Displays menu and gets input from user on if player or computer plays first
int continue_check(char (*This_board)[COL]);								//Checks to see if the game is over, who has won and if the game should continue
int disp_menu3();															//Gets input from user to check if they want to play again
int tactical_move(char (*play_board)[COL], int *x, int *y, char marker);	//Checks to see if it can make winning move or, if not, block opponent

//Main Function
void main(void)
{
	char play_board[ROW][COL];
	int opt1, opt2, game_on = 1;
	
	//Will do until user enters to not replay
	do
	{
		create_board(play_board);
		opt1 = disp_menu1();
		
		//Selects option that user has chosen in previous function
		switch(opt1)
		{
			//Player Vs Player mode
			case 1:
				printf("\nPlayer Vs Player Mode Selected\n");
				play_multiplayer(play_board);
				break;
			
			//Player vs Computer
			case 2:
				printf("\nPlayer vs Computer Mode Selected\n");
				opt2 = disp_menu2();
				play_solo(play_board, opt2);
				break;
		}
		
		//Checks if user wants to play again
		game_on = disp_menu3();
		
	}while(game_on == 1);
}

//Creates the board
void create_board(char (*play_board)[COL])
{
	int i, j;
	
	for (i = 0; i < COL; i++)
	{
		for(j = 0; j < COL; j++)
		{
			play_board[i][j] = ' ';
		}
	}
}

//Menu to select mode
int disp_menu1()
{
	int option = 0;
	
	do
	{
		printf("MENU:");
		printf("\n1 - Multiplayer: Player vs Player");
		printf("\n2 - Single Player: Player vs Computer");
		printf("\nSelect an option: ");
		fflush(stdin);
		scanf("%d", &option);
	}while(option != 1 && option != 2);
	
	return option;
}

//Plays multiplayer
void play_multiplayer(char (*play_board)[COL])
{
	int x, y, game_over, cont = NO_WINNER, first, opt2 = 0;
	char marker = 'X';
		
	//Prints board after when game begins
	print_board(play_board);
	//Will do until game has concluded
	do
	{
		//Allows players to enter position until they enter valid position
		do
		{
			printf("\nEnter board position x: ");
			fflush(stdin);
			scanf("%d", &x);
			printf("Enter board position y: ");
			fflush(stdin);
			scanf("%d", &y);
		}while(check_board(play_board, x, y, opt2));
	
		//Enters chosen move
		play_board[x-1][y-1] = marker;
		
		
		//Changes which marker is being used to change who is the current player
		if(marker == MARKER_1)
		{
			printf("\nPlayer 2s turn:\n");
			marker = MARKER_2;
		}
		
		else
		{
			printf("\nPlayer 1s turn:\n");
			marker = MARKER_1;
		}
		
		//Prints board when move has been made
		print_board(play_board);
		
		//Checks to see if game should continue and output to user
		switch(continue_check(play_board))
		{
			case NO_WINNER:
				printf("\nThere is no winner yet\n");
				break;
			case WINNER_1:
				printf("\nPlayer 1 wins\n");
				cont = WINNER_1;
				break;
			case WINNER_2:
				printf("\nPlayer 2 wins\n");
				cont = WINNER_2;
				break;
			case DRAW:
				printf("\nDraw\n");
				cont = DRAW;
				break;
		}
		
	}while(cont == NO_WINNER);
}

//Prints board to user with numbers above and to side
void print_board(char (*play_board)[COL])
{
	int i, j;
	
	printf("\n\n");
	printf("   ");
	for (i = 0; i < COL; i++)
	{
		printf("  %d ", i+1);
	}
	printf("\n");
	
	for (i = 0; i < COL; i++)
	{
		printf("   ");
		for(j = 0; j < COL; j++)
		{
			printf("+---");
		}
		printf("+\n");
		
		printf(" %d ", i + 1);
		for(j = 0; j < COL; j++)
		{
			printf("| %c ", play_board[i][j]);
		}
		printf("|\n");
	}
	
	printf("   +");
	for(j = 0; j < COL; j++)
	{
		printf("---+");
	}
	printf("\n");
	
}

//Checks to see if position entered is valid
int check_board(char (*play_board)[COL], int i, int j, int opt2)
{
	//Checks if move is out of bounds
	if(i < 1 || j < 1 || i > ROW || j > COL)
	{
		//Will output only if user has made move and not computer
		if(opt2 == 1)
		{
			printf("\nPOSITION INVALID: Out of Bounds");
		}
		return -1;
	}
	
	//Checks if position is already taken
	if(play_board[i-1][j-1] != ' ')
	{
		//Will output only if user has made move and not computer
		if(opt2 == 1)
		{
			printf("\nPOSITION INVALID: Already Taken");
		}
		return -1;
	}
	
	return 0;
}

//Play against the computer
//Who plays first depends on previous option
void play_solo(char (*play_board)[COL], int opt2)
{
	int x, y, game_over, cont = NO_WINNER, first, decision_made = 0, undecided_num = 0, a;
	char marker = MARKER_1;
	char opp_marker = MARKER_2;
	
	printf("\nGame Mode Selected: Single Player\n\n");
		
		print_board(play_board);
		
		//Will continue until game has concluded
		do
		{
			//If it is players turn
			if(opt2 == 1)
			{
				do
				{
					printf("\nEnter board position x: ");
					fflush(stdin);
					scanf("%d", &x);
					printf("Enter board position y: ");
					fflush(stdin);
					scanf("%d", &y);
				}while(check_board(play_board, x, y, opt2));
				
				opt2 = 2;
				
			}
			
			//If it is not players turn then it is the computers
			else
			{
				//Do until valid move found
				do
				{
					//Creates a random move for computer
					x = rand() % 5;
					y = rand() % 5;
					x = x +1;
					y = y +1;
					
					
				}while(check_board(play_board, x, y, opt2));

				//Will replace move if a better tactical move can be made
				decision_made = 0;
				decision_made = tactical_move(play_board, &x, &y, marker);
				
				
				opt2 = 1;
			}
			
			//If there is a possible winning move, this will place that marker
			//If there is no possible winning move, but it can block opponent from winning, places that marker.
			//If there is no possible winning or blocking move, it places a random number
			play_board[x-1][y-1] = marker;
			
			//Changes the marker. This method means that X will always go first regardless of if it is player or computer
			if(marker == MARKER_1)
			{
				printf("\nPlayer 2s turn:\n");
				marker = MARKER_2;
				opp_marker = MARKER_1;
			}
			
			else
			{
				printf("\nPlayer 1s turn:\n");
				marker = MARKER_1;
				opp_marker = MARKER_2;
			}
			
			//Prints board
			print_board(play_board);
			
			//Checks to see if it should continue
			switch(continue_check(play_board))
			{
				case NO_WINNER:
					printf("\nThere is no winner\n");
					break;
				case WINNER_1:
					printf("\nPlayer 1 wins\n");
					cont = WINNER_1;
					break;
				case WINNER_2:
					printf("\nPlayer 2 wins\n");
					cont = WINNER_2;
					break;
				case DRAW:
					printf("\nDraw\n");
					cont = DRAW;
					break;
			}
			
		}while(cont == NO_WINNER);
}

//Checks if player or computer should move first
int disp_menu2()
{
	int option = 0;
	
	do
	{
		printf("Who plays first?");
		printf("\n1: Player");
		printf("\n2: Computer\n");
		fflush(stdin);
		scanf("%d", &option);
	}while(option != 1 && option != 2);
	
	return option;		//returns users option after valid input
}

//Checks to see if game should continue
int continue_check(char (*play_board)[COL])
{
	int i, j, marker_num_1, marker_num_2, empty_pos;
	
	//Checks for win in rows
	for(i=0; i<ROW; i++)
	{
		marker_num_1 = marker_num_2 = 0;
		for(j=0; j<COL;j++)
		{
			switch(play_board[i][j])
			{
				case MARKER_1:
					marker_num_1++;
					break;
				case MARKER_2:
					marker_num_2++;
					break;
			}
		}
		if(marker_num_1 == COL)
		{
			return WINNER_1;
		}
		if(marker_num_2 == COL)
		{
			return WINNER_2;
		}
	}
	
	//CHECKS FOR WIN IN COL
	for(j=0; j<ROW; j++)
	{
		marker_num_1 = marker_num_2 = 0;
		for(i=0; i<COL;i++)
		{
			switch(play_board[i][j])
			{
				case MARKER_1:
					marker_num_1++;
					break;
				case MARKER_2:
					marker_num_2++;
					break;
			}
		}
		if(marker_num_1 == COL)
		{
			return WINNER_1;
		}
		if(marker_num_2 == COL)
		{
			return WINNER_2;
		}
	}
	
	//Check diagonal
	marker_num_1 = marker_num_2 = 0;
	for(j=0; j<COL; j++)
	{
		switch(play_board[j][j])
		{
			case MARKER_1:
				marker_num_1++;
				break;
			case MARKER_2:
				marker_num_2++;
				break;
		}
	}
	
	if(marker_num_1 == COL)
	{
		return WINNER_1;
	}
	
	if(marker_num_2 == COL)
	{
		return WINNER_2;
	}
	
	//Check second diagonal
	marker_num_1 = marker_num_2 = 0;
	for(j=0; j<COL; j++)
	{
		switch(play_board[j][COL-1-j])
		{
			case MARKER_1:
				marker_num_1++;
				break;
			case MARKER_2:
				marker_num_2++;
				break;
		}
	}
	if(marker_num_1 == COL)
	{
		return WINNER_1;
	}
	
	if(marker_num_2 == COL)
	{
		return WINNER_2;
	}
	
	//Check if board is full
	empty_pos = 0;
	for(i=0; i<COL; i++)
	{
		for(j=0; j<COL; j++)
		{
			if(play_board[i][j] == ' ')
			{
				empty_pos++;
			}
		}
	}
	
	if(empty_pos == 0)
	{
		return DRAW;
	}
	
	else
	{
		return NO_WINNER;
	}
}

//Checks if user wants to play again
int disp_menu3()
{
	int option = 0;
	
	do
	{
		printf("Do you want to play again?");
		printf("\n1: Yes");
		printf("\n2: No\n");
		fflush(stdin);
		scanf("%d", &option);
	}while(option != 1 && option != 2);
	
	return option;
}

//
int tactical_move(char (*play_board)[COL], int *x, int *y, char marker)
{
	int i, j, z, marker_num_1, empty_pos, decision_made = 0;
	
	
	
	for(z=0; z<2; z++)
	{
	
		//Checks for possible win in rows
		for(i=0; i<ROW; i++)
		{
			marker_num_1 = empty_pos = 0;
			for(j=0; j<COL;j++)
			{

				if(play_board[i][j] == marker)
				{
					marker_num_1++;
				}
				
				if(play_board[i][j] == NO_MARKER)
				{
					empty_pos++;
				}
		
			}
			
			if(marker_num_1 == COL-1 && empty_pos == 1)
			{
				*x = i+1;
				for(j=0; j<COL; j++)
				{
					if(play_board[i][j] == NO_MARKER)
					{
						*y = j+1;
						decision_made = 1;
						return decision_made;
						
					}
				}
			}
		}
			
		
		//Checks for possible win in columns
		for(j=0; j<ROW; j++)
		{
			marker_num_1 = empty_pos = 0;
			for(i=0; i<COL;i++)
			{
				if(play_board[i][j] == marker)
				{
					marker_num_1++;
				}
				
				if(play_board[i][j] == NO_MARKER)
				{
					empty_pos++;
				}
				
			}
			if(marker_num_1 == COL-1 && empty_pos == 1)
			{
				*y = j+1;
				for(i=0; i<COL; i++)
				{
					if(play_board[i][j] == NO_MARKER)
					{
						*x = i+1;
						decision_made = 1;
						return 1;
					}
				}
			}
	
		}
		
		
		//Check diagonal
		marker_num_1 = empty_pos = 0;
		for(j=0; j<COL; j++)
		{
			
			if(play_board[j][j] == marker)
			{
				marker_num_1++;
			}
			
			if(play_board[j][j] == NO_MARKER)
			{
				empty_pos++;
			}
		}
		
		if(marker_num_1 == COL-1 && empty_pos == 1)
		{
	
			for(j=0; j<COL; j++)
			{	
				if(play_board[j][j] == NO_MARKER)
				{
					*x = j + 1;
					*y = j + 1;
				}
			}	
		}
		
		//Check second diagonal
		marker_num_1 = empty_pos = 0;
		for(j=0; j<COL; j++)
		{
			
			if(play_board[j][COL-j-1] == marker)
			{
				marker_num_1++;
			}
		
			if(play_board[j][COL-j-1] == NO_MARKER)
			{
				empty_pos++;
			}
		}
		
		if(marker_num_1 == COL-1 && empty_pos == 1)
		{
			for(j=0; j<COL; j++)
			{	
				if(play_board[j][COL-1-j] == NO_MARKER)
				{
					*x = j + 1;
					*y = COL-j;
				}
			}	
		}
		
		//Changes around what marker is being used for blocking move. This way computer can either be 'X' or 'O'
		if(marker == MARKER_1)
		{
			marker = MARKER_2;
		}
		
		else
		{
			marker = MARKER_1;
		}
	
	}

	return decision_made;
}

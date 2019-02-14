#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ai.h"
#include "functions.h"
#include <stdlib.h>

#include <signal.h>


int genmove(unsigned char boardsize, unsigned char **board, unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls,unsigned char enemywalls,unsigned char color)
{
	
	unsigned char mydistances[boardsize][boardsize];     //distance of every square on the board to my target position
	unsigned char enemydistances[boardsize][boardsize];  //distance of every square on the board to enemy's target position
	unsigned char newboard[boardsize][boardsize];
	unsigned char x,y,enemydistance;   //coordinates and register value
	
	
	//attach initial values, without obstacles, only the absolute distance
	for( y = 0;y<boardsize;y++)
	{
		enemydistance=boardsize-y-1;
	    for(x = 0;x<boardsize;x++)
		{
			mydistances[x][y]=y;
			enemydistances[x][y]=enemydistance;
			newboard[x][y]=0;  //init board
		}
	}
	
	//scanning each square for walls and reasign distances
	for( x = 0;x<boardsize;x++)
	    for(y = 0;y<boardsize;y++)
		{
			newboard[x][y]=board[y][x];//inverse x y
			putwallinmap(boardsize,newboard,mydistances,enemydistances,x,y);
		}
		
	if(color==2)
		return tree(boardsize,newboard,mydistances,enemydistances,enemyposy, enemyposx,  myposy, myposx,enemywalls,mywalls,5);
	else if(color==1)
		return tree(boardsize,newboard,enemydistances,mydistances,myposy, myposx,  enemyposy, enemyposx,mywalls,enemywalls,5);//just invert enemy's with mine for different color
	else if(color==3)
	  if(enemydistances[myposy][myposx]>198 || mydistances[enemyposy][enemyposx]>198)
	    return 0;
	  else
	    return 1;
	
}
		

void putwallinmap(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char x,unsigned char y)
{
	unsigned char entry1[2]={255,255},entry2[2]={255,255};
	unsigned char next=0;
	unsigned char a=255,b=255;
	if(board[x][y]==1)  //horizontal wall
	{
		//my dirty area cleaning
		if((mydistances[x][y]+1)==mydistances[x][y+1])
		{
			cleanrobot(boardsize,board, mydistances,x,y+1);//clean
			entryfinder(boardsize,board, mydistances,x,y+1,entry1);//find entry
			maprobot(boardsize, board, mydistances, entry1[0],entry1[1]);//my pathmapping 
		}
		else if((mydistances[x][y])==mydistances[x][y+1]+1)
		{
			cleanrobot(boardsize,board, mydistances,x,y);//clean
			entryfinder(boardsize,board, mydistances,x,y,entry1);//find entry
			maprobot(boardsize, board, mydistances, entry1[0],entry1[1]);//my pathmapping 
		}
				
		if((mydistances[x+1][y]+1)==mydistances[x+1][y+1])
		{
			cleanrobot(boardsize,board, mydistances,x+1,y+1);		
			entryfinder(boardsize,board, mydistances,x+1,y+1,entry2);
			maprobot(boardsize, board, mydistances, entry2[0],entry2[1]);
		}
		else if((mydistances[x+1][y])==mydistances[x+1][y+1]+1)
		{
			cleanrobot(boardsize,board, mydistances,x+1,y);
			entryfinder(boardsize,board, mydistances,x+1,y,entry2);
			maprobot(boardsize, board, mydistances, entry2[0],entry2[1]);
		}
		//enemy's dirty area cleaning
		entry1[0]=255,entry2[0]=255;
		entry1[1]=255,entry2[1]=255;
		
		if((enemydistances[x][y]+1)==enemydistances[x][y+1])
		{
			cleanrobot(boardsize,board, enemydistances,x,y+1);//clean
			entryfinder(boardsize,board, enemydistances,x,y+1,entry1);//find entry
			maprobot(boardsize, board, enemydistances, entry1[0],entry1[1]);//enemy's pathmapping 
		}
		else if((enemydistances[x][y])==enemydistances[x][y+1]+1)
		{
			cleanrobot(boardsize,board, enemydistances,x,y);//clean
			entryfinder(boardsize,board, enemydistances,x,y,entry1);//find entry
			maprobot(boardsize, board, enemydistances, entry1[0],entry1[1]);//enemy's pathmapping 
		}
				
		if((enemydistances[x+1][y]+1)==enemydistances[x+1][y+1])
		{
			cleanrobot(boardsize,board, enemydistances,x+1,y+1);		
			entryfinder(boardsize,board, enemydistances,x+1,y+1,entry2);
			maprobot(boardsize, board, enemydistances, entry2[0],entry2[1]);
		}
		else if((enemydistances[x+1][y])==enemydistances[x+1][y+1]+1)
		{
			cleanrobot(boardsize,board, enemydistances,x+1,y);
			entryfinder(boardsize,board, enemydistances,x+1,y,entry2);
			maprobot(boardsize, board, enemydistances, entry2[0],entry2[1]);
		}
	}
	
	else if (board[x][y]==2) //vertical wall
	{
		//my dirty area cleaning
		if((mydistances[x][y]+1)==mydistances[x+1][y])
		{
			cleanrobot(boardsize,board, mydistances,x+1,y);//clean
			entryfinder(boardsize,board, mydistances,x+1,y,entry1);//find entry
			maprobot(boardsize, board, mydistances, entry1[0],entry1[1]);//my pathmapping 
		}
		else if((mydistances[x][y])==mydistances[x+1][y]+1)
		{
			cleanrobot(boardsize,board, mydistances,x,y);//clean
			entryfinder(boardsize,board, mydistances,x,y,entry1);//find entry
			maprobot(boardsize, board, mydistances, entry1[0],entry1[1]);//my pathmapping 
		}
				
		if((mydistances[x][y+1]+1)==mydistances[x+1][y+1])
		{
			cleanrobot(boardsize,board, mydistances,x+1,y+1);		
			entryfinder(boardsize,board, mydistances,x+1,y+1,entry2);
			maprobot(boardsize, board, mydistances, entry2[0],entry2[1]);
		}
		else if((mydistances[x][y+1])==mydistances[x+1][y+1]+1)
		{
			cleanrobot(boardsize,board, mydistances,x,y+1);
			entryfinder(boardsize,board, mydistances,x,y+1,entry2);
			maprobot(boardsize, board, mydistances, entry2[0],entry2[1]);
		}
			
		//enemy's dirty area cleaning
		entry1[0]=255,entry2[0]=255;
		entry1[1]=255,entry2[1]=255;
		if((enemydistances[x][y]+1)==enemydistances[x+1][y])
		{
			cleanrobot(boardsize,board, enemydistances,x+1,y);//clean
			entryfinder(boardsize,board, enemydistances,x+1,y,entry1);//find entry
			maprobot(boardsize, board, enemydistances, entry1[0],entry1[1]);//enemy's pathmapping 
		}
		else if((enemydistances[x][y])==enemydistances[x+1][y]+1)
		{
			cleanrobot(boardsize,board, enemydistances,x,y);//clean
			entryfinder(boardsize,board, enemydistances,x,y,entry1);//find entry
			maprobot(boardsize, board, enemydistances, entry1[0],entry1[1]);//enemy's pathmapping 
		}
				
		if((enemydistances[x][y+1]+1)==enemydistances[x+1][y+1])
		{
			cleanrobot(boardsize,board, enemydistances,x+1,y+1);		
			entryfinder(boardsize,board, enemydistances,x+1,y+1,entry2);
			maprobot(boardsize, board, enemydistances, entry2[0],entry2[1]);
		}
		else if((enemydistances[x][y+1])==enemydistances[x+1][y+1]+1)
		{
			cleanrobot(boardsize,board, enemydistances,x,y+1);
			entryfinder(boardsize,board, enemydistances,x,y+1,entry2);
			maprobot(boardsize, board, enemydistances, entry2[0],entry2[1]);
		}
	}
}
int tree(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls,unsigned char enemywalls, signed char depth)
{
	unsigned char closecheck[boardsize][boardsize];
	unsigned char mynewdistances[boardsize][boardsize];
	unsigned char enemynewdistances[boardsize][boardsize];
	unsigned char x,y,mytarget=0,enemytarget=0;
	int bs=boardsize*boardsize*sizeof(unsigned char);
	int value=999, minvalue=1000,maxvalue=-1000;
	unsigned char outx,outy,wall;
	unsigned char legal_moves[13][2];
	unsigned char setheuristic;
	
	if(mydistances[3][0]==0)enemytarget=boardsize-1;//define target y
	else mytarget=boardsize-1;
	
	memset(closecheck,0,bs);//init closecheck
	
	if(boardsize<14)depth=4;// adapt depth to boardsize
	else depth=2;
	
	if((boardsize>10)&&(boardsize<16))setheuristic=1;//adapt heuristic prunning to boardsize and depth
	else if(boardsize>18)setheuristic=1;
	else setheuristic=0;
	if(boardsize ==5 )setheuristic=1;
	//simple moves
	find_legal_moves(boardsize,board,0,legal_moves,enemyposy,enemyposx,myposy,myposx);
	for(x=0;(x<13)&&(legal_moves[x][0]!=255);x++)
	{
		if(mydistances[myposx][myposy]>=mydistances[legal_moves[x][1]][legal_moves[x][0]])
		{
			value = branch(boardsize,board,enemydistances,mydistances,enemyposx, enemyposy, legal_moves[x][1],legal_moves[x][0],enemywalls,mywalls,depth-1);
			
			if(minvalue>value)
			{
				minvalue=value;
				outx=legal_moves[x][0],outy=legal_moves[x][1] ,wall=0;
			}
		}
	}
	
	//find min and max for every wall
	if(mywalls>0){
	for(  x = 0;x+1<boardsize;x++)
        for( y = 0;y+1<boardsize;y++)
	{
	
		if(board[x][y]==0)
		{
			if ((board[x-1][y]!=1)&&(board[x+1][y]!=1))//if wall is legal
			{
				//copying board and distances for use on the next level
					memcpy(mynewdistances,mydistances,bs);
					memcpy(enemynewdistances,enemydistances,bs);
				
				board[x][y]=1;
				putwallinmap(boardsize,board,mynewdistances,enemynewdistances,x,y);
				if((mynewdistances[myposx][myposy]<199)&&(enemynewdistances[enemyposx][enemyposy]<199))//not closing paths
					if(closetracer(boardsize, board, closecheck, myposx, myposy, mytarget)==1)//ensure openness
				{
					memset(closecheck,0,bs);
					if(closetracer(boardsize, board, closecheck, enemyposx, enemyposy, enemytarget)==1)
					{
						memset(closecheck,0,bs);
						
						if((setheuristic==0)||((mynewdistances[myposx][myposy]-enemynewdistances[enemyposx][enemyposy])<(mydistances[myposx][myposy]-enemydistances[enemyposx][enemyposy])))//heuristic prunning
						{
							value = branch(boardsize,board,enemynewdistances,mynewdistances,enemyposx, enemyposy,  myposx, myposy,enemywalls,mywalls-1,depth-1);
							if(minvalue>value)
							{
								minvalue=value;
								outx=x,outy=y,wall=1;
							}
						}

					}
				}
			
			}
			if ((board[x][y-1]!=2)&&(board[x][y+1]!=2))//if legal wall
			{
				//copying board and distances for use on the next level
					memcpy(mynewdistances,mydistances,bs);
					memcpy(enemynewdistances,enemydistances,bs);
				
				board[x][y]=2;
				putwallinmap(boardsize,board,mynewdistances,enemynewdistances,x,y);
				if((mynewdistances[myposx][myposy]<199)&&(enemynewdistances[enemyposx][enemyposy]<199))
					if(closetracer(boardsize, board, closecheck, myposx, myposy, mytarget)==1)
				{
					memset(closecheck,0,bs);
					if(closetracer(boardsize, board, closecheck, enemyposx, enemyposy, enemytarget)==1)
					{
						memset(closecheck,0,bs);
					
						if((setheuristic==0)||((mynewdistances[myposx][myposy]-enemynewdistances[enemyposx][enemyposy])<(mydistances[myposx][myposy]-enemydistances[enemyposx][enemyposy])))//heuristic prunning
						{
							value = branch(boardsize,board,enemynewdistances,mynewdistances,enemyposx, enemyposy,  myposx, myposy,enemywalls,mywalls-1,depth-1);
							if(minvalue>value)
							{
								minvalue=value;
								outx=x,outy=y,wall=2;
							}
						}

					}
				}
				
			}
			board[x][y]=0;
		}
	}
	
	          }
	
		value=wall;
		minvalue=value<<16;
		value=outy;
		value=value<<8;
		minvalue=minvalue|value;
		value=outx;
		return (minvalue|value);
}

int branch(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls,unsigned char enemywalls, signed char depth)
{
	//unsigned char newboard[boardsize][boardsize];
	unsigned char mynewdistances[boardsize][boardsize];
	unsigned char enemynewdistances[boardsize][boardsize];
	unsigned char x,y;
	int bs=boardsize*boardsize*sizeof(unsigned char);
	int value, minvalue=1000,maxvalue=-1000;
	unsigned char maxx,maxy,mwall=0;
	unsigned char legal_moves[13][2];
	
	if(mydistances[myposx][myposx]==0)return 999;//win situation
	if(enemydistances[enemyposx][enemyposy]==0)return -999;//loose situation
	
	if(depth==0)//if at the leaves of the tree count heuristic and return
		{
			return enemydistances[enemyposx][enemyposy]-mydistances[myposx][myposy];//heuristic max for the current player min for the previous
		}


	
	find_legal_moves(boardsize,board,0,legal_moves,enemyposy,enemyposx,myposy,myposx);
	for(x=0;(x<13)&&(legal_moves[x][0]!=255);x++)
	{
		if(mydistances[myposx][myposy]>=mydistances[legal_moves[x][1]][legal_moves[x][0]])
		{
			value = branch(boardsize,board,enemydistances,mydistances,enemyposx, enemyposy, legal_moves[x][1],legal_moves[x][0],enemywalls,mywalls,depth-1);
			
			if(minvalue>value)
				minvalue=value;
		}
	}
	
	
	if(mywalls>0){
	for(  x = 0;x+1<boardsize;x++)
        for( y = 0;y+1<boardsize;y++)
	{
	
		if(board[x][y]==0)
		{
			if ((board[x-1][y]!=1)&&(board[x+1][y]!=1))//if wall is legal
			{
				//copying board and distances for use on the next level
					memcpy(mynewdistances,mydistances,bs);
					memcpy(enemynewdistances,enemydistances,bs);
				
				board[x][y]=1;
				putwallinmap(boardsize,board,mynewdistances,enemynewdistances,x,y);
				if((mynewdistances[myposx][myposy]<199)&&(enemynewdistances[enemyposx][enemyposy]<199))
					if((mynewdistances[myposx][myposy]-enemynewdistances[enemyposx][enemyposy])<(mydistances[myposx][myposy]-enemydistances[enemyposx][enemyposy]))//heuristic prunning
					{
						value = branch(boardsize,board,enemynewdistances,mynewdistances,enemyposx, enemyposy,  myposx, myposy,enemywalls,mywalls-1,depth-1);
						if(minvalue>value)
							minvalue=value;
				
					}

			}
			if ((board[x][y-1]!=2)&&(board[x][y+1]!=2))//if legal wall
			{
				//copying board and distances for use on the next level
					memcpy(mynewdistances,mydistances,bs);
					memcpy(enemynewdistances,enemydistances,bs);
				
				board[x][y]=2;
				putwallinmap(boardsize,board,mynewdistances,enemynewdistances,x,y);
				if((mynewdistances[myposx][myposy]<199)&&(enemynewdistances[enemyposx][enemyposy]<199))
					if((mynewdistances[myposx][myposy]-enemynewdistances[enemyposx][enemyposy])<(mydistances[myposx][myposy]-enemydistances[enemyposx][enemyposy]))
					{
						value = branch(boardsize,board,enemynewdistances,mynewdistances,enemyposx, enemyposy,  myposx, myposy,enemywalls,mywalls-1,depth-1);
						
						if(minvalue>value)
							minvalue=value;
						
					}
			}
			board[x][y]=0;
		}
	}
	
	}		   
			   
	return -minvalue;// inverting with - : max is previous player's min 
}



void cleanrobot(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y)
{
	///////////no walls above///////
	if((y>0)&&(board[x][y-1]!=1))if(((x>0)&&(board[x-1][y-1]!=1))||(x==0))
	{
		if(distances[x][y]+1==distances[x][y-1])////currentsquare connects to above//////
		{
			cleanrobot(boardsize, board, distances, x, y-1);
		}
	}
	
	////////no walls left///////
	if((x>0)&&(board[x-1][y]!=2))if((y==0)||((y>0)&&(board[x-1][y-1]!=2)))
	{
		if(distances[x][y]+1==distances[x-1][y])//////currentsquare connects to left///////
		{
			cleanrobot(boardsize, board, distances, x-1, y);
		}
	}
	
	/////////////no walls right///////
	if((x+1<boardsize)&&(board[x][y]!=2))if(((y>0)&&(board[x][y-1]!=2))||(y==0))
	{
		if(distances[x][y]+1==distances[x+1][y])///////currentsquare connects to right//////
		{
			cleanrobot(boardsize, board, distances, x+1, y);
		}
	}
	
	////////////////no walls below///////
	if((y+1<boardsize)&&(board[x][y]!=1))if((x==0)||(board[x-1][y]!=1))
	{
		if(distances[x][y]+1==distances[x][y+1])///////currentsquare connects to below/////
		{
			cleanrobot(boardsize, board, distances, x, y+1);
		}
	}
	distances[x][y]=200; //actual cleaning
}


void entryfinder(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y,unsigned char entry[])
{
	unsigned char possibleentry[2]={255,255};
	
	distances[x][y]=199; //mark as checked
	
	///////////no walls above///////
	if((y>0)&&(board[x][y-1]!=1))if(((x>0)&&(board[x-1][y-1]!=1))||(x==0))
	{
		if(distances[x][y-1]==200)////currentsquare connects to above//////
		{
			entryfinder(boardsize, board, distances, x, y-1,possibleentry);
		}
		else
		{
			possibleentry[0]=x;
			possibleentry[1]=y-1;
		}
		if(possibleentry[0]!=255)
		{
			if((entry[0]==255)||(distances[entry[0]][entry[1]]>distances[possibleentry[0]][possibleentry[1]]))
			{
			entry[0]=possibleentry[0];
			entry[1]=possibleentry[1];
			}
		}
	
	}
	
	////////no walls left///////
	if((x>0)&&(board[x-1][y]!=2))if((y==0)||((y>0)&&(board[x-1][y-1]!=2)))
	{
		if(distances[x-1][y]==200)//////currentsquare connects to left///////
		{
			entryfinder(boardsize, board, distances, x-1, y,possibleentry);
		}
		else
		{
			possibleentry[0]=x-1;
			possibleentry[1]=y;
		
		}
		if(possibleentry[0]!=255)
		{
			if((entry[0]==255)||(distances[entry[0]][entry[1]]>distances[possibleentry[0]][possibleentry[1]]))
			{
			entry[0]=possibleentry[0];
			entry[1]=possibleentry[1];
			
			}
		}
	}
	
	/////////////no walls right///////
	if((x+1<boardsize)&&(board[x][y]!=2))if(((y>0)&&(board[x][y-1]!=2))||(y==0))
	{
		if(distances[x+1][y]==200)///////currentsquare connects to right//////
		{
			entryfinder(boardsize, board, distances, x+1, y,possibleentry);
		}
		else
		{
			possibleentry[0]=x+1;
			possibleentry[1]=y;
		}
		if(possibleentry[0]!=255)
		{
			if((entry[0]==255)||(distances[entry[0]][entry[1]]>distances[possibleentry[0]][possibleentry[1]]))
			{
			entry[0]=possibleentry[0];
			entry[1]=possibleentry[1];
			}
		}
		
		
	}
	
	////////////////no walls below///////
	if((y+1<boardsize)&&(board[x][y]!=1))if(((x>0)&&(board[x-1][y]!=1))||(x==0))
	{
		if(distances[x][y+1]==200)///////currentsquare connects to below/////
		{
			entryfinder(boardsize, board, distances, x, y+1,possibleentry);
		}
		else
		{
			possibleentry[0]=x;
			possibleentry[1]=y+1;
		}
		if(possibleentry[0]!=255)
		{
			if((entry[0]==255)||(distances[entry[0]][entry[1]]>distances[possibleentry[0]][possibleentry[1]]))
			{
			entry[0]=possibleentry[0];
			entry[1]=possibleentry[1];
			}
		}
	}
}
unsigned char legalwall(unsigned char boardsize, unsigned char board[][boardsize],unsigned char x, unsigned char y, unsigned char orientation)
{
	if(board[x][y]==0)
	{
		if(orientation==1)
		{
			if ((board[x-1][y]!=1)&&(board[x+1][y]!=1))return 1;
		}
		else if(orientation==2)
		{
			if ((board[x][y-1]!=2)&&(board[x][y+1]!=2))return 1;
		}
	}
	return 0;
}

void maprobot(unsigned char boardsize, unsigned char board[][boardsize], unsigned char distances[][boardsize], unsigned char x,unsigned char y)
{
	///////////no walls above///////
	if((y>0)&&(board[x][y-1]!=1))if(((x>0)&&(board[x-1][y-1]!=1))||(x==0))
	{
		
		if(distances[x][y]+1<distances[x][y-1])////currentsquare < to above//////
		{
			distances[x][y-1]=distances[x][y]+1;
			maprobot(boardsize, board, distances, x, y-1);
		}
		else if(distances[x][y]>1+distances[x][y-1])
		{
			distances[x][y]=distances[x][y-1]+1;
			maprobot(boardsize, board, distances, x, y);
		}
	}
	
	////////no walls left///////
	if((x>0)&&(board[x-1][y]!=2))if((y==0)||((y>0)&&(board[x-1][y-1]!=2)))
	{
		
		if(distances[x][y]+1<distances[x-1][y])////currentsquare < to above//////
		{
			distances[x-1][y]=distances[x][y]+1;
			maprobot(boardsize, board, distances, x-1, y);
		}
		else if(distances[x][y]>1+distances[x-1][y])
		{
			distances[x][y]=distances[x-1][y]+1;
			maprobot(boardsize, board, distances, x, y);
		}
	}
	
	/////////////no walls right///////
	if((x+1<boardsize)&&(board[x][y]!=2))if(((y>0)&&(board[x][y-1]!=2))||(y==0))
	{
		
		if(distances[x][y]+1<distances[x+1][y])////currentsquare < to above//////
		{
			distances[x+1][y]=distances[x][y]+1;
			maprobot(boardsize, board, distances, x+1, y);
		}
		else if(distances[x][y]>1+distances[x+1][y])
		{
			distances[x][y]=distances[x+1][y]+1;
			maprobot(boardsize, board, distances, x, y);
		}
		
	}
	
	////////////////no walls below///////
	if((y+1<boardsize)&&(board[x][y]!=1))if(((x>0)&&(board[x-1][y]!=1))||(x==0))
	{
		
		if(distances[x][y]+1<distances[x][y+1])////currentsquare < to above//////
		{
			distances[x][y+1]=distances[x][y]+1;
			maprobot(boardsize, board, distances, x, y+1);
		}
		else if(distances[x][y]>1+distances[x][y+1])
		{
			distances[x][y]=distances[x][y+1]+1;
			maprobot(boardsize, board, distances, x, y);
		}
		
	}
}

int closetracer(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y,unsigned char target)
{
	int door;
	if(y==target)return 1;//found way out
	
	
	distances[x][y]=1;//mark current square as tracked
	
	
	///////////no walls above///////
	if((y>0)&&(board[x][y-1]!=1))if(((x>0)&&(board[x-1][y-1]!=1))||(x==0))
	{
		//printf("in\n");
		if(1!=distances[x][y-1])////not scanned square above//////
		{
			if(closetracer(boardsize, board, distances, x, y-1,target)==1)return 1;
		}
	}
	
	////////no walls left///////
	if((x>0)&&(board[x-1][y]!=2))if((y==0)||((y>0)&&(board[x-1][y-1]!=2)))
	{
		if(1!=distances[x-1][y])//////not scanned quare left///////
		{
			if(closetracer(boardsize, board, distances, x-1, y,target)==1)return 1;
		}
	}
	
	/////////////no walls right///////
	if((x+1<boardsize)&&(board[x][y]!=2))if(((y>0)&&(board[x][y-1]!=2))||(y==0))
	{
		if(1!=distances[x+1][y])///////not scanned square right//////
		{
			if(closetracer(boardsize, board, distances, x+1, y,target)==1)return 1;
		}
	}
	
	////////////////no walls below///////
	if((y+1<boardsize)&&(board[x][y]!=1))if((x==0)||(board[x-1][y]!=1))
	{
		if(1!=distances[x][y+1])///////not scanned square below/////
		{
			if(closetracer(boardsize, board, distances, x, y+1,target)==1)return 1;
		}
	}
	return 0;//no way out
}



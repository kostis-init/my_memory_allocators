#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "ai.h"
#include "../myallocators/myallocators.h"
#include <stdlib.h>
#include <signal.h>


int main(void)
{

	////////
  struct history *historyptr,*startptr;/*all structs have been declared in functions.h*/
  unsigned char boardsize=9, newboardsize, bwalls=10, wwalls=10,i,j,**board,cmd[50], *token, *tempp, color, wall,bpi,bpj,wpi,wpj;
  const unsigned char *commands[]={"name","known_command","list_commands","quit","boardsize","clear_board","walls","playmove","playwall","genmove","undo","winner","showboard"};
  int temp;
  int outwall,outx,outy,outmove;//for genmove
  if((board=mymalloc(boardsize*sizeof(unsigned char *)))==NULL)  /*default allocation*/
    return -1;
  for(i=0;i<boardsize;i++)
    if((board[i]=mymalloc(boardsize*sizeof(unsigned char)))==NULL)
      return -1;
    
	  
    if((startptr=mymalloc(sizeof(struct history)))==NULL)
      return -1;
    startptr->num=0;
    startptr->next=NULL;
    historyptr=startptr;
	  
	  
  do
  {  
    
	fgets(cmd,sizeof(cmd),stdin);/*get the line*/
	
	if((tempp=strchr(cmd,'#'))!=NULL)/*discard the comments*/
	  *tempp=0;
	
	token=strtok(cmd," \t\n");/*keep the first word*/
	
	if(token==NULL)/*discard empty lines(or only-comment lines)*/
	  continue;
	else if(!strcmp(token,"boardsize"))   			/*****boardsize command*****/
	{
	  token=strtok(NULL," \t\n");/*keep the size*/
	  if(token==NULL || (newboardsize=atoi(token))<3 || newboardsize>25 || newboardsize%2==0)    
	  {
	    printf("? unacceptable size\n\n");
	    fflush(stdout);
	    continue;
	  }
	  
	  for(i=0;i<boardsize;i++)/*free the last board*/
	    myfree(board[i]);
	  myfree(board);
	  
	  if((board=mymalloc(newboardsize*sizeof(unsigned char *)))==NULL) /*allocate for new size*/
	    return -1;
	  for(i=0;i<newboardsize;i++)
	    if((board[i]=mymalloc(newboardsize*sizeof(unsigned char)))==NULL)
	      return -1;
	  boardsize=newboardsize;
	  printf("=\n\n");
	  fflush(stdout);
	  if(clear_board(boardsize,board,&bpi,&bpj,&wpi,&wpj,&startptr)==-1)
	    return -1;
	  historyptr=startptr;
	}
	else if(!strcmp(token,"clear_board"))			/*****clear_board command*****/
	{
	  if(clear_board(boardsize,board,&bpi,&bpj,&wpi,&wpj,&startptr)==-1)
	    return -1;
	  historyptr=startptr;
	  printf("=\n\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"walls"))				/*****walls command*****/
	{
	  token=strtok(NULL," \t\n");/*keep the number of walls*/
	  if(token==NULL || (bwalls=wwalls=atoi(token))==0)
	  {
	    printf("? unacceptable number of walls given\n\n");
	    fflush(stdout);
	    continue;
	  }
	  printf("=\n\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"name"))				/*****name command*****/
	{
	  printf("= 15-098\n\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"known_command"))			/*****known_command command*****/
	{
	  token=strtok(NULL," \t\n");/*keep the command*/
	  if(token==NULL)
	  {
	    printf("= false\n\n");
	    fflush(stdout);
	    continue;
	  }
	  for(i=0;i<13;i++)
	    if(!strcmp(token,commands[i]))
	    {
	      printf("= true\n\n");
	      fflush(stdout);
	      break;
	    }
	  if(i==13)
	    printf("= false\n\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"list_commands"))			/*****list_commands command*****/
	{
	  printf("= ");
	  for(i=0;i<13;i++)
	    printf("%s\n",commands[i]);
	  printf("\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"quit"))				/*****quit command*****/
	{
	  printf("=\n\n");
	  fflush(stdout);
	  break;
	}
	else if(!strcmp(token,"playmove"))			/*****playmove command*****/
	{
	  token=strtok(NULL," \t\n");/*keep the color*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  convert(token);
	  if(!strcmp(token,"w") || !strcmp(token,"white"))
	    color=2;
	  else if(!strcmp(token,"b") || !strcmp(token,"black"))
	    color=1;
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  
	  token=strtok(NULL," \t\n");/*keep the vertex*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  convert(token);
	  if(token[0] >= 'a' && token[0] <= 'y' - (25-boardsize))
	  {
	    j = token[0] - 'a';
	    token++;
	  }
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  if((i=atoi(token))<=0 || i>boardsize)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  i = boardsize - i; /*i,j express the pawns' wanted position in the board*/
	  
	  if(move(boardsize,board,color,i,j,&historyptr,&bpi,&bpj,&wpi,&wpj,0)==-1)
	    return -1;
	}
	else if(!strcmp(token,"playwall"))			/*****playwall command*****/
	{      
	  token=strtok(NULL," \t\n");/*keep the color*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	 
	  convert(token);
	  if(!strcmp(token,"w") || !strcmp(token,"white"))
	    color=2;
	  else if(!strcmp(token,"b") || !strcmp(token,"black"))
	    color=1;
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  
	  token=strtok(NULL," \t\n");/*keep the vertex*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  convert(token);
	  if(token[0] >= 'a' && token[0] <= 'y' - (25-boardsize))
	  {
	    j = token[0] - 'a';
	    token++;
	  }
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  if((i=atoi(token))<=0 || i>boardsize)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  i = boardsize - i; /*i,j express the wall's wanted position in the board*/
	  
	  token=strtok(NULL," \t\n");/*keep the orientation*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  convert(token);
	  if(!strcmp(token,"horizontal") || !strcmp(token,"h"))
	    wall=1;
	  else if(!strcmp(token,"vertical") || !strcmp(token,"v"))
	    wall=2;
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }	  
	  if(wall_placement(boardsize,board,color,wall,&bwalls,&wwalls,i,j,&historyptr,bpi,bpj,wpi,wpj,0)==-1)
	    return -1;	  
	}
	else if(!strcmp(token,"genmove"))			/*****genmove command*****/
	{
	  token=strtok(NULL," \t\n");/*keep the color*/
	  if(token==NULL)
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  convert(token);
	  if(!strcmp(token,"w") || !strcmp(token,"white"))
	    color=2;
	  else if(!strcmp(token,"b") || !strcmp(token,"black"))
	    color=1;
	  else
	  {
	    printf("? syntax error\n\n");
	    fflush(stdout);
	    continue;
	  }
	  
		
	  outmove=genmove(boardsize, board, bpi,bpj,wpi,wpj,bwalls,wwalls,color);
	  outy=outmove&0xff;
	  outmove=outmove>>8;
	  outx=outmove&0xff;
	  outwall=outmove>>8;
	  
	  if(outwall==0)
	  {
	    if(move(boardsize,board,color,outy,outx,&historyptr,&bpi,&bpj,&wpi,&wpj,1)==-1)
	      return -1;
	    printf("= %c%i\n\n",'A'+outx,boardsize-outy);
	    fflush(stdout);
	  }
	  else
	  {
	    if(wall_placement(boardsize,board,color,outwall,&bwalls,&wwalls,outx,outy,&historyptr,bpi,bpj,wpi,wpj,1)==-1)
	      return -1;
	    printf("= %c%i %c\n\n",'A'+outy,boardsize-outx,(outwall==1)?('h'):('v'));
	    fflush(stdout);
	  }
	    
	}
	else if(!strcmp(token,"undo"))				/*****undo command*****/
	{
	  int times;
	  token=strtok(NULL," \t\n");
	  if(token==NULL)
	    times=1;
	  else
	    times=atoi(token);
	  
	  if(times>historyptr->num)
	    printf("? cannot undo\n\n");
	  else
	  {
	    for(i=1;i<=times;i++)
	    {
	      undo(boardsize,board,&bwalls,&wwalls,&historyptr,startptr,&bpi,&bpj,&wpi,&wpj);
	    }
	    printf("=\n\n");
	  }
	  fflush(stdout);
	}
	else if(!strcmp(token,"winner"))			/*****winner command*****/
	{
	  if(bpi==boardsize-1)
	    printf("= true black\n\n");
	  else if(wpi==0)
	    printf("= true white\n\n");
	  else
	    printf("= false\n\n");
	  fflush(stdout);
	}
	else if(!strcmp(token,"showboard"))			/*****showboard command*****/
	{
	  printf("=\n\n    ");
	  for(i=0;i<boardsize;i++)
	    printf(" %c  ",'A'+i);
	  printf("   \n   +");
	  for(i=0;i<boardsize;i++)
	    printf("---+");
	  printf("   \n");
	  
	  for(i=boardsize;i>0;i--)
	  {
	    printf(" %2d |",i);
	    for(j=0;j<boardsize;j++)
	    {
	      if(board[boardsize-i][j]!=2 && (i==boardsize || board[boardsize-i-1][j]!=2))
	      {
		if((bpi!=boardsize-i || bpj!=j)&&(wpi!=boardsize-i || wpj!=j))
		  printf("   |");
		else if(bpi==boardsize-i && bpj==j)
		  printf(" B |");
		else if(wpi==boardsize-i && wpj==j)
		  printf(" W |");
	      }
	      else
	      {
		if((bpi!=boardsize-i || bpj!=j)&&(wpi!=boardsize-i || wpj!=j))
		  printf("   H");
		else if(bpi==boardsize-i && bpj==j)
		  printf(" B H");
		else if(wpi==boardsize-i && wpj==j)
		  printf(" W H");
	      }
	    }
	    if(i==boardsize)
	      printf(" %d  black walls: %d\n   +",i,bwalls);
	    else if(i==boardsize-1)
	      printf(" %d  white walls: %d\n   +",i,wwalls);
	    else
	      printf(" %d \n   +",i);
	    
	    for(j=0;j<boardsize;j++)
	    {
	      if(board[boardsize-i][j]!=1 && (j==0 || board[boardsize-i][j-1]!=1))
		printf("---");
	      else
		printf("===");
	      
	      if(board[boardsize-i][j]==1)
		printf("=");
	      else if(board[boardsize-i][j]==2)
		printf("H");
	      else
		printf("+");		
	    }
	    printf("   \n");
	  }
	  
	  printf("    ");
	  for(i=0;i<boardsize;i++)
	    printf(" %c  ",'A'+i);
	  printf("   \n\n");
	  fflush(stdout);
	}
	else
	  printf("? unknown command\n\n");    
  }while(1);
  fflush(stdout);
  
  for(i=0;i<boardsize;i++)/*free the last board*/
    myfree(board[i]);
  myfree(board);
  free_history(startptr);/*free history list*/
  //////
  
  //////
  return 0;  
}

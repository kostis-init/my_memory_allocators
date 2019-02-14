#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "../myallocators/myallocators.h"
void free_history(struct history *startptr)
{
  struct history *temp;
  while (startptr != NULL)
  {
    temp = startptr;
    startptr = startptr->next;
    myfree(temp);
  }
}


int clear_board(unsigned char n, unsigned char **A,unsigned char *bpi,unsigned char *bpj,unsigned char *wpi, unsigned char *wpj, struct history **startptr_addr)
{
  unsigned char i,j;
  for(i=0;i<n;i++)
    for(j=0;j<n;j++)
      A[i][j]=0;
  *bpi=0; *bpj=n/2;
  *wpi=n-1; *wpj=n/2;
  free_history(*startptr_addr);
  if((*startptr_addr=mymalloc(sizeof(struct history)))==NULL)
    return -1;
  (*startptr_addr)->num=0;
  (*startptr_addr)->next=NULL;
  return 0;
}

void convert(unsigned char *a)
{
    unsigned char i;
    for(i=0; i<strlen(a); i++)
        a[i]=tolower(a[i]);
}

int move(unsigned char n, unsigned char **A, unsigned char color, unsigned char i, unsigned char j, struct history **historyptr_addr,unsigned char *bpi,unsigned char *bpj,unsigned char *wpi,unsigned char *wpj,unsigned char gen)
{
  unsigned char t,found;
  int temp;
  unsigned char legal_moves[13][2];
  unsigned char a,b;
  
  unsigned char board[n][n];
  for (a=0;a<n;a++)for(b=0;b<n;b++)board[b][a]=A[a][b];//workarround for incompatible array types
  
  find_legal_moves(n,board,color,legal_moves,*bpi,*bpj,*wpi,*wpj);
 
  for(t=0,found=0;legal_moves[t][0]!=255;t++)
  {
    
    if(legal_moves[t][0]==i && legal_moves[t][1]==j)
      found=1;
  }
  if(found==0)
    printf("? illegal move\n\n");
  else
  {
    if(color==1)
    {*bpi=i; *bpj=j;}
    else
    {*wpi=i; *wpj=j;}
    
    /*continuing the history list*/
    (*historyptr_addr)->color=color;
    (*historyptr_addr)->wall=0;
    (*historyptr_addr)->i=i;
    (*historyptr_addr)->j=j;
    temp=(*historyptr_addr)->num;
    if(((*historyptr_addr)->next=mymalloc(sizeof(struct history)))==NULL)
      return -1;
    (*historyptr_addr)=(*historyptr_addr)->next;
    (*historyptr_addr)->num=temp+1;
    (*historyptr_addr)->next=NULL;
    if(gen==0)printf("=\n\n");
  }
  fflush(stdout);
  return 0;
}

void find_legal_moves(unsigned char n, unsigned char A[][n],unsigned char color,unsigned char legal_moves[][2],unsigned char bpi,unsigned char bpj,unsigned char wpi,unsigned char wpj)
{
  unsigned char a=0,k,l,i,j;
  if(color==1)
  {k=bpi; l=bpj; i=wpi; j=wpj;}
  else
  {k=wpi; l=wpj; i=bpi; j=bpj;}
  if(k>0 && (k-1!=i||l!=j) && A[l][k-1]!=1 && (l==0 || A[l-1][k-1]!=1))
  {
    legal_moves[a][0]=k-1;
    legal_moves[a++][1]=l;
  }
  if(l<n-1 && (k!=i||l+1!=j) && A[l][k]!=2 && (k==0 || A[l][k-1]!=2))
  {
    legal_moves[a][0]=k;
    legal_moves[a++][1]=l+1;
  }
  if(k<n-1 && (k+1!=i||l!=j) && A[l][k]!=1 && (l==0 || A[l-1][k]!=1))
  {
    legal_moves[a][0]=k+1;
    legal_moves[a++][1]=l;
  }
  if(l>0 && (k!=i||l-1!=j) && A[l-1][k]!=2 && (k==0 || A[l-1][k-1]!=2))
  {
    legal_moves[a][0]=k;
    legal_moves[a++][1]=l-1;
  }
  
  if(k>1 && (k-1==i && l==j) && A[l][k-1]!=1 && (l==0 || A[l-1][k-1]!=1) && A[l][k-2]!=1 && (l==0 || A[l-1][k-2]!=1))
  {
    legal_moves[a][0]=k-2;
    legal_moves[a++][1]=l;
  }
  if(l<n-2 && (k==i && l+1==j) && A[l][k]!=2 && (k==0 || A[l][k-1]!=2) && A[l+1][k]!=2 && (k==0 || A[l+1][k-1]!=2))
  {
    legal_moves[a][0]=k;
    legal_moves[a++][1]=l+2;
  }
  if(k<n-2 && (k+1==i && l==j) && A[l][k]!=1 && (l==0 || A[l-1][k]!=1) && A[l][k+1]!=1 && (l==0 || A[l-1][k+1]!=1))
  {
    legal_moves[a][0]=k+2;
    legal_moves[a++][1]=l;
  }
  if(l>1 && (k==i && l-1==j) && A[l-1][k]!=2 && (k==0 || A[l-1][k-1]!=2) && A[l-2][k]!=2 && (k==0 || A[l-2][k-1]!=2))
  {
    legal_moves[a][0]=k;
    legal_moves[a++][1]=l-2;
  }
  
  if(l<n-1 && k>0 && (((k-1==i && l==j) && (k==1 || A[l][k-2]==1 || (l>0 && A[l-1][k-2]==1)) && A[l][k-1]==0 && (l==0 || A[l-1][k-1]!=1) && (k==1 || A[l][k-2]!=2)) || ((k==i && l+1==j) && (l==n-2 || A[l+1][k]==2 || A[l+1][k-1]==2) && A[l][k-1]==0 && A[l+1][k-1]!=1 && A[l][k]!=2)))
  {
    legal_moves[a][0]=k-1;
    legal_moves[a++][1]=l+1;
  }
  if(l<n-1 && k<n-1 && (((k==i && l+1==j) && (l==n-2 || A[l+1][k]==2 || (k>0 && A[l+1][k-1]==2)) && (k==0 ||A[l][k-1]!=2) && A[l][k]==0 && A[l+1][k]!=1) || ((k+1==i && l==j) && (k==n-2 || A[l][k+1]==1 || (l>0 && A[l-1][k+1]==1)) && A[l][k]==0 && A[l][k+1]!=2 && (l==0 || A[l-1][k]!=1))))
  {
    legal_moves[a][0]=k+1;
    legal_moves[a++][1]=l+1;
  }
  if(l>0 && k<n-1 && (((k+1==i && l==j) && (k==n-2 || A[l][k+1]==1 || A[l-1][k+1]==1) && A[l-1][k]==0 && A[l][k]!=1 && A[l-1][k+1]!=2) || ((k==i && l-1==j) && (l==1 || A[l-2][k]==2 || (k>0 && A[l-2][k-1]==2)) && A[l-1][k]==0 && (k==0 || A[l-1][k-1]!=2) && (l==1 || A[l-2][k]!=1))))
  {
    legal_moves[a][0]=k+1;
    legal_moves[a++][1]=l-1;
  }
  if(l>0 && k>0 && (((k==i && l-1==j) && (l==1 || A[l-2][k]==2 || A[l-2][k-1]==2) && A[l-1][k-1]==0 && A[l-1][k]!=2 && (l==1 || A[l-2][k-1]!=1))||((k-1==i && l==j) && (k==1 || A[l][k-2]==1 || A[l-1][k-2]==1) && A[l-1][k-1]==0 && A[l][k-1]!=1 && (k==1 || A[l-1][k-2]!=2))))
  {
    legal_moves[a][0]=k-1;
    legal_moves[a++][1]=l-1;
  }
  legal_moves[a][0]=255;  /*sign to stop*/
  legal_moves[a][1]=255;  
}

int wall_placement(unsigned char boardsize, unsigned char **board, unsigned char color,unsigned char wall, unsigned char *bwalls, unsigned char *wwalls,unsigned char i,unsigned char j, struct history **historyptr_addr,unsigned char bpi,unsigned char bpj,unsigned char wpi,unsigned char wpj,unsigned char gen)
{
  int temp;
  if(((color==1)?(*bwalls):(*wwalls))==0)
    printf("? no walls remaining for this player\n\n");
  else if(i==boardsize-1 || j==boardsize-1)
    printf("? illegal move\n\n");
  else if(wall==1 && (board[i][j]!=0 || (j>0 && board[i][j-1]==1) || board[i][j+1]==1))
    printf("? illegal move\n\n");
  else if(wall==2 && (board[i][j]!=0 || (i>0 && board[i-1][j]==2) || board[i+1][j]==2))
    printf("? illegal move\n\n");
  else
  {
    board[i][j]=wall;
    if(genmove(boardsize,board,bpi,bpj,wpi,wpj,bwalls,wwalls,3)==0)
    {
      printf("? illegal move\n\n");
      board[i][j]=0;
    }
    else
    {
      (color==1)?((*bwalls)--):((*wwalls)--);
      
      /*continuing the history list*/
      (*historyptr_addr)->color=color;
      (*historyptr_addr)->wall=wall;
      (*historyptr_addr)->i=i;
      (*historyptr_addr)->j=j;
      temp=(*historyptr_addr)->num;
      if(((*historyptr_addr)->next=mymalloc(sizeof(struct history)))==NULL)
	return -1;
      (*historyptr_addr)=(*historyptr_addr)->next;
      (*historyptr_addr)->num=temp+1;
      (*historyptr_addr)->next=NULL;
      if(gen==0) printf("=\n\n");
    }
  }
  fflush(stdout);
  return 0;
}

void undo(unsigned char boardsize,unsigned char **board,unsigned char *bwalls,unsigned char *wwalls,struct history **historyptr_addr, struct history *startptr,unsigned char *bpi,unsigned char *bpj,unsigned char *wpi,unsigned char *wpj)
{
  struct history *temp1=startptr;
  struct history *temp=startptr;
  
  while(temp->num != (*historyptr_addr)->num-1) /*temp will point at the last move of the history list*/
    temp=temp->next;
  
  myfree(temp->next);
  temp->next=NULL;
  if(temp->wall!=0)/*case that the last move was a wall*/
  {  
    (temp->color==1)?((*bwalls)++):((*wwalls)++); /*give the wall back to the player*/
    board[temp->i][temp->j]=0;	/*cleaning the wall from the board*/
  }
  else/*case that the last move was not a wall*/
  {
    int i=0;
    unsigned char f=0;
    
    do
    {
      temp1=startptr;
      
      if(i>=temp->num) /*there has not be a previous move of this color*/
      {
	f=1;
	break;
      }
      
      while(temp1->num!=temp->num -1 -i)/*temp1 will point to the previous move, so that we know where to put the pawn....*/
	temp1=temp1->next;
      i++;
    }while(temp1->color!=temp->color || temp1->wall!=0);/*........but the previous move must be of the same color*/
    
    if(f)/*case that there is only 1 move of the color of the last move. So this pawn is going at the start*/
    {
      if(temp->color==1)
      {*bpi=0; *bpj=boardsize/2;}
      else
      {*wpi=boardsize-1; *wpj=boardsize/2;}
    }
    else if(temp1->color==1) /*the pawn goes in his previous position*/
    {*bpi=temp1->i; *bpj=temp1->j;}
    else
    {*wpi=temp1->i; *wpj=temp1->j;}
  }
  *historyptr_addr=temp;
}

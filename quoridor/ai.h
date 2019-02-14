
void Rmaprobot(unsigned char boardsize, unsigned char board[][boardsize], unsigned char distances[][boardsize], unsigned char x,unsigned char y,unsigned char comefrom);
int genmove(unsigned char boardsize, unsigned char **board, unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls,unsigned char enemywalls,unsigned char color);
		

void putwallinmap(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char x,unsigned char y);

int branch(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls, unsigned char enemywalls, signed char depth);

		


void maprobot(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize], unsigned char x, unsigned char y);



void cleanrobot(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y);


unsigned char legalwall(unsigned char boardsize, unsigned char board[][boardsize],unsigned char x, unsigned char y, unsigned char orientation);

void entryfinder(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y,unsigned char entry[]);


int tree(unsigned char boardsize,unsigned char board[][boardsize],unsigned char mydistances[][boardsize],unsigned char enemydistances[][boardsize],unsigned char myposx, unsigned char myposy, unsigned char enemyposx, unsigned char enemyposy,unsigned char mywalls,unsigned char enemywalls, signed char depth);

int closetracer(unsigned char boardsize,unsigned char board[][boardsize], unsigned char distances[][boardsize],unsigned char x, unsigned char y, unsigned char target);

	//////depth////////$0$//////count///////(*(*(++++))())/////

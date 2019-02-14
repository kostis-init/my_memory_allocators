struct history {	/*list for game history*/
  unsigned char color;/*1(for black) or 2(for white)*/
  unsigned char wall;/*0 or 1(for horizontal wall) or 2(for vertical)*/
  unsigned char i;
  unsigned char j;
  int num;
  struct history *next;
};
void free_history(struct history *); 
int clear_board(unsigned char, unsigned char **,unsigned char *,unsigned char *,unsigned char *,unsigned char *, struct history **);
void convert(unsigned char *);
int move(unsigned char , unsigned char **, unsigned char, unsigned char, unsigned char, struct history **,unsigned char *,unsigned char *,unsigned char *,unsigned char *,unsigned char);
void find_legal_moves(unsigned char n, unsigned char [][n], unsigned char,unsigned char [][2],unsigned char,unsigned char,unsigned char,unsigned char);
int wall_placement(unsigned char, unsigned char **, unsigned char,unsigned char, unsigned char *, unsigned char *,unsigned char,unsigned char, struct history **,unsigned char,unsigned char,unsigned char,unsigned char,unsigned char);
void undo(unsigned char,unsigned char **,unsigned char *,unsigned char *,struct history **,struct history *,unsigned char *,unsigned char *,unsigned char *,unsigned char *);


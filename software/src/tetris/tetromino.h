#ifndef TETROMINO
#define TETROMINO

#define TETROMINO_W 4
#define TETROMINO_H 4

#define TETROMINO_UP_H 2
#define TETROMINO_UP_W 4

#define TETROMINO_GRID_H 3
#define TETROMINO_GRID_W 3

#define SRS_TABLE_NUMBER 3
#define SRS_TEST_NUMBER 5

#define SPAWN_TEST_NUMBER 2

typedef enum
{
    TETRO_J,
    TETRO_L,
    TETRO_T,
    TETRO_S,
    TETRO_Z,
    TETRO_I,
    TETRO_O,
    TETRO_NUMBER,
    TETRO_NIL,
} TetroType;

typedef enum
{
    DIR_U,
    DIR_R,
    DIR_D,
    DIR_L,
    DIR_NUMBER,
    DIR_NIL,
} DirType;

typedef struct
{
    signed char x, y;
} Point;

typedef struct
{
    TetroType type;
    DirType dir;
    signed char x, y;
    unsigned char board[TETROMINO_W];
    unsigned short grid[TETROMINO_H * TETROMINO_GRID_H];
} Tetromino;

void tetromino_board_set(unsigned char *board, TetroType type, DirType dir);
void tetromino_grid_set(unsigned short *grid, unsigned char *board);
void tetromino_ghost_grid_set(unsigned short *grid, unsigned char *board);
void tetromino_srs_move_get(TetroType type, DirType ds, DirType dt, unsigned char test_num, signed char *dx, signed char *dy);
void tetromino_size_get(TetroType type, DirType dt, signed char *w, signed char *h);

#endif

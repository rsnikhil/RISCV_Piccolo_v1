// Adding <118, 203, 238> rectangle with dimensions (1 x 1) at (3, 3).
// Adding <100, 97, 127> rectangle with dimensions (2 x 1) at (3, 2).
// Adding <72, 225, 84> rectangle with dimensions (1 x 1) at (4, 3).
// Adding <34, 151, 188> rectangle with dimensions (2 x 2) at (3, 0).
// Adding <108, 225, 197> rectangle with dimensions (1 x 2) at (4, 0).
// Adding <142, 160, 83> rectangle with dimensions (1 x 1) at (2, 1).

#define DATA_SIZE 6 

int input_data_r[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0,  34, 108,   0},
  {  0,   0, 142,  34, 108,   0},
  {  0,   0,   0, 100, 100,   0},
  {  0,   0,   0, 118,  72,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};

int input_data_g[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0, 151, 225,   0},
  {  0,   0, 160, 151, 225,   0},
  {  0,   0,   0,  97,  97,   0},
  {  0,   0,   0, 203, 225,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};

int input_data_b[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0, 188, 197,   0},
  {  0,   0,  83, 188, 197,   0},
  {  0,   0,   0, 127, 127,   0},
  {  0,   0,   0, 238,  84,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};

int verify_data_r[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0,  34, 108,   0},
  {  0,   0, 255,   0, 255,   0},
  {  0,   0,   0, 248, 220,   0},
  {  0,   0,   0, 255, 142,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};

int verify_data_g[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0, 151, 225,   0},
  {  0,   0, 255, 122, 255,   0},
  {  0,   0,   0,  34,   0,   0},
  {  0,   0,   0, 255, 255,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};

int verify_data_b[DATA_SIZE][DATA_SIZE] = 
{
  {  0,   0,   0, 188, 197,   0},
  {  0,   0, 227, 255, 255,   0},
  {  0,   0,   0,  82, 227,   0},
  {  0,   0,   0, 255,  55,   0},
  {  0,   0,   0,   0,   0,   0},
  {  0,   0,   0,   0,   0,   0}
};


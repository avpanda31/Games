// cs_snake.c
// Written by Avani Agarwal (z5728975) on <INSERT DATE>
//
// Description: <INSERT DESCRIPTION OF PROGRAM>

// Provided Libraries
#include <stdio.h>

// Add your own #include statements below this line
#include <string.h>

// Provided constants
#define ROWS 10
#define COLS 10

#define NO_SNAKE -1

// Add your own #define constants below this line
#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'

#define HORIZ_WALL 'h'
#define VERT_WALL 'v'

#define PASS_UP '^'
#define PASS_DOWN 'v'
#define PASS_LEFT '<'
#define PASS_RIGHT '>'

// Provided enums
// Enum for features on the game board
enum entity {
    BODY_SEGMENT,
    EXIT_LOCKED,
    EXIT_UNLOCKED,
    WALL,
    APPLE_NORMAL,
    APPLE_REVERSE,
    APPLE_SPLIT,
    APPLE_EXPLODE,
    EXPLOSION,
    PASSAGE_UP,
    PASSAGE_DOWN,
    PASSAGE_LEFT,
    PASSAGE_RIGHT,
    PORTAL,
    EMPTY
};

// Add your own enums below this line

// Provided structs
// Represents a tile/cell on the game board
struct tile {
    enum entity entity;
    int radius;
};

// Add your own structs below this line
struct map_commands {
    char wall;
    char apple;
    char normal;
    char reverse;
    char split;
    char explode;
    char exit;
    char long_wall;
    char stop;
    char points;
    char passages;
    char reset;
    char portal;
    char second_snake;
    int two_snakes_true;
};

struct position {
    int row;
    int col;
};

struct point_system {
    int points;
    int moves_made;
    int num_apples_eaten;
    int num_apples_remaining;
    double completion_percentage;
    int maximum_points_remaining;
    int initial_max_points;
};

struct portal_pairs {
    int row1;
    int row2;
    int col1;
    int col2;
};

struct explosion_location {
    int active;
    int current_radius;
    int max_radius;
    int middle_row;
    int middle_col;
};

// Provided Function Prototypes
void setup_initial_points(struct point_system* point_system,
                          int num_initial_apples);

void initialise_board(struct tile board[ROWS][COLS]);
void print_board(struct tile board[ROWS][COLS], int snake_row, int snake_col);
void print_board_with_second_snake(struct tile board[ROWS][COLS], int snake_row,
                                   int snake_col, int rival_snake_position_row,
                                   int rival_snake_position_col);
void point_add(int* row, int* col, struct tile board[ROWS][COLS],
               struct point_system* point_system, int* num_initial_apples);
void print_game_statistics(int points, int moves_made, int num_apples_eaten,
                           int num_apples_remaining,
                           double completion_percentage,
                           int maximum_points_remaining);

void print_game_statistics_with_rival(
    int original_points, int original_moves_made, int original_num_apples_eaten,
    int rival_points, int rival_moves_made, int rival_num_apples_eaten,
    int num_apples_remaining, double completion_percentage,
    int maximum_points_remaining);
void print_board_line(void);
void print_tile_spacer(void);
void print_board_header(void);

// Add your function prototypes below this line
struct position board_setup(struct tile board[ROWS][COLS],
                            int* num_initial_apples,
                            struct point_system* point_system,
                            int* snake_start_row, int* snake_start_col,
                            struct position snake_body[],
                            struct map_commands command,
                            struct tile initial_board[ROWS][COLS],
                            struct portal_pairs portals[], int* num_portals,
                            int* radius, struct position* rival_snake_position);

int row_col_error_check(int row, int col);
int lose_row_col_error_check(int row, int col);
int tile_repeat_check(int row, int col, struct tile board[ROWS][COLS]);
int lose_tile_repeat_check(int row, int col, struct tile board[ROWS][COLS]);
int wall_tiles_check(int row, int col, struct tile board[ROWS][COLS]);
int wall_bounds_check(int row, int col);

struct position snake_spawn(struct tile board[ROWS][COLS],
                            struct point_system* point_system,
                            int* snake_start_row, int* snake_start_col,
                            struct position snake_body[]);

struct position rival_snake_spawn(struct tile board[ROWS][COLS],
                                  struct point_system* point_system,
                                  int* snake_start_row, int* snake_start_col,
                                  struct position snake_body[],
                                  struct position* original_snake_position);

int passages_check(int row, int col, struct tile board[ROWS][COLS], char move);

void portal_check(int* row, int* col, struct tile board[ROWS][COLS], char move,
                  struct portal_pairs portals[], int num_portals);

void apple_check(int row, int col, struct tile board[ROWS][COLS],
                 struct point_system* point_system, int* num_initial_apples,
                 int* snake_start_row, int* snake_start_col, int* reverse_snake,
                 int* split_snake, int* explode_snake, int* radius);

void unlock_exit(struct tile board[ROWS][COLS],
                 struct point_system* point_system);

static void compute_board_completion(const struct tile board[ROWS][COLS],
                                     int initial_apples, int* num_remaining,
                                     double* completion_percentage,
                                     int* max_points_remaining);

void snake_movement(int* row, int* col, struct tile board[ROWS][COLS],
                    struct point_system* point_system,
                    struct map_commands* command, int* num_initial_apples,
                    int* snake_start_row, int* snake_start_col,
                    int* reverse_snake, int* split_snake, int* explode_snake,
                    int* snake_length, struct position snake_body[],
                    struct position position,
                    struct tile initial_board[ROWS][COLS],
                    struct portal_pairs portals[], int* num_portals,
                    int* radius, char* move);

void gameplay_setup(int row, int col, struct tile board[ROWS][COLS],
                    struct point_system* point_system,
                    struct map_commands* command, int* num_initial_apples,
                    int* snake_start_row, int* snake_start_col,
                    int* reverse_snake, int* split_snake, int* explode_snake,
                    int* snake_length, struct position snake_body[],
                    struct position position,
                    struct tile initial_board[ROWS][COLS],
                    struct portal_pairs portals[], int* num_portals,
                    int* radius, int* split_head_row, int* split_head_col);

void gameplay_setup_two_snakes(
    int row, int col, struct tile board[ROWS][COLS],
    struct point_system* orig_ps, struct map_commands* command,
    int* num_initial_apples, int* orig_start_row, int* orig_start_col,
    int* orig_reverse, int* orig_split, int* orig_explode, int* orig_len,
    struct position orig_body[], struct position orig_spawn_pos,
    struct tile initial_board[ROWS][COLS], struct portal_pairs portals[],
    int* num_portals, int* radius, int* orig_split_head_row,
    int* orig_split_head_col,

    int rival_row, int rival_col, struct point_system* rival_ps,
    int* rival_start_row, int* rival_start_col, int* rival_reverse,
    int* rival_split, int* rival_explode, int* rival_len,
    struct position rival_body[], int* rival_split_head_row,
    int* rival_split_head_col);

int process_move_result(int row, int col, struct tile board[ROWS][COLS],
                        char move, struct map_commands* command,
                        struct position snake_body[], int* snake_length);

int handle_special_apples(
    int* row, int* col, struct tile board[ROWS][COLS],
    struct point_system* point_system, struct map_commands* command,
    int* num_initial_apples, int* snake_start_row, int* snake_start_col,
    int* reverse_snake, int* split_snake, int* explode_snake, int* snake_length,
    struct position snake_body[], struct position position,
    struct tile initial_board[ROWS][COLS], struct portal_pairs portals[],
    int* num_portals, int* radius, char* move, int* split_head_row,
    int* split_head_col, struct explosion_location* explosion);

void gameplay_begins(int row, int col, struct tile board[ROWS][COLS],
                     struct point_system* point_system,
                     struct map_commands* command, int* num_initial_apples,
                     int* snake_start_row, int* snake_start_col,
                     int* reverse_snake, int* split_snake, int* explode_snake,
                     int* snake_length, struct position snake_body[],
                     struct position position,
                     struct tile initial_board[ROWS][COLS],
                     struct portal_pairs portals[], int* num_portals,
                     int* radius, int* split_head_row, int* split_head_col);

void reverse_snake_true(struct tile board[ROWS][COLS], int* snake_length,
                        int* row, int* col, int* snake_start_row,
                        int* snake_start_col, struct position snake_body[],
                        int* reverse_snake, int* split_head_row,
                        int* split_head_col, struct map_commands* command);

void split_snake_true(struct tile board[ROWS][COLS], int* snake_length,
                      int* row, int* col, int* snake_start_row,
                      int* snake_start_col, struct position snake_body[],
                      int* split_snake, int* split_head_row,
                      int* split_head_col, struct map_commands* command);

void update_explosion_bounds(int* up_row, int* down_row, int* left_col,
                             int* right_col);

void apply_explosion_wave(struct tile board[ROWS][COLS], int middle_row,
                          int middle_col, int radius,
                          struct point_system* point_system,
                          int* num_initial_apples);

void clear_explosions(struct tile board[ROWS][COLS]);

void explode_snake_true(int* row, int* col, struct tile board[ROWS][COLS],
                        int* radius, int* middle_row, int* middle_col,
                        int* explode_snake,
                        struct explosion_location* explosion,
                        struct point_system* point_system,
                        int* num_initial_apples, struct map_commands* command);

void continue_explosion(struct tile board[ROWS][COLS],
                        struct explosion_location* explosion, int row, int col,
                        struct point_system* point_system,
                        int* num_initial_apples);

void explosion_direction(struct tile board[ROWS][COLS], int* row, int* col,
                         struct point_system* point_system,
                         int* num_initial_apples);

void reset_command(struct tile board[ROWS][COLS],
                   struct tile initial_board[ROWS][COLS],
                   struct position position, int* snake_length, int* row,
                   int* col, int* snake_start_row, int* snake_start_col,
                   struct position snake_body[],
                   struct point_system* point_system, int* num_initial_apples,
                   int* reverse_snake, int* split_snake, int* explode_snake,
                   struct map_commands* command);

void print_game_statistics(int points, int moves_made, int num_apples_eaten,
                           int num_apples_remaining,
                           double completion_percentage,
                           int maximum_points_remaining);

int out_of_bounds(int row, int col);
int bounds_checker(int row, int col);

int win_check(int row, int col, struct tile board[ROWS][COLS],
              struct point_system* point_system);

void lose_check(struct point_system* point_system);

void apple_explode_input(struct tile board[ROWS][COLS], int* num_initial_apples,
                         struct point_system* point_system,
                         struct map_commands* command, char* input2,
                         int* radius);

void apples_input(struct tile board[ROWS][COLS], int* num_initial_apples,
                  struct point_system* point_system,
                  struct map_commands* command, char* input2);

void long_walls_input(struct tile board[ROWS][COLS]);
void passages_input(struct tile board[ROWS][COLS]);

void portals_input(struct tile board[ROWS][COLS], struct portal_pairs portals[],
                   int* num_portals);

void walls_input(struct tile board[ROWS][COLS], struct map_commands* command,
                 char input1);

// static int two_snakes_g = 0;

// Provided sample main() function (you will need to modify this)
int main(void) {
    printf("Welcome to CS Snake!\n");
    struct map_commands command = {.wall = 'w',
                                   .apple = 'a',
                                   .normal = 'n',
                                   .reverse = 'r',
                                   .split = 's',
                                   .explode = 'e',
                                   .exit = 'e',
                                   .long_wall = 'W',
                                   .stop = 's',
                                   .points = 'p',
                                   .passages = 'p',
                                   .reset = 'r',
                                   .portal = 't',
                                   .second_snake = 'S',
                                   .two_snakes_true = 0};

    struct portal_pairs portals[5];
    struct position snake_body[ROWS * COLS];
    struct tile board[ROWS][COLS];
    struct tile initial_board[ROWS][COLS];
    struct point_system point_system;

    int num_initial_apples = 0;
    int snake_start_row = 0;
    int snake_start_col = 0;
    int reverse_snake = 0;
    int split_snake = 0;
    int explode_snake = 0;
    int snake_length = 0;
    int split_head_row = 0;
    int split_head_col = 0;
    int num_portals = 0;
    point_system.maximum_points_remaining = 0;
    int radius;
    initialise_board(board);

    // rival
    struct position rival_snake_position;
    rival_snake_position.row = NO_SNAKE;
    rival_snake_position.col = NO_SNAKE;

    struct position rival_snake_body[ROWS * COLS];
    struct point_system rival_point_system;
    int rival_snake_start_row = 0;
    int rival_snake_start_col = 0;
    int rival_reverse_snake = 0;
    int rival_split_snake = 0;
    int rival_explode_snake = 0;
    int rival_snake_length = 0;
    int rival_split_head_row = 0;
    int rival_split_head_col = 0;
    int two_snake = 0;  // 0 = single player, 1 = two player

    struct position snake_position =
        board_setup(board, &num_initial_apples, &point_system, &snake_start_row,
                    &snake_start_col, snake_body, command, initial_board,
                    portals, &num_portals, &radius, &rival_snake_position);

    point_system.initial_max_points = point_system.maximum_points_remaining;

    if (rival_snake_position.row != NO_SNAKE &&
        rival_snake_position.col != NO_SNAKE) {
        two_snake = 1;

        command.two_snakes_true = 1;

        rival_snake_start_row = rival_snake_position.row;
        rival_snake_start_col = rival_snake_position.col;
        rival_snake_body[0] = rival_snake_position;

        rival_point_system.maximum_points_remaining =
            point_system.maximum_points_remaining;
        setup_initial_points(&rival_point_system, num_initial_apples);
        rival_point_system.initial_max_points = point_system.initial_max_points;
    }

    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            initial_board[row][col] = board[row][col];
        }
    }

    setup_initial_points(&point_system, num_initial_apples);

    if (!two_snake) {
        gameplay_setup(snake_position.row, snake_position.col, board,
                       &point_system, &command, &num_initial_apples,
                       &snake_start_row, &snake_start_col, &reverse_snake,
                       &split_snake, &explode_snake, &snake_length, snake_body,
                       snake_position, initial_board, portals, &num_portals,
                       &radius, &split_head_row, &split_head_col);
    } else {
        gameplay_setup_two_snakes(
            snake_position.row, snake_position.col, board, &point_system,
            &command, &num_initial_apples, &snake_start_row, &snake_start_col,
            &reverse_snake, &split_snake, &explode_snake, &snake_length,
            snake_body, snake_position, initial_board, portals, &num_portals,
            &radius, &split_head_row, &split_head_col, rival_snake_position.row,
            rival_snake_position.col, &rival_point_system,
            &rival_snake_start_row, &rival_snake_start_col,
            &rival_reverse_snake, &rival_split_snake, &rival_explode_snake,
            &rival_snake_length, rival_snake_body, &rival_split_head_row,
            &rival_split_head_col);
    }

    return 0;
}

// Add your function definitions below this line

void setup_initial_points(struct point_system* point_system,
                          int num_initial_apples) {
    point_system->points = 0;
    point_system->moves_made = 0;
    point_system->num_apples_eaten = 0;
    point_system->num_apples_remaining = num_initial_apples;

    if (num_initial_apples == 0) {
        point_system->completion_percentage = 100.0;
    } else {
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / num_initial_apples;
    }
}

struct position board_setup(
    struct tile board[ROWS][COLS], int* num_initial_apples,
    struct point_system* point_system, int* snake_start_row,
    int* snake_start_col, struct position snake_body[],
    struct map_commands command, struct tile initial_board[ROWS][COLS],
    struct portal_pairs portals[], int* num_portals, int* radius,
    struct position* rival_snake_position) {
    printf("\n");
    printf("--- Map Setup ---\n");

    char input1;
    char input2;

    while (scanf(" %c", &input1) == 1) {
        if (input1 == command.stop) {
            print_board(board, NO_SNAKE, NO_SNAKE);
            struct position position =
                snake_spawn(board, point_system, snake_start_row,
                            snake_start_col, snake_body);
            print_board(board, position.row, position.col);
            return position;
        } else if (input1 == command.second_snake) {
            print_board(board, NO_SNAKE, NO_SNAKE);
            struct position position =
                snake_spawn(board, point_system, snake_start_row,
                            snake_start_col, snake_body);
            *rival_snake_position =
                rival_snake_spawn(board, point_system, snake_start_row,
                                  snake_start_col, snake_body, &position);
            print_board_with_second_snake(board, position.row, position.col,
                                          rival_snake_position->row,
                                          rival_snake_position->col);
            return position;
        }

        if (input1 == command.apple) {
            scanf(" %c", &input2);
            if (input2 == command.explode) {
                apple_explode_input(board, num_initial_apples, point_system,
                                    &command, &input2, radius);
            } else {
                apples_input(board, num_initial_apples, point_system, &command,
                             &input2);
            }
        } else if (input1 == command.long_wall) {
            long_walls_input(board);
        } else if (input1 == command.passages) {
            passages_input(board);
        } else if (input1 == command.portal) {
            portals_input(board, portals, num_portals);
        } else {
            walls_input(board, &command, input1);
        }
    }

    struct position default_pos = {NO_SNAKE, NO_SNAKE};
    return default_pos;
}

void apple_explode_input(struct tile board[ROWS][COLS], int* num_initial_apples,
                         struct point_system* point_system,
                         struct map_commands* command, char* input2,
                         int* radius) {
    int row;
    int col;
    scanf(" %d %d %d", radius, &row, &col);
    board[row][col].radius = *radius;

    if ((row_col_error_check(row, col)) == 1) {
        return;
    }
    if (tile_repeat_check(row, col, board) == 1) {
        return;
    }

    (*num_initial_apples) += 1;
    point_system->maximum_points_remaining += 20;
    board[row][col].entity = APPLE_EXPLODE;
}

void apples_input(struct tile board[ROWS][COLS], int* num_initial_apples,
                  struct point_system* point_system,
                  struct map_commands* command, char* input2) {
    int row;
    int col;

    scanf("%d %d", &row, &col);

    if ((row_col_error_check(row, col) == 1)) {
        return;
    }
    if (tile_repeat_check(row, col, board) == 1) {
        return;
    }
    if (*input2 == command->normal) {
        (*num_initial_apples) += 1;
        point_system->maximum_points_remaining += 5;
        board[row][col].entity = APPLE_NORMAL;
    } else if (*input2 == command->reverse) {
        (*num_initial_apples) += 1;
        point_system->maximum_points_remaining += 10;
        board[row][col].entity = APPLE_REVERSE;
    } else if (*input2 == command->split) {
        (*num_initial_apples) += 1;
        point_system->maximum_points_remaining += 20;
        board[row][col].entity = APPLE_SPLIT;
    }
}

void long_walls_input(struct tile board[ROWS][COLS]) {
    char direction;
    int row;
    int col;
    int length;

    scanf(" %c %d %d %d", &direction, &row, &col, &length);
    if ((row_col_error_check(row, col)) == 1) {
        return;
    }

    int orig_row = row;
    int orig_col = col;
    int bounds_checker = 1;

    if (direction == HORIZ_WALL) {
        if ((wall_bounds_check(row, col + length - 1)) == 1) {
            bounds_checker = 0;
        }
    } else if (direction == VERT_WALL) {
        if ((wall_bounds_check(row + length - 1, col)) == 1) {
            bounds_checker = 0;
        }
    }
    if (bounds_checker != 0) {
        for (int i = 0; i < length; i++) {
            if ((wall_tiles_check(row, col, board)) == 1) {
                bounds_checker = 0;
                break;
            }
            if (direction == HORIZ_WALL) {
                col++;
            } else if (direction == VERT_WALL) {
                row++;
            }
        }
    }

    if (bounds_checker == 1) {
        for (int i = 0; i < length; i++) {
            board[orig_row][orig_col].entity = WALL;
            if (direction == HORIZ_WALL) {
                orig_col++;
            } else if (direction == VERT_WALL) {
                orig_row++;
            }
        }
    }
}

void passages_input(struct tile board[ROWS][COLS]) {
    char direction;
    int row;
    int col;

    scanf(" %c %d %d", &direction, &row, &col);

    if ((row_col_error_check(row, col)) == 1) {
        return;
    }
    if (tile_repeat_check(row, col, board) == 1) {
        return;
    }

    if (direction == PASS_UP) {
        board[row][col].entity = PASSAGE_UP;
    } else if (direction == PASS_DOWN) {
        board[row][col].entity = PASSAGE_DOWN;
    } else if (direction == PASS_LEFT) {
        board[row][col].entity = PASSAGE_LEFT;
    } else if (direction == PASS_RIGHT) {
        board[row][col].entity = PASSAGE_RIGHT;
    }
}

void portals_input(struct tile board[ROWS][COLS], struct portal_pairs portals[],
                   int* num_portals) {
    int row_1;
    int col_1;
    int row_2;
    int col_2;

    int portal_amount = 0;
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            if (board[row][col].entity == PORTAL) {
                portal_amount++;
            }
        }
    }
    if (portal_amount >= 10) {
        printf("ERROR: Invalid placement, ");
        printf("maximum number of portal pairs already reached!\n");
        return;
    }

    scanf("%d %d %d %d", &row_1, &col_1, &row_2, &col_2);

    if ((out_of_bounds(row_1, col_1))) {
        printf("ERROR: Invalid position for first portal in pair, ");
        printf("%d %d is out of bounds!\n", row_1, col_1);
        return;
    }
    if ((out_of_bounds(row_2, col_2))) {
        printf("ERROR: Invalid position for second portal in pair, ");
        printf("%d %d is out of bounds!\n", row_2, col_2);
        return;
    }
    if (board[row_1][col_1].entity != EMPTY) {
        printf("ERROR: Invalid tile for first portal in pair, ");
        printf("%d %d is occupied!\n", row_1, col_1);
        return;
    }
    if (board[row_2][col_2].entity != EMPTY) {
        printf("ERROR: Invalid tile for second portal in pair, ");
        printf("%d %d is occupied!\n", row_2, col_2);
        return;
    }

    board[row_1][col_1].entity = PORTAL;
    board[row_2][col_2].entity = PORTAL;

    portals[*num_portals].row1 = row_1;
    portals[*num_portals].col1 = col_1;
    portals[*num_portals].row2 = row_2;
    portals[*num_portals].col2 = col_2;

    (*num_portals) += 1;
}

void walls_input(struct tile board[ROWS][COLS], struct map_commands* command,
                 char input1) {
    int row;
    int col;

    scanf("%d %d", &row, &col);

    if ((row_col_error_check(row, col)) == 1) {
        return;
    }
    if (tile_repeat_check(row, col, board) == 1) {
        return;
    }
    if (input1 == command->wall) {
        board[row][col].entity = WALL;
    } else if (input1 == command->exit) {
        board[row][col].entity = EXIT_LOCKED;
    }
}

int out_of_bounds(int row, int col) {
    return (row >= ROWS || row < 0 || col >= COLS || col < 0);
}

int bounds_checker(int row, int col) {
    return (row >= 0 && row < ROWS && col >= 0 && col < COLS);
}

int row_col_error_check(int row, int col) {
    if (out_of_bounds(row, col)) {
        printf("ERROR: Invalid position, %d %d is out of bounds!\n", row, col);
        return 1;
    }
    return 0;
}

int tile_repeat_check(int row, int col, struct tile board[ROWS][COLS]) {
    if (board[row][col].entity != EMPTY) {
        printf("ERROR: Invalid tile, %d %d is occupied!\n", row, col);
        return 1;
    }
    return 0;
}

int lose_row_col_error_check(int row, int col) {
    if (out_of_bounds(row, col)) {
        return 1;
    }
    return 0;
}

int lose_tile_repeat_check(int row, int col, struct tile board[ROWS][COLS]) {
    if ((board[row][col].entity != EMPTY) &&
        (board[row][col].entity != EXIT_UNLOCKED)) {
        return 1;
    }
    return 0;
}

int wall_tiles_check(int row, int col, struct tile board[ROWS][COLS]) {
    if ((board[row][col].entity != EMPTY)) {
        printf("ERROR: Invalid tile, part of the wall is occupied!\n");
        return 1;
    }
    return 0;
}

int wall_bounds_check(int row, int col) {
    if (out_of_bounds(row, col)) {
        printf("ERROR: Invalid position, part of the wall is out of ");
        printf("bounds!\n");
        return 1;
    }
    return 0;
}

struct position snake_spawn(struct tile board[ROWS][COLS],
                            struct point_system* point_system,
                            int* snake_start_row, int* snake_start_col,
                            struct position snake_body[]) {
    struct position position;
    printf("--- Spawning Snake ---\n");
    while (1) {
        printf("Enter the snake's starting position: ");
        scanf("%d %d", &position.row, &position.col);
        if ((row_col_error_check(position.row, position.col)) == 1) {
            continue;
        }
        if (tile_repeat_check(position.row, position.col, board) == 1) {
            continue;
        }
        break;
    }

    *snake_start_row = position.row;
    *snake_start_col = position.col;

    snake_body[0].row = position.row;
    snake_body[0].col = position.col;

    return position;
}

struct position rival_snake_spawn(struct tile board[ROWS][COLS],
                                  struct point_system* point_system,
                                  int* snake_start_row, int* snake_start_col,
                                  struct position snake_body[],
                                  struct position* original_snake_position) {
    struct position position;
    printf("--- Spawning Rival Snake ---\n");
    while (1) {
        printf("Enter the rival snake's starting position: ");
        scanf("%d %d", &position.row, &position.col);
        if ((row_col_error_check(position.row, position.col)) == 1) {
            continue;
        }
        if (tile_repeat_check(position.row, position.col, board) == 1) {
            continue;
        }
        if (position.row == original_snake_position->row &&
            position.col == original_snake_position->col) {
            printf("ERROR: Invalid tile, %d %d is occupied!\n", position.row,
                   position.col);
            continue;
        }
        break;
    }

    *snake_start_row = position.row;
    *snake_start_col = position.col;

    snake_body[0].row = position.row;
    snake_body[0].col = position.col;

    return position;
}

void gameplay_setup(int row, int col, struct tile board[ROWS][COLS],
                    struct point_system* point_system,
                    struct map_commands* command, int* num_initial_apples,
                    int* snake_start_row, int* snake_start_col,
                    int* reverse_snake, int* split_snake, int* explode_snake,
                    int* snake_length, struct position snake_body[],
                    struct position position,
                    struct tile initial_board[ROWS][COLS],
                    struct portal_pairs portals[], int* num_portals,
                    int* radius, int* split_head_row, int* split_head_col) {
    printf("--- Gameplay Phase ---\n");
    *snake_length = 1;

    gameplay_begins(row, col, board, point_system, command, num_initial_apples,
                    snake_start_row, snake_start_col, reverse_snake,
                    split_snake, explode_snake, snake_length, snake_body,
                    position, initial_board, portals, num_portals, radius,
                    split_head_row, split_head_col);
}
void gameplay_setup_two_snakes(
    int row, int col, struct tile board[ROWS][COLS],
    struct point_system* orig_ps, struct map_commands* command,
    int* num_initial_apples, int* orig_start_row, int* orig_start_col,
    int* orig_reverse, int* orig_split, int* orig_explode, int* orig_len,
    struct position orig_body[], struct position orig_spawn_pos,
    struct tile initial_board[ROWS][COLS], struct portal_pairs portals[],
    int* num_portals, int* radius, int* orig_split_head_row,
    int* orig_split_head_col,

    int rival_row, int rival_col, struct point_system* rival_ps,
    int* rival_start_row, int* rival_start_col, int* rival_reverse,
    int* rival_split, int* rival_explode, int* rival_len,
    struct position rival_body[], int* rival_split_head_row,
    int* rival_split_head_col) {
    printf("--- Gameplay Phase ---\n");

    if (*orig_len == 0) {
        *orig_len = 1;
    }
    if (*rival_len == 0) {
        *rival_len = 1;
    }

    struct explosion_location orig_explosion = {0, 0, 0, 0, 0};
    struct explosion_location rival_explosion = {0, 0, 0, 0, 0};

    int turn = 0;  // 0 = original, 1 = rival

    char move;
    while (1) {
        int result = scanf(" %c", &move);
        if (result == EOF) {
            printf("--- Quitting Game ---\n");
            break;
        }

        int* cur_row;
        int* cur_col;
        struct point_system* ps;

        int* start_row;
        int* start_col;

        int* rev_flag;
        int* split_flag;
        int* expl_flag;

        int* snake_len;
        struct position* body;

        int* act_split_head_row;
        int* act_split_head_col;

        struct explosion_location* explosion;

        struct position spawn_pos;

        if (turn == 0) {
            cur_row = &row;
            cur_col = &col;
            ps = orig_ps;

            start_row = orig_start_row;
            start_col = orig_start_col;

            rev_flag = orig_reverse;
            split_flag = orig_split;
            expl_flag = orig_explode;

            snake_len = orig_len;
            body = orig_body;

            act_split_head_row = orig_split_head_row;
            act_split_head_col = orig_split_head_col;

            explosion = &orig_explosion;

            spawn_pos = orig_spawn_pos;
        } else {
            cur_row = &rival_row;
            cur_col = &rival_col;
            ps = rival_ps;

            start_row = rival_start_row;
            start_col = rival_start_col;

            rev_flag = rival_reverse;
            split_flag = rival_split;
            expl_flag = rival_explode;

            snake_len = rival_len;
            body = rival_body;

            act_split_head_row = rival_split_head_row;
            act_split_head_col = rival_split_head_col;

            explosion = &rival_explosion;

            spawn_pos.row = *rival_start_row;
            spawn_pos.col = *rival_start_col;
        }

        snake_movement(cur_row, cur_col, board, ps, command, num_initial_apples,
                       start_row, start_col, rev_flag, split_flag, expl_flag,
                       snake_len, body, spawn_pos, initial_board, portals,
                       num_portals, radius, &move);

        if (explosion->active) {
            continue_explosion(board, explosion, *cur_row, *cur_col, ps,
                               num_initial_apples);
        }

        if (move == command->points || move == command->reset) {
            if (move == command->points) {
                int rem, maxp;
                double comp;
                compute_board_completion(board, *num_initial_apples, &rem,
                                         &comp, &maxp);
                print_game_statistics_with_rival(
                    orig_ps->points, orig_ps->moves_made,
                    orig_ps->num_apples_eaten, rival_ps->points,
                    rival_ps->moves_made, rival_ps->num_apples_eaten, rem, comp,
                    maxp);
            }
            if (move == command->reset) {
                explosion->active = 0;
            }
            continue;
        }

        int move_error = process_move_result(*cur_row, *cur_col, board, move,
                                             command, body, snake_len);
        if (move_error) {
            print_board_with_second_snake(board, row, col, rival_row,
                                          rival_col);

            printf("--- Game Over ---\n");
            if (move_error == 2) {
                if (turn == 0) {
                    printf("Guessss I was the prey today.\n");
                    printf("Rival snake wins!\n");
                } else {
                    printf("Guessss I was the prey today.\n");
                    printf("Original snake wins!\n");
                }
            } else {
                printf("Guessss I was the prey today.\n");
            }
            int rem, maxp;
            double comp;
            compute_board_completion(board, *num_initial_apples, &rem, &comp,
                                     &maxp);

            print_game_statistics_with_rival(
                orig_ps->points, orig_ps->moves_made, orig_ps->num_apples_eaten,
                rival_ps->points, rival_ps->moves_made,
                rival_ps->num_apples_eaten, rem, comp, maxp);

            break;
        }

        if (handle_special_apples(
                cur_row, cur_col, board, ps, command, num_initial_apples,
                start_row, start_col, rev_flag, split_flag, expl_flag,
                snake_len, body, spawn_pos, initial_board, portals, num_portals,
                radius, &move, act_split_head_row, act_split_head_col,
                explosion)) {
            print_board_with_second_snake(board, row, col, rival_row,
                                          rival_col);
            printf("--- Game Over ---\n");
            printf("Guessss I was the prey today.\n");
            if (turn == 0) {
                printf("Rival snake wins!\n");
            } else {
                printf("Original snake wins!\n");
            }
            int rem, maxp;
            double comp;
            compute_board_completion(board, *num_initial_apples, &rem, &comp,
                                     &maxp);

            print_game_statistics_with_rival(
                orig_ps->points, orig_ps->moves_made, orig_ps->num_apples_eaten,
                rival_ps->points, rival_ps->moves_made,
                rival_ps->num_apples_eaten, rem, comp, maxp);

            break;
        }

        if (*expl_flag == 1) {
            int middle_row = *cur_row;
            int middle_col = *cur_col;
            explode_snake_true(cur_row, cur_col, board, radius, &middle_row,
                               &middle_col, expl_flag, explosion, ps,
                               num_initial_apples, command);
        }

        if (row == rival_row && col == rival_col) {
            print_board_with_second_snake(board, row, col, rival_row,
                                          rival_col);

            printf("--- Game Over ---\n");

            if (board[row][col].entity == EXIT_UNLOCKED) {
                printf("Ssslithered out with a full stomach!\n");
            } else {
                printf("Guessss I was the prey today.\n");
            }
            printf("Original snake wins!\n");

            int rem, maxp;
            double comp;
            compute_board_completion(board, *num_initial_apples, &rem, &comp,
                                     &maxp);

            print_game_statistics_with_rival(
                orig_ps->points, orig_ps->moves_made, orig_ps->num_apples_eaten,
                rival_ps->points, rival_ps->moves_made,
                rival_ps->num_apples_eaten, rem, comp, maxp);

            break;
        }

        if (board[*cur_row][*cur_col].entity == EXIT_UNLOCKED) {
            print_board_with_second_snake(board, row, col, rival_row,
                                          rival_col);

            printf("--- Game Over ---\n");
            printf("Ssslithered out with a full stomach!\n");
            if (turn == 0) {
                printf("Original snake wins!\n");
            } else {
                printf("Rival snake wins!\n");
            }

            int rem, maxp;
            double comp;
            compute_board_completion(board, *num_initial_apples, &rem, &comp,
                                     &maxp);

            print_game_statistics_with_rival(
                orig_ps->points, orig_ps->moves_made, orig_ps->num_apples_eaten,
                rival_ps->points, rival_ps->moves_made,
                rival_ps->num_apples_eaten, rem, comp, maxp);

            break;
        }

        print_board_with_second_snake(board, row, col, rival_row, rival_col);

        turn ^= 1;
    }
}

int process_move_result(int row, int col, struct tile board[ROWS][COLS],
                        char move, struct map_commands* command,
                        struct position snake_body[], int* snake_length) {
    if ((lose_row_col_error_check(row, col)) == 1) {
        return 1;
    }
    if (board[row][col].entity == WALL) {
        return 2;
    }
    if ((passages_check(row, col, board, move)) == 1) {
        if (command->two_snakes_true == 0) {
            print_board(board, row, col);
        }
        return 1;
    }
    return 0;
}

int handle_special_apples(
    int* row, int* col, struct tile board[ROWS][COLS],
    struct point_system* point_system, struct map_commands* command,
    int* num_initial_apples, int* snake_start_row, int* snake_start_col,
    int* reverse_snake, int* split_snake, int* explode_snake, int* snake_length,
    struct position snake_body[], struct position position,
    struct tile initial_board[ROWS][COLS], struct portal_pairs portals[],
    int* num_portals, int* radius, char* move, int* split_head_row,
    int* split_head_col, struct explosion_location* explosion) {
    if (*reverse_snake == 1) {
        reverse_snake_true(board, snake_length, row, col, snake_start_row,
                           snake_start_col, snake_body, reverse_snake,
                           split_head_row, split_head_col, command);
    } else if (*split_snake == 1) {
        split_snake_true(board, snake_length, row, col, snake_start_row,
                         snake_start_col, snake_body, split_snake,
                         split_head_row, split_head_col, command);
    } else if (*explode_snake == 1) {
        int middle_row = *row;
        int middle_col = *col;
        explode_snake_true(row, col, board, radius, &middle_row, &middle_col,
                           explode_snake, explosion, point_system,
                           num_initial_apples, command);
        return 0;
    } else {
        snake_body[*snake_length].row = *row;
        snake_body[*snake_length].col = *col;
        *snake_length += 1;
        if (command->two_snakes_true == 0) print_board(board, *row, *col);
    }

    if (lose_tile_repeat_check(*row, *col, board) == 1) {
        return 1;
    }

    return 0;
}

void gameplay_begins(int row, int col, struct tile board[ROWS][COLS],
                     struct point_system* point_system,
                     struct map_commands* command, int* num_initial_apples,
                     int* snake_start_row, int* snake_start_col,
                     int* reverse_snake, int* split_snake, int* explode_snake,
                     int* snake_length, struct position snake_body[],
                     struct position position,
                     struct tile initial_board[ROWS][COLS],
                     struct portal_pairs portals[], int* num_portals,
                     int* radius, int* split_head_row, int* split_head_col) {
    char move;
    struct explosion_location explosion = {0, 0, 0, 0, 0};

    while (1) {
        int result = scanf(" %c", &move);
        if (result == EOF) {
            printf("--- Quitting Game ---\n");
            break;
        }

        snake_movement(&row, &col, board, point_system, command,
                       num_initial_apples, snake_start_row, snake_start_col,
                       reverse_snake, split_snake, explode_snake, snake_length,
                       snake_body, position, initial_board, portals,
                       num_portals, radius, &move);

        if (explosion.active) {
            continue_explosion(board, &explosion, row, col, point_system,
                               num_initial_apples);
        }

        if (move == command->points || move == command->reset) {
            if (move == command->reset) {
                explosion.active = 0;
            }
            continue;
        }

        int move_error = process_move_result(row, col, board, move, command,
                                             snake_body, snake_length);

        if (move_error) {
            if (command->two_snakes_true == 0 &&
                board[row][col].entity == WALL) {
                print_board(board, row, col);
            }
            lose_check(point_system);
            break;
        }

        int special_error = handle_special_apples(
            &row, &col, board, point_system, command, num_initial_apples,
            snake_start_row, snake_start_col, reverse_snake, split_snake,
            explode_snake, snake_length, snake_body, position, initial_board,
            portals, num_portals, radius, &move, split_head_row, split_head_col,
            &explosion);

        if (*explode_snake == 1) {
            int middle_row = row;
            int middle_col = col;
            explode_snake_true(&row, &col, board, radius, &middle_row,
                               &middle_col, explode_snake, &explosion,
                               point_system, num_initial_apples, command);
        }

        if (special_error) {
            if (command->two_snakes_true == 0 &&
                board[row][col].entity == WALL) {
                print_board(board, row, col);
            }
            lose_check(point_system);
            break;
        }

        if (win_check(row, col, board, point_system) == 1) {
            break;
        }
    }
}

void snake_movement(int* row, int* col, struct tile board[ROWS][COLS],
                    struct point_system* point_system,
                    struct map_commands* command, int* num_initial_apples,
                    int* snake_start_row, int* snake_start_col,
                    int* reverse_snake, int* split_snake, int* explode_snake,
                    int* snake_length, struct position snake_body[],
                    struct position position,
                    struct tile initial_board[ROWS][COLS],
                    struct portal_pairs portals[], int* num_portals,
                    int* radius, char* move) {
    if (*move == MOVE_UP) {
        board[*row][*col].entity = BODY_SEGMENT;
        *row -= 1;
    } else if (*move == MOVE_LEFT) {
        board[*row][*col].entity = BODY_SEGMENT;
        *col -= 1;
    } else if (*move == MOVE_DOWN) {
        board[*row][*col].entity = BODY_SEGMENT;
        *row += 1;
    } else if (*move == MOVE_RIGHT) {
        board[*row][*col].entity = BODY_SEGMENT;
        *col += 1;
    } else if (*move == command->reset) {
        reset_command(board, initial_board, position, snake_length, row, col,
                      snake_start_row, snake_start_col, snake_body,
                      point_system, num_initial_apples, reverse_snake,
                      split_snake, explode_snake, command);
        return;
    } else if (*move == command->points) {
        if (command->two_snakes_true == 0) {
            print_game_statistics(point_system->points,
                                  point_system->moves_made,
                                  point_system->num_apples_eaten,
                                  point_system->num_apples_remaining,
                                  point_system->completion_percentage,
                                  point_system->maximum_points_remaining);
        }
        return;
    }

    point_system->moves_made += 1;

    portal_check(row, col, board, *move, portals, *num_portals);
    apple_check(*row, *col, board, point_system, num_initial_apples,
                snake_start_row, snake_start_col, reverse_snake, split_snake,
                explode_snake, radius);
}

static void compute_board_completion(const struct tile board[ROWS][COLS],
                                     int initial_apples, int* num_remaining,
                                     double* completion_percentage,
                                     int* max_points_remaining) {
    int remaining = 0;
    int maxp = 0;

    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            enum entity e = board[r][c].entity;

            if (e == APPLE_NORMAL) {
                remaining++;
                maxp += 5;
            } else if (e == APPLE_REVERSE) {
                remaining++;
                maxp += 10;
            } else if (e == APPLE_SPLIT) {
                remaining++;
                maxp += 20;
            } else if (e == APPLE_EXPLODE) {
                remaining++;
                maxp += 20;
            }
        }
    }

    *num_remaining = remaining;

    if (initial_apples == 0) {
        *completion_percentage = 100.0;
    } else {
        int eaten = initial_apples - remaining;
        *completion_percentage = 100.0 * eaten / initial_apples;
    }

    *max_points_remaining = maxp;
}

void reverse_snake_true(struct tile board[ROWS][COLS], int* snake_length,
                        int* row, int* col, int* snake_start_row,
                        int* snake_start_col, struct position snake_body[],
                        int* reverse_snake, int* split_head_row,
                        int* split_head_col, struct map_commands* command) {
    board[*snake_start_row][*snake_start_col].entity = EMPTY;
    board[*row][*col].entity = BODY_SEGMENT;

    int temporary_row = *snake_start_row;
    int temporary_col = *snake_start_col;

    *snake_start_row = *row;
    *snake_start_col = *col;

    *row = temporary_row;
    *col = temporary_col;

    for (int i = 0; i < (*snake_length - 1); i++) {
        struct position temporary = snake_body[i];
        snake_body[i] = snake_body[*snake_length - 1 - i];
        snake_body[*snake_length - 1 - i] = temporary;
    }

    *reverse_snake = 0;
    if (command->two_snakes_true == 0) print_board(board, *row, *col);
}

void split_snake_true(struct tile board[ROWS][COLS], int* snake_length,
                      int* row, int* col, int* snake_start_row,
                      int* snake_start_col, struct position snake_body[],
                      int* split_snake, int* split_head_row,
                      int* split_head_col, struct map_commands* command) {
    int n = *snake_length;
    int remove_half;

    if (n % 2 == 0) {
        remove_half = n / 2;
    } else {
        remove_half = (n + 1) / 2;
    }

    for (int i = 0; i < remove_half; i++) {
        board[snake_body[i].row][snake_body[i].col].entity = EMPTY;
    }

    int new_length = n - remove_half;
    for (int i = 0; i < new_length; i++) {
        snake_body[i] = snake_body[i + remove_half];
    }

    snake_body[new_length].row = *row;
    snake_body[new_length].col = *col;
    new_length++;

    *snake_length = new_length;
    *split_snake = 0;

    *snake_start_row = snake_body[0].row;
    *snake_start_col = snake_body[0].col;

    if (command->two_snakes_true == 0) print_board(board, *row, *col);
}

void update_explosion_bounds(int* up_row, int* down_row, int* left_col,
                             int* right_col) {
    (*up_row) -= 1;
    (*down_row) += 1;
    (*left_col) -= 1;
    (*right_col) += 1;
}

void apply_explosion_wave(struct tile board[ROWS][COLS], int middle_row,
                          int middle_col, int radius,
                          struct point_system* point_system,
                          int* num_initial_apples) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            int row_diff = row - middle_row;
            if (row_diff < 0) {
                row_diff = -row_diff;
            }
            int col_diff = col - middle_col;
            if (col_diff < 0) {
                col_diff = -col_diff;
            }
            int diamond_diff = row_diff + col_diff;
            if (diamond_diff == radius) {
                int row2 = row;
                int col2 = col;
                explosion_direction(board, &row2, &col2, point_system,
                                    num_initial_apples);
            }
        }
    }
    if (point_system->maximum_points_remaining == 0)
        unlock_exit(board, point_system);
}

void clear_explosions(struct tile board[ROWS][COLS]) {
    for (int temp_row = 0; temp_row < ROWS; temp_row++) {
        for (int temp_col = 0; temp_col < COLS; temp_col++) {
            if (board[temp_row][temp_col].entity == EXPLOSION) {
                board[temp_row][temp_col].entity = EMPTY;
            }
        }
    }
}

void explode_snake_true(int* row, int* col, struct tile board[ROWS][COLS],
                        int* radius, int* middle_row, int* middle_col,
                        int* explode_snake,
                        struct explosion_location* explosion,
                        struct point_system* point_system,
                        int* num_initial_apples, struct map_commands* command) {
    explosion->active = 1;
    explosion->current_radius = 1;
    explosion->max_radius = *radius;
    explosion->middle_row = *middle_row;
    explosion->middle_col = *middle_col;

    apply_explosion_wave(board, *middle_row, *middle_col, 1, point_system,
                         num_initial_apples);

    if (command->two_snakes_true == 0) print_board(board, *row, *col);
    *explode_snake = 0;
}

void continue_explosion(struct tile board[ROWS][COLS],
                        struct explosion_location* explosion, int row, int col,
                        struct point_system* point_system,
                        int* num_initial_apples) {
    if (explosion->active == 0) {
        return;
    }

    if (explosion->current_radius >= explosion->max_radius) {
        clear_explosions(board);
        explosion->active = 0;
        return;
    }

    clear_explosions(board);
    explosion->current_radius++;

    apply_explosion_wave(board, explosion->middle_row, explosion->middle_col,
                         explosion->current_radius, point_system,
                         num_initial_apples);
}

void explosion_direction(struct tile board[ROWS][COLS], int* row, int* col,
                         struct point_system* point_system,
                         int* num_initial_apples) {
    if (bounds_checker(*row, *col)) {
        if (board[*row][*col].entity != EXIT_LOCKED &&
            board[*row][*col].entity != EXIT_UNLOCKED) {
            point_add(row, col, board, point_system, num_initial_apples);
            board[*row][*col].entity = EXPLOSION;
        }
    }
}

void point_add(int* row, int* col, struct tile board[ROWS][COLS],
               struct point_system* point_system, int* num_initial_apples) {
    if (board[*row][*col].entity == APPLE_SPLIT) {
        *num_initial_apples -= 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 20;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
    } else if (board[*row][*col].entity == APPLE_NORMAL) {
        *num_initial_apples -= 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 5;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
    } else if (board[*row][*col].entity == APPLE_REVERSE) {
        *num_initial_apples -= 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 10;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
    }
}

void reset_command(struct tile board[ROWS][COLS],
                   struct tile initial_board[ROWS][COLS],
                   struct position position, int* snake_length, int* row,
                   int* col, int* snake_start_row, int* snake_start_col,
                   struct position snake_body[],
                   struct point_system* point_system, int* num_initial_apples,
                   int* reverse_snake, int* split_snake, int* explode_snake,
                   struct map_commands* command) {
    *row = position.row;
    *col = position.col;

    *snake_length = 1;

    *snake_start_row = *row;
    *snake_start_col = *col;

    snake_body[0].row = *row;
    snake_body[0].col = *col;

    for (int temp_row = 0; temp_row < ROWS; temp_row++) {
        for (int temp_col = 0; temp_col < COLS; temp_col++) {
            board[temp_row][temp_col] = initial_board[temp_row][temp_col];
        }
    }

    point_system->maximum_points_remaining = point_system->initial_max_points;

    point_system->points = 0;
    point_system->moves_made = 0;
    point_system->num_apples_eaten = 0;
    point_system->num_apples_remaining = *num_initial_apples;

    if (num_initial_apples == 0) {
        point_system->completion_percentage = 100.0;
    } else {
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
    }

    *reverse_snake = 0;
    *split_snake = 0;
    *explode_snake = 0;

    printf("--- Resetting Map ---\n");
    if (command->two_snakes_true == 0) print_board(board, *row, *col);
}

int passages_check(int row, int col, struct tile board[ROWS][COLS], char move) {
    if (board[row][col].entity == PASSAGE_UP) {
        if (move != MOVE_UP) {
            return 1;
        }
    } else if (board[row][col].entity == PASSAGE_DOWN) {
        if (move != MOVE_DOWN) {
            return 1;
        }
    } else if (board[row][col].entity == PASSAGE_LEFT) {
        if (move != MOVE_LEFT) {
            return 1;
        }
    } else if (board[row][col].entity == PASSAGE_RIGHT) {
        if (move != MOVE_RIGHT) {
            return 1;
        }
    }

    if (board[row][col].entity == PASSAGE_UP ||
        board[row][col].entity == PASSAGE_DOWN ||
        board[row][col].entity == PASSAGE_LEFT ||
        board[row][col].entity == PASSAGE_RIGHT) {
        board[row][col].entity = EMPTY;
    }
    return 0;
}

void portal_check(int* row, int* col, struct tile board[ROWS][COLS], char move,
                  struct portal_pairs portals[], int num_portals) {
    if (board[*row][*col].entity != PORTAL) {
        return;
    }

    int paired_row = -1;
    int paired_col = -1;

    for (int i = 0; i < num_portals; i++) {
        if (portals[i].row1 == *row && portals[i].col1 == *col) {
            paired_row = portals[i].row2;
            paired_col = portals[i].col2;
        } else if (portals[i].row2 == *row && portals[i].col2 == *col) {
            paired_row = portals[i].row1;
            paired_col = portals[i].col1;
        }
    }

    if (paired_row != -1 && paired_col != -1) {
        *row = paired_row;
        *col = paired_col;
    }

    if (move == MOVE_UP) {
        *row = *row - 1;
    } else if (move == MOVE_DOWN) {
        *row = *row + 1;
    } else if (move == MOVE_LEFT) {
        *col = *col - 1;
    } else if (move == MOVE_RIGHT) {
        *col = *col + 1;
    }
}

void apple_check(int row, int col, struct tile board[ROWS][COLS],
                 struct point_system* point_system, int* num_initial_apples,
                 int* snake_start_row, int* snake_start_col, int* reverse_snake,
                 int* split_snake, int* explode_snake, int* radius) {
    if (board[row][col].entity == APPLE_NORMAL) {
        point_system->points += 5;
        point_system->num_apples_eaten += 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 5;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
        board[row][col].entity = EMPTY;
    } else if (board[row][col].entity == APPLE_REVERSE) {
        point_system->points += 10;
        point_system->num_apples_eaten += 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 10;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
        board[row][col].entity = EMPTY;

        *reverse_snake = 1;
    } else if (board[row][col].entity == APPLE_SPLIT) {
        point_system->points += 20;
        point_system->num_apples_eaten += 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 20;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
        board[row][col].entity = EMPTY;

        *split_snake = 1;
    } else if (board[row][col].entity == APPLE_EXPLODE) {
        *radius = board[row][col].radius;
        point_system->points += 20;
        point_system->num_apples_eaten += 1;
        point_system->num_apples_remaining -= 1;
        point_system->maximum_points_remaining -= 20;
        point_system->completion_percentage =
            100.0 * point_system->num_apples_eaten / *num_initial_apples;
        board[row][col].entity = EMPTY;

        *explode_snake = 1;
    }

    if (point_system->num_apples_remaining == 0) {
        unlock_exit(board, point_system);
    }

    return;
}

void unlock_exit(struct tile board[ROWS][COLS],
                 struct point_system* point_system) {
    for (int row_checker = 0; row_checker < ROWS; row_checker++) {
        for (int col_checker = 0; col_checker < COLS; col_checker++) {
            if (board[row_checker][col_checker].entity == EXIT_LOCKED) {
                board[row_checker][col_checker].entity = EXIT_UNLOCKED;
            }
        }
    }
}

int win_check(int row, int col, struct tile board[ROWS][COLS],
              struct point_system* point_system) {
    if (board[row][col].entity == EXIT_UNLOCKED) {
        printf("--- Game Over ---\n");
        printf("Ssslithered out with a full stomach!\n");
        print_game_statistics(point_system->points, point_system->moves_made,
                              point_system->num_apples_eaten,
                              point_system->num_apples_remaining,
                              point_system->completion_percentage,
                              point_system->maximum_points_remaining);
        return 1;
    }
    return 0;
}

void lose_check(struct point_system* point_system) {
    printf("--- Game Over ---\n");
    printf("Guessss I was the prey today.\n");
    print_game_statistics(point_system->points, point_system->moves_made,
                          point_system->num_apples_eaten,
                          point_system->num_apples_remaining,
                          point_system->completion_percentage,
                          point_system->maximum_points_remaining);
}

// =============================================================================
// EDITABLE Provided Functions
// =============================================================================

// Given a 2D board array, initialise all tiles to EMPTY.
void initialise_board(struct tile board[ROWS][COLS]) {
    for (int row = 0; row < ROWS; row++) {
        for (int col = 0; col < COLS; col++) {
            board[row][col].entity = EMPTY;
        }
    }
}

void print_board_with_second_snake(struct tile board[ROWS][COLS], int snake_row,
                                   int snake_col, int rival_snake_position_row,
                                   int rival_snake_position_col) {
    print_board_line();
    print_board_header();
    print_board_line();
    for (int row = 0; row < ROWS; row++) {
        print_tile_spacer();
        for (int col = 0; col < COLS; col++) {
            printf(" ");
            if (row == snake_row && col == snake_col) {
                printf("^~^");
            } else if (row == rival_snake_position_row &&
                       col == rival_snake_position_col) {
                printf("^v^");
            } else {
                struct tile tile = board[row][col];
                if (tile.entity == WALL) {
                    printf("|||");
                } else if (tile.entity == BODY_SEGMENT) {
                    printf("###");
                } else if (tile.entity == EXIT_LOCKED) {
                    printf("[X]");
                } else if (tile.entity == EXIT_UNLOCKED) {
                    printf("[ ]");
                } else if (tile.entity == APPLE_NORMAL) {
                    printf("(`)");
                } else if (tile.entity == APPLE_REVERSE) {
                    printf("(R)");
                } else if (tile.entity == APPLE_SPLIT) {
                    printf("(S)");
                } else if (tile.entity == APPLE_EXPLODE) {
                    printf("(%d)", tile.radius);
                } else if (tile.entity == PASSAGE_UP) {
                    printf("^^^");
                } else if (tile.entity == PASSAGE_DOWN) {
                    printf("vvv");
                } else if (tile.entity == PASSAGE_LEFT) {
                    printf("<<<");
                } else if (tile.entity == PASSAGE_RIGHT) {
                    printf(">>>");
                } else if (tile.entity == PORTAL) {
                    printf("~O~");
                } else if (tile.entity == EXPLOSION) {
                    printf("***");
                } else {
                    printf("   ");
                }
            }
        }
        printf("\n");
    }
    print_tile_spacer();
}

// Prints the game board, showing the snake's head position on the board.
void print_board(struct tile board[ROWS][COLS], int snake_row, int snake_col) {
    print_board_line();
    print_board_header();
    print_board_line();
    for (int row = 0; row < ROWS; row++) {
        print_tile_spacer();
        for (int col = 0; col < COLS; col++) {
            printf(" ");
            struct tile tile = board[row][col];
            // if (row ==)
            if (row == snake_row && col == snake_col) {
                printf("^~^");
            } else if (tile.entity == WALL) {
                printf("|||");
            } else if (tile.entity == BODY_SEGMENT) {
                printf("###");
            } else if (tile.entity == EXIT_LOCKED) {
                printf("[X]");
            } else if (tile.entity == EXIT_UNLOCKED) {
                printf("[ ]");
            } else if (tile.entity == APPLE_NORMAL) {
                printf("(`)");
            } else if (tile.entity == APPLE_REVERSE) {
                printf("(R)");
            } else if (tile.entity == APPLE_SPLIT) {
                printf("(S)");
            } else if (tile.entity == APPLE_EXPLODE) {
                printf("(%d)", board[row][col].radius);
            } else if (tile.entity == PASSAGE_UP) {
                printf("^^^");
            } else if (tile.entity == PASSAGE_DOWN) {
                printf("vvv");
            } else if (tile.entity == PASSAGE_LEFT) {
                printf("<<<");
            } else if (tile.entity == PASSAGE_RIGHT) {
                printf(">>>");
            } else if (tile.entity == PORTAL) {
                printf("~O~");
            } else if (tile.entity == EXPLOSION) {
                printf("***");
            } else {
                printf("   ");
            }
        }
        printf("\n");
    }
    print_tile_spacer();
}

// =============================================================================
// DO NOT EDIT Provided Functions
// =============================================================================

// Prints statistics about the game
void print_game_statistics(int points, int moves_made, int num_apples_eaten,
                           int num_apples_remaining,
                           double completion_percentage,
                           int maximum_points_remaining) {
    printf("============ Game Statistics ============\n");
    printf("Totals:\n");
    printf("  - Points: %d\n", points);
    printf("  - Moves Made: %d\n", moves_made);
    printf("  - Number of Apples Eaten: %d\n", num_apples_eaten);
    printf("Completion:\n");
    printf("  - Number of Apples Remaining: %d\n", num_apples_remaining);
    printf("  - Apple Completion Percentage: %.1f%%\n", completion_percentage);
    printf("  - Maximum Points Remaining: %d\n", maximum_points_remaining);
    printf("=========================================\n");
}

// Prints statistics about the game for both snakes when there are two players
void print_game_statistics_with_rival(
    int original_points, int original_moves_made, int original_num_apples_eaten,
    int rival_points, int rival_moves_made, int rival_num_apples_eaten,
    int num_apples_remaining, double completion_percentage,
    int maximum_points_remaining) {
    printf("============ Game Statistics ============\n");
    printf("Original Snake Totals:\n");
    printf("  - Points: %d\n", original_points);
    printf("  - Moves Made: %d\n", original_moves_made);
    printf("  - Number of Apples Eaten: %d\n", original_num_apples_eaten);
    printf("Rival Snake Totals:\n");
    printf("  - Points: %d\n", rival_points);
    printf("  - Moves Made: %d\n", rival_moves_made);
    printf("  - Number of Apples Eaten: %d\n", rival_num_apples_eaten);
    printf("Completion:\n");
    printf("  - Number of Apples Remaining: %d\n", num_apples_remaining);
    printf("  - Apple Completion Percentage: %.1f%%\n", completion_percentage);
    printf("  - Maximum Points Remaining: %d\n", maximum_points_remaining);
    printf("=========================================\n");
}

// Helper function for print_board().
void print_board_header(void) {
    printf("|            C S _ S N A K E            |\n");
}

// Helper function for print_board().
void print_board_line(void) {
    printf("+");
    for (int col = 0; col < COLS; col++) {
        printf("---+");
    }
    printf("\n");
}

// Helper function for print_board().
void print_tile_spacer(void) {
    printf("+");
    for (int col = 0; col < COLS; col++) {
        printf("   +");
    }
    printf("\n");
}

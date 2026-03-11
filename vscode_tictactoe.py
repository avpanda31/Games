def print_board(board):
    print(f"  {board[0]}  |  {board[1]}  |  {board[2]}")
    print("-----+-----+----")
    print(f"  {board[3]}  |  {board[4]}  |  {board[5]}")
    print("-----+-----+----")
    print(f"  {board[6]}  |  {board[7]}  |  {board[8]}")
    print("\n")


def player_chance(board, player, player_name):
    print_board(board)

    choice = input(f"{player_name}, your turn. Choose a number from 1 to 9:")
    while choice not in board:
        choice = input(f"{player_name}, your turn. Choose a number from 1 to 9:")

    if player == 1:
        board[int(choice) - 1] = "X"
        print_board(board)
    elif player == 2:
        board[int(choice) - 1] = "O"
        print_board(board)

    return board


def check_win(board):
    # THIS IS FOR ROWS!!:
    for start in [0, 3, 6]:
        if board[start] == board[start + 1] == board[start + 2] != "":
            return False
        # THIS IS FOR COLUMNS!!
    for start in [0, 1, 2]:
        if board[start] == board[start + 3] == board[start + 6] != "":
            return False
    # THIS IS FOR DIAGONALS!!
    if board[0] == board[4] == board[8] != "":
        return False
    if board[2] == board[4] == board[6] != "":
        return False
    return True


def check_board_spaces_left(board):
    for i in range(1, 10):
        if str(i) in board:
            return True
    return False


name = input("Player 1, enter your name:")
name2 = input("Player 2, enter your name:")

board = ["1", "2", "3", "4", "5", "6", "7", "8", "9"]

no_win = True
board_spaces_left = True
player = 1
while no_win and board_spaces_left:
    if player == 1:
        board = player_chance(board, player, name)
        player = 2
    elif player == 2:
        board = player_chance(board, player, name2)
        player = 1

    board_spaces_left = check_board_spaces_left(board)
    no_win = check_win(board)

if no_win == False:
    if player == 2:
        print(f"{name} WINS!")
    else:
        print(f"{name2} WINS!")
elif board_spaces_left == False:
    print("TIE GAME :(")

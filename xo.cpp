#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <ctime>
#include <cstdlib>
#define XO_BOARD_ROW_SIZE 3
#define XO_BOARD_COLUMN_SIZE 3

void clear_screen()
{
#ifdef _WIN32
  system("cls"); // For Windows
#else
  system("clear"); // For Unix/Linux
#endif
}

void print_arr(char arr[][XO_BOARD_COLUMN_SIZE], int arr_size)
{
  char letters[] = {'A', 'B', 'C'};
  int line = 1;

  clear_screen();

  std::cout << "  ";
  for (char l : letters)
  {
    std::cout << l << "\t";
  }

  std::cout << std::endl
            << std::endl;

  for (int i = 0; i < arr_size; i++)
  {
    std::cout << line << " ";
    for (char ch : arr[i])
    {
      std::cout << ch << "\t";
    }
    std::cout << "\n\n";
    line++;
  }
}

void fill_arr(char arr[][XO_BOARD_COLUMN_SIZE], int size_a, char ch)
{
  for (int i = 0; i < size_a; i++)
  {
    for (int j = 0; j < XO_BOARD_COLUMN_SIZE; j++)
    {
      arr[i][j] = ch;
    }
  }
}

enum Player
{
  X_PLAYER,
  O_PLAYER
};

void playTurn(int row, int column, Player p, char arr[][XO_BOARD_COLUMN_SIZE])
{
  arr[row][column] = p == X_PLAYER ? 'X' : 'O';
}

bool check_win(char arr[][XO_BOARD_COLUMN_SIZE], Player p)
{
  char turn = p == X_PLAYER ? 'X' : 'O';
  if ((arr[0][0] == turn && arr[0][1] == turn && arr[0][2] == turn) || // Top row
      (arr[1][0] == turn && arr[1][1] == turn && arr[1][2] == turn) || // Middle row
      (arr[2][0] == turn && arr[2][1] == turn && arr[2][2] == turn) || // Bottom row
      (arr[0][0] == turn && arr[1][0] == turn && arr[2][0] == turn) || // Left column
      (arr[0][1] == turn && arr[1][1] == turn && arr[2][1] == turn) || // Middle column
      (arr[0][2] == turn && arr[1][2] == turn && arr[2][2] == turn) || // Right column
      (arr[0][0] == turn && arr[1][1] == turn && arr[2][2] == turn) || // Diagonal from top-left to bottom-right
      (arr[0][2] == turn && arr[1][1] == turn && arr[2][0] == turn))   // Diagonal from top-right to bottom-left
  {
    return true;
  }
  else
  {
    return false;
  }
}

int main()
{

  std::cout << "******* XO ******" << std::endl;

  // Setup
  char xo_arr[XO_BOARD_ROW_SIZE][XO_BOARD_COLUMN_SIZE];

  fill_arr(xo_arr, XO_BOARD_ROW_SIZE, '.');

  // Print
  print_arr(xo_arr, XO_BOARD_ROW_SIZE);

  std::cout << std::endl;

  std::vector<std::string> possible_plays;

  while (true)
  {
    // keep playing
    std::cout << "Your turn Play (row x col): ";
    std::string play_value;
    std::cin >> play_value;

    // evaluate_choice(xo_arr, play_value);
    char choice_row = play_value[0];
    char choice_column = play_value[1];
    if (choice_row == '1')
    {
      choice_row = 0;
    }
    else if (choice_row == '2')
    {
      choice_row = 1;
    }
    else if (choice_row == '3')
    {
      choice_row = 2;
    }
    else if (choice_row == 'X' || choice_row == 'x')
    {
      std::cout << "Bye";
      break;
    }
    else
    {
      std::cout << "Invalid row play again" << std::endl;
      continue;
    }

    if (choice_column == 'A')
    {
      choice_column = 0;
    }
    else if (choice_column == 'B')
    {
      choice_column = 1;
    }
    else if (choice_column == 'C')
    {
      choice_column = 2;
    }
    else if (choice_column == 'X' || choice_column == 'x')
    {
      std::cout << "Bye";
      break;
    }
    else
    {
      std::cout << "Invalid column play again" << std::endl;
      continue;
    }

    playTurn(choice_row, choice_column, X_PLAYER, xo_arr);

    // Print
    print_arr(xo_arr, XO_BOARD_ROW_SIZE);

    // Check win
    if (check_win(xo_arr, X_PLAYER))
    {
      std::cout << "You won !!!!!!!";
      break;
    }

    // computer play - Find available spots and pick best option (random for now)
    std::vector<std::array<int, 2>> row_col_arr;
    for (int i = 0; i < XO_BOARD_ROW_SIZE; i++)
    {
      for (int j = 0; j < XO_BOARD_COLUMN_SIZE; j++)
      {
        if (xo_arr[i][j] == '.')
        {
          row_col_arr.push_back({i, j});
        }
      }
    }

    // seed for random pick
    srand(static_cast<unsigned>(time(nullptr)));

    int random_idx = rand() % row_col_arr.size();

    playTurn(row_col_arr[random_idx][0], row_col_arr[random_idx][1], O_PLAYER, xo_arr);

    // Print outcome
    print_arr(xo_arr, XO_BOARD_ROW_SIZE);

    // Check computer win
    if (check_win(xo_arr, O_PLAYER))
    {
      std::cout << "You failed !!!!!!!";
      break;
    }
  }

  std::cout << std::endl;

  return 0;
}
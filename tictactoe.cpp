#include<iostream>
#include<vector>
using namespace std;


struct turns
{
    int x, y;
    double win, draw, defeat;
};


int is_win(const vector<vector<int>> field)//0 - не закончена игра, -1 - ничья, 1 - победили крестики, 2 - победили нолики
{
    for (int i = 0; i < 3; ++i)
    {
        if (field[i][0] == field[i][1] && field[i][1] == field[i][2] && field[i][0] != 0)
            return field[i][0];
        if (field[0][i] == field[1][i] && field[1][i] == field[2][i] && field[0][i] != 0)
            return field[0][i];
    }
    if ((field[1][1] == field[2][2] && field[2][2] == field[0][0] && field[0][0] != 0) || (field[0][2] == field[1][1] && field[1][1] == field[2][0] && field[1][1] != 0))
        return field[1][1];
    
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            if (field[i][j] == 0)
                return 0;
    
    return -1;
}


void out_field(vector<vector<int>> field)
{
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            switch (field[i][j])
            {
                case 0:
                {
                    cout << " ";
                    break;
                }
                case 1:
                {
                    cout << "X";
                    break;
                }
                case 2:
                {
                    cout << "O";
                    break;
                }
            }
            if (j != 2)
                cout << "|";
        }
        cout << endl;
        if (i != 2)
            cout << "-+-+-" << endl;
    }
    cout << endl;
}


void fill_field(vector<vector<int>> & field, int x, int y, int type)
{
    field[x][y] = type;
}//надо сделать проверку на ввод недопустимых значений


void analyze(vector<vector<int>> & field, vector<int> & results, int type)
{//results[0] - ничья, results[1] - поражение, results[2] - победа
    int wins = is_win(field);
    switch (wins)
    {
        case -1:
        {
            results[0]++;
            return;
        }
        case 1:
        {
            results[1]++;
            return;
        }
        case 2:
        {
            results[2]++;
            return;
        }
    }


    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            if (field[i][j] == 0)
            {
                field[i][j] = type;
                analyze(field, results, type == 1 ? 2 : 1);
                field[i][j] = 0;
            }
        }
    }
    return;
}


void fill_results(turns & results, vector<int> res, int x, int y)
{
    int sum = res[0] + res[1] + res[2];
    results.x = x;
    results.y = y;
    results.defeat = float(res[1]) / sum;
    results.draw = float(res[0]) / sum;
    results.win = float(res[2]) / sum;
}


void copy(const vector<vector<int>> field, vector<vector<int>> & copy_field)
{
    for (int k = 0; k < 3; ++k)
        for (int l = 0; l < 3; ++l)
            copy_field[k][l] = field[k][l];
}


turns find_best_turn(turns * arr, int size)
{
    turns min_def;
    min_def.defeat = 1;
    for (int i = 0; i < size; ++i)
    {
        if (arr[i].defeat < min_def.defeat)
            min_def = arr[i];

        if (arr[i].defeat == min_def.defeat)
            if (arr[i].win > min_def.win)
                min_def = arr[i];
    }
    return min_def;
}


void pc_turn(vector<vector<int>> & field)
{
    turns arr[9];
    vector<int> res(3, 0);
    vector<vector<int>> copy_field(3, vector<int>(3, 0));
    int r = 0;


    for (int i = 0; i < 3; ++i)
    {   
        for (int j = 0; j < 3; ++j)
        {
            if (field[i][j] == 0)
            {
                copy(field, copy_field);

                copy_field[i][j] = 2;

                analyze(copy_field, res, 1);

                fill_results(arr[r++], res, i, j);

                res[0] = res[1] = res[2] = 0;
            }
        }
    }

    turns best_turn = find_best_turn(arr, r);
    
    
    fill_field(field, best_turn.x, best_turn.y, 2);    
}


void game_pc_turn(vector<vector<int>> & field)
{
    pc_turn(field);


    out_field(field);
}


void game_user_turn(vector<vector<int>> & field)
{
    int x, y;
    cin >> x >> y;

    while (!(x >= 0 && x < 3 && y >= 0 && y < 3 && field[x][y] == 0))
    {
        cout << "Wrong input. Try again" << endl;
        cin >> x >> y;
    }

    fill_field(field, x - 1, y - 1, 1);

    out_field(field);
}


bool out_game_situation(vector<vector<int>> field)
{
    int win = is_win(field);

    if (win == -1)
    {
        cout << "Draw" << endl;
        return true;
    }

    if (win != 0)
    {
        cout << "Win " << win << endl;
        return true;
    }
    return false;
}



void start_game()
{
    vector<vector<int>> field(3, vector<int>(3, 0));

    bool first_turn;
    cout << "Who first turn? PC - 0, man - 1  ";
    cin >> first_turn;


    for (int k = 0; k < 5; ++k)
    {

        if (!first_turn)
        {
            game_pc_turn(field);

            if (out_game_situation(field))
                break;

            game_user_turn(field);

            if (out_game_situation(field))
                break;
                
            continue;
        }
        else
        {
            game_user_turn(field);

            if (out_game_situation(field))
                break;
                

            game_pc_turn(field);

            if (out_game_situation(field))
                break;
        }
    }
}


int main()
{
    start_game();
    return 0;
}
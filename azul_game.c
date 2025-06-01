#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TRED "\x1B[31m"
#define TBLUE "\x1B[34m"
#define TYELLOW "\x1B[33m"
#define TWHITE "\x1B[37m"
#define TBLACK "\x1B[32m" // green
#define TRESET "\x1B[0m"
#define TMAG "\x1B[35m"



//  ************ STRUCTURES********************

enum player_nb
{
    PLAYER_1 = 0,
    PLAYER_2,
    PLAYER_3,
    PLAYER_4
};

enum colors
{
    BLUE = 0,
    RED,
    YELLOW,
    BLACK,
    WHITE,
    MARKER

};

struct player_data
{
    int pflag; // 1 no skip/ 0 skip round
    char name[15];
    int rounds_won;
} plyr[4];

struct game_bag
{ // 20/color
    int tiles[5];
} bag;

struct game_lid
{ // 20/color
    int tiles[5];
} lid;

struct individual_game_board
{
    int score;
    char lines[5][5];  //-1 initially
    char wall[5][5];   //-1
    char floor[7];     //-1
    char newall[5][5]; //-1 for keeping score

} brd[4];

typedef struct center
{
    int tiles[5];
    int st_pl_marker; ///// 1/0
} cent;

typedef struct factory_display
{
    int fflag;
    int tiles[5];
} fact;

struct game_table
{
    cent ct;
    fact fact_dys[9];
} table;

// *************GAME/ROUND PREPARATION********************

void init_game(int nbplayer, int new_token)
{ // only at start of game (setup structures)

    for (int i = 0; i < 9; i++)
    { // tile data
        if (i < 5)
        {
            bag.tiles[i] = 20;
            lid.tiles[i] = 0;
            table.ct.tiles[i] = 0;
        }
        if (i < (nbplayer * 2 + 1))
        {
            for (int j = 0; j < 9; j++)
            {
                table.fact_dys[i].tiles[j] = 0;
            }
            table.fact_dys[i].fflag = 1;
        }
        else
        {
            table.fact_dys[i].fflag = 0;
        }
    }
    table.ct.st_pl_marker = 1;

    for (int i = 0; i < 4; i++)
    { // player data
        if (i < nbplayer)
        {
            plyr[i].pflag = 1;
            for (int h = 0; h <= 6; h++)
            {
                brd[i].floor[h] = -1;
            }
        }
        else
        {
            plyr[i].pflag = 0;
        }

        if (new_token)
        {
            brd[i].score = 0;
        }

        for (int g = 0; g < 5; g++)
        {
            for (int h = 0; h < 5; h++)
            {
                brd[i].lines[g][h] = -1;
                brd[i].wall[g][h] = -1;
            }
        }
    }
}

void new_round_setup(int nbplayer)
{ // each new round (setup structures) ,pair with close_round(), calc score, delete board

    //// random filling of tiles
    table.ct.st_pl_marker = 1;

    for (int i = 0; i < nbplayer * 2 + 1; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            int ok = 1;
            while (ok)
            {
                int color = rand() % 5;
                if (bag.tiles[color])
                {
                    bag.tiles[color]--;
                    table.fact_dys[i].tiles[color]++;
                    ok = 0;
                }
            }
        }
        if (i < nbplayer)
            for (int g = 0; g < 5; g++)
            {
                for (int h = 0; h < 5; h++)
                {

                    brd[i].newall[g][h] = -1;
                }
            }
    }
}

// ****************Printing/Debugging**************************
void print_tile(int i, int board)
{
    switch (i)
    {
    case BLUE:
        printf(TBLUE "▦ " TRESET);
        break;
    case RED:
        printf(TRED "▧ " TRESET);
        break;
    case YELLOW:
        printf(TYELLOW "▩ " TRESET);
        break;
    case BLACK:
        printf(TBLACK "▧ " TRESET);
        break;
    case WHITE:
        printf(TWHITE "▣ " TRESET);
        break;
    case MARKER:
        printf(TMAG "◈ " TRESET);
        break;
    case -1:
        printf("⌑ ");
        break;
    default:
        printf("⌑ ");
        break;
    }
}

void print_debug(int nbplayer)
{
    printf("*************DEBUGGING*************\n\n");
    /// bag
    printf("Game bag: \n");
    for (int i = 0; i < 5; i++)
    {
        switch (i)
        {
        case BLUE:
            printf("No of blue tiles: %d  \n", bag.tiles[i]);
            break;
        case RED:
            printf("No of red tiles: %d  \n", bag.tiles[i]);
            break;
        case YELLOW:
            printf("No of yellow tiles: %d  \n", bag.tiles[i]);
            break;
        case BLACK:
            printf("No of black tiles: %d  \n", bag.tiles[i]);
            break;
        case WHITE:
            printf("No of white tiles: %d  \n", bag.tiles[i]);
            break;
        }
    }
    printf("\nGame lid: \n");
    for (int i = 0; i < 5; i++)
    {
        switch (i)
        {
        case BLUE:
            printf("No of blue tiles: %d  \n", lid.tiles[i]);
            break;
        case RED:
            printf("No of red tiles: %d  \n", lid.tiles[i]);
            break;
        case YELLOW:
            printf("No of yellow tiles: %d  \n", lid.tiles[i]);
            break;
        case BLACK:
            printf("No of black tiles: %d  \n", lid.tiles[i]);
            break;
        case WHITE:
            printf("No of black tiles: %d  \n", lid.tiles[i]);
            break;
        }
    }
    printf("\n");
    printf("\nCenter of the table:\n");
    printf("Player marker: %d\n", table.ct.st_pl_marker);
    printf("Tiles in the center: ");
    for (int i = 0; i < 5; i++)
    {
        switch (i)
        {
        case BLUE:
            printf(TBLUE "BLUE: %d  " TRESET, table.ct.tiles[i]);
            break;
        case RED:
            printf(TRED "RED: %d  " TRESET, table.ct.tiles[i]);
            break;
        case YELLOW:
            printf(TYELLOW "YELLOW: %d   " TRESET, table.ct.tiles[i]);
            break;
        case BLACK:
            printf(TBLACK "BLACK: %d  " TRESET, table.ct.tiles[i]);
            break;
        case WHITE:
            printf(TWHITE "WHITE: %d  " TRESET, table.ct.tiles[i]);
            break;
        }
    }
    printf("\n\n");

    printf("Factory Displays:\n");
    for (int t = 0; t < nbplayer * 2 + 1; t++)
    {
        printf("Tile_%d: ", t + 1);
        for (int i = 0; i < 5; i++)
        {

            switch (i)
            {
            case 0:
                printf(TBLUE "BLUE: %d  " TRESET, table.fact_dys[t].tiles[i]);

                break;
            case 1:
                printf(TRED "RED: %d  " TRESET, table.fact_dys[t].tiles[i]);
                break;
            case 2:
                printf(TYELLOW "YELLOW: %d   " TRESET, table.fact_dys[t].tiles[i]);
                break;
            case 3:
                printf(TBLACK "BLACK: %d  " TRESET, table.fact_dys[t].tiles[i]);
                break;
            case 4:
                printf(TWHITE "WHITE: %d  " TRESET, table.fact_dys[t].tiles[i]);
                break;
            }
        }
        printf("\n");
    }

    printf("\n\n\n");
}

void print_table(int nbplayer)
{
    printf("====================================================================================================================\n");
    printf("\nCenter of the table:\n");
    printf("Player marker: ");
    if (table.ct.st_pl_marker)
    {
        print_tile(5, 0);
    }
    else
    {
        printf("Taken");
    }
    putchar('\n');

    printf("Tiles in the center: ");
    for (int i = 0; i < 5; i++)
    {
        switch (i)
        {
        case BLUE:
            printf(TBLUE "%dx" TRESET, table.ct.tiles[i]);
            print_tile(0, 0);
            break;
        case RED:
            printf(TRED "  %dx" TRESET, table.ct.tiles[i]);
            print_tile(1, 0);
            break;
        case YELLOW:
            printf(TYELLOW "  %dx" TRESET, table.ct.tiles[i]);
            print_tile(2, 0);
            break;
        case BLACK:
            printf(TBLACK "  %dx" TRESET, table.ct.tiles[i]);
            print_tile(3, 0);
            break;
        case WHITE:
            printf(TWHITE "  %dx" TRESET, table.ct.tiles[i]);
            print_tile(4, 0);
            break;
        }
    }
    printf("\n\n");

    for (int t = 0; t < nbplayer * 2 + 1; t++)
    {
        printf("Fact_Disp_%d:    ", t + 1);
    }
    printf("\n ");

    for (int t = 0; t < nbplayer * 2 + 1; t++)
    {
        printf("/――――――――∖      ");
    }
    printf("\n ");
    /*
    for (int t = 0; t < nbplayer * 2 + 1; t++){
        printf("|      |        ");
    }
    printf("\n");
    */

    for (int t = 0; t < nbplayer * 2 + 1; t++)
    {
        printf("|");
        int empty = 1;
        for (int i = 0; i < 5; i++)
        {
            int cnt = 0;

            switch (i)
            {
            case 0:
                cnt = table.fact_dys[t].tiles[i];
                if (cnt < 4)
                    while (cnt > 0)
                    {
                        print_tile(i, 0);
                        cnt--;

                        empty = 0;
                    }

                break;
            case 1:
                cnt = table.fact_dys[t].tiles[i];
                if (cnt <= 4)
                    while (cnt > 0)
                    {
                        print_tile(i, 0);
                        cnt--;
                        empty = 0;
                    }

                break;
            case 2:
                cnt = table.fact_dys[t].tiles[i];
                if (cnt <= 4)
                    while (cnt > 0)
                    {
                        print_tile(i, 0);

                        cnt--;
                        empty = 0;
                    }

                break;

            case 3:
                cnt = table.fact_dys[t].tiles[i];
                if (cnt <= 4)
                    while (cnt > 0)
                    {
                        print_tile(i, 0);

                        cnt--;
                        empty = 0;
                    }

                break;
            case 4:
                cnt = table.fact_dys[t].tiles[i];
                if (cnt <= 4)
                    while (cnt > 0)
                    {
                        print_tile(i, 0);
                        cnt--;
                        empty = 0;
                    }
                break;
            }
        }
        if (empty)
        {
            printf("        ");
        }

        printf("|      ");
    }
    printf("\n ");
    for (int t = 0; t < nbplayer * 2 + 1; t++)
    {
        printf("∖――――――――/      ");
    }
    printf("\n ");

    /*
        for (int t = 0; t < nbplayer * 2 + 1; t++)
        {


            printf("Factory Display_%d: \n", t + 1);

            printf("/――――――∖\n");
            printf("|      |\n");
            printf("| ");

            int two = 0;
            for (int i = 0; i < 5; i++)
            {

                int cnt = 0;

                switch (i)
                {
                case 0:
                    cnt = table.fact_dys[t].tiles[i];
                    if (cnt < 4)
                        while (cnt > 0)
                        {
                            print_tile(i, 0);
                            cnt--;
                            two++;
                            if (two == 2)
                            {
                                printf(" |\n| ");
                                two = 0;
                            }
                        }

                    break;
                case 1:
                    cnt = table.fact_dys[t].tiles[i];
                    if (cnt <= 4)
                        while (cnt > 0)
                        {
                            print_tile(i, 0);
                            cnt--;
                            two++;
                            if (two == 2)
                            {
                                printf(" |\n| ");
                                two = 0;
                            }
                        }

                    break;
                case 2:
                    cnt = table.fact_dys[t].tiles[i];
                    if (cnt <= 4)
                        while (cnt > 0)
                        {
                            print_tile(i, 0);
                            // printf("%d",cnt);
                            cnt--;
                            two++;
                            if (two == 2)
                            {
                                printf(" |\n| ");
                                two = 0;
                            }
                        }

                    break;

                case 3:
                    cnt = table.fact_dys[t].tiles[i];
                    if (cnt <= 4)
                        while (cnt > 0)
                        {
                            print_tile(i, 0);

                            cnt--;
                            two++;
                            if (two == 2)
                            {
                                printf(" |\n| ");
                                two = 0;
                            }
                        }

                    break;
                case 4:
                    cnt = table.fact_dys[t].tiles[i];
                    if (cnt <= 4)
                        while (cnt > 0)
                        {
                            print_tile(i, 0);
                            cnt--;
                            two++;
                            if (two == 2)
                            {
                                printf(" |\n| ");
                                two = 0;
                            }
                        }
                    break;
                }
            }



            }
            printf("     |\n");
            printf("∖――――――/\n");

        }*/

    printf("\n");
    // player boards and data
    printf("********GAME TABLE*********\n");
    for (int i = 0; i < nbplayer; i++)
    {
        // printf("-----------------------------------\n");
        printf("﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏\n");
        printf("PLAYER %d: %s  score: %d\n", i + 1, plyr[i].name, brd[i].score);

        printf("Lines:       Wall:\n");
        for (int j = 0; j < 5; j++)
        {
            for (int k = 0; k < 5; k++)
            {
                if (k <= 4 - j - 1)
                {
                    printf("■ ");
                }
                else
                {
                    print_tile(brd[i].lines[j][k], 0);
                }
            }
            printf("▶  ");
            for (int k = 0; k < 5; k++)
            {
                if (brd[i].wall[j][k] != -1)
                {
                    print_tile(brd[i].wall[j][k], 0);
                }
                else
                { ////////verif matrice
                    if (k == j)
                    {
                        printf(TBLUE "⌑ " TRESET);
                    }
                    else if ((k - j == 1) || (j - k == 4))
                    {
                        printf(TYELLOW "⌑ " TRESET);
                    }
                    else if ((j - k == 1) || (k - j == 4))
                    {
                        printf(TWHITE "⌑ " TRESET);
                    }
                    else if ((k - j == 2) || (j - k == 3))
                    {
                        printf(TBLACK "⌑ " TRESET);
                    }
                    else if ((k - j == 3) || (j - k == 2))
                    {
                        printf(TRED "⌑ " TRESET);
                    }
                }
            }
            printf("\n");
        }

        // printf("Floor line: -1 -1 -2 -2 -2 -3 -3\n             ");
        printf("Floor line:\n-1 -1 -2 -2 -2 -3 -3\n ");
        for (int j = 0; j <= 6; j++)
        {
            print_tile(brd[i].floor[j], 0); /// 5-marker
            putchar(' ');
        }
        // printf("\n-----------------------------------\n");
        printf("\n﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏﹏\n");

        printf("\n");
    }
    printf("====================================================================================================================\n");
}

//*******************Round actions******************************
void add_floor(int player, int tcolor)
{
    for (int i = 0; i < 7; i++)
    {
        if (brd[player].floor[i] == -1)
        {
            brd[player].floor[i] = tcolor;
            break;
        }
    }
}

int check_board(int player, int tcolor, int line)
{
    int j = line;
    for (int k = 0; k < 5; k++)
    {
        if ((brd[player].wall[j][k] != -1) && (tcolor==brd[player].wall[j][k]))
        {
            switch (tcolor)
            {
            case BLUE:
                if (k == j)
                {
                    return 1;
                }
                break;
            case RED:
                if ((k - j == 1) || (j - k == 4))
                {
                    return 1;
                }
                break;
            case YELLOW:
                if ((j - k == 1) || (k - j == 4))
                {
                    return 1;
                }
            case BLACK:
                if ((k - j == 2) || (j - k == 3))
                {
                    return 1;
                }
                break;
            case WHITE:
                if ((k - j == 3) || (j - k == 2))
                {
                    return 1;
                }
                break;
            }
        }
    }
    return 0;
}

void tile_place(int player, int tcolor, int tnb)
{

    int ok = 0;

    while (!ok)
    {
        printf("The tiles will be placed on the following line(1-5):");
        int line;
        scanf("%d", &line);
        getchar();
        line--;
        if (line > 4 || line < 0)
        {
            printf("Line non-existent. Try again.\n");
        }
        else
        {
            if ((brd[player].lines[line][4] != tcolor) && (brd[player].lines[line][4] != -1))
            {
                printf("Line occupied by other color. Try again.\n");
            }
            else if (check_board(player, tcolor, line))
            {
                printf("Board line occupied by the color. Try again.\n");
            }
            else
            {

                int j = line;
                for (int k = 4; k >= 0; k--)
                {
                    if (k > 4 - j - 1)
                    {
                        if (brd[player].lines[j][k] == -1)
                        {
                            if (tnb > 0)
                            {
                                brd[player].lines[j][k] = tcolor;
                                tnb--;
                            }
                        }
                    }
                }
                while (tnb > 0)
                {
                    add_floor(player, tcolor);
                    tnb--;
                }
                ok = 1;
            }
        }
    }
}

void tile_select(int player, int player_nb)
{
    int dis_nb;
    int tcolor;
    int tnb;
    printf("\nPlayer %d:%s, please pick tiles from a display...\n", player + 1, plyr[player].name);
    printf("Displays:1-%d or Middle area:10 or Skip round:99\nColors:0-Blue,1-Red,2-Yellow,3-Black,4-White\n", player_nb * 2 + 1);
    int ok = 0;
    while (!ok)
    {
        printf("Enter display and color(numbers):");

        int aux1, aux2;
        scanf("%d %d", &aux1, &aux2);
        getchar();
        
        if(aux1==99){// cant place anymore
            plyr[player].pflag=0;
            printf("Skipping round....\n");
            break;
        }
        dis_nb = aux1 - 1;
        tcolor = aux2;

        
        if (tcolor >= 0 && tcolor < 5)
        {
            if (dis_nb == 9)
            { // middle
                tnb = table.ct.tiles[tcolor];
                if (!tnb)
                {
                    printf("Middle area doesn't have that color. Try again.\n");
                }
                else
                {
                    if (table.ct.st_pl_marker != 0)
                    {
                        add_floor(player, MARKER);
                    }
                    else
                    {
                        add_floor(player, -1);
                    }
                    table.ct.st_pl_marker = 0;

                    table.ct.tiles[tcolor] = 0;
                    printf("Tiles picked up!\n");
                    tile_place(player, tcolor, tnb);
                    ok = 1;
                }
            }

            else if ((dis_nb < (player_nb * 2 + 1)) && (dis_nb >= 0))
            { // fact dys
                tnb = table.fact_dys[dis_nb].tiles[tcolor];
                if (tnb == 0)
                {
                    printf("Selected display doesn't have that color. Try again.\n");
                }
                else
                {
                    tile_place(player, tcolor, tnb);
                    printf("Tiles picked up!\n");
                    table.fact_dys[dis_nb].tiles[tcolor] = 0;
                    for (int i = 0; i < 5; i++)
                    {
                        {
                            table.ct.tiles[i] = table.ct.tiles[i] + table.fact_dys[dis_nb].tiles[i];
                            table.fact_dys[dis_nb].tiles[i] = 0;
                        }
                    }
                    ok = 1;
                }
            }
            else
            {
                printf("Display does not exist. Try again.\n");
            }
        }
        else
        {
            printf("Invalid color\n");
        }
    }
}

void cleanup(int line, int player) // line
{
    int tcolor = brd[player].lines[line][4];
    for (int i = 0; i < 5; i++)
    {

        if (tcolor != -1)
        {
            if (i > 4 - line - 1)
            {
                bag.tiles[tcolor]++;
                brd[player].lines[line][i] = -1;
            }
        }
    }
    //bag.tiles[tcolor]--;
}

void fill_wall(int player_nb)
{

    for (int i = 0; i < player_nb; i++) // player nb
    {

        for (int k = 4; k >= 0; k--) // column on lines
        {
            int line = 4 - k; // line
            if (brd[i].lines[line][k] != -1)
            {
                for (int m = 0; m < 5; m++)
                { // column on wall
                    
                    switch (brd[i].lines[line][k])
                    {
                    case BLUE:
                        if (line == m)
                        {
                            brd[i].wall[line][m] = 0;
                            brd[i].newall[line][m] = 0;
                            bag.tiles[i]--;
                            cleanup(line, i);
                        }
                        break;
                    case RED:
                        if ((line - m == 2) || (m - line == 3))
                        {
                            brd[i].wall[line][m] = 1;
                            brd[i].newall[line][m] = 1;
                            bag.tiles[i]--;
                            cleanup(line, i);
                        }
                        break;
                    case YELLOW:

                        if ((m - line == 1) || (line - m == 4))
                        {
                            brd[i].wall[line][m] = 2;
                            brd[i].newall[line][m] = 2;
                            bag.tiles[i]--;
                            cleanup(line, i);
                        }
                        break;

                    case BLACK:

                        if ((line - m == 3) || (m - line == 2))
                        {
                            brd[i].wall[line][m] = 3;
                            brd[i].newall[line][m] = 3;
                            bag.tiles[i]--;
                            cleanup(line, i);
                        }
                        break;
                    case WHITE:
                        if ((line - m == 1) || (m - line == 4))
                        {
                            brd[i].wall[line][m] = 4;
                            brd[i].newall[line][m] = 4;
                            bag.tiles[i]--;
                            cleanup(line, i);
                        }
                        break;
                    }
                }
            }
        }
    }
}

void clear_display(int nbplayer)
{

    for (int i = 0; i < nbplayer * 2 + 1; i++)
    {
        for (int color = 0; color <= 4; color++)
        {

            table.fact_dys[i].tiles[color] = -1;
        }
    }
}

int clear_check(int nbplayer)
{
    for (int i = 0; i < 5; i++)
    {
        if (table.ct.tiles[i] != 0)
            return 0;
    }
    for (int t = 0; t < 2 * nbplayer + 1; t++)
    {
        for (int i = 0; i < 5; i++)
        {
            if (table.fact_dys[t].tiles[i] != 0)
                return 0;
        }
    }
    return 1;
}

void game_round(int nbplayer)
{ // 0 tiles left(clearcheck())=>

    int done = 0;
    while (!done)
    {
        for (int player = 0; player < nbplayer; player++)
        {

            tile_select(player, nbplayer);
            print_table(nbplayer);

            if (clear_check(nbplayer)||!plyr[player].pflag)
            { // end round
                plyr[player].pflag=1;
                fill_wall(nbplayer);
                print_table(nbplayer);
                
                for (int pl = 0; pl < nbplayer*2+1; pl++){
                    for(int i=0;i<5;i++){

                        if(table.fact_dys[pl].tiles[i]!=0){
                            bag.tiles[i]+=table.fact_dys[pl].tiles[i];
                            table.fact_dys[pl].tiles[i]=0;
                        }

                    }
                }
                for(int i=0;i<5;i++){
                    bag.tiles[i]+=table.ct.tiles[i];
                    table.ct.tiles[i]=0;
                }
                
                done = 1;
                break;
            }
        }
        // print_debug(nbplayer);
    }
}

//*******************************SCORING*********************************************

int check_end(int nbplayer)
{
    int check;
    for (int plyr = 0; plyr < nbplayer; plyr++)
    {
        for (int line = 0; line < 5; line++)
        {
            check = 1;
            for (int col = 0; col < 5; col++)
            {
                if (brd[plyr].wall[line][col] == -1)
                {
                    check = 0;
                    break;
                }
            }
            if (check)
            {
                return 0;
            }
        }
    }
    return 1;
}

void deduct_floor(int nbplayer)
{
    int arr[] = {1, 1, 2, 2, 2, 3, 3};
    for (int plyr = 0; plyr < nbplayer; plyr++)
    {
        for (int line = 0; line <7; line++)
        {
            if (brd[plyr].floor[line] == -1)
            {
                break;
            }
            if (brd[plyr].floor[line] == MARKER)
            {
                table.ct.st_pl_marker = 1;
                brd[plyr].floor[line] = -1;
                brd[plyr].score -= arr[line];
                
            }
            else
            {
                bag.tiles[atoi(&brd[plyr].floor[line])]++;
                brd[plyr].floor[line] = -1;
                brd[plyr].score -= arr[line];
                
            }
        }
    }
   
}

void round_score(int nbplayer)
{
    for (int i = 0; i < nbplayer; i++)
    {

        for (int g = 0; g < 5; g++) // linii
        {
            for (int h = 0; h < 5; h++) // coloane
            {

                if (brd[i].newall[g][h] != -1)
                {

                    int add1 = 0;
                    int add2 = 0;
                    for (int k = g + 1; k < 5; k++) // jos
                    {
                        if (brd[i].wall[k][h] == -1)
                        {
                            break;
                        }

                        brd[i].score++;
                        add1 = 1;
                    }
                    for (int k = h + 1; k < 5; k++) // dreapta
                    {
                        if (brd[i].wall[g][k] == -1)
                        {
                            break;
                        }

                        brd[i].score++;
                        add2 = 1;
                    }
                    for (int k = h - 1; k >= 0; k--) // stanga
                    {
                        if (brd[i].wall[g][k] == -1)
                        {
                            break;
                        }

                        brd[i].score++;
                        add2 = 1;
                    }
                    for (int k = g - 1; k >= 0; k--) // sus
                    {
                        if (brd[i].wall[k][h] == -1)
                        {
                            break;
                        }

                        brd[i].score++;
                        add1 = 1;
                    }
                    if (add1 || add2)
                    {
                        brd[i].score = brd[i].score + add1 + add2;
                    }
                    else
                    {
                        brd[i].score++;
                    }
                }
            }
        }
    }
    
    for (int i = 0; i < nbplayer; i++)//empty newall
         for (int g = 0; g < 5; g++) // linii
            for (int h = 0; h < 5; h++) // coloane
                brd[i].newall[g][h]=-1;

    
   
}

void game_score(int nbplayer)
{

    for (int plyr = 0; plyr < nbplayer; plyr++)
    {
        
        for (int line = 0; line < 5; line++) //horiz
        {
            int oriz = 0;
        
            for (int col = 0; col < 5; col++)
            {
                if (brd[plyr].wall[line][col] == -1)
                {

                    break;
                }
                
                oriz++;
            }
            if (oriz == 5)
            {
                brd[plyr].score += 2;
            }
        }

        for (int col = 0; col < 5; col++) // vertical
        {
            int horiz = 0;
            for (int line = 0; line < 5; line++)
            {
                if (brd[plyr].wall[line][col] == -1)
                {

                    break;
                }
                horiz++;
            }
            if (horiz == 5)
            {
                brd[plyr].score += 7;
            }
        }
        int allcolours[5]={0};
        for (int line = 0; line < 5; line++) //  all tiles
        {
        
            for (int col = 0; col < 5; col++)
            {
                if (brd[plyr].wall[line][col] != -1){
                    allcolours[brd[plyr].wall[line][col]-0]++;
                }
                
            }
            
        }
        for(int i=0;i<5;i++){
            if(allcolours[i]==5){
                printf("DAAAAAAAAAAAAAAAA\n");
                brd[plyr].score+=10;
        }
    }
    }
   
}

void winner(int nbplayer)
{

    int plyr_score[nbplayer];
    int plyr_top[nbplayer];
    for (int plyr = 0; plyr < nbplayer; plyr++)
    {
        plyr_score[plyr] = brd[plyr].score;
        plyr_top[plyr] = plyr;
    }

    for (int plyr = 0; plyr < nbplayer; plyr++)
    {
        for (int pl = plyr; pl < nbplayer; pl++)
        {
            if (plyr_score[plyr] < plyr_score[pl])
            {
                int aux1 = plyr_score[pl];
                int aux2 = plyr_top[pl];
                plyr_score[pl] = plyr_score[plyr];
                plyr_top[pl] = plyr_top[plyr];

                plyr_score[plyr] = aux1;
                plyr_top[plyr] = aux2;
            }
        }
    }
    printf("\n====================================================================================================================\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~SCOREBOARD~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n====================================================================================================================\n\n");
    printf(TYELLOW"              .-=========-.\n"TRESET);
    printf(TYELLOW"              ∖'-=======-'/\n"TRESET);
    printf(TYELLOW"              _|   .=.   |_\n"TRESET);
    printf(TYELLOW"             ((| {"TBLUE"A"TRED"Z"TYELLOW"U"TBLACK"L"TWHITE"♛" TYELLOW"} |))\n"TRESET);
    printf(TYELLOW"              ∖|   /|∖   |/\n"TRESET);
    printf(TYELLOW"               ∖__ '`' __/\n"TRESET);
    printf(TYELLOW"                 _`) (`_\n"TRESET);
    printf(TYELLOW"               _/_______∖_\n"TRESET);
    printf(TYELLOW"              /___________∖\n\n"TRESET);


    printf("WINNER: ");
    for (int pl = 0; pl < nbplayer; pl++)
    {
        if (pl < nbplayer - 1)
        {
            printf("%s  with %d points\n%dnd place: ", plyr[plyr_top[pl]].name,plyr_score[pl] ,pl+2);
        }
        else
        {
            printf("%s with %d points\n", plyr[plyr_top[pl]].name,plyr_score[pl]);
        }
        
    }
    printf("\n====================================================================================================================\n");
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ THE END ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    printf("\n====================================================================================================================\n\n");
}
//*******************************GAME*********************************************
void game(int debug)
{

    printf("Please select number of players (2-4) and their names:\n");
    int check = 0;
    int nbplayer;
    while (!check)
    {
        printf("Player number selection(2-4): ");
        scanf("%d", &nbplayer);

        getchar();

        if (nbplayer >= 2 && nbplayer <= 4)
        {
            check = 1;
        }
        else
        {
            nbplayer = 0;
            check = 0;
            printf("Wrong imput, try again.\n");
        }
    }
    printf("\n");

    for (int i = 0; i < nbplayer; i++)
    {
        printf("Player %d:", i + 1);
        scanf("%15s", plyr[i].name);
    }
    printf("\n");

    srand(time(0));

    int end = 1; // 0 la finalul jocului
    int newtoken = 0;
    init_game(nbplayer, newtoken); // setup before game

////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////TESTING  (check end conditions for 2 player)
///////////////////////////////////////////////////////////////////////////////////////////////

    // bag.tiles[0]-=6;
    // bag.tiles[1]-=3;
    // bag.tiles[2]-=2;
    // bag.tiles[3]-=2;
    // bag.tiles[4]-=2;

    // brd[0].wall[4][0]=2;
    // brd[0].wall[4][1]=3;
    // brd[0].wall[4][2]=1;
    // brd[0].wall[4][3]=4;

    // brd[1].wall[3][0]=3;
    // brd[1].wall[3][1]=1;
    // brd[1].wall[3][2]=4;
    // brd[1].wall[3][3]=0;

    // brd[0].wall[0][0]=0;
    // brd[0].wall[1][1]=0;
    // brd[0].wall[2][2]=0;
    // brd[0].wall[3][3]=0;

    // brd[1].wall[0][0]=0;
    // brd[1].wall[1][0]=4;
    // brd[1].wall[2][0]=1;
    // brd[1].wall[4][0]=2;


///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////

    while (end)
    {

        new_round_setup(nbplayer);
        print_table(nbplayer);

        game_round(nbplayer);

        round_score(nbplayer);
        deduct_floor(nbplayer);

        if (debug)
        {
            print_debug(nbplayer);
        }
        end = check_end(nbplayer);
        if (!end)
        {
            game_score(nbplayer); // 2 do
            ////optional ca sa tinem scorul la 0+
            for(int pl=0;pl<nbplayer;pl++){
                if (brd[pl].score < 0)
                {
                    brd[pl].score = 0;
                }
            }
            
            print_table(nbplayer);
            winner(nbplayer);
        }
    }
}

int menu()
{

    printf("///////////////////////////////////////////////////////////////////////\n");
    printf("Welcome to AZUL\n");
    printf("0 - Exit game\n");
    printf("1 - Display rules\n");
    printf("2 - Start game\n");
    printf("3 - Start game in debug mode\n");
    printf("///////////////////////////////////////////////////////////////////////\n");

    int check = 0;
    int nb;
    while (!check)
    {
        printf("Your selection(0-3): ");
        scanf("%d", &nb);
        if (nb >= 0 && nb < 4)
        {
            return nb;
            check = 1;
        }
        else
        {
            printf("Wrong imput, try again.\n");
        }
    }
    return 0;
}

int main()
{

    int opt = menu();
    do
    {
        switch (opt)
        {
        case 0:
            printf("Exiting game...\n");
            break;

        case 1:
            printf("Displaying rules\n");

            FILE *sf;
            sf = fopen("rules.txt", "r");
            char *buf;
            do
            {
                buf = malloc(256 * sizeof(char));
                fread(buf, 256, 1, sf);
                if (!feof(sf))
                {
                    printf("%s", buf);
                }
                free(buf);

            } while (!feof(sf));

            fclose(sf);
            opt = menu();
            break;
        case 2:
            printf("Launching New Game!\n");
            game(0);
            opt = 0;
            break;

        case 3:
            printf("Debug game mode, showing all available data\n");
            game(1);
            opt = 0;
            break;

        default:
            printf("\n Wrong input, try again!!\n");
            opt = menu();
            break;
        }

    } while (opt != 0);

    return 0;
}

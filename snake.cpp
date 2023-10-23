#include <string>
#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <thread>
#include <windows.h>
#include <time.h>

// implement a Snake game using multithread 1 thread for handling game logic and other for handling user inputs

// Todo implement apple, then let that apple appear in random location in map

// if Snake head ate it, increment the Vector2's length by 1, use linked list to remember it's tail and previous location

#define quit_button '`'
#define height 10
#define width 10

class Vector2
{
public:
    int x;
    int y;

    Vector2 *next = NULL;
    Vector2 *prev = NULL;

    Vector2()
    {
        x = 0;
        y = 0;
    }

    Vector2(int NewX, int NewY)
    {
        this->x = NewX;
        this->y = NewY;
    }

    // return true if it is the same position
    bool IsAtPosition(Vector2 position2)
    {
        if (this->x == position2.x && this->y == position2.y)
        {
            return true;
        }

        else
        {
            return false;
        }
    }
};

// GLOBAL VARIABLES:
//--------------------------------------------

// Storing player_snake_head inputs
static char player_input = 'd';

// Count Snake Length
static int snake_length = 0;

// Apple variable
Vector2 the_apple(4, 4);

// Declare Vector2 player_snake_head
Vector2 player_snake_head;

// Declare Vector2 player_snake_tail poiter
Vector2 *player_snake_tail = &player_snake_head;

// Game Map 2d array
char game_map[height][width];

// Update count, update to segment arcoding to which number in which game loop
int update_count;

// Game Over
bool game_over = false;

//---------------------------------------------

// FUNCTIONS:
// get player_snake_head input
void GetplayerInput(char &player_input)
{
    while (!game_over)
    {
        player_input = _getch();
    }

    return;
}

// print out map
void RenderMap(char game_map[height][width])
{
    // Move the cursor to the top-left corner
    printf("\033[H");

    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            printf("%c", game_map[h][w]);
        }

        printf("\n");
    }

    return;
}

// update player_snake_head position to map
void UpdateMapPosition(Vector2 player_object, char map[height][width])
{
    // apple position
    game_map[the_apple.y][the_apple.x] = '@';

    // player position
    map[player_object.y][player_object.x] = '?';
}

// update player_snake_head position based on input
void UpdatePlayerInput(Vector2 &player_object)
{

    if (player_input == 'w')
    {
        // up out of bound
        if (player_object.y == 0)
        {
            player_object.y = height - 1;
        }

        else
        {
            --player_object.y;
        }
    }

    else if (player_input == 's')
    {
        // down out of bound
        if (player_object.y == (height - 1))
        {
            player_object.y = 0;
        }

        else
        {
            ++player_object.y;
        }
    }

    else if (player_input == 'a')
    {
        if (player_object.x == 0)
        {
            player_object.x = width - 1;
        }

        else
        {
            --player_object.x;
        }
    }

    else if (player_input == 'd')
    {
        if (player_object.x == (width - 1))
        {
            player_object.x = 0;
        }

        else
        {
            ++player_object.x;
        }
    }

    return;
}

// remove snake previous location
void RemoveOldPosition()
{
    Vector2 *prev = player_snake_tail;

    // Vector2 *prev = &player_snake_head;

    game_map[prev->y][prev->x] = '.';

    return;
}

// Set apple
void SetApple() // Set apple to a random location in map
{
    srand(time(NULL));

    the_apple.y = rand() % height;
    the_apple.x = rand() % width;
}

// If snake eat apple, return true if yes, else false
bool SnakeAteTheApple()
{
    if (player_snake_head.IsAtPosition(the_apple))
    {
        return true;
    }

    return false;
}

// store previous locations as long as the snake length

// loop count == snake length when done 1 loop count set the new positionl
void AllocatingSnakeSegments()
{
    // make linked list size == to snake length

    static int node_count = 0;

    while (node_count <= snake_length)
    {
        // allocating a new node
        Vector2 *tmp = new Vector2;

        tmp->prev = player_snake_tail;

        player_snake_tail->next = tmp;

        player_snake_tail = tmp;

        ++node_count;
    }
}

void FreeSnakeSegments()
{
    Vector2 *tmp = player_snake_tail;

    while (player_snake_tail->prev != NULL)
    {
        player_snake_tail = player_snake_tail->prev;

        printf("Freeing: %#p\n", tmp);
        free(tmp);
    }

    printf("Sucessfully freeing all memory\n");
}

// calculating snake segments positions
void CalculateSnakeSegments()
{
    Vector2 *tmp = player_snake_tail;

    while (tmp->prev != NULL)
    {
        tmp->x = tmp->prev->x;
        tmp->y = tmp->prev->y;
        tmp = tmp->prev;
    }
}

void PrintLinkedList()
{

    Vector2 *tmp = &player_snake_head;

    int tmp_count = 0;

    while (tmp->next != NULL)
    {
        printf("Node %i mem address: %#p\n", tmp_count, tmp);

        tmp = tmp->next;

        ++tmp_count;
    }
}

bool IsHeadTouchingBody()
{
    if (snake_length > 0)
    {
        Vector2 *body_segment = player_snake_head.next->next;

        while (body_segment->next != NULL)
        {
            if (player_snake_head.IsAtPosition(*body_segment))
            {
                return true;
            }

            body_segment = body_segment->next;
        }
    }

    return false;
}

int main(int argc, char *argv[])
{
    // hide terminal cursor
    printf("\033[?25l");

    // populate the map
    for (int h = 0; h < height; ++h)
    {
        for (int w = 0; w < width; ++w)
        {
            game_map[h][w] = '.';
        }

        printf("\n");
    }

    system("cls");

    // new thread
    std::thread input_thread(GetplayerInput, std::ref(player_input));

    // set apple in map
    SetApple();

    // loop for game logic
    while (!game_over)
    {
        // update Player position to map
        UpdateMapPosition(player_snake_head, game_map);

        // add length if snake ate the apple
        if (SnakeAteTheApple())
        {
            ++snake_length;

            SetApple();

            // allocating new segment for new snake length
            AllocatingSnakeSegments();
        }

        CalculateSnakeSegments();

        if (IsHeadTouchingBody())
        {
            game_over = true;
        }

        // render map out
        RenderMap(game_map);

        // print out info
        printf("player_snake_head Input: %c\n", player_input);
        std::cout << "Snake at apple: " << player_snake_head.IsAtPosition(the_apple) << std::endl;
        printf("Snake length: %i\n", snake_length);
        printf("head position:\n(x, y)\n(%i, %i)\n", player_snake_head.x, player_snake_head.y);
        printf("tail position:\n(x, y)\n(%i, %i)\n", player_snake_tail->x, player_snake_tail->y);

        std::cout << "head touching body: " << IsHeadTouchingBody() << std::endl;

        PrintLinkedList();

        // remove previous location
        RemoveOldPosition();

        if (player_input == quit_button)
        {
            game_over = true;
        }

        // update player_snake_head position from input
        UpdatePlayerInput(player_snake_head);

        Sleep(100);
    }

    // wait for main thread to end;
    input_thread.join();

    system("cls");

    std::cout << "\n                           GAME OVER!" << std::endl;

    FreeSnakeSegments();
}
#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define KEY_ESC 65307
#define KEY_W 119
#define KEY_A 97
#define KEY_S 115
#define KEY_D 100
#define KEY_LEFT 65363
#define KEY_RIGHT 65361
#define screenWidth 800
#define screenHeight 600
#define MINIMAP_SCALE 5
#define MINIMAP_SIZE 100
#define ROT_SPEED 0.05 // Rotation speed in radians
#define MOVE_SPEED 0.2 // Movement speed
#define CEILING_COLOR 0x87CEEB  // Light blue for the ceiling
#define FLOOR_COLOR 0x008000    // Green for the floor

typedef struct s_map {
    char **grid;
    int width;
    int height;
} t_map;

typedef struct s_player {
    double posX;
    double posY;
    double dirX;
    double dirY;
    double planeX;
    double planeY;
} t_player;

typedef struct s_game {
    void *mlx;
    void *win;
    void *img;
    char *data_addr;
    int bpp;
    int size_line;
    int endian;
    t_player player;
    t_map map;
} t_game;

void initialize_map(t_map *map)
{
    static char *example_map[] = {
        "11111111",
        "10000001",
        "10100N01",
        "10000001",
        "11111111",
        NULL
    };

    int height = 0;
    while (example_map[height])
        height++;

    map->grid = example_map;
    map->width = strlen(example_map[0]);
    map->height = height;
}

void initialize_player(t_player *player, t_map *map)
{
    for (int y = 0; y < map->height; y++) {
        for (int x = 0; x < map->width; x++) {
            if (map->grid[y][x] == 'N') {
                player->posX = x + 0.5;
                player->posY = y + 0.5;
                player->dirX = -1;
                player->dirY = 0;
                player->planeX = 0;
                player->planeY = 0.66;
                printf("Player initialized at position: (%f, %f)\n", player->posX, player->posY);
                return;
            }
        }
    }
}

void initialize_game(t_game *game)
{
    game->mlx = mlx_init();
    game->win = mlx_new_window(game->mlx, screenWidth, screenHeight, "cub3D");
    game->img = mlx_new_image(game->mlx, screenWidth, screenHeight);
    game->data_addr = mlx_get_data_addr(game->img, &game->bpp, &game->size_line, &game->endian);

    initialize_map(&game->map);
    initialize_player(&game->player, &game->map);
}

void verLine(t_game *game, int x, int start, int end, int color)
{
    int y;
    printf("drawStart: %d, drawEnd: %d\n", start, end);
    for (y = start; y < end; y++)
    {
        if (y >= 0 && y < screenHeight)
        {
            *(unsigned int *)(game->data_addr + (y * game->size_line + x * (game->bpp / 8))) = color;
        }
    }
}

void draw_minimap_pixel(t_game *game, int x, int y, int color)
{
    if (x >= 0 && x < MINIMAP_SIZE && y >= 0 && y < MINIMAP_SIZE) {
        *(unsigned int *)(game->data_addr + (y * game->size_line + x * (game->bpp / 8))) = color;
    }
}

void draw_minimap(t_game *game)
{
    int x, y;
    int color;

    // Draw the map
    for (y = 0; y < game->map.height; y++)
    {
        for (x = 0; x < game->map.width; x++)
        {
            if (game->map.grid[y][x] == '1')
                color = 0xFFFFFF; // White for walls
            else
                color = 0x000000; // Black for empty spaces

            for (int i = 0; i < MINIMAP_SCALE; i++) {
                for (int j = 0; j < MINIMAP_SCALE; j++) {
                    int pixel_x = x * MINIMAP_SCALE + i;
                    int pixel_y = y * MINIMAP_SCALE + j;
                    draw_minimap_pixel(game, pixel_x, pixel_y, color);
                }
            }
        }
    }

    // Draw the player position as a single pixel
    int player_x = game->player.posX * MINIMAP_SCALE;
    int player_y = game->player.posY * MINIMAP_SCALE;
    draw_minimap_pixel(game, player_x, player_y, 0xFF0000); // Red for the player

    // Draw the player's direction
    int line_length = 10; // Length of the direction line
    for (int i = 0; i < line_length; i++) {
        int step_x = player_x + i * game->player.dirX;
        int step_y = player_y + i * game->player.dirY;
        draw_minimap_pixel(game, step_x, step_y, 0xFF0000); // Red for the direction line
    }
}

void draw_rays_on_minimap(t_game *game, double rayDirX, double rayDirY)
{
    double rayPosX = game->player.posX;
    double rayPosY = game->player.posY;

    //printf("IN DRAW RAYS ON MINIMAP\n");
    //printf("rayPosX: %f\nrayPosY: %f\n", rayPosX, rayPosY);

    while (1)
    {
        int mapX = (int)rayPosX;
        int mapY = (int)rayPosY;

        // Ensure mapX and mapY are within bounds
        if (mapX < 0 || mapX >= game->map.width || mapY < 0 || mapY >= game->map.height) {
            printf("OUT OF BOUND\n");
            break;
        }

        // Draw the ray pixel on the minimap
        draw_minimap_pixel(game, mapX * MINIMAP_SCALE, mapY * MINIMAP_SCALE, 0x00FF00); // Green for rays

        // Stop if the ray hits a wall
        if (game->map.grid[mapY][mapX] == '1') {
            //printf("WALL HIT\n");
            break;
        }

        // Increment the ray position slightly
        rayPosX += rayDirX * 0.05;
        rayPosY += rayDirY * 0.05;
    }
    //printf("mapX: %d\nmapY: %d\n", (int)rayPosX, (int)rayPosY);
}

void cast_rays(t_game *game)
{
    for (int x = 0; x < screenWidth; x++)
    {
        double cameraX = 2 * x / (double)screenWidth - 1;
        double rayDirX = game->player.dirX + game->player.planeX * cameraX;
        double rayDirY = game->player.dirY + game->player.planeY * cameraX;

        int mapX = (int)game->player.posX;
        int mapY = (int)game->player.posY;

        double sideDistX;
        double sideDistY;

        double deltaDistX = fabs(1 / rayDirX);
        double deltaDistY = fabs(1 / rayDirY);
        double perpWallDist;

        int stepX;
        int stepY;

        int hit = 0;
        int side;

        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (game->player.posX - mapX) * deltaDistX;
        }
        else
        {
            stepX = 1;
            sideDistX = (mapX + 1.0 - game->player.posX) * deltaDistX;
        }
        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (game->player.posY - mapY) * deltaDistY;
        }
        else
        {
            stepY = 1;
            sideDistY = (mapY + 1.0 - game->player.posY) * deltaDistY;
        }

        while (hit == 0)
        {
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // Check if mapX and mapY are within the bounds of the map
            if (mapX < 0 || mapX >= game->map.width || mapY < 0 || mapY >= game->map.height)
            {
                hit = 1; // Stop the loop if out of bounds
                printf("OUT OF BOUND\n");
                break;
            }
            else if (game->map.grid[mapY][mapX] == '1')
            {
                hit = 1; // Wall hit
            }
        }

        if (hit == 0) {
            continue;
        }

        if (side == 0)
            perpWallDist = (mapX - game->player.posX + (1 - stepX) / 2) / rayDirX;
        else
            perpWallDist = (mapY - game->player.posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(screenHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        int color = 0xFF0000; // Default color in case of error

        // Debug print to check mapX and mapY before accessing
        if (mapX >= 0 && mapX < game->map.width && mapY >= 0 && mapY < game->map.height)
        {
            switch(game->map.grid[mapY][mapX])
            {
                case '1':  color = 0xFF0000;  break;
                default: color = 0xFFFFFF; break;
            }
        }

        if (side == 1) {color = color / 2;}

        verLine(game, x, drawStart, drawEnd, color);

        // Draw the ray on the minimap
        draw_rays_on_minimap(game, rayDirX, rayDirY);
    }
}

void draw_floor_and_ceiling(t_game *game)
{
    int x, y;

    // Draw ceiling
    for (y = 0; y < screenHeight / 2; y++)
    {
        for (x = 0; x < screenWidth; x++)
        {
            *(unsigned int *)(game->data_addr + (y * game->size_line + x * (game->bpp / 8))) = CEILING_COLOR;
        }
    }

    // Draw floor
    for (y = screenHeight / 2; y < screenHeight; y++)
    {
        for (x = 0; x < screenWidth; x++)
        {
            *(unsigned int *)(game->data_addr + (y * game->size_line + x * (game->bpp / 8))) = FLOOR_COLOR;
        }
    }
}

void rotate_player(t_player *player, double angle)
{
    double oldDirX = player->dirX;
    player->dirX = player->dirX * cos(angle) - player->dirY * sin(angle);
    player->dirY = oldDirX * sin(angle) + player->dirY * cos(angle);

    double oldPlaneX = player->planeX;
    player->planeX = player->planeX * cos(angle) - player->planeY * sin(angle);
    player->planeY = oldPlaneX * sin(angle) + player->planeY * cos(angle);
}

/* void move_player(t_game *game, double moveX, double moveY)
{
    if (game->map.grid[(int)(game->player.posY + moveY)][(int)(game->player.posX + moveX)] == '0')
    {
        game->player.posX += moveX;
        game->player.posY += moveY;
        printf("Player moved to position: (%f, %f)\n", game->player.posX, game->player.posY); // Debug print
    }
    else
    {
        printf("Collision detected, movement blocked\n"); // Debug print
    }
} */

void move_player(t_game *game, double moveX, double moveY)
{
    int newPosX = (int)(game->player.posX + moveX);
    int newPosY = (int)(game->player.posY + moveY);

    // Ensure the new position is within the map boundaries
    if (newPosX < 0 || newPosY < 0 || newPosY >= game->map.height || newPosX >= game->map.width)
    {
        printf("Out of bounds: newPosX=%d, newPosY=%d\n", newPosX, newPosY);
        return;
    }

    char mapCell = game->map.grid[newPosY][newPosX];
    //printf("Checking new position: newPosX=%d, newPosY=%d, mapCell=%c\n", newPosX, newPosY, mapCell);

    if (mapCell == '0' || mapCell == 'N')
    {
        game->player.posX += moveX;
        game->player.posY += moveY;
        //printf("Player moved to position: (%f, %f)\n", game->player.posX, game->player.posY); // Debug print
    }
    else
    {
        printf("Collision detected, movement blocked\n"); // Debug print
    }
}


int close_window(void *param)
{
    exit(0);
    return (0);
}

int key_hook(int keycode, t_game *game)
{
    if (keycode == KEY_ESC)
        close_window(game);
    else if (keycode == KEY_LEFT)
        rotate_player(&game->player, -ROT_SPEED);
    else if (keycode == KEY_RIGHT)
        rotate_player(&game->player, ROT_SPEED);
    else if (keycode == KEY_W)
        move_player(game, game->player.dirX * MOVE_SPEED, game->player.dirY * MOVE_SPEED);
    else if (keycode == KEY_S)
        move_player(game, -game->player.dirX * MOVE_SPEED, -game->player.dirY * MOVE_SPEED);
    else if (keycode == KEY_A)
        move_player(game, -game->player.dirY * MOVE_SPEED, game->player.dirX * MOVE_SPEED);
    else if (keycode == KEY_D)
        move_player(game, game->player.dirY * MOVE_SPEED, -game->player.dirX * MOVE_SPEED);
    return (0);
}

int main_loop(t_game *game)
{
    draw_floor_and_ceiling(game);
    cast_rays(game);
    draw_minimap(game);
    mlx_put_image_to_window(game->mlx, game->win, game->img, 0, 0);
    return (0);
}

int main()
{
    t_game game;

    initialize_game(&game);
    mlx_loop_hook(game.mlx, main_loop, &game);
    mlx_hook(game.win, 2, 1L<<0, key_hook, &game);
    mlx_hook(game.win, 17, 0L, close_window, &game);
    mlx_loop(game.mlx);
    return 0;
}

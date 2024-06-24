#include <mlx.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define KEY_ESC 65307
#define screenWidth 800
#define screenHeight 600
#define MINIMAP_SCALE 10
#define MINIMAP_SIZE 200

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
        "1111111111",
        "100N000001",
        "1010010001",
        "1000000001",
        "1111111111",
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

    for (y = start; y < end; y++)
    {
        *(unsigned int *)(game->data_addr + (y * game->size_line + x * (game->bpp / 8))) = color;
    }
}

void cast_rays(t_game *game)
{
    size_t  counter = 0;
    for (int x = 0; x < screenWidth; x++)
    {
        counter++;
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
            if (game->map.grid[mapX][mapY] > '0') hit = 1;
        }

        if (side == 0) perpWallDist = (mapX - game->player.posX + (1 - stepX) / 2) / rayDirX;
        else           perpWallDist = (mapY - game->player.posY + (1 - stepY) / 2) / rayDirY;

        int lineHeight = (int)(screenHeight / perpWallDist);

        int drawStart = -lineHeight / 2 + screenHeight / 2;
        if(drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if(drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        int color;
        switch(game->map.grid[mapX][mapY])
        {
            case '1':  color = 0xFF0000;  break;
            default: color = 0xFFFFFF; break;
        }

        if (side == 1) {color = color / 2;}

        verLine(game, x, drawStart, drawEnd, color);
    }
    printf("counter: %zu\n", counter);
}

void draw_minimap_pixel(t_game *game, int x, int y, int color)
{
    int i;
    int j;

    for (i = 0; i < MINIMAP_SCALE; i++)
    {
        for (j = 0; j < MINIMAP_SCALE; j++)
        {
            int pixel_x = x * MINIMAP_SCALE + i;
            int pixel_y = y * MINIMAP_SCALE + j;
            if (pixel_x < MINIMAP_SIZE && pixel_y < MINIMAP_SIZE)
            {
                *(unsigned int *)(game->data_addr + (pixel_y * game->size_line + pixel_x * (game->bpp / 8))) = color;
            }
        }
    }
}

void draw_minimap(t_game *game)
{
    int x, y;
    int color;

    for (y = 0; y < game->map.height; y++)
    {
        for (x = 0; x < game->map.width; x++)
        {
            if (game->map.grid[y][x] == '1')
                color = 0xFFFFFF; // White for walls
            else
                color = 0x000000; // Black for empty spaces

            draw_minimap_pixel(game, x, y, color);
        }
    }

    // Draw the player position
    int player_x = game->player.posX * MINIMAP_SCALE;
    int player_y = game->player.posY * MINIMAP_SCALE;
    for (int i = 0; i < MINIMAP_SCALE; i++)
    {
        for (int j = 0; j < MINIMAP_SCALE; j++)
        {
            if (player_x + i < MINIMAP_SIZE && player_y + j < MINIMAP_SIZE)
            {
                *(unsigned int *)(game->data_addr + ((player_y + j) * game->size_line + (player_x + i) * (game->bpp / 8))) = 0xFF0000; // Red for the player
            }
        }
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
    return (0);
}

int main_loop(t_game *game)
{
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

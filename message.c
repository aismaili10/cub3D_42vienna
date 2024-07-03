```c
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

            if (mapX < 0 || mapX >= game->map.width || mapY < 0 || mapY >= game->map.height)
            {
                hit = 1; // Stop the loop if out of bounds
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
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + screenHeight / 2;
        if (drawEnd >= screenHeight) drawEnd = screenHeight - 1;

        // Determine the correct texture based on the side of the wall hit
        int texNum = 0; // Default texture index
        if (side == 0)
        {
            if (rayDirX > 0)
                texNum = 0; // East
            else
                texNum = 1; // West
        }
        else
        {
            if (rayDirY > 0)
                texNum = 2; // South
            else
                texNum = 3; // North
        }

        // Calculate the x-coordinate on the texture
        double wallX;
        if (side == 0)
            wallX = game->player.posY + perpWallDist * rayDirY;
        else
            wallX = game->player.posX + perpWallDist * rayDirX;
        wallX -= floor(wallX);

        int texX = (int)(wallX * (double)TEX_WIDTH);
        if (side == 0 && rayDirX > 0)
            texX = TEX_WIDTH - texX - 1;
        if (side == 1 && rayDirY < 0)
            texX = TEX_WIDTH - texX - 1;

        for (int y = drawStart; y < drawEnd; y++)
        {
            int d = y * 256 - screenHeight * 128 + lineHeight * 128;
            int texY = ((d * TEX_HEIGHT) / lineHeight) / 256;
            int color = game->textures[texNum].data[TEX_HEIGHT * texY + texX];
            verLine(game, x, y, y + 1, color);
        }
    }
}
```
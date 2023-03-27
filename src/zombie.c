#include "common.h"
#include <stdlib.h>
#include <math.h>

#define ZOMBIE_DETECTION_RADIUS 200

SDL_Texture* zombieTexture = NULL;

static void touch(Entity* self, Entity* other);

void initZombie(void)
{
    Entity* e;

    e = malloc(sizeof(Entity));
    if (!e) {
        fprintf(stderr, "Failed to allocate memory for Entity\n");
        return;
    }
    memset(e, 0, sizeof(Entity));
    stage.entityTail->next = e;
    stage.entityTail = e;

    e->health = 1;
    e->texture = loadTexture("gfx/zombie.png");
    zombieTexture = e->texture;

    SDL_QueryTexture(e->texture, NULL, NULL, &e->w, &e->h);

    // Assign touch function to e->touch
    e->touch = touch;

    // randomly position zombie entities within the screen bounds
    e->x = rand() % (SCREEN_WIDTH - e->w);
    e->y = rand() % (SCREEN_HEIGHT - e->h);

    // randomly set zombie entities' horizontal movement direction
    e->dx = (rand() % 2 == 0) ? ZOMBIE_MOVE_SPEED : -ZOMBIE_MOVE_SPEED;

    // initialize new Entity fields
    e->ex = rand() % 100; // Random number between 0 and 99
    e->ey = rand() % 100;
    e->sx = rand() % 100;
    e->sy = rand() % 100;
    e->dy = rand() % 100;
    e->isOnGround = rand() % 2; // Random number either 0 or 1
    e->value = rand() % 100;
    e->flags = rand() % 256; // Random number between 0 and 255
    e->riding = NULL;
    e->next = NULL;
}

static void touch(Entity* self, Entity* other)
{
    if (!self || !other)
    {
        fprintf(stderr, "Self or other pointer is NULL in touch()\n");
        return;
    }

    // check if other entity is the player
    if (other == player)
    {
        // set player position to (0,0)
        player->x = player->y = 0;
    }
    else
    {
        // reverse zombie's velocities
        self->dx = -self->dx;
        self->dy = -self->dy;
    }
}

void doZombies(void)
{
    for (int i = 0; i < MAX_ZOMBIES; i++)
    {
        // check if zombie entity pointer is NULL
        if (!zombies[i])
        {
            fprintf(stderr, "Zombie entity pointer is NULL\n");
            continue;
        }
        // update zombie entities' horizontal position based on movement direction
        zombies[i]->x += zombies[i]->dx;
        zombies[i]->texture = zombieTexture;

        // if zombie entity has collided with left or right screen bounds, change its horizontal direction
        if (zombies[i]->x <= 0 || zombies[i]->x + zombies[i]->w >= SCREEN_WIDTH)
        {
            zombies[i]->dx = -zombies[i]->dx;
        }

        // chase the player
        chasePlayer(zombies[i]);
    }
}

void chasePlayer(Entity* zombie)
{
    if (!zombie || !player)
    {
        fprintf(stderr, "Zombie or player pointer is NULL in chasePlayer()\n");
        return;
    }

    // calculate distance between zombie and player
    int distX = player->x - zombie->x;
    int distY = player->y - zombie->y;
    double distance = sqrt(distX * distX + distY * distY);

    // if player is within detection radius, chase player
    if (distance <= ZOMBIE_DETECTION_RADIUS)
    {
        // calculate the zombie's direction to the player
        double dx = distX / distance;
        double dy = distY / distance;

        // update zombie velocity
        zombie->dx = dx * ZOMBIE_MOVE_SPEED;
        zombie->dy = dy * ZOMBIE_MOVE_SPEED;
    }
    else
    {
        // zombie is not chasing player, set velocity to 0
        zombie->dx = 0;
        zombie->dy = 0;
    }
}

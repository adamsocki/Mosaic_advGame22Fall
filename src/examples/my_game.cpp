#include "gameCode/structs.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"


void MyInit() {
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;

    
    InitializeEntityManager();
    InitializeEntityBuffers();
    
    LoadSprites();
    LoadSFX();

    CreatePlayer();
    CreateLevel(1);
}

void MyGameUpdate() {
    //**********************
    //  LOGIC
    //**********************
    //      Hide mouse cursor & Get Mouse Position
    while (ShowCursor(false) >= 0);
    vec2 mousePos = Input->mousePosNormSigned;
    mousePos.x = mousePos.x * 8;
    mousePos.y = mousePos.y * 4.5f;

    //      For all entity types, first Point to EntityBuffers (Logic A: 1 of 2 steps)
    EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    EntityTypeBuffer* playerCarryBuffer = &Data->em.buffers[EntityType_PlayerCarry];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];

    //      For all entity types, second Point to EntityBuffer->entities (Logic A: 2 of 2 steps)
    Base* baseEntitiesInBuffer = (Base*)baseBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
    PlayerCarry* playerCarryEntitiesInBuffer = (PlayerCarry*)playerCarryBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;


    //      Handle Player Input for Movement

    InputPlayerController(&playerEntitiesInBuffer[0]);


    // Detect Collision
    //      Player 
    for (int i = 0; i < playerBuffer->count; i++) {
        // PlayerCarry
        Rect playerRect;
        playerRect.max = playerEntitiesInBuffer[0].size;
        playerRect.min = -playerEntitiesInBuffer[0].size;
        for (int j = 0; j < playerCarryBuffer->count; j++) {
            Rect playerCarryRect;
            EntityHandle playerCarryHandle = playerCarryEntitiesInBuffer[j].handle;
            PlayerCarry* playerCarryEntity = (PlayerCarry*)GetEntity(&Data->em, playerCarryHandle);
            playerCarryRect.max = playerCarryEntity->size;
            playerCarryRect.min = -playerCarryEntity->size;
            vec2 dir = V2(0);
            if (RectTest(playerCarryRect, playerRect, playerCarryEntity->position, playerEntitiesInBuffer[0].position, &dir)) {
                playerCarryEntity->canBePickedUp = true;
            }
            else
            {
                playerCarryEntity->canBePickedUp = false;
            }
        }
        // Player Walls
        for (int j = 0; j < barrierBuffer->count; j++) {
            Rect barrierRect;
            EntityHandle barrierHandle = barrierEntitiesInBuffer[j].handle;
            Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
            barrierRect.max = barrierEntity->size;
            barrierRect.min = -barrierEntity->size;
            vec2(dir) = V2(0, 0);
            if (RectTest(barrierRect, playerRect, barrierEntity->position, playerEntitiesInBuffer[0].position, &dir)) {
                playerEntitiesInBuffer[0].position = playerEntitiesInBuffer[0].previousPosition;
            }
        }
    }

    for (int i = 0; i < playerCarryBuffer->count; i++) {
        Rect playerCarryRect;
        EntityHandle playerCarryHandle = playerCarryEntitiesInBuffer[i].handle;
        PlayerCarry* playerCarryEntity = (PlayerCarry*)GetEntity(&Data->em, playerCarryHandle);
        playerCarryRect.max = playerCarryEntity->size;
        playerCarryRect.min = -playerCarryEntity->size;
        vec2 dir = V2(0);
        Rect mouseRect;
        mouseRect.max = V2(0.1f, 0.1f);
        mouseRect.min = -V2(0.1f, 0.1f);
        if (RectTest(mouseRect, playerCarryRect, playerCarryEntity->position, mousePos, &dir)) {
            if (!playerCarryEntity->soundPlayed) {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);
            }
            playerCarryEntity->soundPlayed = true;
            mousePos = playerCarryEntity->position;
        }
        else
        {
            playerCarryEntity->soundPlayed = false;
            playerCarryEntity->canBePickedUp = false;
        }
    }



    //**********************
    //  RENDER
    //**********************
    
    //      This sets the background color. 
    ClearColor(RGB(0.0f, 0.0f, 0.0f));

    //      Render Barrier
    for (int i = 0; i < barrierBuffer->count; i++) {
        EntityHandle barrierHandle = barrierEntitiesInBuffer[i].handle;
        Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
        if (barrierEntity != NULL) {
            if (barrierEntity->toDelete) {
                DeleteEntity(&Data->em, barrierEntity->handle);
            }
            else {
                DrawSprite(barrierEntity->position, barrierEntity->size, barrierEntity->sprite);
            }
        }
    }


    //      Render Player Carry
    for (int i = 0; i < playerCarryBuffer->count; i++) {
        EntityHandle playerCarryHandle = playerCarryEntitiesInBuffer[i].handle;
        Player* playerCarryEntity = (Player*)GetEntity(&Data->em, playerCarryHandle);
        if (playerCarryEntity != NULL) {
            if (playerCarryEntity->toDelete) {
                DeleteEntity(&Data->em, playerCarryEntity->handle);
            }
            else {
                DrawSprite(playerCarryEntity->position, playerCarryEntity->size, playerCarryEntity->sprite);
            }
        }
    }

    //      Render Player
    for (int i = 0; i < playerBuffer->count; i++) {
        EntityHandle playerHandle = playerEntitiesInBuffer[i].handle;
        Player* playerEntity = (Player*)GetEntity(&Data->em, playerHandle);
        if (playerEntity != NULL) {
            if (playerEntity->toDelete) {
                DeleteEntity(&Data->em, playerEntity->handle);
            }
            else {
                DrawSprite(playerEntity->position, playerEntity->size, playerEntity->sprite);
            }
        }
    }

    //RenderMouse

    DrawSprite(mousePos, V2(0.3f, 0.3f), &Data->sprites.crosshair1Sprite);
}

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

    CreatePlayer();
    CreateLevel(1);

}


void MyGameUpdate() {
    //**********************
    //  LOGIC
    //**********************
    
    //      For all entity types, first Point to EntityBuffers (Logic A: 1 of 2 steps)
    EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];

    //      For all entity types, second Point to EntityBuffer->entities (Logic A: 2 of 2 steps)
    Base* baseEntitiesInBuffer = (Base*)baseBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
    



    //      Handle Player Input for Movement
    InputPlayerController(&playerEntitiesInBuffer[0]);

    //**********************
    //  RENDER
    //**********************
    
    //      This sets the background color. 
    ClearColor(RGB(0.0f, 0.0f, 0.0f));

    //      Render Base
    for (int i = 0; i < baseBuffer->count; i++) {
        EntityHandle baseHandle = baseEntitiesInBuffer[i].handle;
        Player* baseEntity = (Player*)GetEntity(&Data->em, baseHandle);
        if (baseEntity != NULL) {
            if (baseEntity->toDelete) {
                DeleteEntity(&Data->em, baseEntity->handle);
            }
            else {
                DrawSprite(baseEntity->position, baseEntity->size, baseEntity->sprite);
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

}

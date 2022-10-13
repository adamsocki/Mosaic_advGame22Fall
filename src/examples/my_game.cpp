#include "gameCode/structs.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"

void MyInit() {
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;

    
    InitializeEntityManager();
    InitializeEntityBuffers();
    
    LoadSprites();
    LoadSFX();

    CreatePlayer();

    CreateLevel(2);
    //CreateLevel(1);

    InitializeMouse();

    Data->mouseCrosshair.isLocked = false;
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
    Data->mouseCrosshair.position = mousePos;

    

    //      For all entity types, first Point to EntityBuffers (Logic A: 1 of 2 steps)
    EntityTypeBuffer* baseBuffer = &Data->em.buffers[EntityType_Base];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    EntityTypeBuffer* playerCarryBuffer = &Data->em.buffers[EntityType_PlayerCarry];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
    EntityTypeBuffer* levelPortalBuffer = &Data->em.buffers[EntityType_LevelPortal];
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* roomTriggerBuffer = &Data->em.buffers[EntityType_RoomTrigger];

    //      For all entity types, second Point to EntityBuffer->entities (Logic A: 2 of 2 steps)
    Base* baseEntitiesInBuffer = (Base*)baseBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
    PlayerCarry* playerCarryEntitiesInBuffer = (PlayerCarry*)playerCarryBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    LevelPortal* levelPortalEntitiesInBuffer = (LevelPortal*)levelPortalBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    RoomTrigger* roomTriggerEntitiesInBuffer = (RoomTrigger*)roomTriggerBuffer->entities;

    //      Handle Player Input for Movement
    InputPlayerController(&playerEntitiesInBuffer[0]);

    // Detect Collision
    //      Player 
    for (int i = 0; i < playerBuffer->count; i++) {
        // PlayerCarry
        Rect playerRect = playerCollisionRect(1, &playerEntitiesInBuffer[0]);
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
        // Player Barriers 
        for (int j = 0; j < barrierBuffer->count; j++) {
            Rect barrierRect;
            EntityHandle barrierHandle = barrierEntitiesInBuffer[j].handle;
            Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
            barrierRect.max = barrierEntity->size;
            barrierRect.min = -barrierEntity->size;
            vec2(dir) = V2(0, 0);
            if (RectTest(barrierRect, playerRect, barrierEntity->position, playerEntitiesInBuffer[0].position, &dir)) {
                if (!barrierEntity->isDoor && !barrierEntity->isOpen) {
                    playerEntitiesInBuffer[0].position = playerEntitiesInBuffer[0].previousPosition;
                }
               /* else if (barrierEntity->mouseIsOver) {
                    MouseInputClick();
                }*/
            }
        }
        //  Player to Doors
        for (int j = 0; j < doorBuffer->count; j++) {
            Rect doorRect;
            EntityHandle doorHandle = doorEntitiesInBuffer[j].handle;
            Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
            doorRect.max = doorEntity->size;
            doorRect.min = -doorEntity->size;
            vec2(dir) = V2(0, 0);
            if (RectTest(doorRect, playerRect, doorEntity->position, playerEntitiesInBuffer[0].position, &dir)) {
                if (!doorEntity->isDoorOpen) {
                    playerEntitiesInBuffer[0].position = playerEntitiesInBuffer[0].previousPosition;
                }
            }
        }
        // Player to LevelTrigger
        for (int j = 0; j < roomTriggerBuffer->count; j++) {

            Rect roomTriggerRect;
            RoomTrigger* roomTriggerEntity = &roomTriggerEntitiesInBuffer[j];
            DrawSprite(roomTriggerEntity->position, roomTriggerEntity->size, roomTriggerEntity->sprite);
            roomTriggerRect.max = roomTriggerEntity->size;
            roomTriggerRect.min = -roomTriggerEntity->size;
            vec2(dir) = V2(0, 0);
            if (RectTest(roomTriggerRect, playerRect, roomTriggerEntity->position, playerEntitiesInBuffer[0].position, &dir)) {
                DrawSprite(V2(1, 1), V2(1, 1), &Data->sprites.stairsDown1Sprite);
            }
        }
    }

    //  Crosshair Collission Check
    //      Check to PlayerCarry

    Rect mouseRect;
    mouseRect.max = V2(0.2f, 0.2f);
    mouseRect.min = -V2(0.2f, 0.2f);
    vec2 dir = V2(0);
    for (int i = 0; i < playerCarryBuffer->count; i++) {
        Rect playerCarryRect;
        EntityHandle playerCarryHandle = playerCarryEntitiesInBuffer[i].handle;
        PlayerCarry* playerCarryEntity = (PlayerCarry*)GetEntity(&Data->em, playerCarryHandle);
        playerCarryRect.max = playerCarryEntity->size;
        playerCarryRect.min = -playerCarryEntity->size;
       
        if (RectTest(mouseRect, playerCarryRect, playerCarryEntity->position, mousePos, &dir)) {
            if (!playerCarryEntity->mouseOverPlayerCarry) {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                Data->mouseCrosshair.isLocked = true;
                playerCarryEntity->mouseOverPlayerCarry = true;
            }
            Data->mouseCrosshair.position = playerCarryEntity->position;
        }
        else
        {
            playerCarryEntity->mouseOverPlayerCarry = false;
            playerCarryEntity->canBePickedUp = false;
            Data->mouseCrosshair.isLocked = false;
        }
    }
    //      Check to Doors
    for (int i = 0; i < doorBuffer->count; i++) {
        
        EntityHandle doorHandle = doorEntitiesInBuffer[i].handle;
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
        Rect centerDoorRect;
        centerDoorRect.max = doorEntity->size;
        centerDoorRect.min = -doorEntity->size;
        if (RectTest(mouseRect, centerDoorRect, doorEntity->position, mousePos, &dir)) {

            if (!doorEntity->mouseIsOver) {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                doorEntity->mouseIsOver = true;
                Data->mouseCrosshair.isLocked = true;
            }
            Data->mouseCrosshair.position = doorEntity->position;
        }
        else {
            doorEntity->mouseIsOver = false;
            Data->mouseCrosshair.isLocked = false;
        }
        if (doorEntity->isDoorCenter) {
            
        }
        // OPEN DOOR ON CLICK && PLAYER PROXIMITY
        if (doorEntity->mouseIsOver) {
            if (InputPressed(Mouse, Input_MouseLeft)) {
                // door proximity
                Rect expandedDoorProximityRect;
                expandedDoorProximityRect.max = doorEntity->size * 2;
                expandedDoorProximityRect.min = -doorEntity->size * 2;
                Rect playerRect = playerCollisionRect(1, &playerEntitiesInBuffer[0]);
                
                if (RectTest(playerRect, expandedDoorProximityRect, playerEntitiesInBuffer[0].position, doorEntity->position, &dir)) { 
                    
                    doorEntity->isDoorOpen = true;
                    doorEntity->sprite = &Data->sprites.floor1Sprite;
                }
            }
        }
    }

    //      Check to Stairs
    for (int i = 0; i < levelPortalBuffer->count; i++) {
        EntityHandle levelPortalHandle = levelPortalEntitiesInBuffer[i].handle;
        LevelPortal* levelPortalEntity = (LevelPortal*)GetEntity(&Data->em, levelPortalHandle);
        if (levelPortalEntity != NULL) {
            Rect portalRect;
            portalRect.max = levelPortalEntity->size;
            portalRect.min = -levelPortalEntity->size;
            if (RectTest(mouseRect, portalRect, levelPortalEntity->position, mousePos, &dir)) {
                if (!levelPortalEntity->mouseIsOver) {
                    PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                    levelPortalEntity->mouseIsOver = true;
                    Data->mouseCrosshair.isLocked = true;
                }
                Data->mouseCrosshair.position = levelPortalEntity->position;
            }
            else {
                levelPortalEntity->mouseIsOver = false;
                Data->mouseCrosshair.isLocked = false;
            }
        }
    }


    //**********************
    //  RENDER
    //**********************
    
    //      This sets the background color. 
    ClearColor(RGB(0.0f, 0.0f, 0.0f));
    
    //      Render Base
    for (int i = 0; i < baseBuffer->count; i++) {
        EntityHandle baseHandle = baseEntitiesInBuffer[i].handle;
        Base* baseEntity = (Base*)GetEntity(&Data->em, baseHandle);
        if (baseEntity != NULL) {
            if (baseEntity->activeRoom) {
                if (baseEntity->toDelete) {
                    DeleteEntity(&Data->em, baseEntity->handle);
                }
                else {
                    if (baseEntity->isQuad) {

                        DrawRect(baseEntity->position, baseEntity->size, RGB(1.0f, 0.3f, 0.3f));
                    }
                    else {
                        DrawSprite(baseEntity->position, baseEntity->size, baseEntity->sprite);
                    }

                }
            }
           
        }
    }
    //      Render Barriers
    for (int i = 0; i < barrierBuffer->count; i++) {
        EntityHandle barrierHandle = barrierEntitiesInBuffer[i].handle;
        Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
        if (barrierEntity != NULL) {
            if (barrierEntity->activeRoom) {
                if (barrierEntity->toDelete) {
                    DeleteEntity(&Data->em, barrierEntity->handle);
                }
                else {
                    DrawSprite(barrierEntity->position, barrierEntity->size, barrierEntity->sprite);
                }
            }
            
        }
    }
    //      Render Doors
    for (int i = 0; i < doorBuffer->count; i++) {
        EntityHandle doorHandle = doorEntitiesInBuffer[i].handle;
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
        if (doorEntity != NULL) {
            if (doorEntity->activeRoom) {
                if (doorEntity->toDelete) {
                    DeleteEntity(&Data->em, doorEntity->handle);
                }
                else {
                    DrawSprite(doorEntity->position, doorEntity->size, doorEntity->sprite);
                }
            }
          
        }
    }

    //      
    
    //      Render Level Portals
    for (int i = 0; i < levelPortalBuffer->count; i++) {
        EntityHandle levelPortalHandle = levelPortalEntitiesInBuffer[i].handle;
        LevelPortal* levelPortalEntity = (LevelPortal*)GetEntity(&Data->em, levelPortalHandle);
        if (levelPortalEntity != NULL) {
            if (levelPortalEntity->toDelete) {
                DeleteEntity(&Data->em, levelPortalEntity->handle);
            }
            else {
                if (levelPortalEntity->isQuad) {

                    DrawRect(levelPortalEntity->position, levelPortalEntity->size, RGB(1.0f, 0.3f, 0.3f));
                }
                else {
                    DrawSprite(levelPortalEntity->position, levelPortalEntity->size, levelPortalEntity->sprite);
                }

            }
        }
    }


    //      Render PlayerCarry
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

    // TODO: Fix this render issue where the value isLocked doesn't turn off
    //RenderMouse
    if (!Data->mouseCrosshair.isLocked) {
        
    }
    else {

    }
    
 DrawSprite(Data->mouseCrosshair.position, V2(0.3f, 0.3f), &Data->sprites.crosshairUnlocked1Sprite);
}


MemoryArena arena = {};
MemoryArena roomArena = {};
MemoryArena monsterArena = {};
MemoryArena doorArena = {};
MemoryArena objectArena = {};
MemoryArena editorObjectsArena = {};
MemoryArena playerArena = {};
MemoryArena barrierArena = {};

#include "gameCode/structs.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"
#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"
#include "gameCode/levelParser.cpp"

void MyInit()
{
	Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

	AllocateMemoryArena(&arena, Megabytes(64));
    AllocateMemoryArena(&roomArena, Megabytes(2));
    AllocateMemoryArena(&monsterArena, Megabytes(2));
    AllocateMemoryArena(&doorArena, Megabytes(2));
    AllocateMemoryArena(&objectArena, Megabytes(4));
	AllocateMemoryArena(&barrierArena, Megabytes(4));
	AllocateMemoryArena(&editorObjectsArena, Megabytes(2));
    AllocateMemoryArena(&playerArena, Megabytes(2));
    
	Data = (MyData*)Game->myData;

	InitializeEntityManager();
    InitializeEntityBuffers(false);
    
    LoadSprites();
    LoadSFX();
	
	//CreatePlayer();

    Data->currentLevel = 1;
	LevelState levelState[5];
	LoadLevelParse(Data->currentLevel, levelState);
	
	
    InitializeMouse();

    Data->mouseCrosshair.isLocked = false;
	Camera* cam = &Game->camera;

    cam->width = 128;
    cam->height = 72;
	//cam->cameraPosition.x += cam->width / 2;
    Game->cameraPosition = V3(cam->width / 2, cam->height / 2,0);
    
	cam->projection = Orthographic(cam->width * -0.5f * cam->size, cam->width * 0.5f * cam->size,
        cam->height * -0.5f * cam->size, cam->height * 0.5f * cam->size, 0.0, 100.0f);
/*
    UpdateCamera(cam, Game->cameraPosition, Game->cameraRotation);
*/
}


void MyGameUpdate() 
{
	//**********************
	//**********************
    //  LOGIC
    //**********************
	//**********************
    
    //      Hide mouse cursor & Get Mouse Position
    while (ShowCursor(false) >= 0);
    vec2 mousePos = Input->mousePosNormSigned;
    mousePos.x = mousePos.x * 8;
    mousePos.y = mousePos.y * 4.5f;
    Data->mouseCrosshair.position = mousePos;
	
	 
    //      CREATE ENTITY BUFFER REFERENCES
    //          BUFFER
    EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
    EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
	
	//      Handle Player Input for Movement
    InputPlayerController(&playerEntitiesInBuffer[0]);
	
	
	//  Crosshair Collission Check
    //      Check to PlayerCarry

    Rect mouseRect;
    mouseRect.max = V2(0.2f, 0.2f);
    mouseRect.min = -V2(0.2f, 0.2f);
    vec2 dir = V2(0);
    for (int i = 0; i < objectBuffer->count; i++) 
    {
        Rect objectRect;
        EntityHandle objectHandle = objectEntitiesInBuffer[i].handle;
        Object* objectEntity = (Object*)GetEntity(&Data->em, objectHandle);
        objectRect.max = objectEntity->size;
        objectRect.min = -objectEntity->size;
       
        if (RectTest(mouseRect, objectRect, objectEntity->position, mousePos, &dir)) {
            if (!objectEntity->mouseOverobject) 
            {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                Data->mouseCrosshair.isLocked = true;
                objectEntity->mouseOverobject = true;
            }
            Data->mouseCrosshair.position = objectEntity->position;
        }
        else
        {
            objectEntity->mouseOverobject = false;
            objectEntity->canPickUp = false;
            Data->mouseCrosshair.isLocked = false;
        }
    }
    //      Check to Doors
    for (int i = 0; i < doorBuffer->count; i++) 
    {
        EntityHandle doorHandle = doorEntitiesInBuffer[i].handle;
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorHandle);
        Rect centerDoorRect;
        centerDoorRect.max = doorEntity->size;
        centerDoorRect.min = -doorEntity->size;
        if (RectTest(mouseRect, centerDoorRect, doorEntity->position, mousePos, &dir)) 
        {

            if (!doorEntity->mouseIsOver) 
            {
                PlaySound(&Game->audioPlayer, Data->sound.crosshairSound1, 1.0f, false);

                doorEntity->mouseIsOver = true;
                Data->mouseCrosshair.isLocked = true;
            }
            Data->mouseCrosshair.position = doorEntity->position;
        }
        else 
        {
            doorEntity->mouseIsOver = false;
            Data->mouseCrosshair.isLocked = false;
        }
        if (doorEntity->isDoorCenter) 
        {
            
        }
        // OPEN DOOR ON CLICK && PLAYER PROXIMITY
        // SHOW HIDDEN ROOM IF DOOR OPEN ACTIVATES ROOM
        if (doorEntity->mouseIsOver) 
        {/*	
            if (InputPressed(Mouse, Input_MouseLeft)) 
            {
                // door proximity
                Rect expandedDoorProximityRect;
                expandedDoorProximityRect.max = doorEntity->size * 2;
                expandedDoorProximityRect.min = -doorEntity->size * 2;
                Rect playerRect = playerCollisionRect(1, &playerEntitiesInBuffer[0]);
                if (RectTest(playerRect, expandedDoorProximityRect, playerEntitiesInBuffer[0].position, doorEntity->position, &dir)) 
                { 
                    doorEntity->isDoorOpen = !doorEntity->isDoorOpen;
                    if (doorEntity->doorActivatesRoom) 
                    {
                        for (int i = 0; i < baseBuffer->count; i++) 
                        {
                            Base* baseEntity = (Base*)GetEntity(&Data->em, baseEntitiesInBuffer[i].handle);
                            if (baseEntity->roomNumber == doorEntity->roomDoorActivates)
                            {
                                baseEntity->activeRoom = true;
                            }
                        }
                    }
                }
            }*/
        }
    }
	
	
	
	//**********************
	//**********************
    //  RENDER
    //**********************
	//**********************
	ClearColor(RGB(0.0f, 0.0f, 0.0f));

	// 				RENDER ROOM
	for (int i = 0; i < roomBuffer->count; i++) 
    {
        Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[i].handle);
        if (roomEntity != NULL)
        {
           // vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(roomEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
           // vec2 sizeForEntityInIndex = convertSizeToIndexSize(roomEntity->size, sizeOfLevelCanvas);

            if (!roomEntity->activeRoom)
            {
                DrawRectBottomLeft(roomEntity->position1, roomEntity->size, 0, RGB(1.0f, 0.5f, 0.5f));
            }
            else
            {
                DrawRectBottomLeft(roomEntity->position1, roomEntity->size, 0, RGB(1.0f, 0.8f, 0.8f));
            }
        }
    }	
	//      Render Player
    //              PLAYER
    for (int i = 0; i < playerBuffer->count; i++)
    {
        Player* playerEntity = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[i].handle);
        if (playerEntity != NULL)
        {
            //vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(playerEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            //vec2 sizeForEntityInIndex = convertSizeToIndexSize(playerEntity->size, sizeOfLevelCanvas);
            DrawSpriteBottomLeft(playerEntity->position1, playerEntity->size, 0, &Data->sprites.playerSprite);
        }
    }

}

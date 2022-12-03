
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
#include "gameCode/MouseLogic.cpp"
#include "gameCode/EventManager.cpp"
#include "gameCode/ObjectLogicRender.cpp"

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

    Data->currentLevel = 0;
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
    Camera* cam = &Game->camera;
    while (ShowCursor(false) >= 0);
    vec2 mousePos = Input->mousePosNormSigned;
    mousePos.x = mousePos.x * cam->width;
    mousePos.y = mousePos.y * cam->height;
    Data->mouseCrosshair.position = mousePos;
	
	 
    //      CREATE ENTITY BUFFER REFERENCES
    //          BUFFER
    EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
    EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
    EntityTypeBuffer* barrierBuffer = &Data->em.buffers[EntityType_Barrier];
    EntityTypeBuffer* doorBuffer = &Data->em.buffers[EntityType_Door];
    EntityTypeBuffer* objectBuffer = &Data->em.buffers[EntityType_Object];
    EntityTypeBuffer* playerBuffer = &Data->em.buffers[EntityType_Player];
	EntityTypeBuffer* eventBuffer = &Data->em.buffers[EntityType_Event];
    EntityTypeBuffer* overlayBuffer = &Data->em.buffers[EntityType_Overlay];

    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;
    Player* playerEntitiesInBuffer = (Player*)playerBuffer->entities;
	Event* eventEntitiesInBuffer = (Event*)eventBuffer->entities;
    Overlay* overlayEntitiesInBuffer = (Overlay*)overlayBuffer->entities;

	//      Handle Player Input for Movement
    InputPlayerController(&playerEntitiesInBuffer[0]);
	
	
	//  Crosshair Collission Check
    CrosshairCheck();
   
	AssignCurrentRoom();
    BarrierCheck();
	

    ObjectLogic();
	
    ExecuteEvents();



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

            if (roomEntity->activeRoom)
            {
                DrawRectBottomLeft(roomEntity->position1, roomEntity->size, 0, RGB(1.0f, 0.8f, 0.8f));
            }
            else
            {
                //DrawRectBottomLeft(roomEntity->position1, roomEntity->size, 0, RGB(1.0f, 0.5f, 0.5f));

            }
        }
    }	
    //              RENDER OBJECTS
    ObjectRender();
    //              RENDER DOOR
    for (int i = 0; i < doorBuffer->count; i++) {
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
        if (doorEntity != NULL) {
            if (doorEntity->activeRoom) {
                if (doorEntity->isDoorOpen)
                {

                }
                else
                {
                    DrawSpriteBottomLeft(doorEntity->position1, doorEntity->size, 0, &Data->sprites.doorClosed1Sprite);
                }
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

    //      Render Overlay
    //              OVERLAY
    for (int i = 0; i < overlayBuffer->count; i++)
    {
        Overlay* overlayEntity = (Overlay*)GetEntity(&Data->em, overlayEntitiesInBuffer[i].handle);
        if (overlayEntity != NULL)
        {

            //vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(playerEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            //vec2 sizeForEntityInIndex = convertSizeToIndexSize(playerEntity->size, sizeOfLevelCanvas);
            //DrawSpriteBottomLeft(playerEntity->position1, playerEntity->size, 0, &Data->sprites.playerSprite);
        }
    }


    //      RENDER MOUSE CROSSHAIR

    DrawSprite(Data->mouseCrosshair.position, V2(1.0f, 1.0f), &Data->sprites.crosshairUnlocked1Sprite);




    //          ***************
    //          ***************
    //          DELETE ENTITIES
    //          ***************
    //          ***************

    for (int i = 0; i < eventBuffer->count; i++)
    {
        Event* eventEntity = (Event*)GetEntity(&Data->em, eventEntitiesInBuffer[i].handle);
        if (eventEntity->toDelete)
        {
            DeleteEntity(&Data->em, eventEntity->handle);
        }
    }


}

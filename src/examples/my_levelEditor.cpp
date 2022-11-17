
MemoryArena arena = {};
MemoryArena roomArena = {};
MemoryArena monsterArena = {};
MemoryArena doorArena = {};
MemoryArena objectArena = {};

MemoryArena editorObjectsArena = {};
MemoryArena playerArena = {};

#include "gameCode/structs.cpp"
#include "gameCode/convertTools.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"

#include "gameCode/levelEditor.cpp"
#include "gameCode/levelParser.cpp"

#include "gameCode/RenderLE.cpp"


//TODO: properly set up entity Manager and all input files

vec2 mousePosRender;


LevelState levelState[5];
DynamicArray<EditorPlacementObject> editorObjects;

void MyInit() 
{
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;

    Data->levelEditor.editorState = NotEditingWithSprite;

    AllocateMemoryArena(&arena, Megabytes(64));
    AllocateMemoryArena(&roomArena, Megabytes(2));
    AllocateMemoryArena(&monsterArena, Megabytes(2));
    AllocateMemoryArena(&doorArena, Megabytes(2));
    AllocateMemoryArena(&objectArena, Megabytes(4));

    AllocateMemoryArena(&editorObjectsArena, Megabytes(2));
    AllocateMemoryArena(&playerArena, Megabytes(2));

    editorObjects = MakeDynamicArray<EditorPlacementObject>(&editorObjectsArena, 120);
    
    InitializeEntityManager();
    InitializeEntityBuffers(false);
    InitializeMouse();
    InitializeLevelEditor();
    LoadSprites();

    Camera* cam = &Game->camera;

    cam->width = 16;
    cam->height = 9;
    cam->projection = Orthographic(cam->width * -0.5f * cam->size, cam->width * 0.5f * cam->size,
        cam->height * -0.5f * cam->size, cam->height * 0.5f * cam->size, 0.0, 100.0f);

    UpdateCamera(cam, Game->cameraPosition, Game->cameraRotation);

}

bool* test = false;
void MyGameUpdate()
{
    //**********************
    //  LOGIC
    //**********************
    
    //     HIDE CURSOR
    while (ShowCursor(false) >= 0);

    //      LOCAL VARIABLES
    real32 factorValue = 1.25f;
    vec2 levelMapOffset = V2(1, 0);
    bool updateMouseRender = false;
    bool showMouseOnMap = false;
    vec2 tileSize = V2(0.125f, 0.125f);
    vec2 mouseIndex;
    vec2 realMousePosition = Input->mousePosNormSigned;
    realMousePosition.x *= 8;
    realMousePosition.y *= 4.5f;
    EditorPlacementObject levelEntityStorage = {};


    //      LEVEL MAP LOGIC Ratio
    vec2 levelMapdisplayRatio = V2(8 / factorValue + (tileSize.x / 2), 4.5f / factorValue + (tileSize.y / 2));
    vec2 sizeOfLevelCanvas = V2(levelMapdisplayRatio.x * 2, levelMapdisplayRatio.y * 2);
    vec2 startingPosForLevelCanvasBottomLeft = V2(-levelMapdisplayRatio.x + 1, -levelMapdisplayRatio.y);


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

    //      CAMERA DATA
    Camera* cam = &Game->camera;
        
    //      GET MOUSE POSITION
    vec2 mousePos = Input->mousePosNormSigned;
    mousePos.x = mousePos.x * (8 * factorValue) - (levelMapOffset.x * factorValue) + 8;
    mousePos.y = mousePos.y * (4.5f * factorValue) - (levelMapOffset.y * factorValue) + 4.5f;
    //      CREATE MOUSE RECT
    Rect mouseRect;

    //      DETECT MOUSE WITHIN LEVEL MAP
    if (mousePos.x >= 0 && mousePos.x <= 16 &&
        mousePos.y >= 0 && mousePos.y <=  9)
    {
        showMouseOnMap = true;
        //      STEP G: LOGIC TO SET UP HOW MOUSE OPERATES WHEN IT IS WITHIN MAP (MOVE AT TILESIZE INTERVALS)
        mousePos.x = SnapUp(mousePos.x, tileSize.x);
        mousePos.y = SnapUp(mousePos.y, tileSize.y);
        if (Data->mouseCrosshair.previousPosition != mousePos)
        {
            updateMouseRender = true;
        }
    }
    else
    {   
        // : make a new mosue variable ??? 
        mousePos = V2(0, 0);
        showMouseOnMap = false;
    }
    //      SET MOUSE PREVIOUS POSITION FOR INTERVAL MOVEMENT IN STEP G
    Data->mouseCrosshair.previousPosition = mousePos;
  
    //      SET UP MOUSE INDEX
    if (showMouseOnMap)
    {
        mouseIndex = V2((mousePos.x / 16) * (16 / tileSize.x), (mousePos.y / 9) * (9 / tileSize.y));
    }
    
    // **************
    // **************
    // 
    //      SPRITE PALATE LOGIC
    // 
    // **************
    // **************
    // 
    //          TODO: ALLOW PLUS/MINUS FOR SWITCHING BETWEEN ALL OBJECTS PLACED SO FAR IN LEVEL
    //          TODO: FROM ABOVE, HAVE A CURSOR THAT RENDERS AT THE OBJECT START POSITION
    //          TODO: FROM ABOVE, CHANGE LISTING OF ENTITY SPECIFICATIONS
   
    //          base sprite pallate
    //      Collision with Mouse +/-
    //      Collission with Mouse Obj 1

    //      MOUSE RECT & SPRITE PALETTTE SELECTION RECT
    //mouseRect;
    mouseRect.max =  V2(0.125f, 0.125f);
    mouseRect.min = -V2(0.125f, 0.125f);
    realMousePosition.y += 0.125f;
    vec2 dir = V2(0);
    Rect spritePalellteSelectionRect;
    spritePalellteSelectionRect = mouseRect;

    //      CYCLE THROUGH ENTITIES - PALATTE
    Rect cycleEntitiesPlusRect;
    Rect cycleEntitiesMinusRect;
    vec2 cycleEntitiesExtraPosition = V2(-6.275f, 4);
    real32 cycleEntitiesplusMinusOffset = 0.25f;
    vec2 cycleEntitiesMinusPosition = cycleEntitiesExtraPosition;
    cycleEntitiesMinusPosition.x = cycleEntitiesMinusPosition.x - cycleEntitiesplusMinusOffset;
    vec2 cycleEntitiesPlusPosition = cycleEntitiesExtraPosition;
    cycleEntitiesPlusPosition.x = cycleEntitiesPlusPosition.x + cycleEntitiesplusMinusOffset;
    vec2 cycleEntitiesPlusMinusSize = V2(0.1f, 0.1f);
    cycleEntitiesPlusRect.max =   V2(cycleEntitiesPlusMinusSize.x / 2, cycleEntitiesPlusMinusSize.y / 2);
    cycleEntitiesPlusRect.min =  -V2(cycleEntitiesPlusMinusSize.x / 2, cycleEntitiesPlusMinusSize.y / 2);
    cycleEntitiesMinusRect.max =  V2(cycleEntitiesPlusMinusSize.x / 2, cycleEntitiesPlusMinusSize.y / 2);
    cycleEntitiesMinusRect.min = -V2(cycleEntitiesPlusMinusSize.x / 2, cycleEntitiesPlusMinusSize.y / 2);
    //      plus collission
    bool overCycleEntitiesPlus = false;
    if (RectTest(cycleEntitiesPlusRect, mouseRect, realMousePosition, cycleEntitiesPlusPosition, &dir))
    {
        overCycleEntitiesPlus = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {

        }
    }
    //      MINUS collission
    bool overCycleEntitiesMinus = false;
    if (RectTest(cycleEntitiesMinusRect, mouseRect, realMousePosition, cycleEntitiesMinusPosition, &dir))
    {
        overCycleEntitiesMinus = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {

        }
    }

    //      BASE RECT - PALATTE
    bool overRoom = false;
    vec2 roomSpritePalattePosition = V2(-7.25f, 2.75f);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, roomSpritePalattePosition, &dir))
    {
        
        overRoom = true;
        if (InputPressed(Mouse, Input_MouseLeft)) {
            Data->levelEditor.arrowToObject = {};
            Data->levelEditor.editorType = EntityType_Room;
            Data->levelEditor.editorState = EditingWithSprite;
        }
    }
    //      MONSTER RECT - PALATTE
    bool overMonsterPalatte = false;
    vec2 monsterSpritePalattePosition = V2(-6.25f, 2.75f);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, monsterSpritePalattePosition, &dir))
    {
        overMonsterPalatte = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {
            Data->levelEditor.arrowToObject = {};
            Data->levelEditor.editorType = EntityType_Monster;
            Data->levelEditor.editorState = EditingWithSprite;
        }
    }
    //      DOOR RECT - PALATTE
    bool overDoorPalatte = false;
    vec2 doorSpritePalattePosition = V2(-7.25f, 2.2f);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, doorSpritePalattePosition, &dir))
    {
        overDoorPalatte = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject = {};
            Data->levelEditor.editorType = EntityType_Door;
            Data->levelEditor.editorState = EditingWithSprite;  
        }
    }
    //      OBJECT RECT - PALATTE
    bool overObjectPalatte = false;
    vec2 objectSpritePalattePosition = V2(-6.25f, 2.2f);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, objectSpritePalattePosition, &dir))
    {
        overObjectPalatte = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject = {};
            Data->levelEditor.editorType = EntityType_Object;
            Data->levelEditor.editorState = EditingWithSprite;
        }
    }
    //      PLAYER RECT - PALATTE
    bool overPlayerPalatte = false;
    vec2 playerSpritePalattePosition = V2(-7.25f, 1.75f);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, playerSpritePalattePosition, &dir))
    {
        overPlayerPalatte = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject = {};
            Data->levelEditor.editorType = EntityType_Player;
            Data->levelEditor.editorState = EditingWithSprite;
        }
    }


    //      ****
    //      DETELE SPRITE LOGIC
    bool overDeleteSprite = false;
    Rect trashCanSpriteRect = spritePalellteSelectionRect;
    vec2 deleteButtonSpritePosition = V2(-6.65f, -1.3f);
    if (RectTest(trashCanSpriteRect, mouseRect, realMousePosition, deleteButtonSpritePosition, &dir))
    {
        overDeleteSprite = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editDeleteEntity = true;

            Data->levelEditor.arrowToObject.changeDelete = true;
        }
    }

    //      SPRITE EDITING LOGIC
    //      creation of object when placed on the mpa
    if (Data->levelEditor.editorState == EditingWithSprite)
    {
        if (showMouseOnMap)
        {
            bool canPlaceObject = true;

            if (Data->levelEditor.editorType == EntityType_Door) {
                // TODO: MAKE SURE THAT IT WILL BE PLACED AT THE EDGE OF A WALL
                
                // canPlaceObject = false;

                // TODO: if (object is in right place) { canPlaceObject = true}
            }
           
            if (InputPressed(Mouse, Input_MouseLeft && canPlaceObject))
            {
                // Creates the object when it is placed.
                StartObjectCreation(Data->levelEditor.editorType, mouseIndex, Data->currentLevel);
            }

            while (Data->levelEditor.levelObjects[Data->levelEditor.count].isSizable)
            {
                ObjectSizer(mouseIndex);
            }
        }
    }


    //      ***********
    //      ARROW POINTING TO CURRENT OBEJCT
    //      ***********
    switch (Data->levelEditor.editorType)
    {
        case EntityType_Monster:
        {
            if (monsterBuffer->count != 0)
            {
                InputForEntityArrowMovement(InputPressed(Keyboard, Input_Q), InputPressed(Keyboard, Input_W), monsterBuffer->count);
                
                int32 arrowCount = Data->levelEditor.arrowToObject.counter;
                Monster* monsterEntity = (Monster*)GetEntity(&Data->em, monsterEntitiesInBuffer[arrowCount].handle);
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(monsterEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(monsterEntity->size, sizeOfLevelCanvas);
                vec2 offsetForArrow = V2(sizeForEntityInIndex.x / 2, -0.125f);

                Data->levelEditor.arrowToObject.position = startPositionForEntityInIndex + offsetForArrow;
                Data->levelEditor.arrowToObject.indexPosition = monsterEntity->position1;
                Data->levelEditor.arrowToObject.objectSizeIndex = monsterEntity->size;
                Data->levelEditor.arrowToObject.entityRoomNum = monsterEntity->roomNumber;
                Data->levelEditor.arrowToObject.activeRoom = monsterEntity->activeRoom;
                Data->levelEditor.arrowToObject.isEntityDeleted = monsterEntity->toDelete;
                
                if (Data->levelEditor.arrowToObject.editEntityPos)
                {   // edit entity position,
                    monsterEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    monsterEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {   // edit entity size;
                    monsterEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    monsterEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
                if (Data->levelEditor.arrowToObject.activeEdit)
                {   // edit entity active
                    monsterEntity->activeRoom = !monsterEntity->activeRoom;
                    Data->levelEditor.arrowToObject.activeEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editRoomNum)
                {   // edit room Num;
                    monsterEntity->roomNumber += Data->levelEditor.arrowToObject.roomNumEditInt;
                    Data->levelEditor.arrowToObject.roomNumEditInt = 0;
                    Data->levelEditor.arrowToObject.editRoomNum = false;
                }
                if (Data->levelEditor.arrowToObject.editDeleteEntity)
                {
                    DeleteEntity(&Data->em, monsterEntitiesInBuffer[arrowCount].handle);
                    Data->levelEditor.arrowToObject.editDeleteEntity = false;
                }
            }
            else
            {   // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
                Data->levelEditor.arrowToObject.showArrow = false;
            }
            break;
        }
        case EntityType_Room:
        {
            if (roomBuffer->count != 0) {
                InputForEntityArrowMovement(InputPressed(Keyboard, Input_Q), InputPressed(Keyboard, Input_W), roomBuffer->count);
                int32 arrowCount = Data->levelEditor.arrowToObject.counter;
                Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[arrowCount].handle);
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(roomEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(roomEntity->size, sizeOfLevelCanvas);
                vec2 offsetForArrow = V2(sizeForEntityInIndex.x / 2, -0.125f);

                Data->levelEditor.arrowToObject.position = startPositionForEntityInIndex + offsetForArrow;
                Data->levelEditor.arrowToObject.indexPosition = roomEntity->position1;
                Data->levelEditor.arrowToObject.objectSizeIndex = roomEntity->size;
                Data->levelEditor.arrowToObject.entityRoomNum = roomEntity->roomNumber;
                Data->levelEditor.arrowToObject.activeRoom = roomEntity->activeRoom;


                if (Data->levelEditor.arrowToObject.editEntityPos)
                {   // edit entity position
                    roomEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    roomEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {   // edit entity size;
                    roomEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    roomEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
                if (Data->levelEditor.arrowToObject.activeEdit)
                {   // edit room active;
                    roomEntity->activeRoom = !roomEntity->activeRoom;
                    Data->levelEditor.arrowToObject.activeEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editRoomNum)
                {   // edit room Num;
                    roomEntity->roomNumber += Data->levelEditor.arrowToObject.roomNumEditInt;
                    Data->levelEditor.arrowToObject.roomNumEditInt = 0;
                    Data->levelEditor.arrowToObject.editRoomNum = false;
                }
                if (Data->levelEditor.arrowToObject.editDeleteEntity)
                {
                    DeleteEntity(&Data->em, roomEntitiesInBuffer[arrowCount].handle);
                    Data->levelEditor.arrowToObject.editDeleteEntity = false;
                }
            }
            else
            {
                // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
                Data->levelEditor.arrowToObject.showArrow = false;
            }
            break;
        }
        case EntityType_Door:
        {
            if (doorBuffer->count != 0)
            {
                InputForEntityArrowMovement(InputPressed(Keyboard, Input_Q), InputPressed(Keyboard, Input_W), doorBuffer->count);
                int32 arrowCount = Data->levelEditor.arrowToObject.counter;
                Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[arrowCount].handle);
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(doorEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(doorEntity->size, sizeOfLevelCanvas);
                vec2 offsetForArrow = V2(sizeForEntityInIndex.x / 2, -0.125f);

                Data->levelEditor.arrowToObject.position = startPositionForEntityInIndex + offsetForArrow;
                Data->levelEditor.arrowToObject.indexPosition = doorEntity->position1;
                Data->levelEditor.arrowToObject.objectSizeIndex = doorEntity->size;
                Data->levelEditor.arrowToObject.doorTo = doorEntity->doorTo;
                Data->levelEditor.arrowToObject.doorFrom = doorEntity->doorFrom;
                Data->levelEditor.arrowToObject.doorType = doorEntity->doorType;
                Data->levelEditor.arrowToObject.activeRoom = doorEntity->activeRoom;
                Data->levelEditor.arrowToObject.levelTo = doorEntity->levelTo;
                Data->levelEditor.arrowToObject.isEntityDeleted = doorEntity->toDelete;


                if (Data->levelEditor.arrowToObject.editEntityPos)
                {   // edit entity position
                    doorEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    doorEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {   // edit entity size;
                    doorEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    doorEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
                if (Data->levelEditor.arrowToObject.activeEdit)
                {   // edit room active;
                    doorEntity->activeRoom = !doorEntity->activeRoom;
                    Data->levelEditor.arrowToObject.activeEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editDoorFrom)
                {   // edit room From;
                    doorEntity->doorFrom += Data->levelEditor.arrowToObject.doorFromEditInt;
                    Data->levelEditor.arrowToObject.doorFromEditInt = 0;
                    Data->levelEditor.arrowToObject.editDoorFrom = false;
                }
                if (Data->levelEditor.arrowToObject.editDoorTo)
                {   // edit room To;
                    doorEntity->doorTo += Data->levelEditor.arrowToObject.doorToEditInt;
                    Data->levelEditor.arrowToObject.doorToEditInt = 0;
                    Data->levelEditor.arrowToObject.editDoorTo = false;
                }
                if (Data->levelEditor.arrowToObject.editDoorType)
                {   // edit room From;
                    doorEntity->doorType += Data->levelEditor.arrowToObject.doorTypeEditInt;
                    Data->levelEditor.arrowToObject.doorTypeEditInt = 0;
                    Data->levelEditor.arrowToObject.editDoorType = false;
                }
                if (Data->levelEditor.arrowToObject.levelChangerEdit)
                {   // edit level changer status
                    doorEntity->levelChanger = !doorEntity->levelChanger;
                    Data->levelEditor.arrowToObject.levelChangerEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editLevelTo)
                {   // edit level to number
                    doorEntity->levelTo += Data->levelEditor.arrowToObject.levelToEditInt;
                    Data->levelEditor.arrowToObject.levelToEditInt = 0;
                    Data->levelEditor.arrowToObject.editLevelTo = false;
                }
                if (Data->levelEditor.arrowToObject.editDeleteEntity)
                {
                    DeleteEntity(&Data->em, doorEntitiesInBuffer[arrowCount].handle);
                    Data->levelEditor.arrowToObject.changeDelete = false;
                    Data->levelEditor.arrowToObject.editDeleteEntity = false;
                }
            }
            else
            {   // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
                Data->levelEditor.arrowToObject.showArrow = false;
            }
            break;
        }
        case EntityType_Object:
        {
            if (objectBuffer->count != 0)
            {
                InputForEntityArrowMovement(InputPressed(Keyboard, Input_Q), InputPressed(Keyboard, Input_W), objectBuffer->count);
                int32 arrowCount = Data->levelEditor.arrowToObject.counter;
                Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[arrowCount].handle);
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(objectEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(objectEntity->size, sizeOfLevelCanvas);
                vec2 offsetForArrow = V2(sizeForEntityInIndex.x / 2, -0.125f);

                Data->levelEditor.arrowToObject.position = startPositionForEntityInIndex + offsetForArrow;
                Data->levelEditor.arrowToObject.indexPosition = objectEntity->position1;
                Data->levelEditor.arrowToObject.objectSizeIndex = objectEntity->size;
                Data->levelEditor.arrowToObject.canPickUpThisObject = objectEntity->canPickUp;
                Data->levelEditor.arrowToObject.spriteNumber = objectEntity->spriteNumber;
                Data->levelEditor.arrowToObject.objectType = objectEntity->objectType;
                Data->levelEditor.arrowToObject.entityRoomNum = objectEntity->roomNumber;
                Data->levelEditor.arrowToObject.isEntityDeleted = objectEntity->toDelete;


                if (Data->levelEditor.arrowToObject.editEntityPos)
                {   // edit entity position
                    objectEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    objectEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {   // edit entity size;
                    objectEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    objectEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
                if (Data->levelEditor.arrowToObject.activeEdit)
                {   // edit room active;
                    objectEntity->activeRoom = !objectEntity->activeRoom;
                    Data->levelEditor.arrowToObject.activeEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editRoomNum)
                {   // edit room Num;
                    objectEntity->roomNumber += Data->levelEditor.arrowToObject.roomNumEditInt;
                    Data->levelEditor.arrowToObject.roomNumEditInt = 0;
                    Data->levelEditor.arrowToObject.editRoomNum = false;
                }
                if (Data->levelEditor.arrowToObject.editSpriteNumber)
                {   // edit sprite number
                    objectEntity->spriteNumber += Data->levelEditor.arrowToObject.editSpriteNumberInt;
                    Data->levelEditor.arrowToObject.editSpriteNumberInt = 0;
                    Data->levelEditor.arrowToObject.editSpriteNumber = false;
                }
                if (Data->levelEditor.arrowToObject.canPickUpThisObjectEdit)
                {   // edit can pick up object;
                    objectEntity->canPickUp = !objectEntity->canPickUp;
                    Data->levelEditor.arrowToObject.canPickUpThisObjectEdit = false;
                }
                if (Data->levelEditor.arrowToObject.editDeleteEntity)
                {
                    DeleteEntity(&Data->em, objectEntitiesInBuffer[arrowCount].handle);
                    Data->levelEditor.arrowToObject.changeDelete = false;
                    Data->levelEditor.arrowToObject.editDeleteEntity = false;
                }

            }
            break;
        }
        case EntityType_Player:
        {
            if (playerBuffer->count != 0)
            {
                InputForEntityArrowMovement(InputPressed(Keyboard, Input_Q), InputPressed(Keyboard, Input_W), playerBuffer->count);
                int32 arrowCount = Data->levelEditor.arrowToObject.counter;
                Player* playerEntity = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[arrowCount].handle);
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(playerEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(playerEntity->size, sizeOfLevelCanvas);
                vec2 offsetForArrow = V2(sizeForEntityInIndex.x / 2, -0.125f);
                    
                Data->levelEditor.arrowToObject.position = startPositionForEntityInIndex + offsetForArrow;
                Data->levelEditor.arrowToObject.indexPosition = playerEntity->position1;
                Data->levelEditor.arrowToObject.objectSizeIndex = playerEntity->size;
                Data->levelEditor.arrowToObject.isEntityDeleted = playerEntity->toDelete;
            
                if (Data->levelEditor.arrowToObject.editEntityPos)
                {   // edit entity position
                    playerEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    playerEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {   // edit entity size;
                    playerEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    playerEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
                if (Data->levelEditor.arrowToObject.editDeleteEntity)
                {
                    DeleteEntity(&Data->em, playerEntitiesInBuffer[arrowCount].handle);
                    Data->levelEditor.arrowToObject.changeDelete = false;
                    Data->levelEditor.arrowToObject.editDeleteEntity = false;
                }
            }
            else
            {   // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
                Data->levelEditor.arrowToObject.showArrow = false;
            }
        }

        default:
        {
            break;
        }
    }


    //      SPRITE SPECIFICS LOGIC

    //  TODO:CREATE OBJECT ENTITY PLUS MINUS THING
   
    Rect spritePlusRect;
    Rect spriteMinusRect;
    vec2 spritePlusMinusSize = V2(0.1f, 0.1f);
    spritePlusRect.max = V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spritePlusRect.min = -V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spriteMinusRect.max = V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spriteMinusRect.min = -V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    vec2 spriteExtra_edit1 = V2(-6.25f, -2.25f);
    real32 plusMinusOffset = 0.25f;
    real32 spriteModOffsetY = 0.5f;
    int32 rowNumber;


    //      CHANGING SPRITE POSITION.X
    rowNumber = 0;

    //          plus collission
    //              ==> POSITION.X SPECIFIC CODE
    vec2 spriteMinusPosition_edit1 = spriteExtra_edit1;
    spriteMinusPosition_edit1.x = spriteMinusPosition_edit1.x - plusMinusOffset;
    vec2 spritePlusPosition_edit1 = spriteExtra_edit1;
    spritePlusPosition_edit1.x = spritePlusPosition_edit1.x + plusMinusOffset;
  
 
    spritePlusPosition_edit1.y = spritePlusPosition_edit1.y - (rowNumber * spriteModOffsetY);
    bool overPlusPosition_edit1 = false;
    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusPosition_edit1, &dir))
    {
        overPlusPosition_edit1 = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {
            Data->levelEditor.arrowToObject.editEntityPos = true;
            Data->levelEditor.arrowToObject.posEdit.x = 1;
        }
    }
    //          minus collission
    bool overMinusPosition_edit1 = false;
    spriteMinusPosition_edit1.y = spriteMinusPosition_edit1.y - (rowNumber * spriteModOffsetY);
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusPosition_edit1, &dir))
    {
        overMinusPosition_edit1 = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {
            Data->levelEditor.arrowToObject.editEntityPos = true;
            Data->levelEditor.arrowToObject.posEdit.x = -1;
        }
    }
    //      CHANGING SPRITE POSITION.Y
    rowNumber = 1;
    //          plus collission
    //              ==> POSITION.Y SPECIFIC CODE
    vec2 spriteMinusPosition_edit2 = spriteExtra_edit1;
    spriteMinusPosition_edit2.x = spriteMinusPosition_edit2.x - plusMinusOffset;
    vec2 spritePlusPosition_edit2 = spriteExtra_edit1;
    spritePlusPosition_edit2.x = spritePlusPosition_edit2.x + plusMinusOffset;

    spritePlusPosition_edit2.y = spritePlusPosition_edit2.y - (rowNumber * spriteModOffsetY);
    bool overPlusPosition_edit2 = false;

    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusPosition_edit2, &dir))
    {
        overPlusPosition_edit2 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntityPos = true;
            Data->levelEditor.arrowToObject.posEdit.y = 1;
        }
    }
    //          minus collission
    bool overMinusPosition_edit2 = false;
    spriteMinusPosition_edit2.y = spriteMinusPosition_edit2.y - (rowNumber * spriteModOffsetY);
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusPosition_edit2, &dir))
    {
        overMinusPosition_edit2 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntityPos = true;
            Data->levelEditor.arrowToObject.posEdit.y = -1;
        }
    }

    //      CHANGING SPRITE SIZE.X
    rowNumber = 2;

    //          plus collission
    //              ==> SIZE.X SPECIFIC CODE
    vec2 spriteMinusSize_edit1 = spriteExtra_edit1;
    spriteMinusSize_edit1.x = spriteMinusSize_edit1.x - plusMinusOffset;
    vec2 spritePlusSize_edit1 = spriteExtra_edit1;
    spritePlusSize_edit1.x = spritePlusSize_edit1.x + plusMinusOffset;
    
    spritePlusSize_edit1.y = spritePlusSize_edit1.y - (rowNumber * spriteModOffsetY);
    bool overPlusSize_edit1 = false;
    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusSize_edit1, &dir))
    {
        overPlusSize_edit1 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntitySize = true;
            Data->levelEditor.arrowToObject.sizeEdit.x = 1;
        }
    }
    //          minus collission
    bool overMinusSize_edit1 = false;
    spriteMinusSize_edit1.y = spriteMinusSize_edit1.y - (rowNumber * spriteModOffsetY);
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusSize_edit1, &dir))
    {
        overMinusSize_edit1 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntitySize = true;
            Data->levelEditor.arrowToObject.sizeEdit.x = -1;
        }
    }
    //      CHANGING SPRITE SIZE.Y
    rowNumber = 3;
    //          plus collission
    //              ==> SIZE.Y SPECIFIC CODE
    vec2 spriteMinusSize_edit2 = spriteExtra_edit1;
    spriteMinusSize_edit2.x = spriteMinusSize_edit2.x - plusMinusOffset;
    vec2 spritePlusSize_edit2 = spriteExtra_edit1;
    spritePlusSize_edit2.x = spritePlusSize_edit2.x + plusMinusOffset;

    spritePlusSize_edit2.y = spritePlusSize_edit2.y - (rowNumber * spriteModOffsetY);
    bool overPlusSize_edit2 = false;

    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusSize_edit2, &dir))
    {
        overPlusSize_edit2 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntitySize = true;
            Data->levelEditor.arrowToObject.sizeEdit.y = 1;
        }
    }
    //          minus collission
    bool overMinusSize_edit2 = false;
    spriteMinusSize_edit2.y = spriteMinusSize_edit2.y - (rowNumber * spriteModOffsetY);
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusSize_edit2, &dir))
    {
        overMinusSize_edit2 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editEntitySize = true;
            Data->levelEditor.arrowToObject.sizeEdit.y = -1;
        }
    }

    //*******
    //*******
    //      RENDER ENTITY SPECIFICS EDITOR #2
    //*******
    //*******
    Rect checkBoxRect = spritePlusRect;
    vec2 columnSpriteEditLocationStart = V2(-5.0f, -4.0f);
    vec2 spriteCheckBoxPos  = columnSpriteEditLocationStart;
    vec2 spriteCheckBoxPos2 = columnSpriteEditLocationStart;
    vec2 spriteCheckBoxPos3 = columnSpriteEditLocationStart;
    int32 columnNumber = 0;
    real32 columnSpriteOffset = 1.4f;
    spriteCheckBoxPos.x = columnSpriteEditLocationStart.x + (columnNumber * columnSpriteOffset);
    spriteCheckBoxPos.y = spriteCheckBoxPos.y;

    //vec2 spriteMinusSize_edit2
    bool overCheckBox1 = false;
    bool overCheckBox2 = false;
    bool overCheckBox3 = false;


    // CHECKBOX LOGIC - ACTIVEROOM
    //****
    columnNumber = 0;

    if (RectTest(checkBoxRect, mouseRect, realMousePosition, spriteCheckBoxPos, &dir)) 
    {
        overCheckBox1 = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.activeRoom = !Data->levelEditor.arrowToObject.activeRoom;
            Data->levelEditor.arrowToObject.activeEdit = true;
        }
    }

    //  ROOM NUMBER CHANGE LOGIC
    //****
    vec2 spriteMinusRoomNumPos_edit2 = columnSpriteEditLocationStart;
    spriteMinusRoomNumPos_edit2.x = spriteMinusRoomNumPos_edit2.x - plusMinusOffset;
    vec2 spritePlusRoomNumPos_edit2 = columnSpriteEditLocationStart;
    spritePlusRoomNumPos_edit2.x = spritePlusRoomNumPos_edit2.x + plusMinusOffset;
    columnNumber = 1;

    //  minus collission
    spriteMinusRoomNumPos_edit2.x = spriteMinusRoomNumPos_edit2.x + (columnNumber * columnSpriteOffset);
    bool overMinusRoomNum = false;
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusRoomNumPos_edit2, &dir))
    {
        overMinusRoomNum = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editRoomNum = true;
            Data->levelEditor.arrowToObject.roomNumEditInt = -1;
        }
    }
    //  plus collission
    bool overPlusRoomNum = false;
    spritePlusRoomNumPos_edit2.x = spritePlusRoomNumPos_edit2.x + (columnNumber * columnSpriteOffset);
    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusRoomNumPos_edit2, &dir))
    {
        overPlusRoomNum = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->levelEditor.arrowToObject.editRoomNum = true;
            Data->levelEditor.arrowToObject.roomNumEditInt = 1;
        }
    }

    //  DOOR EDIT LOGIC
    // **********
    // **********
    //  DOOR FROM EDIT LOGIC


    bool overPlusFromDoor = false;
    bool overMinusFromDoor = false;
    bool overPlusToDoor = false;
    bool overMinusToDoor = false;
    bool overPlusDoorType = false;
    bool overMinusDoorType = false;
    bool overPlusToLevel = false;
    bool overMinusToLevel = false;
    bool overPlusSpriteNumber = false;
    bool overMinusSpriteNumber = false;

    vec2 spriteMinusFromDoorPos = columnSpriteEditLocationStart;
    vec2 spritePlusFromDoorPos = columnSpriteEditLocationStart;
    vec2 spriteMinusToDoorPos = columnSpriteEditLocationStart;
    vec2 spritePlusToDoorPos = columnSpriteEditLocationStart;
    vec2 spriteMinusDoorTypePos = columnSpriteEditLocationStart;
    vec2 spritePlusDoorTypePos = columnSpriteEditLocationStart;
    vec2 spriteMinusToLevelPos = columnSpriteEditLocationStart;
    vec2 spritePlusToLevelPos = columnSpriteEditLocationStart;
    vec2 spriteMinusSpriteNumberPos = columnSpriteEditLocationStart;
    vec2 spritePlusSpriteNumberPos = columnSpriteEditLocationStart;


    if (Data->levelEditor.editorType == EntityType_Door)
    {
        columnNumber = 2;

        spritePlusFromDoorPos.x = spritePlusFromDoorPos.x + (columnNumber * columnSpriteOffset);
        spritePlusFromDoorPos.x = spritePlusFromDoorPos.x + plusMinusOffset;
       
       // spriteMinusFromDoorPos.x = spritePlusFromDoorPos.x - plusMinusOffset;

        // DOOR FROM LOGIC PRESS 
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusFromDoorPos, &dir))
        {
            overPlusFromDoor = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorFromEditInt += 1;
                Data->levelEditor.arrowToObject.editDoorFrom = true;
            }
        }

        spriteMinusFromDoorPos.x = spriteMinusFromDoorPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spriteMinusFromDoorPos, &dir))
        {
            overMinusFromDoor = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorFromEditInt -= 1;
                Data->levelEditor.arrowToObject.editDoorFrom = true;
            }
        }


        columnNumber = 3;

        // DOOR TO LOGIC PRESS 
        spriteMinusToDoorPos.x = spriteMinusToDoorPos.x - plusMinusOffset;
        spritePlusToDoorPos.x = spritePlusToDoorPos.x + plusMinusOffset;
        spritePlusToDoorPos.x = spritePlusToDoorPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusToDoorPos, &dir))
        {
            overPlusToDoor = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorToEditInt += 1;
                Data->levelEditor.arrowToObject.editDoorTo = true;
            }
        }

        spriteMinusToDoorPos.x = spriteMinusToDoorPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spriteMinusToDoorPos, &dir))
        {
            overMinusToDoor = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorToEditInt -= 1;
                Data->levelEditor.arrowToObject.editDoorTo = true;
            }
        }
        //  DOOR TYPE EDIT LOGIC
       // spriteMinusDoorTypePos.x = spriteMinusDoorTypePos.x - plusMinusOffset;
        spritePlusDoorTypePos.x = spritePlusDoorTypePos.x + plusMinusOffset;
        columnNumber = 4;
        spritePlusDoorTypePos.x = spritePlusDoorTypePos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusDoorTypePos, &dir))
        {
            overPlusDoorType = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorTypeEditInt += 1;
                Data->levelEditor.arrowToObject.editDoorType = true;
            }
        }
        spriteMinusDoorTypePos.x = spriteMinusDoorTypePos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spriteMinusDoorTypePos, &dir))
        {
            //  DOOR TO EDIT LOGIC
            overMinusDoorType = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.doorTypeEditInt -= 1;
                Data->levelEditor.arrowToObject.editDoorType = true;
            }
        }

        // TO LEVEL EDIT LOGIC
        spriteMinusToLevelPos.x = spriteMinusToLevelPos.x - plusMinusOffset;
        spritePlusToLevelPos.x = spritePlusToLevelPos.x + plusMinusOffset;
        columnNumber = 5;
        spritePlusToLevelPos.x = spritePlusToLevelPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusToLevelPos, &dir))
        {
            overPlusToLevel = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.levelToEditInt += 1;
                Data->levelEditor.arrowToObject.editLevelTo = true;
            }
        }
        spriteMinusToLevelPos.x = spriteMinusToLevelPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusToLevelPos, &dir))
        {
            overMinusToLevel = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.levelToEditInt -= 1;
                Data->levelEditor.arrowToObject.editLevelTo = true;
            }
        }

        // LEVEL CHANGER EDIT LOGIC
        columnNumber = 7;
        spriteCheckBoxPos2.x = spriteCheckBoxPos2.x + (columnNumber * columnSpriteOffset);
        if (RectTest(checkBoxRect, mouseRect, realMousePosition, spriteCheckBoxPos2, &dir))
        {
            overCheckBox2 = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.levelChanger = !Data->levelEditor.arrowToObject.levelChanger;
                Data->levelEditor.arrowToObject.levelChangerEdit = true;
            }
        }
    }



    //          ENTITY OBJECT pallate EDITS
    
    if (Data->levelEditor.editorType == EntityType_Object)
    {
       
        //  SPRITE NUMBER EDIT LOGIC
        columnNumber = 2;
        spriteMinusSpriteNumberPos.x = spriteMinusSpriteNumberPos.x - plusMinusOffset;
        spritePlusSpriteNumberPos.x = spritePlusSpriteNumberPos.x + plusMinusOffset;
        spritePlusSpriteNumberPos.x = spritePlusSpriteNumberPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusSpriteNumberPos, &dir))
        {
            overPlusSpriteNumber = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.editSpriteNumberInt += 1;
                Data->levelEditor.arrowToObject.editSpriteNumber = true;
            }
        }
        spriteMinusSpriteNumberPos.x = spriteMinusSpriteNumberPos.x + (columnNumber * columnSpriteOffset);
        if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusSpriteNumberPos, &dir))
        {
            overMinusSpriteNumber = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.editSpriteNumberInt -= 1;
                Data->levelEditor.arrowToObject.editSpriteNumber = true;
            }
        }

        //      CHECK BOX CAN PICK UP LOGIC
        columnNumber = 3;
        spriteCheckBoxPos3.x = spriteCheckBoxPos3.x + (columnNumber * columnSpriteOffset);
        if (RectTest(checkBoxRect, mouseRect, realMousePosition, spriteCheckBoxPos3, &dir))
        {
            overCheckBox3 = true;
            if (InputPressed(Mouse, Input_MouseLeft))
            {
                Data->levelEditor.arrowToObject.canPickUpThisObject = !Data->levelEditor.arrowToObject.canPickUpThisObject;
                Data->levelEditor.arrowToObject.canPickUpThisObjectEdit = true;
            }
        }
    }
    
    if (Data->levelEditor.editorType == EntityType_Room)
    {

    }
    
    
    
   



 

    //      SAVE GAME LOGIC
    Rect saveButtonRect;
    vec2 saveButtonLocation = V2(4, 4);
    saveButtonRect.max =  V2(0.125f, 0.125f / 2);
    saveButtonRect.min = -V2(0.125f, 0.125f / 2);
    bool overSave = false;
    if (RectTest(saveButtonRect, mouseRect, realMousePosition, saveButtonLocation, &dir))
    {
        overSave = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {
            SaveAndWriteLevel();
        }
    }
    //      Testing Load Level
    //      Collision with Mouse +/-
    Rect spriteLevelPlusRect;
    Rect spriteLevelMinusRect;
    vec2 spriteLevelExtraPosition = V2(3, 4);
    real32 levelPlusMinusOffset = 0.25f;
    vec2 spriteLevelMinusPosition = spriteLevelExtraPosition;
    spriteLevelMinusPosition.x = spriteLevelMinusPosition.x - levelPlusMinusOffset;
    vec2 spriteLevelPlusPosition = spriteLevelExtraPosition;
    spriteLevelPlusPosition.x = spriteLevelPlusPosition.x + levelPlusMinusOffset;
    vec2 spriteLevelPlusMinusSize = V2(0.1f, 0.1f);
    spriteLevelPlusRect.max =   V2(spriteLevelPlusMinusSize.x / 2, spriteLevelPlusMinusSize.y / 2);
    spriteLevelPlusRect.min =  -V2(spriteLevelPlusMinusSize.x / 2, spriteLevelPlusMinusSize.y / 2);
    spriteLevelMinusRect.max =  V2(spriteLevelPlusMinusSize.x / 2, spriteLevelPlusMinusSize.y / 2);
    spriteLevelMinusRect.min = -V2(spriteLevelPlusMinusSize.x / 2, spriteLevelPlusMinusSize.y / 2);

    //      plus collission
  // mouseRect = spritePlusRect;
    bool overLevelPlus = false;
    if (RectTest(spriteLevelPlusRect, mouseRect, realMousePosition, spriteLevelPlusPosition, &dir))
    {
        overLevelPlus = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->currentLevel++;
            LoadLevelParse(Data->currentLevel, levelState);
        }
    }
    //      plus collission
   // mouseRect = spriteMinusRect;
    bool overLevelMinus = false;
    if (RectTest(spriteLevelMinusRect, mouseRect, realMousePosition, spriteLevelMinusPosition, &dir))
    {
        overLevelMinus = true;
        if (InputPressed(Mouse, Input_MouseLeft))
        {
            Data->currentLevel--;
            LoadLevelParse(Data->currentLevel, levelState);
        }
    }



    if (InputPressed(Mouse, Input_MouseRight))
    {
       // WriteLevel();
    }

    //**********************
    //**********************
    //      RENDER
    //**********************
    //**********************
    
    //      This sets the background color. 
    ClearColor(RGB(0.3f, 0.0f, 0.0f));

    //      Display Game Coordinates & Index
    if (showMouseOnMap)
    {
        DrawTextScreenPixel(&Game->monoFont, V2(530, 20), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X: %.3f", mousePos.x);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y: %.3f", mousePos.y);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 60), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X-Index %.0f of 128", mouseIndex.x);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 80), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y-Index %.0f of 72", mouseIndex.y);
    }
    else 
    {
        DrawTextScreenPixel(&Game->monoFont, V2(530, 20), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X: %.3f", 0.0f);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y: %.3f", 0.0f);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 60), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X-Index %.0f of 128", 0.0f);
        DrawTextScreenPixel(&Game->monoFont, V2(530, 80), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y-Index %.0f of 72",  0.0f);
    }
    

    //      DISPLAY CURRENT LEVEL
    DrawTextScreenPixel(&Game->monoFont, V2(750, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Level: %d", Data->currentLevel);
    
    //      RENDER LEVEL CHANGE BUTTONS
    RenderPlusMinus(spriteLevelPlusPosition, spriteLevelMinusPosition, spriteLevelPlusMinusSize, overLevelPlus, overLevelMinus);

    //      RENDER LEVEL MAP
    //          Render Map Canvas
    DrawSprite(levelMapOffset, levelMapdisplayRatio, 0, &Data->sprites.levelEditorBG1Sprite);
    
    //          Render GameEntities
    // 
    // 
    //              ROOM
    for (int i = 0; i < roomBuffer->count; i++) 
    {
        Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[i].handle);
        if (roomEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(roomEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(roomEntity->size, sizeOfLevelCanvas);

            if (!roomEntity->activeRoom)
            {
                DrawRectBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, RGB(1.0f, 0.5f, 0.5f));
            }
            else
            {
                DrawRectBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, RGB(1.0f, 0.8f, 0.8f));
            }
        }
    }
    //              MONSTER
    for (int i = 0; i < monsterBuffer->count; i++)
    {
        Monster* monsterEntity = (Monster*)GetEntity(&Data->em, monsterEntitiesInBuffer[i].handle);
        if (monsterEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(monsterEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(monsterEntity->size, sizeOfLevelCanvas);
            DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.monster1Sprite);
        }
    }
    //              BARRIER
    for (int i = 0; i < barrierBuffer->count; i++)
    {
        Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierEntitiesInBuffer[i].handle);
        if (barrierEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(barrierEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(barrierEntity->size, sizeOfLevelCanvas);
          //  DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.monster1Sprite);
        }
    }
    //              DOOR
    for (int i = 0; i < doorBuffer->count; i++)
    {
        Door* doorEntity = (Door*)GetEntity(&Data->em, doorEntitiesInBuffer[i].handle);
        if (doorEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(doorEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(doorEntity->size, sizeOfLevelCanvas);
            DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.doorClosed1Sprite);
        }
    }
    //              OBJECT
    for (int i = 0; i < objectBuffer->count; i++)
    {
        Object* objectEntity = (Object*)GetEntity(&Data->em, objectEntitiesInBuffer[i].handle);
        if (objectEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(objectEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(objectEntity->size, sizeOfLevelCanvas);
            DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.objectPalSprite);
        }
    }
    //              PLAYER
    for (int i = 0; i < playerBuffer->count; i++)
    {
        Player* playerEntity = (Player*)GetEntity(&Data->em, playerEntitiesInBuffer[i].handle);
        if (playerEntity != NULL)
        {
            vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(playerEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
            vec2 sizeForEntityInIndex = convertSizeToIndexSize(playerEntity->size, sizeOfLevelCanvas);
            DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.playerSprite);
        }
    }
    // *******************
    // *******************
    // *******************
    //                         RENDER SPRITE PALATTE #1
    // *******************
    // *******************
    // *******************
    DrawSprite(V2(-6.75f,1), V2(1,2.5f), 0, &Data->sprites.spritePalette);
    //          RENDER  NAME OF ACTIVE SPRITE
    //DrawTextScreenPixel(&Game->monoFont, V2(530, 20), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X: %.3f", mousePos.x);
    //          
    //          RENDER CYCLE THROUGH ENTITIES - PALATTE +/-
    RenderPlusMinus(cycleEntitiesPlusPosition, cycleEntitiesMinusPosition, cycleEntitiesPlusMinusSize, overCycleEntitiesPlus, overCycleEntitiesMinus);

    //          BASE ENTITY - PALATTE
    if (Data->levelEditor.editorType == EntityType_Room || overRoom)
    { 
        DrawSprite(roomSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.floor1Sprite);
    }
    else
    {
        DrawSprite(roomSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.wall1Sprite);
    }
    //          MONSTER ENTITY - PALATTE
    if (Data->levelEditor.editorType == EntityType_Monster || overMonsterPalatte)
    {
       DrawSprite(monsterSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.monster1Sprite_Sel);
    }
    else
    {
       DrawSprite(monsterSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.monster1Sprite);
    }
    //          DOOR ENTITY - PALATTE
    if (Data->levelEditor.editorType == EntityType_Door || overDoorPalatte)
    {
        DrawSprite(doorSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.doorClosed1PalSprite_Sel);
    }
    else
    {
        DrawSprite(doorSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.doorClosed1PalSprite);
    }
    //          OBJECT ENTITY - PALATTE
    if (Data->levelEditor.editorType == EntityType_Object || overObjectPalatte)
    {
        DrawSprite(objectSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.objectPalSprite_Sel);
    }
    else
    {
        DrawSprite(objectSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.objectPalSprite);
    }
    //          PLAYER ENTITY - PALATTE
    if (Data->levelEditor.editorType == EntityType_Player || overPlayerPalatte)
    {
        DrawSprite(playerSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.objectPalSprite_Sel);
    }
    else
    {
        DrawSprite(playerSpritePalattePosition, V2(0.25f, 0.25f), 0, &Data->sprites.playerSprite);
    }

    //       RENDER DELETE SPRITE
    if (overDeleteSprite)
    {
        DrawSprite(deleteButtonSpritePosition, V2(0.125f, 0.125f), 0, &Data->sprites.trashCanMouseSprite);
    }
    else
    {
        DrawSprite(deleteButtonSpritePosition, V2(0.125f, 0.125f), 0, &Data->sprites.trashCanSprite);
    }

    //******
    //      RENDER ENTITY SPECIFICS EDITOR #1
    //******
    DrawSprite(V2(-6.75f, -3), V2(1, 1.2f), 0, &Data->sprites.entitySpecificsSprite);
    
    //          RENDER POS EDIT.X
    vec2 posToWrite = Data->levelEditor.arrowToObject.indexPosition;
    DrawTextScreenPixel(&Game->monoFont, V2(30, 680), 8.0f, RGB(0.0f, 0.0f, 0.10f), "PositionX: %.0f", posToWrite.x);
    RenderPlusMinus(spritePlusPosition_edit1, spriteMinusPosition_edit1, spritePlusMinusSize, overPlusPosition_edit1, overMinusPosition_edit1);
    
    //          RENDER POS EDIT.Y
    DrawTextScreenPixel(&Game->monoFont, V2(30, 720), 8.0f, RGB(0.0f, 0.0f, 0.10f), "PositionY: %.0f", posToWrite.y);
    RenderPlusMinus(spritePlusPosition_edit2, spriteMinusPosition_edit2, spritePlusMinusSize, overPlusPosition_edit2, overMinusPosition_edit2);
    
    //          RENDER SIZE EDIT.X
    vec2 sizeToWrite = Data->levelEditor.arrowToObject.objectSizeIndex;
    DrawTextScreenPixel(&Game->monoFont, V2(30, 760), 8.0f, RGB(0.0f, 0.0f, 0.10f), "SizeX: %.0f", sizeToWrite.x);
    RenderPlusMinus(spritePlusSize_edit1, spriteMinusSize_edit1, spritePlusMinusSize, overPlusSize_edit1, overMinusSize_edit1);

    //          RENDER SIZE EDIT.Y
    DrawTextScreenPixel(&Game->monoFont, V2(30, 800), 8.0f, RGB(0.0f, 0.0f, 0.10f), "SizeY: %.0f", sizeToWrite.y);
    RenderPlusMinus(spritePlusSize_edit2, spriteMinusSize_edit2, spritePlusMinusSize, overPlusSize_edit2, overMinusSize_edit2);

    //******
    //******
    //      RENDER ENTITY SPECIFICS EDITOR #2
    //******
    //******

    DrawSpriteBottomLeft(V2(-5.5f, -4.4f), V2(13, 0.65f), 0, &Data->sprites.entitySpecificsSprite);
    //      RENDER CHECKBOX FOR ACTIVE ROOM

    DrawTextScreenPixel(&Game->monoFont, V2(258, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Active Room?", sizeToWrite.x);
    RenderCheckBox(Data->levelEditor.arrowToObject.activeRoom, spriteCheckBoxPos, spritePlusMinusSize, overCheckBox1);

     
    if (Data->levelEditor.editorType == EntityType_Door)
    {   
        //      RENDER PLUS/MINUS FOR DOOR-->FROM
        DrawTextScreenPixel(&Game->monoFont, V2(535, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "From Room: %d", Data->levelEditor.arrowToObject.doorFrom);
        RenderPlusMinus(spritePlusFromDoorPos, spriteMinusFromDoorPos, spritePlusMinusSize, overPlusFromDoor, overMinusFromDoor);

        //      RENDER PLUS/MINUS FOR DOOR-->TO
        DrawTextScreenPixel(&Game->monoFont, V2(680, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "To Room: %d", Data->levelEditor.arrowToObject.doorTo);
        RenderPlusMinus(spritePlusToDoorPos, spriteMinusToDoorPos, spritePlusMinusSize, overPlusToDoor, overMinusToDoor);

        //      RENDER DOOR TYPE PLUS/MINUS
        DrawTextScreenPixel(&Game->monoFont, V2(820, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Door Type: %d", Data->levelEditor.arrowToObject.doorType);
        RenderPlusMinus(spritePlusDoorTypePos, spriteMinusDoorTypePos, spritePlusMinusSize, overPlusDoorType, overMinusDoorType);
    
        //      RENDER PLUS/MINUS toLevel
        DrawTextScreenPixel(&Game->monoFont, V2(950, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "To Level: %d", Data->levelEditor.arrowToObject.levelTo);
        RenderPlusMinus(spritePlusToLevelPos, spriteMinusToLevelPos, spritePlusMinusSize, overPlusToLevel, overMinusToLevel);

        //      RENDER CHECKBOX FOR LEVEL CHANGER
        DrawTextScreenPixel(&Game->monoFont, V2(1200, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Level Changer?");
        RenderCheckBox(Data->levelEditor.arrowToObject.levelChanger, spriteCheckBoxPos2, spritePlusMinusSize, overCheckBox3);


    }
    else
    {
        //      RENDER PLUS MINUS FOR ROOM NUM

        DrawTextScreenPixel(&Game->monoFont, V2(400, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Room Num: %d", Data->levelEditor.arrowToObject.entityRoomNum);
        RenderPlusMinus(spritePlusRoomNumPos_edit2, spriteMinusRoomNumPos_edit2, spritePlusMinusSize, overPlusRoomNum, overMinusRoomNum);

    }

    if (Data->levelEditor.editorType == EntityType_Object)
    {
        //      RENDER PLUS/MINUS FOR spriteNumber
        DrawTextScreenPixel(&Game->monoFont, V2(535, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Sprite Number: %d", Data->levelEditor.arrowToObject.spriteNumber);
        RenderPlusMinus(spritePlusSpriteNumberPos, spriteMinusSpriteNumberPos, spritePlusMinusSize, overPlusSpriteNumber, overMinusSpriteNumber);

        //      RENDER CHECKBOX FOR CAN PICK UP OBJECT
        DrawTextScreenPixel(&Game->monoFont, V2(680, 880), 8.0f, RGB(0.0f, 0.0f, 0.10f), "Can Pick Up Object?");
        RenderCheckBox(Data->levelEditor.arrowToObject.canPickUpThisObject, spriteCheckBoxPos3, spritePlusMinusSize, overCheckBox3);

    }


    
    //      RENDER SAVE BUTTON
    if (overSave)
    {
        DrawSprite(saveButtonLocation, V2(0.25f, 0.125f), 0, &Data->sprites.saveButtonLevelEditor_mouse);
    }
    else
    {
        DrawSprite(saveButtonLocation, V2(0.25f, 0.125f), 0, &Data->sprites.saveSpriteLevelEditor);
    }


    
    //      RENDER ARROW POINTING TO CURRENT OBEJCT
    if (Data->levelEditor.editorState == EditingWithSprite && Data->levelEditor.arrowToObject.showArrow) 
    {
        DrawSprite(Data->levelEditor.arrowToObject.position, V2(0.125f, 0.125f), 0, &Data->sprites.arrowToObjectSprite);
    }
    
    //      RENDER MOUSE
    if (showMouseOnMap)
    {
        if (updateMouseRender)
        {
            mousePosRender = Input->mousePosNormSigned;        
            mousePosRender.x = mousePosRender.x * (8);
            mousePosRender.y = mousePosRender.y * (4.5f);
        }

        //      MOUSE WITHIN MAP
        if (Data->levelEditor.editorState == EditingWithSprite)
        {
            //RenderMouseWithinMapWithSprite(Data->levelEditor.editorType, mousePosRender);
            switch (Data->levelEditor.editorType)
            {
                case EntityType_Room:
                {
                    DrawSprite(mousePosRender, V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.floor1Sprite);
                    break;
                }
                case EntityType_Monster:
                {
                    DrawSprite(mousePosRender, V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.monster1Sprite);
                    break;
                }
                case EntityType_Door:
                {
                    DrawSprite(mousePosRender, V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.doorClosed1PalSprite);
                    break;
                }
                case EntityType_Object:
                {
                    DrawSprite(mousePosRender, V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.objectPalSprite);
                    break;
                }
                case EntityType_Player:
                {
                    DrawSprite(mousePosRender, V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.playerSprite);
                    break;
                }
                default:
                {
                    break;
                }
            }
        }
        else
        {
            DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.xSprite);
        }
    } 
    else
    {
        //      MOUSE OUTSIDE MAP
        mousePosRender = Input->mousePosNormSigned;
        mousePosRender.x = mousePosRender.x * (8);
        mousePosRender.y = mousePosRender.y * (4.5f);
        DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f, 0.125f), 0, &Data->sprites.cursorHandSprite);
    }

    //DrawSpriteBottomLeft(startingPosForLevelCanvasBottomLeft, V2(2, 2), 0, &Data->sprites.monster1Sprite);
    //DrawRectBottomLeft(V2(-levelMapdisplayRatio.x + 1, -levelMapdisplayRatio.y), V2(levelMapdisplayRatio.x *2, levelMapdisplayRatio.y * 2),0,  RGB(1.0f, 0.3f, 0.3f));
}
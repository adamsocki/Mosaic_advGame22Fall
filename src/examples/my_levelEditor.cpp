
MemoryArena arena = {};
MemoryArena roomArena = {};
MemoryArena monsterArena = {};
MemoryArena doorArena = {};
MemoryArena objectArena = {};

MemoryArena editorObjectsArena = {};

#include "gameCode/structs.cpp"
#include "gameCode/convertTools.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"

#include "gameCode/levelEditor.cpp"
#include "gameCode/levelParser.cpp"

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
    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;
    Barrier* barrierEntitiesInBuffer = (Barrier*)barrierBuffer->entities;
    Door* doorEntitiesInBuffer = (Door*)doorBuffer->entities;
    Object* objectEntitiesInBuffer = (Object*)objectBuffer->entities;

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


    //      SPRITE EDITING LOGIC
    if (Data->levelEditor.editorState == EditingWithSprite)
    {
        if (showMouseOnMap)
        {
            bool canPlaceObject = true;

            //switch()


            if (Data->levelEditor.editorType == EntityType_Door) {
                // TODO: MAKE SURE THAT IT WILL BE PLACED AT THE EDGE OF A WALL
                
                canPlaceObject = false;

                // TODO: if (object is in right place) { canPlaceObject = true}
            }
           
            if (InputPressed(Mouse, Input_MouseLeft && canPlaceObject))
            {
                // TODO start here
                StartObjectCreation(Data->levelEditor.editorType, mouseIndex, Data->currentLevel);
            }

            while (Data->levelEditor.levelObjects[Data->levelEditor.count].isSizable)
            {
                ObjectSizer(mouseIndex);
            }

        }
    }

    //      ARROW POINTING TO CURRENT OBEJCT
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
                
                if (Data->levelEditor.arrowToObject.editEntityPos)
                {
                    monsterEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    monsterEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {
                    monsterEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    monsterEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
            }
            else
            {
                // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
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

                if (Data->levelEditor.arrowToObject.editEntityPos)
                {
                    roomEntity->position1.x += Data->levelEditor.arrowToObject.posEdit.x;
                    roomEntity->position1.y += Data->levelEditor.arrowToObject.posEdit.y;
                    Data->levelEditor.arrowToObject.posEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntityPos = false;
                }
                if (Data->levelEditor.arrowToObject.editEntitySize)
                {
                    roomEntity->size.x += Data->levelEditor.arrowToObject.sizeEdit.x;
                    roomEntity->size.y += Data->levelEditor.arrowToObject.sizeEdit.y;
                    Data->levelEditor.arrowToObject.sizeEdit = V2(0, 0);
                    Data->levelEditor.arrowToObject.editEntitySize = false;
                }
            }
            else
            {
                // DON'T SHOW THE ARROW IF THERE ARE NO ENTITIES
                Data->levelEditor.arrowToObject.showArrow = false;
            }
            break;
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



    //      SAVE GAME LOGIC
    bool overSave = false;
    Rect saveButtonRect;
    vec2 saveButtonLocation = V2(4, 4);
    saveButtonRect.max =  V2(0.125f, 0.125f / 2);
    saveButtonRect.min = -V2(0.125f, 0.125f / 2);
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
    if (overLevelMinus)
    {
        DrawSprite(spriteLevelMinusPosition, spriteLevelPlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteLevelMinusPosition, spriteLevelPlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overLevelPlus)
    {
        DrawSprite(spriteLevelPlusPosition, spriteLevelPlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spriteLevelPlusPosition, spriteLevelPlusMinusSize, 0, &Data->sprites.plusSprite);
    }


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
            if (!roomEntity->activeRoom)
            {
                vec2 startPositionForEntityInIndex = IndexForLevelCanvasObjectStartingPosition(roomEntity->position1, sizeOfLevelCanvas, startingPosForLevelCanvasBottomLeft);
                vec2 sizeForEntityInIndex = convertSizeToIndexSize(roomEntity->size, sizeOfLevelCanvas);
                DrawRectBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, RGB(1.0f, 0.3f, 0.3f));
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
            DrawSpriteBottomLeft(startPositionForEntityInIndex, sizeForEntityInIndex, 0, &Data->sprites.doorClosed1Sprite);
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
    if (overCycleEntitiesMinus)
    {
        DrawSprite(cycleEntitiesMinusPosition, cycleEntitiesPlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(cycleEntitiesMinusPosition, cycleEntitiesPlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overCycleEntitiesPlus)
    {
        DrawSprite(cycleEntitiesPlusPosition, cycleEntitiesPlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(cycleEntitiesPlusPosition, cycleEntitiesPlusMinusSize, 0, &Data->sprites.plusSprite);
    }
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

    //******
    //      RENDER ENTITY SPECIFICS EDITOR #1
    //******
    DrawSprite(V2(-6.75f, -3), V2(1, 1.2f), 0, &Data->sprites.entitySpecificsSprite);
    
    //          RENDER POS EDIT.X
    vec2 posToWrite = Data->levelEditor.arrowToObject.indexPosition;
    DrawTextScreenPixel(&Game->monoFont, V2(30, 680), 8.0f, RGB(0.0f, 0.0f, 0.10f), "PositionX: %.0f", posToWrite.x);
    if (overMinusPosition_edit1)
    {
        DrawSprite(spriteMinusPosition_edit1, spritePlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteMinusPosition_edit1, spritePlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overPlusPosition_edit1)
    {
        DrawSprite(spritePlusPosition_edit1, spritePlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spritePlusPosition_edit1, spritePlusMinusSize, 0, &Data->sprites.plusSprite);
    }
    //          RENDER POS EDIT.Y
    DrawTextScreenPixel(&Game->monoFont, V2(30, 720), 8.0f, RGB(0.0f, 0.0f, 0.10f), "PositionY: %.0f", posToWrite.y);
    if (overMinusPosition_edit2)
    {
        DrawSprite(spriteMinusPosition_edit2, spritePlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteMinusPosition_edit2, spritePlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overPlusPosition_edit2)
    {
        DrawSprite(spritePlusPosition_edit2, spritePlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spritePlusPosition_edit2, spritePlusMinusSize, 0, &Data->sprites.plusSprite);
    }
    //          RENDER SIZE EDIT.X
    vec2 sizeToWrite = Data->levelEditor.arrowToObject.objectSizeIndex;
    DrawTextScreenPixel(&Game->monoFont, V2(30, 760), 8.0f, RGB(0.0f, 0.0f, 0.10f), "SizeX: %.0f", sizeToWrite.x);
    if (overMinusSize_edit1)
    {
        DrawSprite(spriteMinusSize_edit1, spritePlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteMinusSize_edit1, spritePlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overPlusSize_edit1)
    {
        DrawSprite(spritePlusSize_edit1, spritePlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spritePlusSize_edit1, spritePlusMinusSize, 0, &Data->sprites.plusSprite);
    }

    //          RENDER SIZE EDIT.Y
    DrawTextScreenPixel(&Game->monoFont, V2(30, 800), 8.0f, RGB(0.0f, 0.0f, 0.10f), "SizeY: %.0f", sizeToWrite.y);
    if (overMinusSize_edit2)
    {
        DrawSprite(spriteMinusSize_edit2, spritePlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteMinusSize_edit2, spritePlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overPlusSize_edit2)
    {
        DrawSprite(spritePlusSize_edit2, spritePlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spritePlusSize_edit2, spritePlusMinusSize, 0, &Data->sprites.plusSprite);
    }

    //******
    //      RENDER ENTITY SPECIFICS EDITOR #2
    //******
    DrawSpriteBottomLeft(V2(-5.5f, -4.4f), V2(13, 0.65f), 0, &Data->sprites.entitySpecificsSprite);













    
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

            switch (Data->levelEditor.editorType)
            {
                case EntityType_Room:
                {
                    DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.floor1Sprite);
                    break;
                }
                case EntityType_Monster:
                {
                    DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.monster1Sprite);
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
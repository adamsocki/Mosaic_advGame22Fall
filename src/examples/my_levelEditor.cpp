
MemoryArena arena = {};
MemoryArena roomArena = {};
MemoryArena monsterArena = {};

#include "gameCode/structs.cpp"
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

void MyInit() 
{
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;

    AllocateMemoryArena(&arena, Megabytes(64));
    AllocateMemoryArena(&roomArena, Megabytes(2));
    AllocateMemoryArena(&monsterArena, Megabytes(2));
    
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

    FileHandle file;
    if (OpenFileForRead("data/file_reading_example.txt", &file, &Game->frameMem))
    {

    }
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

    //      CREATE ENTITY BUFFER REFERENCES
    //          BUFFER
    EntityTypeBuffer* roomBuffer = &Data->em.buffers[EntityType_Room];
    EntityTypeBuffer* monsterBuffer = &Data->em.buffers[EntityType_Monster];
    //          ENTITIES
    Room* roomEntitiesInBuffer = (Room*)roomBuffer->entities;
    Monster* monsterEntitiesInBuffer = (Monster*)monsterBuffer->entities;


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
        // TODO: make a new mosue variable ??? 
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
    
    //      SPRITE PALATE LOGIC
    //      Collission with Mouse Obj 1
    bool overBase = false;
    Rect spritePalellteSelectionRect;
    spritePalellteSelectionRect.max =  V2(0.125f, 0.125f);
    spritePalellteSelectionRect.min = -V2(0.125f, 0.125f);
    mouseRect = spritePalellteSelectionRect;
    realMousePosition.y += 0.125f;
    vec2 dir = V2(0);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, V2(-7.25f, 2.75f), &dir))
    {
        overBase = true;
        if (InputPressed(Mouse, Input_MouseLeft)) {
            Data->levelEditor.editorType = EntityType_Base;
        }
    }

    //      SPRITE SPECIFICS LOGIC
    //      Collision with Mouse +/-
    Rect spritePlusRect;
    Rect spriteMinusRect;
    vec2 spriteExtraPosition = V2(-6.275f, -2.5f);
    real32 plusMinusOffset = 0.25f;
    vec2 spriteMinusPosition = spriteExtraPosition; 
    spriteMinusPosition.x = spriteMinusPosition.x - plusMinusOffset;
    vec2 spritePlusPosition = spriteExtraPosition;
    spritePlusPosition.x = spritePlusPosition.x + plusMinusOffset;
    vec2 spritePlusMinusSize = V2(0.1f, 0.1f);
    spritePlusRect.max =   V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spritePlusRect.min =  -V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spriteMinusRect.max =  V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);
    spriteMinusRect.min = -V2(spritePlusMinusSize.x / 2, spritePlusMinusSize.y / 2);

    //      plus collission
   // mouseRect = spritePlusRect;
    bool overPlus = false;
    if (RectTest(spritePlusRect, mouseRect, realMousePosition, spritePlusPosition, &dir))
    {
        overPlus = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {

        }
    }
    //      plus collission
   // mouseRect = spriteMinusRect;
    bool overMinus = false;
    if (RectTest(spriteMinusRect, mouseRect, realMousePosition, spriteMinusPosition, &dir))
    {
        overMinus = true;
        if (InputPressed(Mouse, Input_MouseLeft)) 
        {
            
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
            SaveLevel();
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
            LoadLevelParse(Data->currentLevel++, levelState);
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
            LoadLevelParse(Data->currentLevel--, levelState);
           // LevelTransition(levelState, false);
        }
    }

    //      LEVEL MAP LOGIC
    vec2 levelMapdisplayRatio = V2(8 / factorValue + (tileSize.x / 2), 4.5f / factorValue + (tileSize.y / 2));
    vec2 sizeOfLevelCanvas = V2(levelMapdisplayRatio.x * 2, levelMapdisplayRatio.y * 2);
    vec2 startingPosForLevelCanvasBottomLeft = V2(-levelMapdisplayRatio.x + 1, -levelMapdisplayRatio.y);

    
    //**********************
    //**********************
    //      RENDER
    //**********************
    //**********************
    
    //      This sets the background color. 
    ClearColor(RGB(0.3f, 0.0f, 0.0f));

    //      Display Game Coordinates & Index
    DrawTextScreenPixel(&Game->monoFont, V2(530, 20), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X: %.3f", mousePos.x);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y: %.3f", mousePos.y);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 60), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X-Index %.0f of 128", mouseIndex.x);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 80), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y-Index %.0f of 72", mouseIndex.y);

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
    //              ROOM
    for (int i = 0; i < roomBuffer->count; i++) 
    {
        Room* roomEntity = (Room*)GetEntity(&Data->em, roomEntitiesInBuffer[i].handle);
        if (roomEntity != NULL)
        {
            if (roomEntity->activeRoom)
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


    //      RENDER SPRITE PALATTE
    DrawSprite(V2(-6.75f,1), V2(1,2.5f), 0, &Data->sprites.spritePalette);
    if (Data->levelEditor.editorType == EntityType_Base || overBase)
    { 
        DrawSprite(V2(-7.25f, 2.75f), V2(0.25f, 0.25f), 0, &Data->sprites.floor1Sprite);
    }
    else
    {
        DrawSprite(V2(-7.25f, 2.75f), V2(0.25f, 0.25f), 0, &Data->sprites.wall1Sprite);
    }

    //      RENDER ENTITY SPECIFICS
    DrawSprite(V2(-6.75f, -3), V2(1, 1.2f), 0, &Data->sprites.entitySpecificsSprite);
    //      for loop across entity specifics, have plus / minus buttons to change.

    DrawTextScreenPixel(&Game->monoFont, V2(30, 680), 10.0f, RGB(0.0f, 0.0f, 0.10f), "Room Number:", mouseIndex.x);
    if (overMinus)
    {
        DrawSprite(spriteMinusPosition, spritePlusMinusSize, 0, &Data->sprites.minusSprite_mouse);
    }
    else
    {
        DrawSprite(spriteMinusPosition, spritePlusMinusSize, 0, &Data->sprites.minusSprite);
    }
    if (overPlus)
    {
        DrawSprite(spritePlusPosition, spritePlusMinusSize, 0, &Data->sprites.plusSprite_mosue);
    }
    else
    {
        DrawSprite(spritePlusPosition, spritePlusMinusSize, 0, &Data->sprites.plusSprite);
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
        if (Data->levelEditor.editorType == EntityType_Base)
        {
            DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.floor1Sprite);
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


#include "gameCode/structs.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"

#include "gameCode/levelEditor.cpp"

//TODO: properly set up entity Manager and all input files

//MyData* Data = NULL;

vec2 mousePosRender;

void MyInit() 
{
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;
    
    InitializeEntityManager();
    InitializeEntityBuffers(false);

    InitializeMouse();

    InitializeLevelEditor();

    LoadSprites();
   
    //GameMemory* gameMem

    Camera* cam = &Game->camera;
    //cam->size = 1.5f;
    // cam->type = CameraType_Orthographic;
    cam->width = 16;
     cam->height = 9;
   cam->projection = Orthographic(cam->width * -0.5f * cam->size, cam->width * 0.5f * cam->size,
      cam->height * -0.5f * cam->size, cam->height * 0.5f * cam->size,
       0.0, 100.0f);

    // Game->camAngle = 0;
    //Game->cameraPosition = V3(1,0,0);
    //Game->cameraRotation = AxisAngle(V3(0, 100, 30), Game->camAngle);

    UpdateCamera(cam, Game->cameraPosition, Game->cameraRotation);


    // read file

    FileHandle file;
    if (OpenFileForRead("data/file_reading_example.txt", &file, &Game->frameMem))
    {

    }
}

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

    //      CAMERA DATA
    Camera* cam = &Game->camera;
        
    //      GET MOUSE POSITION
    vec2 mousePos = Input->mousePosNormSigned;
    mousePos.x = mousePos.x * (8 * factorValue) - (levelMapOffset.x * factorValue) + 8;
    mousePos.y = mousePos.y * (4.5f * factorValue) - (levelMapOffset.y * factorValue) + 4.5f;

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
    Rect mouseRect = spritePalellteSelectionRect;
    realMousePosition.y += 0.125f ;
    vec2 dir = V2(0);
    if (RectTest(spritePalellteSelectionRect, mouseRect, realMousePosition, V2(-7.25f, 2.75f), &dir))
    {
        overBase = true;
        if (InputPressed(Mouse, Input_MouseLeft)) {
            Data->levelEditor.editorType = EntityType_Base;
        }
    }





    //**********************
    //  RENDER
    //**********************

    //      This sets the background color. 
    ClearColor(RGB(0.3f, 0.0f, 0.0f));

    //      Display Game Coordinates & Index
    DrawTextScreenPixel(&Game->monoFont, V2(530, 20), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X: %.3f", mousePos.x);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y: %.3f", mousePos.y);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 60), 10.0f, RGB(1.0f, 1.0f, 1.0f), "X-Index %.0f of 128", mouseIndex.x);
    DrawTextScreenPixel(&Game->monoFont, V2(530, 80), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Y-Index %.0f of 72", mouseIndex.y);

    //      DISPLAY CURRENT LEVEL
    DrawTextScreenPixel(&Game->monoFont, V2(750, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Level:");

    //      RENDER LEVEL MAP
    DrawSprite(levelMapOffset, V2(8 / factorValue + (tileSize.x / 2), 4.5f / factorValue + (tileSize.y / 2)), 0, &Data->sprites.levelEditorBG1Sprite);


    //      RENDER SPRITE PALATE
     DrawSprite(V2(-6.75f,1), V2(1,2.5f), 0, &Data->sprites.spritePalette);
    if (Data->levelEditor.editorType == EntityType_Base || overBase)
    {
       
        DrawSprite(V2(-7.25f, 2.75f), V2(0.25f, 0.25f), 0, &Data->sprites.floor1Sprite);
    }
    else
    {
        DrawSprite(V2(-7.25f, 2.75f), V2(0.25f, 0.25f), 0, &Data->sprites.wall1Sprite);

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
    
}
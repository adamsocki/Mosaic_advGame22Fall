

#include "gameCode/structs.cpp"
#include "gameCode/entityManager.cpp"
#include "gameCode/loadSprites.cpp"
#include "gameCode/LevelManager.cpp"

#include "gameCode/input.cpp"
#include "gameCode/collision.cpp"

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

    // DrawSprite(V2(0), V2(4, 4), 0, sprite2);
   // while (ShowCursor(false) >= 0);

    real32 factorValue = 1.25f;
    vec2 levelMapOffset = V2(1, 0);
    bool updateMouseRender = false;

    //      CAMERA DATA
    Camera* cam = &Game->camera;
    //cam->size = 1.25f;

    //      GET MOUSE POSITION
    vec2 mousePos = Input->mousePosNormSigned;
    //mousePos.x = mousePos.x - Game->cameraPosition.x;
    //mousePos.y = mousePos.y - Game->cameraPosition.y;
   // Data->mouseCrosshair.position = mousePos;
    mousePos.x = mousePos.x * (8 * factorValue) - (levelMapOffset.x * factorValue) + 8;
    mousePos.y = mousePos.y * (4.5f * factorValue) - (levelMapOffset.y * factorValue) + 4.5f;

    //      DETECT MOUSE WITHIN LEVEL
   
    bool showMouse = false;

    if (mousePos.x >= 0 && mousePos.x <= 16 &&
        mousePos.y >= 0 && mousePos.y <=  9)
    {
        showMouse = true;
    }
    else
    {   
        // TODO: make a new mosue variable ??? 
        mousePos = V2(0, 0);
        showMouse = false;
    }

   // vec2 displayMouse = V2(SnapUp(mousePos.x, 0.125f), SnapUp(mousePos.y, 0.125f));
    

   mousePos.x = SnapUp(mousePos.x, 0.125f);
    
   mousePos.y = SnapUp(mousePos.y, 0.125f);
   if (Data->mouseCrosshair.previousPosition != mousePos) {
       updateMouseRender = true;
   }


   Data->mouseCrosshair.previousPosition = mousePos;
   
   /* if (mousePos.x < 16)
    {
        mousePos.x = 16;
    }*/

    // calculate mouseIndex
    vec2 mouseIndex;
    if (showMouse)
    {
        mouseIndex = V2((mousePos.x / 16) * (16 / 0.125f), (mousePos.y / 9) * (9 / 0.125f));
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


    //      Desplay Current Level
    DrawTextScreenPixel(&Game->monoFont, V2(640, 40), 10.0f, RGB(1.0f, 1.0f, 1.0f), "Level:");

    // 1.25 factor
    DrawSprite(levelMapOffset, V2(8 / factorValue + (0.125f / 2), 4.5f / factorValue + (0.125f / 2)), 0, &Data->sprites.levelEditorBG1Sprite);
    //DrawSprite(V2(0), V2(4, 4), 0, &Data->sprite2);



   
    //      RENDER MOUSE
    if (showMouse)
    {
       /* vec2 mousePos = Input->mousePosNormSigned;
        if (mousePos.x > 16)
        {
            mousePos.x = 0;
        }
        if (mousePos.y > 9)
        {
            mousePos.x = 0;
        }*/

        if (updateMouseRender)
        {
            mousePosRender = Input->mousePosNormSigned;
            
            mousePosRender.x = mousePosRender.x * (8);
            mousePosRender.y = mousePosRender.y * (4.5f);
        }

        //mousePos.x = SnapUp(mousePos.x, 0.125f / 8) * 8 - (0.125f / 8);
            //mousePos.y = SnapUp(mousePos.y, 0.125f / 4.5f) * 4.5f;
        
        //if (mousePos.x >= 0 && mousePos.x <= 16 &&
        //    mousePos.y >= 0 && mousePos.y <= 9)
        //{
        //   // showMouse = true;
        //}
        //else
        //{
        //    // TODO: make a new mosue variable ??? 
        //   // mousePos = V2(0, 0);
        //    //showMouse = false;
        //}
        //mousePos.x = mousePos.x * (8 * factorValue) - (levelMapOffset.x * factorValue) + 8;
        ////mousePos.y = mousePos.y * (4.5f * factorValue) - (levelMapOffset.y * factorValue) + 4.5f;*/
        DrawSprite(V2(mousePosRender.x, mousePosRender.y), V2(0.125f / 2, 0.125f / 2), 0, &Data->sprites.cursorForLevelEditorSprite);

    }
    //DrawSprite(Data->mouseCrosshair.position, V2(0.3f, 0.3f), &Data->sprites.crosshairUnlocked1Sprite);
    
   
    
}
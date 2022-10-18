struct MyData {
    Sprite sprite;
    Sprite sprite2;

    SoundClip sound;
};

MyData* Data = NULL;
void MyInit() 
{
    Game->myData = malloc(sizeof(MyData));
    memset(Game->myData, 0, sizeof(MyData));

    Data = (MyData*)Game->myData;
    
    /// <summary>
    /// MODIFY CAMERA
    /// </summary>
    /// 
    LoadSprite(&Data->sprite, "data/levelEditorBG1.png");

    /// 
    LoadSprite(&Data->sprite2, "data/van_gogh.png");

    //GameMemory* gameMem

    Camera* cam = &Game->camera;
    cam->size = 1.25f;
   // cam->type = CameraType_Orthographic;
    //cam->width = 16;
   // cam->height = 9;
    cam->projection = Orthographic(cam->width * -0.5f * cam->size, cam->width * 0.5f * cam->size,
       cam->height * -0.5f * cam->size, cam->height * 0.5f * cam->size,
        0.0, 100.0f);

   // Game->camAngle = 0;
    Game->cameraPosition = V3(-1.5f, 0, 3);
    //Game->cameraRotation = AxisAngle(V3(0, 100, 30), Game->camAngle);

    UpdateCamera(cam, Game->cameraPosition, Game->cameraRotation);
}

void MyGameUpdate()
{
    
    //**********************
    //  LOGIC
    //**********************

    // DrawSprite(V2(0), V2(4, 4), 0, sprite2);



    //**********************
    //  RENDER
    //**********************

    //      This sets the background color. 


    ClearColor(RGB(0.3f, 0.0f, 0.0f));
    DrawSprite(V2(0), V2(8, 4.5f), 0, &Data->sprite);

    DrawSprite(V2(0), V2(4, 4), 0, &Data->sprite2);
    
}
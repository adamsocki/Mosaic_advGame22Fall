void InputPlayerController(Player* p) 
{
	p->previousPosition = p->position1;
	vec2 speedController = V2(0, 0);

	if (InputHeld(Keyboard, Input_UpArrow)) 
	{
		speedController.y = 1;
		if (InputHeld(Keyboard, Input_RightArrow)) 
		{
			speedController.x = 1;
		}
		if (InputHeld(Keyboard, Input_LeftArrow)) 
		{
			speedController.x = -1;
		}
	}
	if (InputHeld(Keyboard, Input_DownArrow)) 
	{
		speedController.y = -1;
		if (InputHeld(Keyboard, Input_RightArrow)) 
		{
			speedController.x = 1;
		}
		if (InputHeld(Keyboard, Input_LeftArrow)) 
		{
			speedController.x = -1;
		}
	}
	if (InputHeld(Keyboard, Input_RightArrow)) 
	{
		speedController.x = 1;
	}
	if (InputHeld(Keyboard, Input_LeftArrow)) 
	{
		speedController.x = -1;
	}
	p->position1.x += p->speed * speedController.x * Game->deltaTime * 4;
	p->position1.y += p->speed * speedController.y * Game->deltaTime * 4;
}



void InitializeMouse() 
{

	Data->mouseCrosshair.isLocked = false;
	Data->mouseCrosshair.playInitialSound = true;
	Data->mouseCrosshair.crosshairUnlockedSprite = &Data->sprites.crosshairUnlocked1Sprite;

	Data->mouseCrosshair.previousPosition = V2(0);

	//Data->mouseCrosshair.crosshairUnlockSprite = &Data->sprites.crosshair1Sprite;
}
 

void MouseInputClick(Barrier* barrierEntity, int32 barrierBufferCount, EntityTypeBuffer* barrierEntitiesInBuffer) {

	if (InputPressed(Mouse, Input_MouseLeft)) 
	{
		/*int32 doorNumberToOpen = barrierEntity->doorNumber;
		barrierEntity->isOpen = true;
		barrierEntity->sprite = &Data->sprites.blankSprite;
		for (int d = 0; d < barrierBufferCount; d++) {
			EntityHandle barrierHandle = barrierEntitiesInBuffer[d].handle;
			Barrier* barrierEntity = (Barrier*)GetEntity(&Data->em, barrierHandle);
			if (barrierEntity->doorNumber = doorNumberToOpen) {
				barrierEntity->isOpen = true;
				barrierEntity->&Data->sprites.blankSprite;
			}
		}*/
		/*Barrier* leftDoorEntity = (Barrier*)GetEntity(&Data->em, barrierEntity->handleForOpenDoorLeft);
		Barrier* rightDoorEntity = (Barrier*)GetEntity(&Data->em, barrierEntity->handleForOpenDoorRight);
		leftDoorEntity->isOpen = true;
		leftDoorEntity->sprite = &Data->sprites.blankSprite;
		rightDoorEntity->isOpen = true;
		rightDoorEntity->sprite = &Data->sprites.blankSprite;*/
	}
}
void InputPlayerController(Player* p) {

	p->previousPosition = p->position;

	vec2 speedController = V2(0, 0);

	if (InputHeld(Keyboard, Input_UpArrow)) {
		speedController.y = 1;
		if (InputHeld(Keyboard, Input_RightArrow)) {
			speedController.x = 1;
		}
		if (InputHeld(Keyboard, Input_LeftArrow)) {
			speedController.x = -1;
		}
	}
	if (InputHeld(Keyboard, Input_DownArrow)) {
		speedController.y = -1;
		if (InputHeld(Keyboard, Input_RightArrow)) {
			speedController.x = 1;
		}
		if (InputHeld(Keyboard, Input_LeftArrow)) {
			speedController.x = -1;
		}
	}
	if (InputHeld(Keyboard, Input_RightArrow)) {
		speedController.x = 1;
	}
	if (InputHeld(Keyboard, Input_LeftArrow)) {
		speedController.x = -1;
	}
	p->position.x += p->speed * speedController.x * Game->deltaTime;
	p->position.y += p->speed * speedController.y * Game->deltaTime;
}

 
void LoadSprites() {
	LoadSprite(&Data->sprites.playerSprite, "data/Player_Simple.png");
	LoadSprite(&Data->sprites.street_Asprite, "data/gameData/street_A.png");
	LoadSprite(&Data->sprites.street_Bsprite, "data/gameData/street_B.png");
//	LoadSprite(&Data->sprites.building_ASprite, "data/gameData/building_A.png");

	LoadSprite(&Data->sprites.suitcaseSprite, "data/suitcase.png");
	LoadSprite(&Data->sprites.wall1Sprite, "data/wall1.png");
	LoadSprite(&Data->sprites.wall1_vertSprite, "data/wall1_vert.png");
	LoadSprite(&Data->sprites.stairsDown1Sprite, "data/stairsDown1.png");
	LoadSprite(&Data->sprites.door1Sprite, "data/door1.png");

	LoadSprite(&Data->sprites.backgroundColor1Sprite, "data/backgroundColor1.png");
	LoadSprite(&Data->sprites.crosshairUnlocked1Sprite, "data/crosshairUnlocked1.png");


	LoadSprite(&Data->sprites.floor1Sprite, "data/floor1.png");
	LoadSprite(&Data->sprites.blankSprite, "data/blank.png");
	LoadSprite(&Data->sprites.doorClosed1Sprite, "data/doorClosed1.png");
	LoadSprite(&Data->sprites.doorClosed1hSprite, "data/doorClosed1h.png");
	LoadSprite(&Data->sprites.doorClosed1vSprite, "data/doorClosed1v.png");

}

void LoadSFX() {
	LoadSoundClip("data/sfx/crosshairSound1.wav", &Data->sound.crosshairSound1);
} 
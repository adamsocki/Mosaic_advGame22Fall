void LoadSprites() {
	LoadSprite(&Data->sprites.playerSprite, "data/gameData/player_simple.png");
	LoadSprite(&Data->sprites.street_Asprite, "data/gameData/street_A.png");
	LoadSprite(&Data->sprites.street_Bsprite, "data/gameData/street_B.png");
	LoadSprite(&Data->sprites.building_ASprite, "data/gameData/building_A.png");

	LoadSprite(&Data->sprites.suitcaseSprite, "data/suitcase.png");
	LoadSprite(&Data->sprites.wall1Sprite, "data/wall1.png");
	LoadSprite(&Data->sprites.door1Sprite, "data/door1.png");

	LoadSprite(&Data->sprites.backgroundColor1Sprite, "data/backgroundColor1.png");
	LoadSprite(&Data->sprites.crosshair1Sprite, "data/crosshair1.png");

}

void LoadSFX() {
	LoadSoundClip("data/sfx/crosshairSound1.wav", &Data->sound.crosshairSound1);
} 
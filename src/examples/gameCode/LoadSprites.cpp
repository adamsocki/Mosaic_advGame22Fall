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
	
	LoadSprite(&Data->sprites.monster1Sprite, "data/monster1Sprite.png");

	LoadSprite(&Data->sprites.floor1Sprite, "data/floor1.png");
	LoadSprite(&Data->sprites.blankSprite, "data/blank.png");
	LoadSprite(&Data->sprites.doorClosed1Sprite, "data/doorClosed1.png");
	LoadSprite(&Data->sprites.doorClosed1hSprite, "data/doorClosed1h.png");
	LoadSprite(&Data->sprites.doorClosed1vSprite, "data/doorClosed1v.png");


	// LEVEL EDITOR
	LoadSprite(&Data->sprites.levelEditorBG1Sprite, "data/levelEditorBG1.png");
	LoadSprite(&Data->sprites.cursorForLevelEditorSprite, "data/cursorLE1.png");
	LoadSprite(&Data->sprites.cursorHandSprite, "data/mouseHand.png");
	LoadSprite(&Data->sprites.spritePalette, "data/spritePalette.png");
	LoadSprite(&Data->sprites.xSprite, "data/xSprite.png");
	LoadSprite(&Data->sprites.saveSpriteLevelEditor, "data/saveSpriteLevelEditor.png");
	LoadSprite(&Data->sprites.saveButtonLevelEditor_mouse, "data/saveButtonLevelEditor_mouse.png");
	LoadSprite(&Data->sprites.minusSprite, "data/minusSprite.png");
	LoadSprite(&Data->sprites.plusSprite, "data/plusSprite.png");
	LoadSprite(&Data->sprites.minusSprite_mouse, "data/minusSprite_mouse.png");
	LoadSprite(&Data->sprites.plusSprite_mosue, "data/plusSprite_mosue.png");
	LoadSprite(&Data->sprites.entitySpecificsSprite, "data/entitySpecificsSprite.png");
	
	LoadSprite(&Data->sprites.monster1Sprite_Sel, "data/monster1Sprite_Sel.png");
	LoadSprite(&Data->sprites.arrowToObjectSprite, "data/arrowToObjectSprite.png");

	LoadSprite(&Data->sprites.noCheckYesMouseSprite, "data/noCheckYesMouseSprite.png");
	LoadSprite(&Data->sprites.yesCheckYesMouseSprite, "data/yesCheckYesMouseSprite.png");
	LoadSprite(&Data->sprites.noCheckNoMouseSprite, "data/noCheckNoMouseSprite.png");
	LoadSprite(&Data->sprites.yesCheckNoMouseSprite, "data/yesCheckNoMouseSprite.png");

	LoadSprite(&Data->sprites.doorClosed1PalSprite, "data/doorClosed1PalSprite.png");
	LoadSprite(&Data->sprites.doorClosed1PalSprite_Sel, "data/doorClosed1PalSprite_Sel.png");
	LoadSprite(&Data->sprites.objectPalSprite, "data/objectPalSprite.png");
	LoadSprite(&Data->sprites.objectPalSprite_Sel, "data/objectPalSprite_Sel.png");
		
	
	LoadSprite(&Data->sprites.trashCanSprite, "data/trashCanSprite.png");
	LoadSprite(&Data->sprites.trashCanMouseSprite, "data/trashCanMouseSprite.png");
	LoadSprite(&Data->sprites.playerSprite_Sel, "data/playerSprite_Sel.png");
	
	LoadSprite(&Data->sprites.barrierPalSprite_Sel, "data/barrierPalSprite_Sel.png");
	LoadSprite(&Data->sprites.barrierPalSprite, "data/barrierPalSprite.png");

	Sprite monster1Sprite;
	
}

void LoadSFX() {
	LoadSoundClip("data/sfx/crosshairSound1.wav", &Data->sound.crosshairSound1);
} 
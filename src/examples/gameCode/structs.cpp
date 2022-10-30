

enum TokenTypeForLevel
{
	TokenType_LeftParen,
	TokenType_RightParen,
	TokenType_AmpSymb,
	TokenType_PoundSymb,
	TokenType_Comma,

	TokenType_Integer,
	TokenType_PeriodSymb,
	TokenType_Identifier,
	TokenType_DollarSymb,
	TokenType_Count,


};

struct TokenVal
{
	TokenTypeForLevel type;
	char* start;
	int32 length;
};


enum EntityType {
	None,
	EntityType_Base,
	EntityType_Player,
	EntityType_Monster1,
	EntityType_Monster,

	EntityType_PlayerCarry,

	EntityType_Barrier,
	EntityType_Door,
	EntityType_RoomTrigger,
	EntityType_Room,

	EntityType_MouseCrosshair,

	EntityType_LevelPortal,

	EntityType_Count,
};

struct LevelEditor
{
	EntityType editorType;
};

struct FreeList {
	int32 freeList[1000];
	int32 freelistCount;

};



struct EntityHandle {
	int32 generation;
	int32 indexInInfo;
	EntityType type;
};


struct EntityInfo {
	int32 generation;
	int32 indexInBuffer;
	
	EntityType type;
};

struct EntityTypeBuffer {
	int32 count;
	int32 capacity;
	int32 sizeInBytes;

	void* entities;
};


struct EntityManager {
	EntityTypeBuffer buffers[EntityType_Count];
	EntityInfo *entities;
	int32 entityCapacity;
	
	FreeList levelIDtest[10];

	int32 nextID;
};


struct EntitySprites {
	Sprite playerSprite;
	Sprite street_Asprite;
	Sprite street_Bsprite;
	Sprite building_ASprite;
	Sprite suitcaseSprite;

	Sprite monster1Sprite;

	Sprite wall1Sprite;
	Sprite wall1_vertSprite;
	Sprite door1Sprite;

	Sprite stairsDown1Sprite;

	Sprite floor1Sprite;
	Sprite blankSprite;

	Sprite doorClosed1Sprite;
	Sprite doorClosed1hSprite;
	Sprite doorClosed1vSprite;
	Sprite crosshairUnlocked1Sprite;

	Sprite levelEditorBG1Sprite;
	Sprite cursorForLevelEditorSprite;
	Sprite cursorHandSprite;
	Sprite spritePalette;
	Sprite xSprite;

	Sprite saveSpriteLevelEditor;
	Sprite saveButtonLevelEditor_mouse;
	Sprite minusSprite;
	Sprite plusSprite;
	Sprite minusSprite_mouse;
	Sprite plusSprite_mosue;
	Sprite entitySpecificsSprite;

	Sprite backgroundColor1Sprite;

};

struct Sounds {
	SoundClip crosshairSound1;
};

struct MouseCrosshair {
	bool isLocked;
	vec2 position;
	bool playInitialSound;
	vec2 previousPosition;

	Sprite* crosshairUnlockedSprite;
	Sprite* crosshairLockSprite;
};

struct MyData {

	EntitySprites sprites;
	Sounds sound;

	MemoryArena tokenArena;

	MouseCrosshair mouseCrosshair;
	LevelEditor levelEditor;

	EntityManager em;
	int32 currentLevel;
};

struct Entity {
	vec2 position;
	Sprite* sprite;
	bool toDelete;
	vec2 size;
	bool isQuad;

	EntityHandle handle;
};



struct Base : Entity {
	int32 id;
	bool activeRoom;
	int32 roomNumber;
};

struct RoomTrigger : Entity {
	bool isTriggered;
	int32 roomToTrigger;
	int32 level;
	vec2 tileSize;

};

struct Player : Entity {
	real32 speed;
	vec2 previousPosition;
};

struct PlayerCarry : Entity {
	bool canBePickedUp;
	bool mouseOverPlayerCarry;
};

struct Barrier : Entity {
	bool isDoor;
	bool mouseIsOver;
	bool isOpen;
	bool isDoorCenter;
	EntityHandle handleForOpenDoorLeft;
	EntityHandle handleForOpenDoorRight;
	int32 doorNumber;
	int32 roomNumber;
	bool activeRoom;
};

struct Monster : Entity
{

	int32 monsterType;
	vec2 size;
	vec2 position1;

	int32 strength;
	real32 speed;
	vec2 previousPosition;
};


struct Room : Entity{
	vec2 size;
	vec2 startingPosition;
	vec2 tileSize;
	Sprite* sprite;
	int32 roomNumber;
	int32 levelNumber;
	vec2 position1;
	vec2 position2;

	bool activeRoom;
};


struct Road {
	vec2 startingPosition;
	vec2 size;
	bool horizontal;
};

struct LevelPortal : Entity {
	bool isLevelPortal;
	bool mouseIsOver;
};

struct Door : Entity {
	vec2 startingPosition;
	vec2 tileSize;
	vec2 doorSize;
	bool horizontal;
	int32 level;
	bool mouseIsOver;
	int32 length;
	bool isDoorCenter;
	bool isDoorOpen;
	int32 roomNumber;
	bool activeRoom;
	int32 count;
	int32 doorNumber;
	int32 doorCenterSeq;
	bool doorActivatesRoom;
	int32 roomDoorActivates;
	//DynamicArray<vec2> doorPositions;
};

struct Wall {

	int32 levelNumber;
	int32 length;
	bool horizontal;
	vec2 tileSize;
	vec2 startingPosition;
	int32 roomNumber;
	bool showSprite;
	bool activeRoom;
};


struct LevelState {
	Room room[12];
	Door door[10];
	Player player[3];
	bool roomActive[10];
	
	//Barrier barrier[
	Base base[2000];

};

struct DoorLevelData {
	vec2 position;

	int32 level;
	
};

struct WallLevelData {
	vec2 position;

	int32 level;
};




struct Level {
	DynamicArray<Door> doors;
	DynamicArray<WallLevelData> walls;

	int32 currentLevel;
};

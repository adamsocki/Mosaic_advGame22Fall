

enum EntityType {
	EntityType_Base,
	EntityType_Player,

	EntityType_PlayerCarry,

	EntityType_Barrier,
	EntityType_Door,

	EntityType_MouseCrosshair,

	EntityType_LevelPortal,

	EntityType_Count,
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
	int32 nextID;
};


struct EntitySprites {
	Sprite playerSprite;
	Sprite street_Asprite;
	Sprite street_Bsprite;
	Sprite building_ASprite;
	Sprite suitcaseSprite;

	Sprite wall1Sprite;
	Sprite wall1_vertSprite;
	Sprite door1Sprite;

	Sprite stairsDown1Sprite;

	Sprite floor1Sprite;
	Sprite blankSprite;

	Sprite doorClosed1Sprite;
	Sprite crosshairUnlocked1Sprite;


	Sprite backgroundColor1Sprite;
};

struct Sounds {
	SoundClip crosshairSound1;
};

struct MouseCrosshair {
	bool isLocked;
	vec2 position;
	bool playInitialSound;

	Sprite* crosshairUnlockedSprite;
	Sprite* crosshairLockSprite;
};

struct MyData {

	EntitySprites sprites;
	Sounds sound;

	MouseCrosshair mouseCrosshair;

	EntityManager em;

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
};


struct Level {
	
	//DynamicArray<Room> rooms;
};

struct Room {
	vec2 size;
	vec2 startingPosition;
	vec2 tileSize;
	Sprite* sprite;
	int32 roomNumber;
	int32 levelNumber;
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

struct Door :Entity {
	vec2 startingPosition;
	vec2 tileSize;
	vec2 doorSize;
	bool horizontal;
	int32 level;
	bool mouseIsOver;
	int32 length;
	bool isDoorCenter;
	bool isDoorOpen;
	int32 count;
	int32 doorNumber;
	int32 doorCenterSeq;
	//DynamicArray<vec2> doorPositions;
};

struct Wall {

	int32 levelNumber;
	int32 length;
	bool horizontal;
	vec2 tileSize;
	vec2 startingPosition;
	int32 roomNumber;
};

enum EntityType {
	EntityType_Base,
	EntityType_Player,

	EntityType_PlayerCarry,
	
	EntityType_Barrier,


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
	Sprite door1Sprite;

	Sprite crosshair1Sprite;


	Sprite backgroundColor1Sprite;
};

struct Sounds {
	SoundClip crosshairSound1;
};

struct MyData {

	EntitySprites sprites;
	Sounds sound;

	EntityManager em;

};

struct Entity {
	vec2 position;
	Sprite* sprite;
	bool toDelete;
	vec2 size;

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
	bool soundPlayed;
};

struct Barrier : Entity {
	bool isDoor;
};


struct Level {

};


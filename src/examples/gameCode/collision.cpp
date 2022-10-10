

Rect playerCollisionRect(int32 expansionFactor, Player* p) {
	Rect playerRect;


	playerRect.max = p->size * expansionFactor;
	playerRect.min = -p->size * expansionFactor;

	return playerRect;
}
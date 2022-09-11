#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"


#pragma region GameObject 
void GameObject::UpdateBoundingBox() {
	boundingBox.a = position;
	boundingBox.b = Vector2(boundingBox.a.x + scale.x, boundingBox.a.y);
	boundingBox.c = Vector2(boundingBox.a.x, boundingBox.a.y + scale.y);
	boundingBox.d = Vector2(boundingBox.b.x, boundingBox.c.y);
}
void GameObject::Scale(int x, int y) {
	scale.x += x;
	scale.y += y;
	UpdateBoundingBox();
}
void GameObject::SetScale(int x, int y) {
	scale.x = x;
	scale.y = y;
	UpdateBoundingBox();
}
void GameObject::Move(int x, int y) {
	position.x += x;
	position.y += y;
	UpdateBoundingBox();
}
void GameObject::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
	UpdateBoundingBox();
}
bool GameObject::IsColliding(GameObject otherObject) {
	return false;
}

#pragma endregion

#pragma region Block
bool Block::Hurt() {
	health--;
	currentSprite = brokenSprite;
	if (health <= 0)
	{
		return true;
	}
	return false;
}
BlockAsset::BlockAsset() {
	id = 1;
	health = 0;
}

#pragma endregion

void Bullet::Tick() {
}
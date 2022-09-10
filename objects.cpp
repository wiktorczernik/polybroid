#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"


#pragma region GameObject 
void GameObject::UpdateBoundingBox() {
	boundingBox.a = position;
	boundingBox.b = Vector2(position.x + scale.x, position.y);
	boundingBox.c = Vector2(position.x, position.y + scale.y);
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
void Block::Hurt() {

}
BlockAsset::BlockAsset() {
	id = -1;
	health = 0;
}

#pragma endregion


#pragma region PhysicsObject
void PhysicsObject::Tick() {

}
#pragma endregion
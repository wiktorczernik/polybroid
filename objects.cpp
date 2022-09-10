#include "Objects.h"
#include "Framework.h"
#include "PolyMath.h"


#pragma region GameObject 

GameObject::GameObject() {

}
GameObject::GameObject(Sprite* IdleSprite) {
	idleSprite = IdleSprite;
	currentSprite = idleSprite;
}
void GameObject::Scale(int x, int y) {
	scale.x += x;
	scale.y += y;
}
void GameObject::SetScale(int x, int y) {
	scale.x = x;
	scale.y = y;
}
void GameObject::Move(int x, int y) {
	position.x += x;
	position.y += y;
}
void GameObject::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
}
bool GameObject::IsColliding(GameObject otherObject) {
	return false;
}

#pragma endregion

#pragma region Block

Block::Block(int Id, int Health, Sprite* IdleSprite, Sprite* BrokenSprite) {
	id = Id;
	health = Health;
	idleSprite = IdleSprite;
	brokenSprite = BrokenSprite;
	currentSprite = idleSprite;
}
void Block::Hurt() {

}
BlockAsset::BlockAsset() {
	id = 1;
	health = 1;
}

#pragma endregion


#pragma region PhysicsObject
void PhysicsObject::Tick() {

}
#pragma endregion
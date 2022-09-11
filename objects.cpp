#include "Objects.h"
#include "PolyMath.h"

void Object::Scale(int x, int y) {
	scale.x += x;
	scale.y += y;
}
void Object::SetScale(int x, int y) {
	scale.x = x;
	scale.y = y;
}
void Object::Move(int x, int y) {
	position.x += x;
	position.y += y;
}
void Object::SetPosition(int x, int y) {
	position.x = x;
	position.y = y;
}
#pragma region GameObject 
void GameObject::UpdateBoundingBox() {
	boundingBox.a = position;
	boundingBox.b = Vector2(boundingBox.a.x + scale.x, boundingBox.a.y);
	boundingBox.c = Vector2(boundingBox.a.x, boundingBox.a.y + scale.y);
	boundingBox.d = Vector2(boundingBox.b.x, boundingBox.c.y);
}
void GameObject::Scale(int x, int y) {
	Object::Scale(x, y);
	UpdateBoundingBox();
}
void GameObject::SetScale(int x, int y) {
	Object::SetScale(x, y);
	UpdateBoundingBox();
}
void GameObject::Move(int x, int y) {
	Object::Move(x, y);
	UpdateBoundingBox();
}
void GameObject::SetPosition(int x, int y) {
	Object::SetPosition(x, y);
	UpdateBoundingBox();
}
bool GameObject::IsColliding(GameObject otherObject) {
	return false;
}
bool GameObject::Hurt() {
	health--;
	if (health == 0)
	{
		return true;
	}
	return false;
}

#pragma endregion

#pragma region Block
bool Block::Hurt() {
	currentSprite = brokenSprite;
	bool result = GameObject::Hurt();
	return result;
}
#pragma endregion

void PhysicsObject::Tick() {
	Move(currentVelocity.x, currentVelocity.y);
	bool border[2];
	if (CollidesBorder(border)) {
		InvertVelocity(border[0], border[1]);
	}
}
void Bullet::Tick(list<GameObject>& objects) {
	PhysicsObject::Tick();
	for (GameObject& object : objects) {
		if (CollidesWith(object)) {
			/*static_cast<Block>(object)
			if (dynamic_cast<Block&>(object) != nullptr)*/
			InvertVelocity(true, false);
			Move(currentVelocity.x, currentVelocity.y);
		}
		if (CollidesWith(object)) {
			InvertVelocity(false, true);
		}
	}
}
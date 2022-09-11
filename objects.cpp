#include "Objects.h"
#include "PolyMath.h"


Timer::Timer() {
	currentTime = 0;
	triggerTime = 10000;
}
void Timer::Setup(bool* Trigger, unsigned int TriggerTime, bool AutoRestart) {
	trigger = Trigger;
	*trigger = false;
	triggerTime = TriggerTime;
	autoRestart = AutoRestart;
	currentTime = 0;
}

void Timer::Tick(int deltaTime) {
	if (autoRestart && *trigger == true) {
		currentTime -= triggerTime;
		*trigger = false;
	}
	currentTime += deltaTime;
	if (currentTime >= triggerTime) {
		*trigger = true;
		if (!autoRestart) {
			currentTime = triggerTime;
		}
	}
}
void Timer::Restart() {
	*trigger = false;
	currentTime = 0;
}
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
		IsAlive = false;
		return true;
	}
	IsAlive = true;
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

void Bullet::Tick(list<Block>& blocks) {
	Move(0, currentVelocity.y);
	for (Block& object : blocks) {
		if (CollidesWith(object)) {
			InvertVelocity(false, true);
			Move(0, currentVelocity.y);
			object.Hurt();
		}
	}
	Move(currentVelocity.x, 0);
	for (Block& object : blocks) {
		if (CollidesWith(object)) {
			InvertVelocity(true, false);
			Move(currentVelocity.x, 0);
			object.Hurt();
		}
	}
	bool border[2];
	if (CollidesBorder(border)) {
		InvertVelocity(border[0], border[1]);
	}
}

void Ability::Tick() {

	/*if (CollidesWith(player)) {
		IsAlive = false;
	}*/
	cout << "Ability ticks" << '\n';

	bool border[2];
	if (CollidesBorder(border)) {
		if (border[1] == true && position.y > canvas.MaxY()/2) {
			cout << "Ability is dead!" << '\n';
			IsAlive = false;
		}
		InvertVelocity(border[0], border[1]);
	}
	Move(currentVelocity.x, currentVelocity.y);
}
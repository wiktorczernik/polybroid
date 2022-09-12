#include "Objects.h"
#include "PolyMath.h"


Timer::Timer() {
	currentTime = 0;
	triggerTime = 10000;
}
void Timer::Setup(bool* Trigger, unsigned int TriggerTime, bool AutoRestart) {
	trigger = Trigger;
	triggered = false;
	*trigger = false;
	triggerTime = TriggerTime;
	autoRestart = AutoRestart;
	currentTime = 0;
}

void Timer::Tick(int deltaTime) {
	if (autoRestart && *trigger == true) {
		currentTime -= triggerTime;
		*trigger = false;
		triggered = false;
	}
	currentTime += deltaTime;
	if (currentTime >= triggerTime) {
		*trigger = true;
		triggered = true;
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

void Bullet::Tick(list<Block>& blocks, Player& player) {
	bool bl = false;
	bool pl = false;

	Move(0, currentVelocity.y);
	for (Block& object : blocks) {
		pl = CollidesWith(player);
		bl = CollidesWith(object);
		if (bl || pl) {
			InvertVelocity(false, true);
			Move(0, currentVelocity.y);
			if (bl && !pl) {
				object.Hurt();
			}
		}
	}
	Move(currentVelocity.x, 0);
	for (Block& object : blocks) {
		pl = CollidesWith(player);
		bl = CollidesWith(object);
		if (bl || pl) {
			InvertVelocity(true, false);
			Move(currentVelocity.x, 0);
			if (bl && !pl) {
				object.Hurt();
			}
		}
	}
	bool border[2];
	if (CollidesBorder(border)) {
		if (border[1] == true && position.y > canvas.MaxY() / 2) {
			//IsAlive = false;
		}
		InvertVelocity(border[0], border[1]);
	}
}

void Player::Tick() {
	currentVelocity = Vector2(initVelocity.x * moveInput, 0);
	Move(currentVelocity.x, currentVelocity.y);
	bool border[2];
	if (CollidesBorder(border)) {
		if (border[0] == true) {
			SetPosition((position.x >= canvas.MaxX()/2) ? canvas.MaxX() - scale.x : canvas.MinX(), position.y);
		}
	}
}

void Player::AddAbility(bool IsPositive) {
	switch (IsPositive)
	{
	case true:
		break;
	case false:
		break;
	}
}

void Ability::Tick(Player& player) {

	/*if (CollidesWith(player)) {
		IsAlive = false;
	}*/

	bool border[2];
	if (CollidesWith(player)) {
		IsAlive = false;
	}
	if (CollidesBorder(border)) {
		if (border[1] == true && position.y > canvas.MaxY()/2) {
			IsAlive = false;
		}
		InvertVelocity(border[0], border[1]);
	}
	Move(currentVelocity.x, currentVelocity.y);
}
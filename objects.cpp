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
		bl = CollidesWith(object);
		if (bl) {
			InvertVelocity(false, true);
			Move(0, currentVelocity.y);
			object.Hurt();
		}
	}
	Move(currentVelocity.x, 0);
	for (Block& object : blocks) {
		bl = CollidesWith(object);
		if (bl) {
			InvertVelocity(true, false);
			Move(currentVelocity.x, 0);
			object.Hurt();
		}
	}
	pl = CollidesWith(player);
	if (pl) {
		Vector2 finalMultiply = Vector2(2, 2);
		if (boundingBox.MaxY() < player.boundingBox.MinY() + player.scale.y / 2) {
			InvertVelocity(false, true);
			finalMultiply.y * 3;
		}
		int max = std::abs(player.scale.x) / 2;

		int bc = position.x + (scale.x / 2);
		int pc = player.position.x + (player.scale.x / 2);

		int diff = std::abs(bc - pc);

		double ratio = std::clamp((double)diff / max, 0.0, 1.0);


		int newX = 3 * ratio;
		int newY = std::abs(4 - newX);

		newX *= (bc < pc ? -1 : 1);
		newY *= currentVelocity.y != 0 ? std::clamp((currentVelocity.y / std::abs(currentVelocity.y)), -1, 1) : -1;


		newY = std::clamp(newY, -3, 3);

		currentVelocity.x = newX;
		currentVelocity.y = newY;

		int speed = std::abs(currentVelocity.x * currentVelocity.y);

		if (speed != 0 && speed != 3) {
			cout << currentVelocity.x << " + " << currentVelocity.y << " = " << speed << "\n";
			cout << speed << "\n";
		}
		

		Move(currentVelocity.x*finalMultiply.x, currentVelocity.y*finalMultiply.y);
		if (CollidesWith(player)) {
			int by = position.y + (scale.y / 2);
			int py = player.position.y + (player.scale.y / 2);
			int a = by < py ? 1 : -1;
			Move(0, scale.y/ 2 * a);
		}
	}
	bool border[2];
	if (CollidesBorder(border)) {
		if (border[1] == true && position.y > canvas.MaxY() / 2) {
			IsAlive = false;
		}
		InvertVelocity(border[0], border[1]);
		Move(currentVelocity.x, currentVelocity.y);
		if (CollidesWith(player)) {
			if (currentVelocity.y > 0) {
				InvertVelocity(false, true);
			}
			Move(0, -scale.y);
		}
	}
}

void Player::Tick(int deltaTime) {

	std::list<Timer>::iterator bi = timers.begin();

	list<Timer>::const_iterator bitr = timers.cbegin();
	while (bitr != timers.cend())
	{
		list<Timer>::const_iterator curr = bitr++;
		if (curr->triggered == true) {
			timers.erase(curr);
		}
	}
	for (auto& timer : timers) {
		timer.Tick(deltaTime);
	}
	if (decreasePositiveAB) {
		abilityMultiplier = std::clamp(abilityMultiplier - 1, -3, 3);
		decreasePositiveAB = false;
	}
	if (decreaseNegativeAB) {
		abilityMultiplier = std::clamp(abilityMultiplier + 1, -3, 3);
		decreaseNegativeAB = false;
	}

	int xSpeed = initVelocity.x;
	if (abilityMultiplier != 0) {
		float mp = (abilityMultiplier > 0 ? 1.4 : 0.6);
		for (int i = 0; i < std::abs(abilityMultiplier); i++) {
			xSpeed *= mp;
		}
	}
	int minSpeed = initVelocity.x / 2;
	int maxSpeed = initVelocity.x * 2;
	xSpeed = xSpeed != 0 ? ((std::clamp(xSpeed, minSpeed, maxSpeed))*moveInput) : 0;

	currentVelocity = Vector2(xSpeed, 0);

	Move(currentVelocity.x, currentVelocity.y);
	bool border[2];
	if (CollidesBorder(border)) {
		if (border[0] == true) {
			SetPosition((position.x >= canvas.MaxX()/2) ? canvas.MaxX() - scale.x : canvas.MinX(), position.y);
		}
	}
}

void Player::AddAbility(bool IsPositive) {
	Timer newTimer = Timer();
	bool* trigger;
	switch (IsPositive)
	{
	case true:
		abilityMultiplier = std::clamp(abilityMultiplier + 1, -3, 3);
		trigger = &decreasePositiveAB;
		break;
	case false:
		abilityMultiplier = std::clamp(abilityMultiplier - 1, -3, 3);
		trigger = &decreaseNegativeAB;
		break;
	}
	newTimer.Setup(trigger, 10000, true);
	timers.push_front(newTimer);
}

void Ability::Tick(Player& player) {

	/*if (CollidesWith(player)) {
		IsAlive = false;
	}*/

	bool border[2];
	if (CollidesWith(player)) {
		IsAlive = false;
		player.AddAbility(isPositive);
	}
	if (CollidesBorder(border)) {
		if (border[1] == true && position.y > canvas.MaxY()/2) {
			IsAlive = false;
		}
		InvertVelocity(border[0], border[1]);
	}
	Move(currentVelocity.x, currentVelocity.y);
}
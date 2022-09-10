#include "Core.h"

#pragma region GameObject 


GameObject::GameObject(Sprite* spr) {
	currentSprite = spr;
}
GameObject::GameObject() {

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

#pragma region PhysicsObject
void PhysicsObject::Tick() {

}
#pragma endregion

#pragma region Scene
Scene::Scene() {

}
void Scene::Draw() {
	/*for each (GameObject var in objects)
	{

	}*/
}
void Scene::PhysTick() {
	for (PhysicsObject physObj : physObjects)
	{
		physObj.Tick();
	}
}
#pragma endregion
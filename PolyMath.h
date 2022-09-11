#pragma once
#include <iostream>
struct Vector2 {
	int x;
	int y;
	Vector2() {
		x = 0;
		y = 0;
	}
	Vector2(int X, int Y) {
		x = X;
		y = Y;
	}
};
struct BoundingBox {
	Vector2 a;
	Vector2 b;
	Vector2 c;
	Vector2 d;
	BoundingBox() {
		a = Vector2(0, 0);
		b = Vector2(1, 0);
		c = Vector2(0, 1);
		d = Vector2(1, 1);
	}
	BoundingBox(Vector2 A, Vector2 B, Vector2 C, Vector2 D) {
		a = A;
		b = B;
		c = C;
		d = D;
	}
	int MinX() {
		return std::min(a.x, c.x);
	}
	int MaxX() {
		return std::max(b.x, d.x);
	}
	int MinY() {
		return std::min(a.y, b.y);
	}
	int MaxY() {
		return std::max(c.y, d.y);
	}
	bool Intersects(BoundingBox bounds) {
		std::cout << MinX() << " <= " << bounds.MaxX() << " = " << (MinX() <= bounds.MaxX()) << '\n';
		std::cout << MaxX() << " >= " << bounds.MinX() << " = " << (MaxX() >= bounds.MinX()) << '\n';
		std::cout << MinY() << " <= " << bounds.MaxY() << " = " << (MinY() <= bounds.MaxY()) << '\n';
		std::cout << MaxY() << " >= " << bounds.MinY() << " = " << (MaxY() >= bounds.MinY()) << "\n\n\n";
		return (
			MinX() <= bounds.MaxX() &&
			MaxX() >= bounds.MinX() &&
			MinY() <= bounds.MaxY() &&
			MaxY() >= bounds.MinY()
			);
	}
};
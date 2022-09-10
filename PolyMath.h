#pragma once
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
};
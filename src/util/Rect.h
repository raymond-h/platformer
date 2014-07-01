/*
 * Rect.h
 *
 *  Created on: 13 jun 2012
 *      Author: Raymond
 */

#include <SDL2/SDL.h>

template <typename T, typename U>
class Rect;

/*
 * A Rect with signed longs for x and y, and unsigned longs for width and height. Fit for exact rectangle measurements
 */
typedef Rect<signed long, unsigned long> LongRect;

/*
 * A Rect with floats for x and y, and unsigned longs for width and height. Fit for ingame objects like Entities.
 */
typedef Rect<float, unsigned long> FloatRect;

#ifndef RECT_H_
#define RECT_H_

#include "Vector.h"

template <typename T, typename U = T>
class Rect {
	public:
		Rect();
		Rect(T, T, U, U);
		template <typename V, typename W> Rect(const Rect<V,W>&);
		~Rect();

		void setPosition(T, T);
		void setSize(U, U);

		T getX() const { return x; };
		T getY() const { return y; };

		U getWidth() const { return w; };
		U getHeight() const { return h; };

		T getLeft() const { return x; };
		T getTop() const { return y; };

		T getRight() const { return x + w; };
		T getBottom() const { return y + h; };

		void setLeft(T pos) { x = pos; };
		void setTop(T pos) { y = pos; };

		void setRight(T pos) { x = pos - w; };
		void setBottom(T pos) { y = pos - h; };

		Vector getTopLeft() const { return Vector(getLeft(), getTop()); };
		void setTopLeft(Vector c) { x = c.x(); y = c.y(); };

		Vector getCenterPos() const { return Vector(getCenterX(), getCenterY()); };
		void setCenterPos(Vector c) { x = c.x() - w/2; y = c.y() - h/2; };

		T getCenterX() const { return x + w/2; };
		T getCenterY() const { return y + h/2; };

		void setCenterX(T pos) { x = pos - w/2; };
		void setCenterY(T pos) { y = pos - h/2; };

		SDL_Rect toSDLRect() {
			SDL_Rect result;
			result.x = short(x);
			result.y = short(y);
			result.w = short(w);
			result.h = short(h);
			return result;
		}

		Rect<T,U> operator+(const Vector& p) {
			return Rect<T,U>(x+p.x, y+p.y, w, h);
		}

		/*
		 * Returns true if this overlaps passed Rect partly or completely
		 */
		template <typename V, typename W> bool overlaps(const Rect<V,W>&);

		/*
		 * Returns true if this overlaps passed Rect completely
		 */
		template <typename V, typename W> bool contains(const Rect<V,W>&);

		/*
		 * Returns true if passed point is inside of this
		 */
		bool contains(const Vector&);

		T x, y;
		U w, h;
};

template <typename T, typename U>
Rect<T,U>::Rect():x(0),y(0),w(0),h(0) {}

template <typename T, typename U>
Rect<T,U>::Rect(T sx, T sy, U sw, U sh):x(sx),y(sy),w(sw),h(sh) {}

template <typename T, typename U> template <typename V, typename W>
Rect<T,U>::Rect(const Rect<V,W>& rhs):x(rhs.x),y(rhs.y),w(rhs.w),h(rhs.h) {}

template <typename T, typename U>
Rect<T,U>::~Rect() {}

template <typename T, typename U>
void Rect<T,U>::setPosition(T sx, T sy) {
	x = sx;
	y = sy;
}

template <typename T, typename U>
void Rect<T,U>::setSize(U sw, U sh) {
	w = sw;
	h = sh;
}

template <typename T, typename U> template <typename V, typename W>
bool Rect<T,U>::overlaps(const Rect<V,W>& rect) {
	return !(getLeft() > rect.getRight() ||
			getRight() < rect.getLeft() ||
			getTop() > rect.getBottom() ||
			getBottom() < rect.getTop());
}

template <typename T, typename U> template <typename V, typename W>
bool Rect<T,U>::contains(const Rect<V,W>& rect) {
	return (getTop() < rect.getBottom() &&
			getLeft() < rect.getRight() &&
			getBottom() >= rect.getTop() &&
			getRight() >= rect.getLeft());
}

template <typename T, typename U>
bool Rect<T,U>::contains(const Vector& pos) {
	return (getTop() <= pos.y() &&
			getLeft() <= pos.x() &&
			getBottom() >= pos.y() &&
			getRight() >= pos.x());
}

#endif /* RECT_H_ */

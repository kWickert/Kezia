#pragma once

#include "Matrix4.h"

#include <stack>

namespace Kezia
{
	template<typename T>
	class Vector3;

	template<typename T>
	class MatrixStack
	{
	public:
		MatrixStack()
		{
			_Stack.push(Matrix4<T>());
		}
		~MatrixStack()
		{}

		void Clear();

		Matrix4<T> GetCurrentMatrix() const;

		void LoadMatrix(const Matrix4<T> & mat);

		void PopMatrix();

		void PushCurrentMatrix();

		void MultMatrix(const Matrix4<T> & mat);

		void Scale(const T x = 1, const T y = 1, const T z = 1);
		void Rotate(const T angleRadians, const Vector3<T> & axis);
		void Translate(T x = 0, T y = 0, T z = 0);
		void Translate(const Vector3<F32> & position = Vector3<F32>());

		void LookAt(const Vector3<T> & eye, const Vector3<T> & up, const Vector3<T> & target);
	private:
		std::stack< Matrix4<T> > _Stack;
	};

	template<typename T>
	void MatrixStack<T>::Clear()
	{
		while(_Stack.empty() == false)
			_Stack.pop();

		_Stack.push(Matrix4<T>());
	}

	template<typename T>
	Matrix4<T> MatrixStack<T>::GetCurrentMatrix() const
	{
		return _Stack.top();
	}

		template<typename T>
	void MatrixStack<T>::LoadMatrix(const Matrix4<T> & mat)
	{
		_Stack.top() = mat;
	}

	template<typename T>
	void MatrixStack<T>::PopMatrix()
	{
		_Stack.pop();

		if(_Stack.size() == 0)
			_Stack.push(Matrix4<T>());
	}

	template<typename T>
	void MatrixStack<T>::PushCurrentMatrix()
	{
		_Stack.push(_Stack.top());
	}

	template<typename T>
	void MatrixStack<T>::MultMatrix(const Matrix4<T> & mat)
	{
		_Stack.top() *= mat;
	}

	template<typename T>
	void MatrixStack<T>::Scale(const T x, const T y, const T z)
	{
		_Stack.top() *= Matrix4<F32>::Scale(x, y, z);
	}

	template<typename T>
	void MatrixStack<T>::Rotate(const T angleRadians, const Vector3<T> & axis)
	{
		_Stack.top() *= Kezia::Rotate(angleRadians, axis);
	}

	template<typename T>
	void MatrixStack<T>::Translate(const T x, const T y, const T z)
	{
		_Stack.top() *= Kezia::Translate(x, y, z);
	}

	template<typename T>
	void MatrixStack<T>::Translate(const Vector3<F32> & position)
	{
		_Stack.top() *= Matrix4<F32>::Translate(position);
	}

	template<typename T>
	void MatrixStack<T>::LookAt(const Vector3<T> & eye, const Vector3<T> & up, const Vector3<T> & target)
	{
		_Stack.top() *= Kezia::LookAt(eye, up, target);
	}
}
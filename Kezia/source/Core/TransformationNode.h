#pragma once

#include "Math/MatrixStack.h"
#include "Math/Matrix3.h"
#include "Math/Quaternion.h"
#include "Math/Vector3.h"

#include <vector>

namespace Kezia
{
	class AttachableComponent;

	class TransformationNode
	{
	public:
		TransformationNode();
		TransformationNode(const TransformationNode & other);
		~TransformationNode();

		void AddChild(TransformationNode * node);
		TransformationNode * GetChildNode(const std::string & name);

		TransformationNode * FullCopy() const;

		inline const Vector3<F32> & GetPosition() const;
		inline void SetPosition(const Vector3<F32> & position);

		inline const Matrix3<F32> & GetOrientation() const;
		inline void SetOrientation(const Quaternion<F32> & orientation);
		inline void SetOrientation(const Matrix3<F32> & orientation);
		inline void Rotate(const F32 yawRadians = 0, const F32 pitchRadians = 0, const F32 rollRadians = 0);

		inline const F32 GetXScale() const;
		inline const F32 GetYScale() const;
		inline const F32 GetZScale() const;
		void SetUniformScale(const F32 scale);
		void SetNonUniformScale(const F32 xScale, const F32 yScale, const F32 zScale);

		inline const Vector3<F32> GetWorldPosition() const;
		inline const Matrix3<F32> GetWorldOrientation() const;
		inline const Matrix4<F32> GetWorldTransformNoScale() const;
		inline const Matrix4<F32> GetWorldTransform() const;

		inline const std::string & GetNodeName() const;
		void SetNodeName(const std::string & name);

		typedef std::vector<AttachableComponent *>::const_iterator ConstComponentIterator;
		inline ConstComponentIterator ComponentBegin() const;
		inline ConstComponentIterator ComponentEnd() const;

		friend class AttachableComponent;
	private:
		void NotifyAttach(AttachableComponent * component);
		void NotifyDetach(AttachableComponent * component);

		void UpdateWorldTransform(MatrixStack<F32> & matrixStack);

		std::string m_Name;

		std::vector<TransformationNode *> m_ChildNodes;
		std::vector<AttachableComponent *> m_AttachedComponents;

		Matrix4<F32> m_WorldTransform;

		Vector3<F32> m_Position;
		Matrix3<F32> m_Orientation;
		Vector3<F32> m_Scale;
		Vector3<F32> m_InverseScale;
	};

	inline const Vector3<F32> & TransformationNode::GetPosition() const
	{
		return m_Position;
	}

	inline void TransformationNode::SetPosition(const Vector3<F32> & position)
	{
		m_Position = position;
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline const Matrix3<F32> & TransformationNode::GetOrientation() const
	{
		return m_Orientation;
	}

	inline void TransformationNode::SetOrientation(const Quaternion<F32> & orientation)
	{
		m_Orientation = orientation;
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline void TransformationNode::SetOrientation(const Matrix3<F32> & orientation)
	{
		m_Orientation = orientation;
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline void TransformationNode::Rotate(const F32 yawRadians, const F32 pitchRadians, const F32 rollRadians)
	{
		m_Orientation = Matrix3<F32>::RotateZ(rollRadians) * Matrix3<F32>::RotateX(pitchRadians) * Matrix3<F32>::RotateY(yawRadians) * m_Orientation;
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline const F32 TransformationNode::GetXScale() const
	{
		return m_Scale.x();
	}

	inline const F32 TransformationNode::GetYScale() const
	{
		return m_Scale.y();
	}

	inline const F32 TransformationNode::GetZScale() const
	{
		return m_Scale.z();
	}

	inline void TransformationNode::SetUniformScale(const F32 scale)
	{
		m_Scale = Vector3<F32>(scale);
		m_InverseScale = Vector3<F32>(1.0f / scale);
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline void TransformationNode::SetNonUniformScale(const F32 xScale, const F32 yScale, const F32 zScale)
	{
		m_Scale = Vector3<F32>(xScale, yScale, zScale);
		m_InverseScale = Vector3<F32>(1.0f / xScale, 1.0f / yScale, 1.0f / zScale);
		UpdateWorldTransform(MatrixStack<F32>());
	}

	inline const Matrix4<F32> TransformationNode::GetWorldTransform() const
	{
		return m_WorldTransform;
	}

	inline const Matrix4<F32> TransformationNode::GetWorldTransformNoScale() const
	{
		return m_WorldTransform * Matrix4<F32>::Scale(m_InverseScale.x(), m_InverseScale.y(), m_InverseScale.z());
	}

	inline const Matrix3<F32> TransformationNode::GetWorldOrientation() const
	{
		return ExtractOrientationComponent(m_WorldTransform);						
	}

	inline const Vector3<F32> TransformationNode::GetWorldPosition() const
	{
		return ExtractTranslationComponent(m_WorldTransform);
	}

	inline const std::string & TransformationNode::GetNodeName() const
	{
		return m_Name;
	}

	inline void TransformationNode::SetNodeName(const std::string & name)
	{
		m_Name = name;
	}

	inline TransformationNode::ConstComponentIterator TransformationNode::ComponentBegin() const
	{
		return m_AttachedComponents.begin();
	}

	inline TransformationNode::ConstComponentIterator TransformationNode::ComponentEnd() const
	{
		return m_AttachedComponents.end();
	}
}
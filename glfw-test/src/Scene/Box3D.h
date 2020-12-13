#pragma oncee

#include "Math.h"

namespace engine
{
	class Box3D
	{
	public:
		//! CTOR/DTOR:
		Box3D(dvec3 min = dvec3(-0.5), dvec3 max = dvec3(0.5));
		virtual ~Box3D();

		// SERVICES:
		Box3D Transform(const dmat4& matrix);
		void Enlarge(dvec3 p);
		void Enlarge(const Box3D& other);
		bool Contains(dvec3 p) const;
		void Reset();

		// ACCESSORS:
		void Set(const dvec3& min, const dvec3& max);
		void Set(const dvec3& extent);
		void Set(const Box3D& other);
		const dvec3& GetMin() const;
		const dvec3& GetMax() const;
		dvec3 GetCenter() const;

	protected:
		// MEMBERS:
		dvec3 m_min;
		dvec3 m_max;
	};

	////////////////////////////////////////////////////////////////////////////////
	// Box3D inline implementation:
	////////////////////////////////////////////////////////////////////////////////
	/*----------------------------------------------------------------------------*/
	inline Box3D::Box3D(dvec3 min, dvec3 max)
		: m_min(min), m_max(max) {
	}
	/*----------------------------------------------------------------------------*/
	inline Box3D::~Box3D() {
	}

	/*----------------------------------------------------------------------------*/
	inline Box3D Box3D::Transform(const dmat4& matrix)
	{
		Box3D box = Box3D(dvec3(matrix[3]) + m_min, dvec3(matrix[3]) + m_max);

		box.Enlarge(dvec3(matrix * dvec4(m_min.x, m_min.y, m_min.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_min.x, m_max.y, m_min.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_min.x, m_min.y, m_max.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_min.x, m_max.y, m_max.z, 1.0)));

		box.Enlarge(dvec3(matrix * dvec4(m_max.x, m_min.y, m_min.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_max.x, m_max.y, m_min.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_max.x, m_min.y, m_max.z, 1.0)));
		box.Enlarge(dvec3(matrix * dvec4(m_max.x, m_max.y, m_max.z, 1.0)));

		return box;
	}
	/*----------------------------------------------------------------------------*/
	inline void Box3D::Enlarge(dvec3 p) {
		m_min = glm::min(m_min, p);
		m_max = glm::max(m_max, p);
	}
	/*----------------------------------------------------------------------------*/
	inline void Box3D::Enlarge(const Box3D& other) {
		m_min = glm::min(m_min, other.m_min);
		m_max = glm::max(m_max, other.m_max);
	}
	/*----------------------------------------------------------------------------*/
	inline bool Box3D::Contains(dvec3 p) const {
		return (
			p.x >= m_min.x && p.x <= m_max.x &&
			p.y >= m_min.y && p.y <= m_max.y &&
			p.z >= m_min.z && p.z <= m_max.z
			);
	}
	/*----------------------------------------------------------------------------*/
	inline void Box3D::Reset() {
		m_min = dvec3(-0.5);
		m_max = dvec3(0.5);
	}

	/*----------------------------------------------------------------------------*/
	inline void Box3D::Set(const dvec3& min, const dvec3& max) {
		m_min = min;
		m_max = max;
	}
	/*----------------------------------------------------------------------------*/
	inline void Box3D::Set(const dvec3& extent) {
		Set(-extent, extent);
	}
	/*----------------------------------------------------------------------------*/
	inline void Box3D::Set(const Box3D& other) {
		Set(other.GetMin(), other.GetMax());
	}
	/*----------------------------------------------------------------------------*/
	inline const dvec3& Box3D::GetMin() const {
		return m_min;
	}
	/*----------------------------------------------------------------------------*/
	inline const dvec3& Box3D::GetMax() const {
		return m_max;
	}
	/*----------------------------------------------------------------------------*/
	inline dvec3 Box3D::GetCenter() const {
		return (m_min + m_max) * 0.5;
	}
}
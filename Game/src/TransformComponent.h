#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <glm.hpp>
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <gtx/euler_angles.hpp>

class TransformComponent {
private:
	glm::vec3 m_translation; // Verschiebung
	glm::vec3 m_rotation; // Drehung
	glm::vec3 m_scaling; // vergrößern / verkleinern


	glm::mat4x4 m_transformationMatrix; // matrix of translation, rotation, scaling

	glm::mat4x4 m_translationsMatrix; // translation matrix
	glm::mat4x4 m_rotationsMatrix; // rotation Matrix
	glm::mat4x4 m_scalingMatrix; // scalierungsmatrix

public:
	void updateTransformationMatrix();

	// constructor
	TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling) {
		m_translation = translation;
		m_rotation = rotation;
		m_scaling = scaling;

		// update transformationsMatrix with initialized values
		updateTransformationMatrix();
	}



	// getter Transformationsmatrix
	glm::mat4x4 const & getTransformationMatrix() { return m_transformationMatrix; }

	// translation
	glm::vec3 const & getTranslation() { return m_translation;}
	void setTranslation(glm::vec3 translation);

	// rotation
	glm::vec3 const & getRotation() { return m_rotation; }
	void setRotation(glm::vec3 rotation);

	// scaling
	glm::vec3 const & getScaling() { return m_scaling; }
	void setScaling(glm::vec3 scaling);
};


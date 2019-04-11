#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>
#include "Component.h"

class TransformComponent : public Component {
private:
	glm::vec3 translation = { 0,0,0 }; // Verschiebung
	glm::vec3 rotation = { 0,0,0 }; // Drehung Y X Z
	glm::vec3 scaling = { 0,0,0 }; // vergr��ern / verkleinern


	glm::mat4x4 transformationMatrix; // matrix of translation, rotation, scaling

	void updateTransformationMatrix();

public:
	
	// constructor
	TransformComponent();
	TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling); 

	
	// getter Transformationsmatrix
	glm::mat4x4 const & getTransformationMatrix() { return this->transformationMatrix; }

	// translation
	glm::vec3 const & getTranslation() const { return this->translation; }
	void setTranslation(glm::vec3 translation);

	// rotation
	glm::vec3 const & getRotation() const { return this ->rotation; }
	void setRotation(glm::vec3 rotation);

	// scaling
	glm::vec3 const & getScaling() const { return this->scaling; }
	void setScaling(glm::vec3 scaling);
};


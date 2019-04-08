#pragma once
#define GLM_ENABLE_EXPERIMENTAL

#include <vec3.hpp>
#include <mat4x4.hpp>

class TransformComponent {
private:
	glm::vec3 translation = { 0,0,0 }; // Verschiebung
	glm::vec3 rotation = { 0,0,0 }; // Drehung Y X Z
	glm::vec3 scaling = { 0,0,0 }; // vergrößern / verkleinern


	glm::mat4x4 transformationMatrix; // matrix of translation, rotation, scaling

	void updateTransformationMatrix();

public:
	
	// constructor
	TransformComponent(glm::vec3, glm::vec3, glm::vec3); 

	TransformComponent();



	// getter Transformationsmatrix
	glm::mat4x4 const & getTransformationMatrix() { return this->transformationMatrix; }

	// translation
	glm::vec3 const & getTranslation() { return this->translation;}
	void setTranslation(glm::vec3 translation);

	// rotation
	glm::vec3 const & getRotation() { return this ->rotation; }
	void setRotation(glm::vec3 rotation);

	// scaling
	glm::vec3 const & getScaling() { return this->scaling; }
	void setScaling(glm::vec3 scaling);
};


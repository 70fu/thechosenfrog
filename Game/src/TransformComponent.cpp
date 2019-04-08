#include "TransformComponent.h"
#include <iostream>
#include <ext.hpp>
#include <gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <gtx/euler_angles.hpp>

TransformComponent::TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling) {
	this->translation = translation;
	this->rotation = rotation;
	this->scaling = scaling;

	// update transformationsMatrix with initialized values
	updateTransformationMatrix();
}

TransformComponent::TransformComponent() {
	std::cout << "TransformComponent Contructor init with 0" << std::endl;
}

void TransformComponent::updateTransformationMatrix()
{	
	glm::mat4x4 translationsMatrix; // translation matrix
	glm::mat4x4 rotationsMatrix; // rotation Matrix
	glm::mat4x4 scalingMatrix; // scalierungsmatrix

	// fill each matrix
	scalingMatrix = glm::scale(glm::mat4(1.0f), getScaling());
	translationsMatrix = glm::translate(glm::mat4(1.0f), getTranslation());
	rotationsMatrix = glm::orientate4(getRotation()); // Y * X * Z

	// multiply matrix
	transformationMatrix = scalingMatrix * translationsMatrix * rotationsMatrix;

	// test prints
	std::cout << "---- Testausgaben: init Werte - Anfang ------" << std::endl;
	std::cout << "ScalingMatrix: "<< glm::to_string(scalingMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "TranslationMatrix: " << glm::to_string(translationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "RotationMatrix: " << glm::to_string(rotationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "---- Testausgaben: init Werte - Ende ------" << std::endl;
	std::cout << "--TransformationsMatrix ---" << std::endl;
	std::cout <<glm::to_string( transformationMatrix) << std::endl;
}

// Translation setter + Update Transformationsmatrix

void TransformComponent::setTranslation(glm::vec3 translation)  {
	this->translation = translation;
	updateTransformationMatrix();
}

// Rotation setter + Update Transformationsmatrix
void TransformComponent::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
	updateTransformationMatrix();
}


// Scaling setter + Update Transformationsmatrix

void TransformComponent::setScaling(glm::vec3 scaling) {
	this->scaling = scaling;
	updateTransformationMatrix();
}

#include "TransformComponent.h"
#include <iostream>
#include <ext.hpp>


void TransformComponent::updateTransformationMatrix()
{	

	// fill each matrix
	m_scalingMatrix = glm::scale(glm::mat4(1.0f), getScaling());
	m_translationsMatrix = glm::translate(glm::mat4(1.0f), getTranslation());
	m_rotationsMatrix = glm::orientate4(getRotation()); // Y * X * Z

	// multiply matrix
	m_transformationMatrix = m_scalingMatrix * m_translationsMatrix * m_rotationsMatrix;

	// test prints
	std::cout << "---- Testausgaben: init Werte - Anfang ------" << std::endl;
	std::cout << "ScalingMatrix: "<< glm::to_string(m_scalingMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "TranslationMatrix: " << glm::to_string(m_translationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "RotationMatrix: " << glm::to_string(m_rotationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "---- Testausgaben: init Werte - Ende ------" << std::endl;
	std::cout << "--TransformationsMatrix ---" << std::endl;
	std::cout <<glm::to_string( m_transformationMatrix) << std::endl;
}

// Translation setter + Update Transformationsmatrix

void TransformComponent::setTranslation(glm::vec3 translation)  {
	m_translation = translation;
	updateTransformationMatrix();
}

// Rotation setter + Update Transformationsmatrix
void TransformComponent::setRotation(glm::vec3 rotation) {
	m_rotation = rotation;
	updateTransformationMatrix();
}


// Scaling setter + Update Transformationsmatrix

void TransformComponent::setScaling(glm::vec3 scaling) {
	m_scaling = scaling;
	updateTransformationMatrix();
}

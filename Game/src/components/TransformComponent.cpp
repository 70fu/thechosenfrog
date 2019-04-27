#include "TransformComponent.h"
#define GLM_ENABLE_EXPERIMENTAL
//#include <iostream> // will be commented out
//#include <ext.hpp> // will be commented out with all the test prints
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
	//std::cout << "TransformComponent Contructor init with 0" << std::endl;
	updateTransformationMatrix();
}

void TransformComponent::makeDirty()
{
	dirty|=(DIRTY_LOCAL|DIRTY_GLOBAL);
	if(firstChild)//propagate change
		firstChild->parentChanged();
}

void TransformComponent::updateTransformationMatrix()
{	
	glm::mat4x4 translationsMatrix; // translation matrix
	glm::mat4x4 rotationsMatrix; // rotation Matrix
	glm::mat4x4 scalingMatrix; // scalierungsmatrix

	// fill each matrix
	scalingMatrix = glm::scale(glm::mat4(1.0f), getScaling());
	translationsMatrix = glm::translate(glm::mat4(1.0f), getTranslation());
	rotationsMatrix = glm::orientate4(glm::vec3(rotation.x,rotation.z,rotation.y)); //TODO why this strange order, WTH, maybe look into quaternions

	// multiply matrix
	transformationMatrix = translationsMatrix * rotationsMatrix * scalingMatrix;

	//reset dirty flag
	dirty&=~DIRTY_LOCAL;

	/*// test prints
	std::cout << "---- Testausgaben: init Werte - Anfang ------" << std::endl;
	std::cout << "ScalingMatrix: "<< glm::to_string(scalingMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "TranslationMatrix: " << glm::to_string(translationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "RotationMatrix: " << glm::to_string(rotationsMatrix) << std::endl;
	std::cout << std::endl;
	std::cout << "---- Testausgaben: init Werte - Ende ------" << std::endl;
	std::cout << "--TransformationsMatrix ---" << std::endl;
	std::cout <<glm::to_string( transformationMatrix) << std::endl;*/
}

void TransformComponent::updateGlobalTransform()
{
	if(parent)
		globalTrans=parent->getGlobalTransform()*transformationMatrix;
	else
		globalTrans=transformationMatrix;

	dirty&=~DIRTY_GLOBAL;
}

void TransformComponent::parentChanged()
{
	//make dirty
	dirty|=DIRTY_GLOBAL;

	//propagate to siblings and children
	if(nextSibling)
		nextSibling->parentChanged();
	if(firstChild)
		firstChild->parentChanged();
}

glm::mat4x4 const &TransformComponent::getTransformationMatrix()
{
	if((dirty&DIRTY_LOCAL)!=0)
		updateTransformationMatrix();
	return transformationMatrix;
}

glm::mat4x4 const &TransformComponent::getGlobalTransform()
{
	if((dirty&DIRTY_GLOBAL)!=0)
	{
		if((dirty&DIRTY_LOCAL)!=0)
			updateTransformationMatrix();

		updateGlobalTransform();
	}
	return globalTrans;
}

// Translation setter + Update Transformationsmatrix

void TransformComponent::setTranslation(glm::vec3 translation)  {
	this->translation = translation;
	makeDirty();
}

glm::vec3 TransformComponent::getGlobalTranslation()
{
	return getGlobalTransform()[3];
}

// Rotation setter + Update Transformationsmatrix
void TransformComponent::setRotation(glm::vec3 rotation) {
	this->rotation = rotation;
	makeDirty();
}


// Scaling setter + Update Transformationsmatrix

void TransformComponent::setScaling(glm::vec3 scaling) {
	this->scaling = scaling;
	makeDirty();
}

void TransformComponent::setTranslationAndRotation(const glm::vec3 &newTranslation, const glm::vec3 &newRotation)
{
	translation=newTranslation;
	rotation=newRotation;
	makeDirty();
}

TransformComponent *TransformComponent::getParent() const
{
	return parent;
}

void TransformComponent::setParent(TransformComponent &newParent, bool keepGlobalPos)
{
	//remove from old parent
	clearParentNoPropagate(keepGlobalPos);

	//add to new parent
	parent = &newParent;
	TransformComponent* tmp = parent->firstChild;
	parent->firstChild = this;
	if(tmp!=nullptr)
	{
		tmp->prevSibling = this;
		nextSibling = tmp;
	}

	if(!keepGlobalPos)
	{
		dirty |= DIRTY_GLOBAL;
		if (firstChild)
			firstChild->parentChanged();
	}
	else
	{
		transformationMatrix = glm::inverse(parent->getGlobalTransform())*transformationMatrix;
	}
}

void TransformComponent::clearParentNoPropagate(bool keepGlobalPos)
{
	if(parent==nullptr)
		return;

	//set local transformation to global transformation if desired
	if(keepGlobalPos)
		transformationMatrix=getGlobalTransform();//call function, just in case global transform is dirty

	if(prevSibling)
		prevSibling->nextSibling = nextSibling;
	if(nextSibling)
		nextSibling->prevSibling=prevSibling;
	if(parent->firstChild==this)
		parent->firstChild = nextSibling;
	parent = nextSibling = prevSibling = nullptr;
}

void TransformComponent::clearParent(bool keepGlobalPos)
{
	clearParentNoPropagate(keepGlobalPos);

	if(!keepGlobalPos)
	{
		dirty |= DIRTY_GLOBAL;
		if (firstChild)
			firstChild->parentChanged();
	}
}

TransformComponent *TransformComponent::getFirstChild() const
{
    return firstChild;
}

TransformComponent *TransformComponent::getNextSibling() const
{
    return nextSibling;
}

TransformComponent *TransformComponent::getPrevSibling() const
{
    return prevSibling;
}

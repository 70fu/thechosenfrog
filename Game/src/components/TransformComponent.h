#pragma once

#include <vec3.hpp>
#include <mat4x4.hpp>
#include "Component.h"

class TransformComponent : public Component {
private:
	enum DirtyFlags: unsigned char
	{
		DIRTY_LOCAL = 1<<0,
		DIRTY_GLOBAL = 1<<1
	};

	glm::vec3 translation = { 0,0,0 }; // Verschiebung
	glm::vec3 rotation = { 0,0,0 }; // Drehung Y X Z
	glm::vec3 scaling = { 1,1,1 }; // vergr��ern / verkleinern


	glm::mat4x4 transformationMatrix = glm::mat4(1); // matrix of translation, rotation, scaling
	glm::mat4x4 globalTrans = glm::mat4(1);

	unsigned char dirty = 0;

	TransformComponent* parent;
	TransformComponent* firstChild;
	//linked list
	TransformComponent* nextSibling;
	TransformComponent* prevSibling;

	//helper methods
	void updateTransformationMatrix();
	void updateGlobalTransform();
	void parentChanged();
	void makeDirty();
	void clearParentNoPropagate(bool keepGlobalPos);
public:
	
	// constructor
	TransformComponent();
	TransformComponent(glm::vec3 translation, glm::vec3 rotation, glm::vec3 scaling); 

	
	// getter Transformationsmatrix
	glm::mat4x4 const & getTransformationMatrix();
	glm::mat4x4 const & getGlobalTransform();

	// translation
	glm::vec3 const & getTranslation() const { return this->translation; }
	void setTranslation(glm::vec3 translation);
	glm::vec3 getGlobalTranslation();

	// rotation
	glm::vec3 const & getRotation() const { return this ->rotation; }
	void setRotation(glm::vec3 rotation);

	// scaling
	glm::vec3 const & getScaling() const { return this->scaling; }
	void setScaling(glm::vec3 scaling);

	//combination setter
	void setTranslationAndRotation(const glm::vec3& translation, const glm::vec3& rotation);

	//parent and children management methods
	/**
	 * @return parent of this transform, may be null
	 */
	TransformComponent* getParent() const;
	/**
	 * Sets parent of this transform
	 * @param parent
	 * @param keepGlobalPos
	 */
	void setParent(TransformComponent& parent, bool keepGlobalPos=true);
	/**
	 * Sets parent to nullptr
	 * @param keepGlobalPos
	 */
	void clearParent(bool keepGlobalPos=true);

	/**
	 * @return the first child of this transform, nullptr if this transform has no child
	 */
	TransformComponent* getFirstChild() const;
	/**
	 * @return the next sibling, or nullptr if this transform has no next sibling (last one)
	 */
    TransformComponent* getNextSibling() const;
    /**
	 * @return the previous sibling, or nullptr if this transform has no previous sibling (first one)
	 */
    TransformComponent* getPrevSibling() const;

    void cleanup(Game &game) override;

};




#include <ObjectInterfacePerModule.h>
#include "Game.h"
#include "RuntimeClasses.h"
#include "IGameRenderer.h"
#include <glad/glad.h>
#include <gtc/type_ptr.inl>
#include "assets/Material.h"
#include "util/CameraUtil.h"

class GameRenderer : public TInterface<RuntimeClassIds ::GAME_RENDERER,IGameRenderer>
{
private:
    template<typename T>
    inline bool checkMask(MaterialData::UniformValue<T>& uniformValue,long int& uniformMask)
    {
        long int locationBit = 1<<uniformValue.getLocation();

        //check if uniform has already been set
        bool ret = (locationBit&uniformMask)==0;
        //set uniform location bit
        uniformMask|=locationBit;

        return ret;
    }

    void bindMaterialData(MaterialData& mat, long int& uniformMask)
    {
        for(auto& p : mat.floats)
        {
            if (checkMask(p.second, uniformMask))
                glUniform1f(p.second.getLocation(), p.second.value);
        }
        for(auto& p : mat.vec2s)
        {
            if (checkMask(p.second, uniformMask))
                glUniform2fv(p.second.getLocation(), 1, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.vec3s)
        {
            if (checkMask(p.second, uniformMask))
                glUniform3fv(p.second.getLocation(), 1, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.vec4s)
        {
            if (checkMask(p.second, uniformMask))
                glUniform4fv(p.second.getLocation(), 1, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.mat2s)
        {
            if (checkMask(p.second, uniformMask))
                glUniformMatrix2fv(p.second.getLocation(), 1, GL_FALSE, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.mat3s)
        {
            if (checkMask(p.second, uniformMask))
                glUniformMatrix3fv(p.second.getLocation(), 1, GL_FALSE, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.mat4s)
        {
            if (checkMask(p.second, uniformMask))
                glUniformMatrix4fv(p.second.getLocation(), 1, GL_FALSE, glm::value_ptr(p.second.value));
        }
        for(auto& p : mat.textures)
        {
            //if (checkMask(p.second, uniformMask))
            //TODO bind textures
        }
    }

public:
    void render(Game& game, int width, int height) override
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //perform rendering for each camera
        for(CameraComponent& camera : game.cameraComps)
        {
            //a camera also needs a position, otherwise no view matrix can be calculated
            if(!game.hasComponents(camera.entity,Components::TRANSFORM_BIT))
                continue;

            //set viewport
            glViewport(camera.getViewportPos().x*width,camera.getViewportPos().y*height,camera.getViewportSize().x*width,camera.getViewportSize().y*height);

            TransformComponent& cTransform = game.transformComps[camera.entity];

            //calculate projection view matrix
            glm::mat4 pv = camera.getProjectionMatrix()*CameraUtil::getViewMatrix(cTransform.getTransformationMatrix());
            //get viewer pos
            glm::vec3 viewerPos = cTransform.getTranslation();

            /* --------------------------------------------- */
            // Render Mesh+Material+Transform
            /* --------------------------------------------- */
            //region draw Mesh+Material+Transform
            constexpr Components::ComponentMask mask = Components::MATERIAL_BIT | Components::MESH_BIT | Components::TRANSFORM_BIT;
            //iterate over all materials and see if entity has all other components as well
            for(MaterialComponent& material : game.materialComps)
            {
                EntityId entity = material.entity;
                if(game.hasComponents(entity,mask))
                {
                    TransformComponent& transform = game.transformComps[entity];
                    MeshComponent& mesh = game.meshComps[entity];

                    //TODO only switch program and set material data if necessary (look which material is currently bound)
                    //TODO make a render list to minimize state changes
                    //bind shader
                    glUseProgram(material.material->shader->getProgramHandle());

                    //bind projection view matrix uniform
                    glUniformMatrix4fv(CommonShaderUniforms::PROJECTION_VIEW_MATRIX,1,GL_FALSE,glm::value_ptr(pv));
                    //bind viewer position uniform
                    glUniform3fv(CommonShaderUniforms::VIEWER_POS,1,glm::value_ptr(viewerPos));

                    //bind uniforms, first material data of component, then of the material asset
                    //stores which uniforms have already been set, long int is at least 32 bit, so uniform locations until 31 are currently supported
                    long int uniformMask = 0;
                    bindMaterialData(material.instanceProp,uniformMask);
                    bindMaterialData(material.material->data,uniformMask);

                    //bind mesh
                    glBindVertexArray(mesh.mesh->getVAOHandle());
                    glUniformMatrix4fv(CommonShaderUniforms::MODEL_MAT, 1, GL_FALSE, glm::value_ptr(transform.getTransformationMatrix()));
                    glm::mat4 inverseTransform = glm::inverse(transform.getTransformationMatrix());
                    glUniformMatrix4fv(CommonShaderUniforms::NORMAL_MAT, 1, GL_FALSE, glm::value_ptr(glm::transpose(inverseTransform)));

                    //draw
                    Surface& surface = mesh.mesh->surface;
                    //check if surfaced is indexed
                    if((surface.dataFormatBitmask&Surface::INDICES_FORMAT)!=0)
                        glDrawElements(GL_TRIANGLES,mesh.mesh->surface.indices.size(),GL_UNSIGNED_INT,0);
                    else
                        glDrawArrays(GL_TRIANGLES,0,surface.positions.size());

                    //unbind
                    glBindVertexArray(0);
                }
            }

            //unbind shader
            glUseProgram(0);

            //endregion
        }

    }
};
REGISTERCLASS(GameRenderer)

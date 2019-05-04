

#include <ObjectInterfacePerModule.h>
#include "Game.h"
#include "RuntimeClasses.h"
#include "IGameRenderer.h"
#include <glad/glad.h>
#include <gtc/type_ptr.hpp>
#include "assets/Material.h"
#include "util/CameraUtil.h"
#include <ISimpleSerializer.h>


class GameRenderer : public TInterface<RuntimeClassIds ::GAME_RENDERER,IGameRenderer>
{

private:
    /* --------------------------------------------- */
    // Text Rendering
    /* --------------------------------------------- */
    static constexpr unsigned int MAX_CHAR_PER_TEXT = 256;
    //cpu side quad buffer bounds for quads as well as uvs for the bitmap texture
    stbtt_aligned_quad charBuffer[MAX_CHAR_PER_TEXT];
    //current amount of quads in charBuffer
    int charBufferSize=0;
    GLuint textVao;
    GLuint textVbo;
    //stores how many characters the vbo can hold, is initialized to MAX_CHAR_PER_TEST when vbo is allocated
    int maxTextQuads;

    //helper variable for texture units which should be cleared after a material bind
    GLuint nextFreeTextureUnit = 0;

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
            if (checkMask(p.second, uniformMask))
            {
                //bind texture to texture unit
                glActiveTexture(GL_TEXTURE0+nextFreeTextureUnit);
                glBindTexture(p.second.value->getTextureParameters().target,p.second.value->getTextureHandle());

                //bind texture unit id to uniform
                glUniform1i(p.second.getLocation(),nextFreeTextureUnit);

                ++nextFreeTextureUnit;
            }
        }
    }

public:
    void Init(bool isFirstInit) override
    {
        if(isFirstInit)
        {
            //activate depth testing
            glEnable(GL_DEPTH_TEST);

            //enable back-face culling
            glEnable(GL_CULL_FACE);

            //initialize text buffer
            maxTextQuads = MAX_CHAR_PER_TEXT;
            glGenVertexArrays(1,&textVao);
            glGenBuffers(1,&textVbo);
            glBindVertexArray(textVao);
            glBindBuffer(GL_ARRAY_BUFFER,textVbo);
            glBufferData(GL_ARRAY_BUFFER,sizeof(float)*4*maxTextQuads*2,nullptr,GL_DYNAMIC_DRAW);
            glVertexAttribPointer(Surface::POSITIONS,2,GL_FLOAT,GL_FALSE,4*sizeof(float),0);
            glEnableVertexAttribArray(Surface::POSITIONS);
            glVertexAttribPointer(Surface::UVS,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
            glEnableVertexAttribArray(Surface::UVS);
            glBindBuffer(GL_ARRAY_BUFFER,0);
            glBindVertexArray(0);
        }
    }

    void Serialize(ISimpleSerializer *pSerializer) override
    {
        IObject::Serialize(pSerializer);

        SERIALIZE(maxTextQuads)
        SERIALIZE(textVao)
        SERIALIZE(textVbo)
    }

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
            glm::mat4 projection = camera.getProjectionMatrix();
            glm::mat4 view = CameraUtil::getViewMatrix(cTransform.getGlobalTransform());
            glm::mat4 pv = projection*view;
            //get viewer pos
            glm::vec3 viewerPos = cTransform.getGlobalTranslation();

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
                    //start binding textures at texture unit 0
                    nextFreeTextureUnit = 0;
                    bindMaterialData(material.instanceProp,uniformMask);
                    bindMaterialData(material.material->data,uniformMask);

                    //bind mesh
                    glBindVertexArray(mesh.mesh->getVAOHandle());
                    glUniformMatrix4fv(CommonShaderUniforms::MODEL_MAT, 1, GL_FALSE, glm::value_ptr(transform.getGlobalTransform()));
                    glm::mat4 inverseTransform = glm::inverse(transform.getGlobalTransform());
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

            /* --------------------------------------------- */
            // Render SkyBox
            /* --------------------------------------------- */
            //region render skybox
            //taken from https://learnopengl.com/Advanced-OpenGL/Cubemaps
            // draw skybox as last
            {
                //uniform locations
                static constexpr int CUBEMAP_TEX = 4;

                glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
                glUseProgram(game.getAssetManager().getShaderProgram(ShaderProgramIds::SKYBOX)->getProgramHandle());
                glm::mat4 skyboxPv = projection * glm::mat4(glm::mat3(view));// remove translation from the view matrix
                //bind projection view matrix uniform
                glUniformMatrix4fv(CommonShaderUniforms::PROJECTION_VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(skyboxPv));
                // skybox cube
                glBindVertexArray(game.getAssetManager().getMesh(MeshIds::SKYBOX)->getVAOHandle());
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_CUBE_MAP, game.activeSkybox->getHandle());
                glUniform1i(CUBEMAP_TEX,0);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
                glDepthFunc(GL_LESS); // set depth function back to default

                //unbind shader
                glUseProgram(0);
            }
            //endregion

            /* --------------------------------------------- */
            // Render Text+Transform
            /* --------------------------------------------- */
            //region Render Text
            {
                //font shader uniform locations
                static constexpr int MODEL_MAT = 0;
                static constexpr int VIEW_MATS = 1;
                static constexpr int PROJ_MAT = 3;
                static constexpr int IN_WORLD_SPACE = 5;
                static constexpr int BITMAP_TEX = 6;
                static constexpr int TEXT_COLOR = 7;

                //enable blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                //bind shader,textVao and texture unit
                glUseProgram(game.getAssetManager().getShaderProgram(ShaderProgramIds::FONT)->getProgramHandle());
                glBindVertexArray(textVao);
                glActiveTexture(GL_TEXTURE0);

                //bind projection, view and texture unit uniforms
                glm::mat4 viewMatrices[2] = {glm::mat4(1),view};
                glm::mat4 projMatrices[2] = {glm::ortho<float>(0,width,0,height),projection};
                glUniform1i(BITMAP_TEX,0);
                glUniformMatrix4fv(VIEW_MATS,2,GL_FALSE,glm::value_ptr(viewMatrices[0]));
                glUniformMatrix4fv(PROJ_MAT,2,GL_FALSE,glm::value_ptr(projMatrices[0]));

                //keep last used font
                BitmapFontAsset* lastBitmap = nullptr;

                //render each text component with transform
                for(TextComponent& text : game.textComps)
                {
                    if(game.hasComponents(text.entity,Components::TRANSFORM_BIT))
                    {
                        TransformComponent& transform = game.transformComps[text.entity];

                        //bind uniforms
                        glUniformMatrix4fv(MODEL_MAT,1,GL_FALSE,glm::value_ptr(transform.getGlobalTransform()));
                        glUniform1i(IN_WORLD_SPACE,text.inScreenspace?0:1);
                        glUniform3fv(TEXT_COLOR,1,glm::value_ptr(glm::vec3(text.color)/255.0f));

                        //bind texture if not the correct one is bound
                        if(text.font!=lastBitmap)
                        {
                            lastBitmap = text.font;
                            glBindTexture(GL_TEXTURE_2D, text.font->getBitmap().getTextureHandle());
                        }

                        //fill buffer with quads
                        charBufferSize = text.font->fillQuadBuffer(text.text,&charBuffer[0],MAX_CHAR_PER_TEXT,0,0,1,-1);

                        //update the vbo
                        int writeSize = std::min(charBufferSize,maxTextQuads);
                        glBindBuffer(GL_ARRAY_BUFFER,textVbo);
                        glBufferSubData(GL_ARRAY_BUFFER,0,writeSize*sizeof(stbtt_aligned_quad),&charBuffer[0]);
                        glBindBuffer(GL_ARRAY_BUFFER,0);

                        //draw text
                        glDrawArrays(GL_LINES,0,writeSize*2);
                    }
                }

                glUseProgram(0);

                //disable blending
                glDisable(GL_BLEND);
            }
            //endregion
        }

    }
};
REGISTERCLASS(GameRenderer)

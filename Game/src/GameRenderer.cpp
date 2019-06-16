

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

    /* --------------------------------------------- */
    // Parabola Rendering
    /* --------------------------------------------- */
    static constexpr unsigned int MAX_PARABOLA_POINTS = 32;
    float parabolaX[MAX_PARABOLA_POINTS];//x values sent to the shader for the parabola formula
    GLuint parabolaVao;
    GLuint parabolaVbo;

    /* --------------------------------------------- */
    // Cloud Rendering
    /* --------------------------------------------- */
    //contains position and scale for each cloud interleaved (pspspspsps...)
    glm::vec3 cloudPosAndScales[2 * Components::MAX_SIZES[Components::CLOUD]];
    glm::mat4 cloudNormalViewModelMats[Components::MAX_SIZES[Components::CLOUD]];
    GLuint cloudRenderDataVbo;
    GLuint cloudPosAndScalesVbo;
    GLuint cloudNormalViewModelMatsVbo;


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
        for(auto& p : mat.ints)
        {
            if (checkMask(p.second, uniformMask))
                glUniform1i(p.second.getLocation(), p.second.value);
        }
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
        for(auto& p : mat.colors)
        {
            if (checkMask(p.second, uniformMask))
                glUniform4fv(p.second.getLocation(), 1, glm::value_ptr(glm::vec4(p.second.value)/255.0f));
        }
    }

public:

    void Serialize(ISimpleSerializer *pSerializer) override
    {
        IObject::Serialize(pSerializer);

        SERIALIZE(maxTextQuads)
        SERIALIZE(textVao)
        SERIALIZE(textVbo)
        SERIALIZE(parabolaVao)
        SERIALIZE(parabolaVbo)
        SERIALIZE(cloudPosAndScalesVbo)
        SERIALIZE(cloudRenderDataVbo)
    }

    void init(Game& game) override
    {
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

        //init parabola vao and vbo
        static constexpr int PARABOLA_VERTEX_ATTRIB_INDEX = 0;
        glGenVertexArrays(1,&parabolaVao);
        glGenBuffers(1,&parabolaVbo);
        glBindVertexArray(parabolaVao);
        glBindBuffer(GL_ARRAY_BUFFER,parabolaVbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(float)*MAX_PARABOLA_POINTS,nullptr,GL_DYNAMIC_DRAW);
        glVertexAttribPointer(PARABOLA_VERTEX_ATTRIB_INDEX,1,GL_FLOAT,GL_FALSE,1*sizeof(float),0);
        glEnableVertexAttribArray(PARABOLA_VERTEX_ATTRIB_INDEX);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);

        //attach vbos to cloud sphere vao
        static constexpr int POSITION_ATTRIB_INDEX = 6;
        static constexpr int SCALE_ATTRIB_INDEX = 7;
        static constexpr int OFFSET_ATTRIB_INDEX = 8;
        static constexpr int COLOR_ATTRIB_INDEX = 9;
        static constexpr int NORMAL_VIEW_MODEL_MAT = 10;//needs locations 10,11,12,13
        glGenBuffers(1,&cloudRenderDataVbo);
        glGenBuffers(1,&cloudPosAndScalesVbo);
        glGenBuffers(1,&cloudNormalViewModelMatsVbo);
        glBindVertexArray(game.getAssetManager().getMesh(MeshIds::CLOUD_SPHERE)->getVAOHandle());
        glBindBuffer(GL_ARRAY_BUFFER,cloudPosAndScalesVbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec3)*2*Components::MAX_SIZES[Components::CLOUD],nullptr,GL_DYNAMIC_DRAW);
        //position attribute
        glVertexAttribPointer(POSITION_ATTRIB_INDEX,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),0);
        glEnableVertexAttribArray(POSITION_ATTRIB_INDEX);
        glVertexAttribDivisor(POSITION_ATTRIB_INDEX,CloudRenderData::SPHERE_COUNT);
        //scale attribute
        glVertexAttribPointer(SCALE_ATTRIB_INDEX,3,GL_FLOAT,GL_FALSE,2*sizeof(glm::vec3),(void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(SCALE_ATTRIB_INDEX);
        glVertexAttribDivisor(SCALE_ATTRIB_INDEX,CloudRenderData::SPHERE_COUNT);
        //bind render data vbo
        glBindBuffer(GL_ARRAY_BUFFER,cloudRenderDataVbo);
        glBufferData(GL_ARRAY_BUFFER,sizeof(CloudRenderData)*Components::MAX_SIZES[Components::CLOUD],nullptr,GL_DYNAMIC_DRAW);
        //offset attribute
        glVertexAttribPointer(OFFSET_ATTRIB_INDEX,3,GL_FLOAT,GL_FALSE,sizeof(CloudRenderData::SphereData),(void*)0);
        glEnableVertexAttribArray(OFFSET_ATTRIB_INDEX);
        glVertexAttribDivisor(OFFSET_ATTRIB_INDEX,1);
        //color attribute
        glVertexAttribPointer(COLOR_ATTRIB_INDEX,3,GL_FLOAT,GL_FALSE,sizeof(CloudRenderData::SphereData),(void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(COLOR_ATTRIB_INDEX);
        glVertexAttribDivisor(COLOR_ATTRIB_INDEX,1);
        //bind normal view model mat vbo
        glBindBuffer(GL_ARRAY_BUFFER,cloudNormalViewModelMatsVbo);
        glBufferData(GL_ARRAY_BUFFER, Components::MAX_SIZES[Components::CLOUD] * sizeof(glm::mat4), &cloudNormalViewModelMats[0], GL_DYNAMIC_DRAW);
        //matrix attribute
        glVertexAttribPointer(NORMAL_VIEW_MODEL_MAT, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)0);
        glVertexAttribPointer(NORMAL_VIEW_MODEL_MAT+1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(sizeof(glm::vec4)));
        glVertexAttribPointer(NORMAL_VIEW_MODEL_MAT+2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(2 * sizeof(glm::vec4)));
        glVertexAttribPointer(NORMAL_VIEW_MODEL_MAT+3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(glm::vec4), (void*)(3 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(NORMAL_VIEW_MODEL_MAT);
        glEnableVertexAttribArray(NORMAL_VIEW_MODEL_MAT+1);
        glEnableVertexAttribArray(NORMAL_VIEW_MODEL_MAT+2);
        glEnableVertexAttribArray(NORMAL_VIEW_MODEL_MAT+3);
        glVertexAttribDivisor(NORMAL_VIEW_MODEL_MAT,CloudRenderData::SPHERE_COUNT);
        glVertexAttribDivisor(NORMAL_VIEW_MODEL_MAT+1,CloudRenderData::SPHERE_COUNT);
        glVertexAttribDivisor(NORMAL_VIEW_MODEL_MAT+2,CloudRenderData::SPHERE_COUNT);
        glVertexAttribDivisor(NORMAL_VIEW_MODEL_MAT+3,CloudRenderData::SPHERE_COUNT);
        //unbind
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glBindVertexArray(0);
    }

    void render(Game& game, int width, int height) override
    {
        //activate depth testing and writing into depth buffer
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);

        //perform rendering for each camera
        for(CameraComponent& camera : game.cameraComps)
        {
            //a camera also needs a position, otherwise no view matrix can be calculated
            if(!game.hasComponents(camera.entity,Components::TRANSFORM_BIT))
                continue;

            //bind framebuffer
            static constexpr GLuint COLOR_ATTACHMENTS[] = {GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3,GL_COLOR_ATTACHMENT4,GL_COLOR_ATTACHMENT5,GL_COLOR_ATTACHMENT6,GL_COLOR_ATTACHMENT7};
            GLuint fboHandle = camera.getFramebuffer()==nullptr?0:camera.getFramebuffer()->getFBOHandle();
            glBindFramebuffer(GL_FRAMEBUFFER,fboHandle);
            glDrawBuffers(camera.getFramebuffer()->getParameters().colorBufferCount,&COLOR_ATTACHMENTS[0]);

            //clear framebuffer
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //set viewport
            glViewport(camera.getScreenViewportPos().x*width, camera.getScreenViewportPos().y*height,
                       camera.getScreenViewportSize().x*width,
                       camera.getScreenViewportSize().y*height);

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
                    //bind normal view model matrix
                    glm::mat4 inverseViewModel = glm::inverse(view*transform.getGlobalTransform());
                    glUniformMatrix4fv(CommonShaderUniforms::NORMAL_VIEW_MODEL_MAT,1,GL_FALSE,glm::value_ptr(glm::transpose(inverseViewModel)));

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
            // Render Clouds
            /* --------------------------------------------- */
            //region Render Clouds
            {
                //get position and scale for every cloud
                glm::vec3* posIt = &cloudPosAndScales[0];
                glm::vec3* scaleIt = &cloudPosAndScales[1];
                glm::mat4* normalViewModelIt = &cloudNormalViewModelMats[0];
                for(CloudComponent& cloud: game.cloudComps)
                {
                    EntityId entity = cloud.entity;

                    //check for transform component
                    //TODO only in debug
                    if(!game.hasComponents(entity,Components::TRANSFORM_BIT))
                    {
                        posIt+=2;
                        scaleIt+=2;
                        ++normalViewModelIt;
                        continue;
                    }

                    //assume a cloud has a transform
                    TransformComponent& transform = game.transformComps[entity];

                    //write pos and scale
                    *posIt = transform.getGlobalTranslation();
                    *scaleIt = transform.getScaling();

                    //calculate normal view matrix
                    *normalViewModelIt = glm::transpose(glm::inverse(view*glm::scale(glm::mat4(1.0f), transform.getScaling())));

                    posIt+=2;
                    scaleIt+=2;
                    ++normalViewModelIt;
                }

                ComponentId numClouds = game.cloudComps.getNumActive();
                MeshAsset& sphereMesh = *game.getAssetManager().getMesh(MeshIds::CLOUD_SPHERE);

                //bind shader
                glUseProgram(game.getAssetManager().getShaderProgram(ShaderProgramIds::CLOUD)->getProgramHandle());

                //bind vao and upload cloud data
                glBindVertexArray(sphereMesh.getVAOHandle());
                //upload position and scales
                glBindBuffer(GL_ARRAY_BUFFER,cloudPosAndScalesVbo);
                glBufferSubData(GL_ARRAY_BUFFER,0,numClouds*2*sizeof(glm::vec3),&cloudPosAndScales[0]);
                glBindBuffer(GL_ARRAY_BUFFER,cloudRenderDataVbo);
                glBufferSubData(GL_ARRAY_BUFFER,0,numClouds*sizeof(CloudRenderData),&game.cloudComps.renderData[0]);
                glBindBuffer(GL_ARRAY_BUFFER,cloudNormalViewModelMatsVbo);
                glBufferSubData(GL_ARRAY_BUFFER,0,numClouds*sizeof(glm::mat4),&cloudNormalViewModelMats[0]);
                glBindBuffer(GL_ARRAY_BUFFER,0);

                //bind uniforms
                //bind projection view matrix uniform
                glUniformMatrix4fv(CommonShaderUniforms::PROJECTION_VIEW_MATRIX, 1, GL_FALSE, glm::value_ptr(pv));
                glUniform1i(CommonShaderUniforms::FRAME,game.getFrame());

                //draw instanced
                glDrawElementsInstanced(GL_TRIANGLES,sphereMesh.surface.indices.size(),GL_UNSIGNED_INT,0,numClouds*CloudRenderData::SPHERE_COUNT);

                //unbind vao
                glBindVertexArray(0);

                //unbind shader
                glUseProgram(0);
            }
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
                        charBufferSize = text.font->fillQuadBuffer(text.text,&charBuffer[0],MAX_CHAR_PER_TEXT,0,0,1,text.wrapWidth);

                        //dont write to depth buffer if text is in screen space
                        glDepthMask(!text.inScreenspace);

                        //update the vbo
                        int writeSize = std::min(charBufferSize,maxTextQuads);
                        glBindBuffer(GL_ARRAY_BUFFER,textVbo);
                        glBufferSubData(GL_ARRAY_BUFFER,0,writeSize*sizeof(stbtt_aligned_quad),&charBuffer[0]);
                        glBindBuffer(GL_ARRAY_BUFFER,0);

                        //draw text
                        glDrawArrays(GL_LINES,0,writeSize*2);
                    }
                }

                //unbind vao
                glBindVertexArray(0);

                //unbind shader
                glUseProgram(0);

                //disable blending
                glDisable(GL_BLEND);

                //reenable writes to depth buffer
                glDepthMask(GL_TRUE);
            }
            //endregion

            /* --------------------------------------------- */
            // Render jump Parabolas (CharControllerComp+Player)
            /* --------------------------------------------- */
            //region Render Jump Parabolas
            {
                /*
                 * The parabolaVbo contains the x values for which the parabola is evaluated and a point with coordinates is calculated in the vertex shader
                 * a geometry shader generates more vertices out of the parabola components
                 */
                //uniform locations
                static constexpr int MAX_X = 16;
                static constexpr int JUMP_DISTANCE = 17;
                static constexpr int PARABOLA=18;

                //enable blending
                glEnable(GL_BLEND);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

                //bind shader
                glUseProgram(game.getAssetManager().getShaderProgram(ShaderProgramIds::PARABOLA)->getProgramHandle());

                //bind projection view matrix and frame time
                glUniformMatrix4fv(CommonShaderUniforms::PROJECTION_VIEW_MATRIX,1,GL_FALSE,glm::value_ptr(pv));
                glUniform1i(CommonShaderUniforms::FRAME,game.getFrame());

                //bind vao
                glBindVertexArray(parabolaVao);

                for(PlayerComponent& player : game.playerComps)
                {
                    if(game.hasComponents(player.entity,Components::CHAR_CONTROLLER_BIT|Components::TRANSFORM_BIT))
                    {
                        CharControllerComponent& controller = game.charControllerComps[player.entity];

                        //is the player charging a jump??, then render
                        if(controller.jumpStrength>0 && controller.grounded)
                        {
                            TransformComponent& transform = game.transformComps[player.entity];

                            //calculate parabola components
                            glm::vec3 jump = controller.calculateJump();
                            glm::vec2 parabolaComponents = {0.5f*jump.z,jump.y};//calculate parabola components

                            //bind uniforms
                            glUniformMatrix4fv(CommonShaderUniforms::MODEL_MAT, 1, GL_FALSE, glm::value_ptr(transform.getGlobalTransform()));
                            glUniform1f(MAX_X,controller.cachedJumpDuration);//TODO make a physics query to stop at the first impact of the parabola
                            glUniform1f(JUMP_DISTANCE,controller.cachedJumpDistance);
                            glUniform2fv(PARABOLA,1,glm::value_ptr(parabolaComponents));
                            glm::mat4 inverseViewModel = glm::inverse(view*transform.getGlobalTransform());
                            glUniformMatrix4fv(CommonShaderUniforms::NORMAL_VIEW_MODEL_MAT,1,GL_FALSE,glm::value_ptr(glm::transpose(inverseViewModel)));

                            //set parabola points
                            for(int i = 0;i<MAX_PARABOLA_POINTS;++i)
                                parabolaX[i] = static_cast<float>(i)/(MAX_PARABOLA_POINTS-1);
                            glBindBuffer(GL_ARRAY_BUFFER,parabolaVbo);
                            glBufferSubData(GL_ARRAY_BUFFER,0,MAX_PARABOLA_POINTS*sizeof(float),&parabolaX[0]);
                            glBindBuffer(GL_ARRAY_BUFFER,0);

                            //draw
                            glDrawArrays(GL_POINTS,0,MAX_PARABOLA_POINTS);
                        }
                    }
                }

                //unbind vao
                glBindVertexArray(0);

                //unbind shader
                glUseProgram(0);

                //disable blending
                glDisable(GL_BLEND);
            }
            //endregion
        }

        /* --------------------------------------------- */
        // switch to default fbo and render main fbo onto a quad
        /* --------------------------------------------- */
        //region switch to default fbo and render main fbo onto a quad
        {
            //uniform locations
            static constexpr int BRIGHTNESS_FACTOR = 0;
            static constexpr int COLOR_BUFFER = 1;

            static constexpr int NORMAL_TEXTURE = 1;
            static constexpr int DEPTH_TEXTURE = 5;

            //disable depth testing and dont write into depth buffer
            glDisable(GL_DEPTH_TEST);
            glDepthMask(GL_TRUE);


            //enable blending
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            //set viewport, draw over entire screen
            glViewport(0,0,width, height);

            glClear(GL_COLOR_BUFFER_BIT);

            FramebufferAsset& fbo = *game.getAssetManager().getFramebuffer(FramebufferIds::DEFAULT);

            //draw normal colored scene onto quad
            //bind shader
            glUseProgram(game.getAssetManager().getShaderProgram(ShaderProgramIds::FBO)->getProgramHandle());

            //bind color buffer of main fbo
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,fbo.getColorBufferHandle(0));
            glUniform1i(COLOR_BUFFER, 0);

            //set brightness factor
            glUniform1f(BRIGHTNESS_FACTOR,game.settings.display.brightnessFactor);

            //bind vao and draw quad
            glBindVertexArray(game.getAssetManager().getMesh(MeshIds::SCREEN_QUAD)->getVAOHandle());
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            //unbind shader
            glUseProgram(0);

            //draw outlines onto quad
            MaterialAsset& material = *game.getAssetManager().getMaterial(MaterialIds::FBO_POST);
            //bind shader
            glUseProgram(material.shader->getProgramHandle());

            //bind uniforms
            long uniformMask = 0;
            bindMaterialData(material.data,uniformMask);
            //bind normal texture of main fbo
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D,fbo.getColorBufferHandle(1));
            glUniform1i(NORMAL_TEXTURE, 0);
            //bind depth texture of main fbo
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D,fbo.getDepthStencilBufferHandle());
            glUniform1i(DEPTH_TEXTURE, 1);

            //bind vao and draw quad
            glBindVertexArray(game.getAssetManager().getMesh(MeshIds::SCREEN_QUAD)->getVAOHandle());
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);

            //unbind shader
            glUseProgram(0);

            //disable blending
            glDisable(GL_BLEND);
        }
        //endregion
    }

    void cleanup(Game &game) override
    {
        //TODO
    }
};
REGISTERCLASS(GameRenderer)

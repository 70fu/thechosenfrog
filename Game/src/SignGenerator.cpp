#include "SignGenerator.h"
#include "components/TransformComponent.h"
#include "components/MaterialComponent.h"
#include "components/TextComponent.h"
#include "Game.h"

TransformComponent &SignGenerator::makeSign(Game &game, SignGenerator::SignParameters params)
{
    EntityId id = game.createEntity();
    TransformComponent& trans = game.transformComps.addComp(id);
    trans.setTranslation(params.translation);
    trans.setRotation(params.rotation);
    trans.setScaling({1,1,1});

    game.meshComps.addComp(id).mesh=game.getAssetManager().getMesh(MeshIds::SIGNPOST);

    MaterialComponent& material = game.materialComps.addComp(id);
    material.material = game.getAssetManager().getMaterial(MaterialIds::UNLIT);
    material.instanceProp.setColor("colorMultiply",{200,160,70,255});
    material.retrieveUniformLocations();

    //create world space text
    EntityId textId = game.createEntity();
    TransformComponent& textTrans = game.transformComps.addComp(textId);
    textTrans.setTranslation({-0.45,1.1,-0.25});
    static constexpr float TEXT_SCALE = 1/768.0f;
    textTrans.setScaling({TEXT_SCALE,TEXT_SCALE,TEXT_SCALE});
    textTrans.setParent(trans,false);

    TextComponent& text = game.textComps.addComp(textId);
    text.text = params.text;
    text.font = game.getAssetManager().getBitmapFont(BitmapFontIds::DEFAULT);
    text.inScreenspace = false;
    text.wrapWidth = 768;
    text.color = {0,0,0,255};

    return trans;
}

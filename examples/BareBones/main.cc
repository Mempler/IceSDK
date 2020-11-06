#include "Utils/Logger.h"

#include "GameBase.h"
#include "Graphics/Debug/Draw.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/Fonts/FontFace.h"
#include "Graphics/ImGui/Widgets/SceneGraph.h"
#include "Graphics/SpriteBatch/SpriteBatch.h"

#include <imgui.h>

using namespace IceSDK;

class Game final : public GameBase
{
public:
protected:
    void Init() override
    {
        /* TODO: create a function in GameBase */
        this->_active_scene = std::make_shared<Scene>();
    }

    void InitDraw() override
    {
        auto activeSceneLock = this->GetActiveScene();
        auto activeScene = activeSceneLock.lock();

        // TODO: let GameBase initialize this.
        Graphics::Entity::Init(this->GetShaderManager());
        Graphics::Entity::InitScene(activeScene);

        this->_boxTexture =
            this->GetAssetManager()->LoadTexture("/Assets/Box.png");

        
        this->_boxTexture2 =
            this->GetAssetManager()->LoadTexture("/Assets/Ground.png");

        this->_box = Graphics::Entity::CreateSprite(
            activeScene, this->GetShaderManager(), this->_boxTexture,
            { 0.f, 0.f, 0.f });

        this->_box2 = Graphics::Entity::CreateSprite(
            activeScene, this->GetShaderManager(), this->_boxTexture2,
            { 400.f, 100.f, 0.f });

        // Make sure Roboto-Regular.ttf is in out/ folder!

        // then uncomment this:
        /*
        this->_faceHandle =
            this->GetFontManager()->LoadFont("Roboto-Regular.ttf");

        if (this->_faceHandle == INVALID_FONT_FACE_HANDLE)
            ICESDK_CRITICAL("Failed to load Font! (-1)");

        this->_text =
            Graphics::Entity::CreateText(activeScene, this->GetShaderManager(),
                                         "Hellö Wörld!", 16, _faceHandle);
            */
    }

    void Draw(float pDelta) override
    {
        ImGuiWidgets::SceneGraph::Frame(this->GetActiveScene());
    }

    /* Not really needed as we use Systems by default */
    void Update(float pDelta) override { }

private:
    Entity _box;
    Entity _box2;
    Memory::Ptr<Graphics::Texture2D> _boxTexture;
    Memory::Ptr<Graphics::Texture2D> _boxTexture2;

    /* Make sure the code above is uncommented.
    Entity _text;
    IceSDK::Graphics::FontFaceHandle _faceHandle;
    */
};

Memory::Ptr<Game> g_Game;
Memory::Ptr<IceSDK::GameBase> g_GameBase;

int IceSDKMain()
{
    g_Game = std::make_shared<Game>();
    g_GameBase = g_Game;

    g_Game->Run();

    g_Game = nullptr;
    g_GameBase = nullptr;
    return 0;
}

Memory::Ptr<GameBase> GetGameBase()
{
    return g_GameBase;
}

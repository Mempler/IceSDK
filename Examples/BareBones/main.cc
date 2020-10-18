#include "Utils/Instrumentor.h"
#include "Utils/Logger.h"

#include "GameBase.h"
#include "Graphics/Debug/Draw.h"
#include "Graphics/EntityHelper.h"
#include "Graphics/Fonts/FontFace.h"
#include "Graphics/ImGui/Widgets/SceneGraph.h"
#include "Graphics/SpriteBatch/SpriteBatch.h"
#include "Graphics/TextureAtlas/Packer.h"
#include "Graphics/Tilesheet/Tilesheet.h"

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

        Graphics::Packer* packer = new Graphics::Packer(512, 512);

        this->_boxTexture =
            this->GetAssetManager()->LoadTexture("/Assets/Box.png");

        this->_boxTexture2 =
            this->GetAssetManager()->LoadTexture("/Assets/Ground.png");

        this->_tileset0 =
            this->GetAssetManager()->LoadTexture("/Assets/mario.png");

        _sheet = std::make_shared<Graphics::Tilesheet>(this->_tileset0, 16);

        /*this->_box = Graphics::Entity::CreateSprite(
            activeScene, this->GetShaderManager(), this->_tileset0,
            { 100.f, 100.f, 0.f }, { 16, 16 }, _sheet->GetTile(0));

        this->_box2 = Graphics::Entity::CreateSprite(
            activeScene, this->GetShaderManager(), this->_boxTexture2,
            { 400.f, 100.f, 0.f });*/

        for (size_t i = 0; i < _sheet->TileSize(); i++)
        {
            uint32_t x, y;
            if (!packer->Push(16, 16, x, y))
            {
                break;
            }
 
            Graphics::Entity::CreateSprite(
                activeScene, this->GetShaderManager(), this->_tileset0,
                { 512 + x, y, 0.f }, { 16, 16 }, _sheet->GetTile(i));
        }

        packer->Clear();

        for (size_t i = 0; i < _sheet->TileSize(); i++)
        {
            uint32_t x, y;
            if (!packer->Push(16, 16, x, y))
            {
                break;
            }

            Graphics::Entity::CreateSprite(
                activeScene, this->GetShaderManager(), this->_tileset0,
                { x, y, 0.f }, { 16, 16 }, _sheet->GetTile(i));
        }
    }

    void Draw(float pDelta) override
    {
        ImGuiWidgets::SceneGraph::Frame(this->GetActiveScene());

        ImGui::ShowDemoWindow();
    }

    /* Not really needed as we use Systems by default */
    void Update(float pDelta) override { }

private:
    Entity _box;
    Entity _box2;

    Memory::Ptr<Graphics::Texture2D> _boxTexture;
    Memory::Ptr<Graphics::Texture2D> _boxTexture2;
    Memory::Ptr<Graphics::Texture2D> _tileset0;
    Memory::Ptr<Graphics::Tilesheet> _sheet;
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

#include "scenemngr.h"

#include <future>

#include "dark.h"
#include "fade.h"
#include "fire.h"
#include "game.h"
#include "prep.h"
#include "title.h"
#include "menu.h"
#include "multi_server_select.h"
#include "result.h"
#include "thunder.h"
#include "title_ver2.h"
#include "wind.h"
#include "lib/imgui/imgui_internal.h"

std::map<std::string, int> attribute_dict = {
    {"FIRE", 0},
    {"WIND", 1},
    {"THUNDER", 2},
    {"DARK", 3},
};

SceneMngr::SceneMngr(SCENE scene)
{
    switch (scene)
    {
    case SCENE_TITLE:
        scene_ = new Title(this);
        break;
    case SCENE_MENU:
        scene_ = new Menu(this);
        break;
    case SCENE_MULTI_SERVER_SELECT:
        scene_ = new Multi_Server_Select(this);
        break;
    case SCENE_PREP:
        scene_ = new Prep(this);
        break;
    case SCENE_GAME:
        scene_ = new Game(this);
        break;
    case SCENE_RESULT:
        scene_ = new Result(this);
        break;
    default:
        break;
    }
}

void SceneMngr::ChangeScene(SCENE scene)
{
    if (scene_)
    {
        delete scene_;
        scene_ = nullptr;
    }
    Fade::StartFadeIn(5.0f);
    std::future<void> loadSceneFuture = std::async(std::launch::async, &SceneMngr::LoadScene, this, scene);
    auto start = std::chrono::high_resolution_clock::now();
    loading_ = true;
    while (true)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start);
        Fade::Update();
        Fade::Draw();
        if (loadSceneFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready && Fade::Finished())
        {
            break;
        }
    }
    loading_ = false;
}

void SceneMngr::DebugMenu()
{
    scene_->DebugMenu();
    ImGui::Begin("screen capture");
    if (ImGui::Button("capture"))
    {
        CaptureScreen();
        captured_ = true;
    }
    ImGui::End();
}

void SceneMngr::LoadScene(SCENE scene)
{
    switch (scene)
    {
    case SCENE_TITLE:
        scene_ = new Title(this);
        break;
    case SCENE_MENU:
        scene_ = new Menu(this);
        break;
    case SCENE_MULTI_SERVER_SELECT:
        scene_ = new Multi_Server_Select(this);
        break;
    case SCENE_PREP:
        scene_ = new Prep(this);
        break;
    case SCENE_GAME:
        scene_ = new Game(this);
        break;
    case SCENE_RESULT:
        scene_ = new Result(this);
        break;
    default:
        break;
    }
}

void SceneMngr::ChangeScene(SCENE scene, const std::string& message)
{
    if (scene_)
    {
        delete scene_;
        scene_ = nullptr;
    }
    std::future<void> loadSceneFuture = std::async(std::launch::async, &SceneMngr::LoadScene, this, scene);
    Load::LoadStart();
    /*Graphical::Clear(Color(1, 1, 1, 1) * 0.5f);
    const float scale_x = static_cast<float>(Graphical::GetWidth()) / 1920;
    const float scale_y = static_cast<float>(Graphical::GetHeight()) / 1080;
    DrawSprite(loading_tex_, Vector2(Graphical::GetWidth() / 2, Graphical::GetHeight() / 2),
               0.0f, Vector2(1920.0f * scale_x, 1080.0f * scale_y), Color(1.0f, 1.0f, 1.0f, 1.0f));
    Graphical::Present();*/
    auto start = std::chrono::high_resolution_clock::now();
    loading_ = true;
    while (true)
    {
        auto now = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - start);
        Load::Update();
        Load::Draw();
        if (loadSceneFuture.wait_for(std::chrono::milliseconds(0)) == std::future_status::ready && duration.count() >=
            2)
        {
            Load::LoadStop();
            break;
        }
    }
    loading_ = false;

    if (scene == SCENE_GAME)
        ParseGame(message);
}


/**
 *voidポインターを返しChangeSceneの中に変換する
 *messageはスペース区切り
 *GAMEの場合、playerの属性を変更する
 *1つ目は移動、2つ目は攻撃
 *messageの例：FIRE WIND
**/

void SceneMngr::ParseGame(const std::string& message)
{
    //spit words
    std::vector<std::string> words;
    std::string word;
    for (auto& c : message)
    {
        if (c == ' ')
        {
            words.push_back(word);
            word.clear();
        }
        else
        {
            word += c;
        }
    }
    Game* game = dynamic_cast<Game*>(scene_);
    Player* player = new Player(game->GetMapMngr()->GetPlayerSpawn(), 0.0f, Vector2(0.0f, 0.0f), game->GetMapMngr());
    switch (attribute_dict.at(words[0]))
    {
    case 0:
        {
            player->SetAttribute(new Fire(player));
        }
        break;
    case 1:
        {
            player->SetAttribute(new Wind(player));
        }
        break;
    case 2:
        {
            player->SetAttribute(new Thunder(player));
        }
        break;
    case 3:
        {
            player->SetAttribute(new Dark(player));
        }
        break;
    default:
        {
            player->SetAttribute(new Fire(player));
        }
    }

    switch (attribute_dict.at(words[1]))
    {
    case 0:
        {
            player->SetAttackAttribute(new Fire(player));
        }
        break;
    case 1:
        {
            player->SetAttackAttribute(new Wind(player));
        }
        break;
    case 2:
        {
            player->SetAttackAttribute(new Thunder(player));
        }
        break;
    case 3:
        {
            player->SetAttackAttribute(new Dark(player));
        }
        break;
    default:
        {
            player->SetAttackAttribute(new Fire(player));
        }
    }
    game->AddPlayer(player);
    game->AddCamera(new Camera(player->GetPos(),
                               Vector2(game->GetMapMngr()->GetMap()->GetWidth(),
                                       game->GetMapMngr()->GetMap()->GetHeight())));

    //プレイヤーのテクスチャを決める
    switch (player->GetAttackAttribute()->GetAttribute())
    {
    case ATTRIBUTE_TYPE_FIRE:
        switch (player->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_DARK:
            player->GetAnimator()->SetTexenum(player1_12);
            break;
        case ATTRIBUTE_TYPE_WIND:
            player->GetAnimator()->SetTexenum(player1_14);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            player->GetAnimator()->SetTexenum(player1_13);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_DARK:
        switch (player->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            player->GetAnimator()->SetTexenum(player1_21);
            break;
        case ATTRIBUTE_TYPE_WIND:
            player->GetAnimator()->SetTexenum(player1_24);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            player->GetAnimator()->SetTexenum(player1_23);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_WIND:
        switch (player->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            player->GetAnimator()->SetTexenum(player1_41);
            break;
        case ATTRIBUTE_TYPE_DARK:
            player->GetAnimator()->SetTexenum(player1_42);
            break;
        case ATTRIBUTE_TYPE_THUNDER:
            player->GetAnimator()->SetTexenum(player1_43);
            break;
        default:
            break;
        }
        break;
    case ATTRIBUTE_TYPE_THUNDER:
        switch (player->GetAttribute()->GetAttribute())
        {
        case ATTRIBUTE_TYPE_FIRE:
            player->GetAnimator()->SetTexenum(player1_31);
            break;
        case ATTRIBUTE_TYPE_DARK:
            player->GetAnimator()->SetTexenum(player1_32);
            break;
        case ATTRIBUTE_TYPE_WIND:
            player->GetAnimator()->SetTexenum(player1_34);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void SceneMngr::CaptureScreen()
{
    // Assuming you have a valid ID3D11DeviceContext* named deviceContext
    // and a valid ID3D11RenderTargetView* named renderTargetView
    ID3D11DeviceContext* deviceContext = Graphical::GetDevice().GetContext();
    ID3D11RenderTargetView* renderTargetView = Graphical::GetRenderer().GetRTV();

    ID3D11Resource* backBufferResource;
    renderTargetView->GetResource(&backBufferResource);

    ID3D11Texture2D* backBufferTexture;
    backBufferResource->QueryInterface(__uuidof(ID3D11Texture2D), (void**)&backBufferTexture);

    D3D11_TEXTURE2D_DESC textureDesc;
    backBufferTexture->GetDesc(&textureDesc);

    // Create a new texture with the same description
    if (savedFrameTexture_ != nullptr)
        savedFrameTexture_->Release();

    Graphical::GetDevice().Get()->CreateTexture2D(&textureDesc, nullptr, &savedFrameTexture_);

    // Copy the back buffer to the new texture
    deviceContext->CopyResource(savedFrameTexture_, backBufferTexture);

    // Now you can use savedFrameTexture later as needed
    DirectX::ScratchImage image;
    HRESULT hr = DirectX::CaptureTexture(Graphical::GetDevice().Get(), Graphical::GetDevice().GetContext(),
                                         savedFrameTexture_, image);
    if (SUCCEEDED(hr))
    {
        const DirectX::Image* img = image.GetImage(0, 0, 0);
        DirectX::SaveToTGAFile(*img, L"screen.tga");
    }

    // Don't forget to release COM objects when you're done with them
    backBufferResource->Release();
    backBufferTexture->Release();
}

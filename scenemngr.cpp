#include "scenemngr.h"

#include "dark.h"
#include "fire.h"
#include "game.h"
#include "prep.h"
#include "title.h"
#include "result.h"
#include "thunder.h"
#include "wind.h"

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
    delete scene_;
    switch (scene)
    {
    case SCENE_TITLE:
        scene_ = new Title(this);
        break;
    case SCENE_MENU:
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

void SceneMngr::DebugMenu()
{
    scene_->DebugMenu();
}

void SceneMngr::ChangeScene(SCENE scene, const std::string& message)
{
    delete scene_;
    switch (scene)
    {
    case SCENE_TITLE:
        scene_ = new Title(this);
        break;
    case SCENE_MENU:
        break;
    case SCENE_PREP:
        scene_ = new Prep(this);
        break;
    case SCENE_GAME:
        scene_ = new Game(this);
        ParseGame(message);
        break;
    case SCENE_RESULT:
        scene_ = new Result(this);
        break;
    default:
        break;
    }
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
}

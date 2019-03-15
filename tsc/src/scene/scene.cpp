/***************************************************************************
 * scene.cpp -- scene handling class
 *
 * Copyright © 2003 - 2011 Florian Richter
 * Copyright © 2012-2019 The TSC Contributors
 ***************************************************************************/
/*
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "scene.hpp"
#include "../core/sprite_manager.hpp"
#include "../core/game_core.hpp"
#include "../core/filesystem/filesystem.hpp"
#include "../input/mouse.hpp"
#include "../input/keyboard.hpp"
#include "../input/joystick.hpp"
#include "../audio/audio.hpp"
#include "../gui/hud.hpp"
#include "../level/level.hpp"
#include "scene_actions.hpp"
#include "01_prologue.hpp"

namespace fs = boost::filesystem;
using namespace std;
using namespace TSC;

cScene::cScene(void)
    : mp_sprite_manager(new cSprite_Manager()),
      mp_camera(new cCamera(mp_sprite_manager)),
      mp_scene_image(new cSprite(mp_sprite_manager))
{
    mp_scene_image->Set_Massive_Type(MASS_PASSIVE);
    mp_scene_image->Set_Active(true);
    mp_scene_image->Set_Spawned(false);
    mp_scene_image->Set_Scale_Affects_Rect(true);
    mp_sprite_manager->Add(mp_scene_image);
}

cScene::~cScene(void)
{
    delete mp_camera;
    delete mp_sprite_manager; // Delete mp_scene_image
}

void cScene::Enter(const GameMode old_mode)
{
    pMouseCursor->Set_Sprite_Manager(mp_sprite_manager);
    pMouseCursor->Set_Active(false);
    gp_hud->Hide();

    pActive_Camera = mp_camera;

    mp_scene_image->Set_Pos(0, -game_res_h, true);
}

void cScene::Leave(const GameMode next_mode)
{
    pMouseCursor->Reset(false);
    pAudio->Fadeout_Music(1000);
    pJoystick->Reset_keys();
}

void cScene::Update(void)
{
    // Execute all actions in the action sequence
    for (auto iter=m_action_sequence.begin(); iter != m_action_sequence.end(); ) {
        if ((*iter)->Execute()) {
            // Remove action when it signals it has finished.
            delete *iter;
            iter = m_action_sequence.erase(iter);
        }
        else {
            // Let it at start, thereby making it available to the input handler functions.
            break;
        }
    }

    pActive_Camera->Update();

    // End the scene if the action sequence is empty, and return
    // to the active level.
    // (Changing Game_Action subsequently causes the main loop
    // to call cScene::Leave() and then destroy it).
    if (m_action_sequence.size() == 0) {
        Game_Action = GA_ENTER_LEVEL;
        Game_Action_Data_Start.add("screen_fadeout", int_to_string(EFFECT_OUT_HORIZONTAL_VERTICAL));
        Game_Action_Data_Start.add("screen_fadeout_speed", "3");
        Game_Action_Data_Middle.add("load_level", pActive_Level->Get_Level_Name().c_str());
        //Game_Action_Data_Middle.add("load_level_entry", str_entry.c_str()); // TODO: Use this to prevent player reset to initial position
        Game_Action_Data_End.add("screen_fadein", int_to_string(EFFECT_IN_BLACK));
        Game_Action_Data_End.add("screen_fadein_speed", "3");
        //Game_Action_Data_End.add("activate_level_entry", str_entry.c_str());
    }
}

void cScene::Draw(void)
{
    // clear
    pVideo->Clear_Screen();
    // Draw scene
    mp_sprite_manager->Draw_Items();
}

void cScene::Set_Scene_Image(std::string scene_image)
{
    mp_scene_image->Set_Image(pVideo->Get_Surface(utf8_to_path(scene_image)), true);
}

bool cScene::Key_Down(const sf::Event& evt)
{
    return m_action_sequence[0]->Key_Down(evt);
}

bool cScene::Key_Up(const sf::Event& evt)
{
    return m_action_sequence[0]->Key_Up(evt);
}

bool cScene::Mouse_Down(sf::Mouse::Button button)
{
    return m_action_sequence[0]->Mouse_Down(button);
}

bool cScene::Mouse_Up(sf::Mouse::Button button)
{
    return m_action_sequence[0]->Mouse_Up(button);
}

bool cScene::Joy_Button_Down(unsigned int button)
{
    return m_action_sequence[0]->Joy_Button_Down(button);
}

bool cScene::Joy_Button_Up(unsigned int button)
{
    return m_action_sequence[0]->Joy_Button_Up(button);
}

cScene* cScene::Load_Scene(enum Scenes scene)
{
    switch (scene) {
    case SCENE_PROLOGUE:
        return new cPrologueScene();
    default:
        throw(std::runtime_error("Unknown scene requested; terminating."));
    }
}

// Currently running scene, NULL if no scene is active
cScene* TSC::pActive_Scene = nullptr;

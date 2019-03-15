/***************************************************************************
 * scene_actions.cpp  -  Individual possible actions in a cScene
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

#include "scene_actions.hpp"
#include "../audio/audio.hpp"
#include "scene.hpp"

namespace fs = boost::filesystem;
using namespace std;
using namespace TSC;
using namespace TSC::SceneActions;

Action::Action(cScene* p_scene)
    : mp_scene(p_scene)
{
}

Action::~Action()
{
}

ImageChange::ImageChange(cScene* p_scene, std::string scene_image)
    : Action(p_scene),
      m_scene_image(scene_image)
{
}

bool ImageChange::Execute()
{
    mp_scene->Set_Scene_Image(m_scene_image);
    return true;
}

MusicChange::MusicChange(cScene* p_scene, std::string music)
    : Action(p_scene),
      m_music(music)
{
}

bool MusicChange::Execute()
{
    pAudio->Play_Music(m_music, true, 0, 1000);
    return true;
}

WaitReturn::WaitReturn(cScene* p_scene)
    : Action(p_scene),
      m_return_pressed(false)
{
}

bool WaitReturn::Execute()
{
    return m_return_pressed;
}

bool WaitReturn::Key_Down(const sf::Event& evt)
{
    if (evt.key.code == sf::Keyboard::Return) {
        m_return_pressed = true;
        return true;
    }

    return false;
}

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

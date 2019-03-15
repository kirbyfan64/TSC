/***************************************************************************
 * mrb_scene.cpp
 *
 * Copyright © 2012-2019 The TSC Contributors
 ***************************************************************************
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "mrb_scene.hpp"
#include "../../../scene/scene.hpp"
#include "../../../core/game_core.hpp"

/**
 * Module: Scene
 *
 * Module for playing the visual novel scenes.
 */

using namespace TSC;
using namespace TSC::Scripting;

// Helper method
static void Play_Scene(enum Scenes scene)
{
    Game_Action = GA_ENTER_SCENE;
    Game_Action_Data_Start.add("screen_fadeout_speed", "1.5");
    Game_Action_Data_Middle.add("load_scene", int_to_string(scene));
    Game_Action_Data_End.add("screen_fadein_speed", "1.5");
}

/**
 * Method: Scene::play
 *
 *   play( scene )
 *
 * Calling this method plays the scene with the name `scene`, which
 * has to be a symbol, e.g. `:prologue` (to play the prologue scene).
 *
 * This method returns, but on the next frame update after return
 * the game mode switches to scene mode. Once the scene has completed,
 * the game mode returns to level mode in the level that was active
 * when scene play was started.
 *
 * *Remark*: This means that this method does not block until the
 * scene has finished!
 */
static mrb_value Play(mrb_state* p_state, mrb_value self)
{
    mrb_sym ident;
    mrb_get_args(p_state, "n", &ident);
    std::string identstr = mrb_sym2name(p_state, ident);

    if (identstr == "prologue") {
        Play_Scene(SCENE_PROLOGUE);
    }
    else {
        mrb_raisef(p_state, MRB_ARGUMENT_ERROR(p_state), "Invalid scene '%s'.", identstr.c_str());
    }

    return mrb_nil_value();
}

void TSC::Scripting::Init_Scene(mrb_state* p_state)
{
    struct RClass* p_rmScene = mrb_define_module(p_state, "Scene");

    mrb_define_module_function(p_state, p_rmScene, "play", Play, MRB_ARGS_REQ(1));
}

/***************************************************************************
 * scene_actions.hpp
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

#ifndef TSC_SCENE_ACTIONS_HPP
#define TSC_SCENE_ACTIONS_HPP
#include "../core/global_basic.hpp"

namespace TSC {

    // forward-declare
    class cScene;

    namespace SceneActions {

        class Action {
        public:
            Action(cScene* p_scene);
            virtual ~Action(void);

            /* This method is called once the action is active. If
             * it returns true, the action is treated as terminated.
             * If it returns false, the action is assumed to be still
             * running. If false is returned, Execute() will be called
             * again once a frame (in cScene::Update()). Prepare for
             * that. Execute() is mostly Update() for actions. */
            virtual bool Execute() = 0;

            // Only if Execute() returned false: the following event
            // handlers will be called when this action is active.
            // They do nothing by default.
            virtual bool Key_Down(const sf::Event& evt) {return false;}
            virtual bool Key_Up(const sf::Event& evt) {return false;}
            virtual bool Joy_Button_Down(unsigned int button) {return false;}
            virtual bool Joy_Button_Up(unsigned int button) {return false;}
            virtual bool Mouse_Down(sf::Mouse::Button button) {return false;}
            virtual bool Mouse_Up(sf::Mouse::Button button) {return false;}

            cScene* mp_scene;
        };

        class ImageChange: public Action {
        public:
            ImageChange(cScene* p_scene, std::string scene_image);
            virtual bool Execute();
            std::string m_scene_image;
        };

        class MusicChange: public Action {
        public:
            MusicChange(cScene* p_scene, std::string music);
            virtual bool Execute();
            std::string m_music;
        };

        class WaitReturn: public Action {
        public:
            WaitReturn(cScene* p_scene);
            virtual bool Execute();
            virtual bool Key_Down(const sf::Event& evt);
        private:
            bool m_return_pressed;
        };

        class WaitTime: public Action {
        };

        class Narration: public Action {
        public:
            Narration(cScene* p_scene, std::initializer_list<std::string> messages);
            virtual bool Execute();
            virtual bool Key_Down(const sf::Event& evt);
            std::vector<std::string> m_messages;
        };

    }

}

#endif /* TSC_SCENE_ACTIONS_HPP */

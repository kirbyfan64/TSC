#ifndef TSC_LEVEL_EDITOR_HPP
#define TSC_LEVEL_EDITOR_HPP
#include "../core/editor/editor.hpp"

namespace TSC {
    class cEditor_Level: public cEditor
    {
    public:
        cEditor_Level();
        virtual ~cEditor_Level();

        virtual void Enable(cSprite_Manager* p_sprite_manager);
        virtual void Disable(void);
        void Set_Level(cLevel* p_level);

        virtual std::string Status_Bar_Ident() const;

        virtual bool Key_Down(const sf::Event& evt);

        // Menu functions
        virtual bool Function_New(void);
        virtual void Function_Load(void);
        virtual void Function_Save(bool with_dialog = 0);
        virtual void Function_Save_as(void);
        virtual void Function_Delete(void);
        virtual void Function_Settings(void);

        virtual vector<cSprite*> Parse_Items_File();

        cLevel_Settings m_settings_screen;

    private:
        static std::vector<cSprite*> items_loader_callback(const std::string& name, XmlAttributes& attributes, int engine_version, cSprite_Manager* p_sprite_manager, void* p_data);
        void load_background_images_into_cegui();
        bool cycle_object_massive_type(cSprite* obj) const;
        cLevel* mp_level;
        size_t m_focused_exit;
    };

    extern cEditor_Level* pLevel_Editor;
}

#endif

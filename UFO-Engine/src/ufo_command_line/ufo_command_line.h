#ifndef UFO_COMMAND_LINE
#define UFO_COMMAND_LINE

#include "../graphics_provider/graphics_provider.h"
#include "../external/olcPixelGameEngine.h"
#include "../console/console.h"

class ufoCommandLine{
public:

    std::string non_shift  = "abcdefghijklmnopqrstuvwxyz1234567890+-',.< ";
    std::string with_shift = "ABCDEFGHIJKLMNOPQRSTUVWXYZ!2#¤%&/()=?_*;:> ";
    std::string with_altgr = "abcdefghijklmnopqrstuvwxyz¡@£$€¥{[]}\n-',.| ";

    std::vector<olc::Key> key_to_char_index{
        olc::A,
        olc::B,
        olc::C,
        olc::D,
        olc::E,
        olc::D,
        olc::E,
        olc::F,
        olc::G,
        olc::H,
        olc::I,
        olc::J,
        olc::K,
        olc::L,
        olc::M,
        olc::N,
        olc::O,
        olc::P,
        olc::Q,
        olc::R,
        olc::S,
        olc::T,
        olc::U,
        olc::V,
        olc::W,
        olc::X,
        olc::Y,
        olc::Z,
        olc::K1,
        olc::K2,
        olc::K3,
        olc::K4,
        olc::K5,
        olc::K6,
        olc::K7,
        olc::K8,
        olc::K9,
        olc::K0,
        olc::EQUALS
    };

    static ufoCommandLine& Get(){
        static ufoCommandLine cl;
        return cl;
    }

public:
    static void Update(){
        std::string input = "";

        std::vector<int> buttons_pressed;
        bool shift = false;
        bool altgr = false;
        if(GraphicsProvider::Get().GetKey(olc::SHIFT).bPressed) shift = true;
        for(int i = 0; i < (int)olc::Key::ENUM_END; i++){
            if(GraphicsProvider::Get().GetKey((olc::Key)i).bPressed){
                input += ufoCommandLine::Get().non_shift[ufoCommandLine::Get().key_to_char_index[i]];
            }
        }

        auto screen_size =  GraphicsProvider::Get().GetScreenSize();
        Console::Out(input);
    }
};

#endif
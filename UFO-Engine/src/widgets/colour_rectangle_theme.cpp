#include "../console/console.h"
#include "theme.h"
#include "../ufo_maths/ufo_maths.h"
#include "widget.h"
#include "../ufo_engine/ufo_engine.h"
#include "../external/olcPixelGameEngine.h"
#include "../graphics/graphics.h"
#include "widget.h"
#include "colour_rectangle_theme.h"
#include "../graphics/graphics.h"

ColourRectangleTheme::ColourRectangleTheme(Colour _colour): colour{_colour}{}

void ColourRectangleTheme::OnDraw(Widget* _widget){
    
    Graphics::Get().DrawRectangleFilled(_widget->GetRectangle().position, _widget->GetRectangle().size, colour);

}
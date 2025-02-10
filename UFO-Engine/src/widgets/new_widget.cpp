#include <memory>
#include <stdexcept>
#include "new_widget.h"
#include "button.h"
#include "../json/json.h"

std::unique_ptr<WidgetBounds> NewWidget(Json* _json){
    if (_json->GetAsString("Type") == "WidgetBounds") return WidgetBounds::NewFromJson(_json);
    if (_json->GetAsString("Type") == "Button") return Button::NewFromJson(_json);
    throw std::runtime_error("[!] [new_widget.h] : No widget could be returned");
}
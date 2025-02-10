#ifndef NEW_WIDGET_H
#define NEW_WIDGET_H
#include <memory>
#include "widget_bounds.h"

std::unique_ptr<WidgetBounds> NewWidget(Json* _json);

#endif
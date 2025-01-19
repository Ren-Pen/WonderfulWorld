#pragma once
#ifndef FILE_SLIMENANO_UI_COMPONENT_H
    #define FILE_SLIMENANO_UI_COMPONENT_H

namespace slimenano::ui {

class view {
public:
    view();
    virtual ~view();

    virtual void draw() = 0;
    virtual void update() = 0;

protected:
    int x, y;
    int width, height;
};

} // namespace slimenano

#endif // FILE_SLIMENANO_UI_COMPONENT_H
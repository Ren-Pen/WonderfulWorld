#pragma once
#ifndef __FILE_SLIMENANO_UI_COMPONENT_H
    #define __FILE_SLIMENANO_UI_COMPONENT_H

namespace slimenano {

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

#endif // __FILE_SLIMENANO_UI_COMPONENT_H
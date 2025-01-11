#pragma once
#ifndef __FILE_SLIMENANO_UI_STYLE_H
    #define __FILE_SLIMENANO_UI_STYLE_H

namespace slimenano {

    namespace ui{

        class style{
            public:
                void set_style(const char* name, const void* buffer);
                void get_style(const char* name, void* buffer);
        };

    }


} // namespace slimenano

#endif // __FILE_SLIMENANO_UI_STYLE_H
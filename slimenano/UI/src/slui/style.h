#pragma once
#ifndef FILE_SLIMENANO_UI_STYLE_H
#    define FILE_SLIMENANO_UI_STYLE_H

namespace slimenano::ui {

    class style {
      public:
        void set_style(const char *name, const void *buffer);
        void get_style(const char *name, void *buffer);
    };

} // namespace slimenano::ui

#endif // FILE_SLIMENANO_UI_STYLE_H
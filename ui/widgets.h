//
// Created by kingtous on 2021/2/12.
// 用于创建各种窗口、ui
// mail: kingtous@qq.com
//

#ifndef SYSYPLUS_COMPILER_WIDGETS_H
#define SYSYPLUS_COMPILER_WIDGETS_H
#include <gtkmm/application.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>

#define WINDOW_NAME "SySyCompiler By Kingtous"

extern Glib::RefPtr<Gtk::Application> m_app;

/**
 * 初始化窗口
 */
int initWindow(int argc,
                char** argv,
                const char* glade_path,
                const char* window_name);

#endif //SYSYPLUS_COMPILER_WIDGETS_H

//
// Created by kingtous on 2021/2/12.
// 用于创建各种窗口、ui
// mail: kingtous@qq.com
//

#ifndef SYSYPLUS_COMPILER_WIDGETS_H
#define SYSYPLUS_COMPILER_WIDGETS_H
#include <gtkmm/application.h>
#include <gtkmm/applicationwindow.h>
#include <gtkmm/window.h>
#include <gtkmm/builder.h>
#include <giomm/simpleactiongroup.h>

#define WINDOW_NAME "SySyCompiler By Kingtous"

using namespace Gtk;
using namespace Glib;
using namespace Gio;

extern Glib::RefPtr<Gtk::Application> m_app;

/**
 * 初始化窗口
 */
int initWindow(int argc,
                char** argv,
                const char* glade_path,
                const char* window_name);

class CompilerWindow : public Gtk::ApplicationWindow {
public:
    CompilerWindow();

    CompilerWindow(BaseObjectType* cobject, const Glib::RefPtr<Gtk::Builder>& builder);

    /**
     * 初始化
     */
    void init();

    void initMenuBar();
    void on_menu_file_new_activate();
    void onFileOpen();
    void onFileExit();

private:
    RefPtr<SimpleActionGroup> file_action_group;
};

#endif //SYSYPLUS_COMPILER_WIDGETS_H

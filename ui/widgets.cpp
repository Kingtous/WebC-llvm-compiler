//
// Created by kingtous on 2021/2/12.
//

#include <ErrHelper.h>
#include "widgets.h"

using namespace Gtk;

Glib::RefPtr<Gtk::Application> m_app;
RefPtr<Gtk::Builder> builder;

int initWindow(int argc, char **argv, const char *glade_path, const char *window_name) {
    m_app = Gtk::Application::create(argc, argv, window_name, Gio::APPLICATION_FLAGS_NONE);
    builder = Gtk::Builder::create_from_file(glade_path);
    CompilerWindow *window = nullptr;
    builder->get_widget_derived("main_window",window);
    if (window == nullptr) {
        LogError("main_window 未找到");
        return -1;
    }
    window->set_size_request(1200,800);
    window->init();
    return m_app->run(*window);
}

void CompilerWindow::initMenuBar() {
    // 文件选项卡
    file_action_group = Gio::SimpleActionGroup::create();
    file_action_group->add_action("新建", sigc::mem_fun(*this, &CompilerWindow::on_menu_file_new_activate));
    file_action_group->add_action("打开", sigc::mem_fun(*this, &CompilerWindow::onFileOpen));
    file_action_group->add_action("退出", sigc::mem_fun(*this, &CompilerWindow::onFileExit));
    insert_action_group("文件", file_action_group);
    Gtk::ImageMenuItem *item;
    builder->get_widget("menu_help_about", item);
    item->signal_select().connect([]() {
        Gtk::AboutDialog *dialog;
        builder->get_widget("main_about", dialog);
        dialog->show();
    });
}

CompilerWindow::CompilerWindow(BaseObjectType* cobject,
                               const Glib::RefPtr<Gtk::Builder>& builder)
                               : Gtk::ApplicationWindow(cobject){

}

void CompilerWindow::on_menu_file_new_activate() {
    LogError("on menu file new activate");
}

void CompilerWindow::onFileOpen() {

}

void CompilerWindow::onFileExit() {

}

void CompilerWindow::init() {
    set_show_menubar(true);
    initMenuBar();
}


CompilerTextView::CompilerTextView() {

}

CompilerTextView::CompilerTextView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) {

}

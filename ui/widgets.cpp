//
// Created by kingtous on 2021/2/12.
//

#include <ErrHelper.h>
#include "widgets.h"

Glib::RefPtr<Gtk::Application> m_app;

int initWindow(int argc, char **argv, const char *glade_path, const char *window_name) {
    m_app = Gtk::Application::create(argc, argv, window_name, Gio::APPLICATION_FLAGS_NONE);
    auto builder = Gtk::Builder::create_from_file(glade_path);
    Gtk::Window *window = nullptr;
    builder->get_widget("main_window", window);
    if (window == nullptr) {
        LogError("main_window 未找到");
        return -1;
    }
    window->set_size_request(1200,800);
    return m_app->run(*window);
}

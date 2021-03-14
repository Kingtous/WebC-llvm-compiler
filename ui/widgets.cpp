//
// Created by kingtous on 2021/2/12.
//

#include <ErrHelper.h>
#include "widgets.h"

using namespace Gtk;

Glib::RefPtr<Gtk::Application> m_app;
RefPtr<Gtk::Builder> builder;
CompilerWindow *window = nullptr;

int initWindow(int argc, char **argv, const char *glade_path, const char *window_name = WINDOW_NAME) {
    m_app = Gtk::Application::create(argc, argv, window_name, Gio::APPLICATION_FLAGS_NONE);
    try {
        builder = Gtk::Builder::create_from_file(glade_path);
        window = nullptr;
        builder->get_widget_derived("main_window", window);
        if (window == nullptr) {
            LogError("main_window 未找到");
            return -1;
        }
        window->set_size_request(1000, 800);
        window->init();
        return m_app->run(*window);
    } catch (BuilderError &e) {
        LogErrorV(e.what().c_str());
    }
    return -1;
}

long writeFile(RefPtr<File> file_ptr, const std::string &content) {
    if (file_ptr->query_exists()){
        file_ptr->remove();
    }
    auto output_stream = file_ptr->create_file();
    long res = output_stream->write(content);
    output_stream->close();
    return res;
}

long writeFile(RefPtr<File> file_ptr, const void *buffer, long sz) {
    if (file_ptr->query_exists()) {
        file_ptr->remove();
    }
    auto output_stream = file_ptr->create_file();
    long res = output_stream->write(buffer, sz);
    output_stream->close();
    return res;
}

void logOnUi(const char *Str) {
    if (window != nullptr) {
        window->log(Str);
    }
}

void CompilerWindow::initMenuBar() {
    m_main_about->signal_response().connect([&](int resp) {
        if (resp == RESPONSE_DELETE_EVENT) {
            m_main_about->hide();
        }
    });
    m_menu_help_about->signal_activate().connect([=]() {
        m_main_about->show();
    });
    // 新建文件
    m_menu_file_new->signal_activate().connect([&](){
        m_file.clear();
        m_textview->get_buffer()->set_text("");
        setTitle(WINDOW_NAME);
    });
    // 打开文件
    m_menu_file_open->signal_activate().connect([&]() {
        auto dialog = new FileChooserDialog("选择一个Sysy文件", FileChooserAction::FILE_CHOOSER_ACTION_OPEN);
        dialog->set_transient_for(*this);
        dialog->add_button("打开", RESPONSE_OK);
        dialog->add_button("取消", RESPONSE_CANCEL);
        dialog->signal_response().connect([=](int respId) {
            switch (respId) {
                case RESPONSE_OK:
                    string s;
                    m_file = dialog->get_file();
                    this->set_title(m_file->get_basename());
                    auto io = m_file->open_readwrite();
                    auto input = io->get_input_stream();
                    auto buf = new char[INIT_IOBUF];
                    int read = input->read(buf,INIT_IOBUF);
                    while(read != 0){
                        s.append(buf,read);
                        read = input->read(buf,INIT_IOBUF);
                    }
                    m_textview->get_buffer()->set_text(s);
                    input->close();
                    io->close();
                    break;
            }
            dialog->close();
            delete dialog;
        });
        dialog->show();
    });
    // 保存文件
    m_menu_file_save->signal_activate().connect([&]() {
        if (m_file.get() == nullptr){
            auto dialog = new FileChooserDialog(*this, "保存文件", FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
            dialog->set_do_overwrite_confirmation(true);
            dialog->add_button("保存", RESPONSE_OK);
            dialog->add_button("取消", RESPONSE_CANCEL);
            dialog->signal_response().connect([=](int resp) {
                if (resp == RESPONSE_OK) {
                    writeFile(dialog->get_file(),
                              string(m_textview->get_buffer()->begin(),m_textview->get_buffer()->end()));
                    setLatestMessage("保存成功");
                }
                dialog->close();
                delete dialog;
            });
            dialog->show();
        } else {
            writeFile(m_file,
                      string(m_textview->get_buffer()->begin(),m_textview->get_buffer()->end()));
            setLatestMessage("保存成功");
        }
    });
    // 另存为
    m_menu_file_saveas->signal_activate().connect([&]() {
        auto dialog = new FileChooserDialog(*this, "另存为", FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
        dialog->set_do_overwrite_confirmation(true);
        dialog->add_button("另存为", RESPONSE_OK);
        dialog->add_button("取消", RESPONSE_CANCEL);
        dialog->set_filename("file.kc");
        dialog->signal_response().connect([=](int resp) {
            if (resp == RESPONSE_OK) {
                writeFile(dialog->get_file(),
                          string(m_textview->get_buffer()->begin(), m_textview->get_buffer()->end()));
                setLatestMessage("保存成功");
            }
            dialog->close();
            delete dialog;
        });
        dialog->show();
    });
    // 构建
    m_menu_build_compile->signal_activate().connect([&]() {
        if (getMState() == IN_EDIT) {
            setStatus(IN_BUILD);
            setLatestMessage("正在编译");
            auto thread = Thread::create([&]() {
                // TODO 编译逻辑
                LogError("building");
                setStatus(IN_EDIT);
            });

        } else {
            setLatestMessage("正在编译，稍后再次尝试编译");
        }
    });
}

CompilerWindow::CompilerWindow(BaseObjectType *cobject,
                               const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::ApplicationWindow(cobject) {
    // 编辑器
    builder->get_widget("main_code_form", m_textview);
    m_textview->set_name("codeForm");
    // 关于
    builder->get_widget("main_about", m_main_about);
    // 选择文件
    builder->get_widget("main_file_chooser", m_main_filechooserdialog);
    // 菜单
    builder->get_widget("menu_file_new", m_menu_file_new);
    builder->get_widget("menu_file_open", m_menu_file_open);
    builder->get_widget("menu_file_save", m_menu_file_save);
    builder->get_widget("menu_file_saveas", m_menu_file_saveas);
    builder->get_widget("menu_file_quit", m_menu_file_quit);
    builder->get_widget("menu_help_about", m_menu_help_about);
    builder->get_widget("menu_build_compile", m_menu_build_compile);
    builder->get_widget("menu_build_run", m_menu_build_run);
    builder->get_widget("main_bottom_status_label", m_main_bottom_status_label);
    builder->get_widget("main_build_notebook", m_main_build_notebook);
    builder->get_widget("main_build_console", m_main_build_console);
    builder->get_widget("main_static_analysis_console", m_main_static_analysis_console);
    builder->get_widget("main_runtime_console", m_main_runtime_console);
}

void CompilerWindow::on_menu_file_new_activate() {
    LogError("on menu file new activate");
}

void CompilerWindow::onFileOpen() {

}

void CompilerWindow::onFileExit() {
    close();
}

void CompilerWindow::init() {
    this->m_state = CompilerWindow::IN_EDIT;
    set_show_menubar(true);
    initMenuBar();
    initCodeForm();
}

void CompilerWindow::setTitle(const ustring &str) {
    this->set_title(str);
}

void CompilerWindow::setLatestMessage(const ustring &msg) {
    // TODO
}

void CompilerWindow::initCodeForm() {
//    Pango::FontMap
// TODO 增加字体可修改
    try {
        auto style_context = get_style_context();
        style_context->add_class("");
        auto provider = Gtk::CssProvider::create();
        provider->load_from_path("../ui/css/main.css");
        auto display = Gdk::Display::get_default();
        auto screen = display->get_default_screen();
        style_context->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
        style_context->add_provider_for_screen(screen, provider, GTK_STYLE_PROVIDER_PRIORITY_USER);
    } catch (CssProviderError &e) {
        LogErrorV(e.what().c_str());
    }
}

void CompilerWindow::log(const char *string) {
    switch (m_state) {
        case M_STATUS::IN_EDIT:
            m_main_static_analysis_console->get_buffer()
                    ->insert(m_main_static_analysis_console->get_buffer()->end(), string);
        case M_STATUS::IN_BUILD:
            m_main_build_console->get_buffer()
                    ->insert(m_main_build_console->get_buffer()->end(), string);
        case M_STATUS::IN_RUNNING:
            m_main_runtime_console->get_buffer()
                    ->insert(m_main_runtime_console->get_buffer()->end(), string);
            break;
        default:
            break;
    }
}

CompilerWindow::CompilerWindow() {
}

void CompilerWindow::setStatus(M_STATUS status) {
    this->m_state = status;
}

CompilerWindow::M_STATUS CompilerWindow::getMState() const {
    return m_state;
}


CompilerTextView::CompilerTextView() {
}

CompilerTextView::CompilerTextView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) {

}

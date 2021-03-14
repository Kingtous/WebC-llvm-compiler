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
    builder->get_widget_derived("main_window", window);
    if (window == nullptr) {
        LogError("main_window 未找到");
        return -1;
    }
    window->set_size_request(1200, 800);
    window->init();
    return m_app->run(*window);
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
    if (file_ptr->query_exists()){
        file_ptr->remove();
    }
    auto output_stream = file_ptr->create_file();
    long res = output_stream->write(buffer,sz);
    output_stream->close();
    return res;
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
    //
    m_menu_file_saveas->signal_activate().connect([&]() {
        auto dialog = new FileChooserDialog(*this, "另存为", FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
        dialog->set_do_overwrite_confirmation(true);
        dialog->add_button("另存为", RESPONSE_OK);
        dialog->add_button("取消", RESPONSE_CANCEL);
        dialog->set_filename("file.kc");
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
    });
}

CompilerWindow::CompilerWindow(BaseObjectType *cobject,
                               const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::ApplicationWindow(cobject) {
    // 编辑器
    builder->get_widget("main_code_form", m_textview);
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
    m_textview->set_monospace(true);
}


CompilerTextView::CompilerTextView() {

}

CompilerTextView::CompilerTextView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder) {

}

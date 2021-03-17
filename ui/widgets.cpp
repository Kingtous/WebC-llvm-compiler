//
// Created by kingtous on 2021/2/12.
//

#include <ErrHelper.h>
#include <gtksourceviewmm.h>
#include "widgets.h"

using namespace Gtk;

Glib::RefPtr<Gtk::Application> m_app;
RefPtr<Gtk::Builder> builder;
CompilerWindow *window = nullptr;

RefPtr<Glib::IOChannel> io_err_channel;
RefPtr<Glib::IOChannel> io_output_channel;
RefPtr<Glib::IOChannel> io_intput_channel;

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
    }
    catch (BuilderError &e) {
        LogErrorV(e.what().c_str());
    }
    return -1;
}

long writeFile(RefPtr<File> file_ptr, const std::string &content) {
    if (file_ptr->query_exists()) {
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
        window->log("\n");
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
    m_menu_file_new->signal_activate().connect([&]() {
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
                    int read = input->read(buf, INIT_IOBUF);
                    while (read != 0) {
                        s.append(buf, read);
                        read = input->read(buf, INIT_IOBUF);
                    }
                    m_textview->get_buffer()->set_text(s);
                    setTitle(m_file->get_basename());
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
        if (m_file.get() == nullptr) {
            auto dialog = new FileChooserDialog(*this, "保存文件", FileChooserAction::FILE_CHOOSER_ACTION_SAVE);
            dialog->set_do_overwrite_confirmation(true);
            dialog->add_button("保存", RESPONSE_OK);
            dialog->add_button("取消", RESPONSE_CANCEL);
            dialog->signal_response().connect([=](int resp) {
                if (resp == RESPONSE_OK) {
                    m_file = dialog->get_file();
                    writeFile(dialog->get_file(),
                              string(m_textview->get_buffer()->begin(), m_textview->get_buffer()->end()));
                    setLatestMessage("保存成功");
                    setTitle(m_file->get_basename());
                }
                dialog->close();
                delete dialog;
            });
            dialog->show();
        } else {
            writeFile(m_file,
                      string(m_textview->get_buffer()->begin(), m_textview->get_buffer()->end()));
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
    // 构建为目标文件
    m_menu_build_compile->signal_activate().connect([&]() {
        if (m_file.get() == nullptr) {
            // 先调用保存
            m_menu_file_save->activate();
            return;
        }
        // ADD
    });
    // 构建为目标文件
    m_menu_build_compile->signal_activate().connect([&]() {
        if (m_file.get() == nullptr) {
            // 先调用保存
            m_menu_file_save->activate();
            return;
        }
        std::set<ArgsParser::Options> s;
        s.insert(ArgsParser::Options::OUTPUT_EXECUTABLE);
        auto code_buffer = m_gsv->get_buffer()->property_text().get_value();
        auto path = m_file->get_path() + ".o";
        buildSrc(s, code_buffer, path);
    });
    // 构建为汇编文件
    m_menu_build_compile_asm->signal_activate().connect([&]() {
        if (m_file.get() == nullptr) {
            // 先调用保存
            m_menu_file_save->activate();
            return;
        }
        std::set<ArgsParser::Options> s;
        s.insert(ArgsParser::Options::OUTPUT_LLVMAS_FILE);
        auto code_buffer = m_gsv->get_buffer()->property_text().get_value();
        auto path = m_file->get_path() + ".asm";
        buildSrc(s, code_buffer, path);
    });
    // 执行文件
    m_menu_build_run->signal_activate().connect([&]() {
        if (m_file.get() == nullptr) {
            // 先调用保存
            m_menu_file_save->activate();
            return;
        }
        // 编译
        std::set<ArgsParser::Options> s;
        s.insert(ArgsParser::Options::OUTPUT_EXECUTABLE);
        auto code_buffer = m_gsv->get_buffer()->property_text().get_value();
        auto path = m_file->get_path() + ".o";
        // 生成exe
        buildSrc(s, code_buffer, path, [](CompilerWindow *window) {
//            window->m_main_build_notebook->set_current_page(RUNTIME_PAGE_ID);
            // 成功回调,g++链接
            Glib::signal_idle().connect_once([=]() {
                auto obj_name = window->m_file->get_path() + ".o";
                auto exe_name = window->m_file->get_path() + ".exe";
                const vector<string> argv = {"/usr/bin/g++", obj_name, "../cmake-build-debug/module/web/libweb.a",
                                             "../cmake-build-debug/module/time/libtime.o", "-o", exe_name};
                GPid outputId;
                GPid inputId;
                GPid errId;
                GPid pid;
                try {
                    Glib::spawn_async_with_pipes("", argv, SPAWN_DEFAULT, SlotSpawnChildSetup(),
                                                 &pid, &inputId, &outputId, &errId);
                    Glib::signal_child_watch().connect([=](GPid pid, int status) {
                        if (status != 0) {
                            // g++编译出错了
                            auto uis = Gio::UnixInputStream::create(errId, true);
                            uis->close();
                        } else {
                            // 运行
                            window->m_main_build_notebook->set_current_page(RUNTIME_PAGE_ID);
                            const vector<string> argv = {exe_name};
                            int pid, inputId, outputId, errId;
                            Glib::spawn_async_with_pipes("", argv, SPAWN_DEFAULT, SlotSpawnChildSetup(),
                                                         &pid, &inputId, &outputId, &errId);
                            // 在窗口的线程池执行
                            boost::asio::post(window->threads,[=](){
                                try{
                                    auto uis = Gio::UnixInputStream::create(outputId, true);
                                    auto buf = new char[INIT_IOBUF];
                                    int read = uis->read(buf, INIT_IOBUF);
                                    while (true) {
                                        if (read != 0){
                                            window->log(buf,M_STATUS::IN_RUNNING);
                                        }
                                        memset(buf,0,INIT_IOBUF);
                                        read = uis->read(buf, INIT_IOBUF);
                                        // 休眠500ms
                                        boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
                                    }
                                    uis->close();
                                } catch (Glib::Error& e) {
                                    window->log(e.what().c_str());
                                }
                                window->setStatus(M_STATUS::IN_EDIT);
                            });
                        }
                    }, pid);
                } catch (SpawnError &e) {
                    window->log(e.what().c_str());
                }
            });
        });
    });
}

CompilerWindow::CompilerWindow(BaseObjectType *cobject,
                               const Glib::RefPtr<Gtk::Builder> &builder)
        : Gtk::ApplicationWindow(cobject) {
    g_object_set(gtk_settings_get_default(), "gtk-application-prefer-dark-theme", TRUE, NULL);
    builder->get_widget("main_code_window", m_main_code_window);
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
    builder->get_widget("main_control_expander", m_main_control_expander);
    builder->get_widget("main_code_pos_label", m_main_code_pos_label);
    builder->get_widget("menu_build_compile_asm", m_menu_build_compile_asm);
    m_tip_buffer = RefPtr<TextBuffer>::cast_dynamic(builder->get_object("code_suggestion_buffer"));
}

void CompilerWindow::on_menu_file_new_activate() {
    LogError("on menu file new activate");
}

void CompilerWindow::onFileOpen() {
}

void CompilerWindow::onFileExit() {
    //    close();
}

void CompilerWindow::init() {
    // 设置为居中
    set_position(Gtk::WindowPosition::WIN_POS_CENTER);
    log_mutex = new Glib::Mutex();
    this->m_state = CompilerWindow::IN_EDIT;
    set_show_menubar(true);
    initMenuBar();
    initCodeForm();
}

void CompilerWindow::setTitle(const ustring &str) {
    this->set_title(str);
}

void CompilerWindow::setLatestMessage(const ustring &msg) {
    m_main_bottom_status_label->set_text(msg);
}

void CompilerWindow::initCodeForm() {
    m_last_edit_time = pt::microsec_clock::local_time();
    // 默认展开
    m_main_control_expander->set_expanded(true);
    // m_textview->set_name("codeForm");
    Gsv::init();
    m_gsv = new Gsv::View();
    m_gsv->set_name("code");
    m_textview = m_gsv;
    m_main_code_window->set_policy(PolicyType::POLICY_AUTOMATIC, PolicyType::POLICY_AUTOMATIC);
    m_main_code_window->add(*m_gsv);
    m_gsv->set_visible(true);
    m_gsv->set_hexpand(true);
    m_gsv->set_vexpand(true);
    m_completion_words = Gsv::CompletionWords::create("代码提示", RefPtr<Gdk::Pixbuf>());
    m_completion_words->register_provider(m_tip_buffer);
    m_gsv->get_completion()->add_provider(m_completion_words);
    // 展示函数
    m_gsv->set_show_line_numbers(true);
    // 高亮
    m_gsv->set_highlight_current_line(true);
    m_gsv->set_auto_indent(true);
    m_gsv->set_show_line_marks(true);
    m_gsv->set_insert_spaces_instead_of_tabs(true);
    m_gsv->set_draw_spaces(Gsv::DRAW_SPACES_ALL);
    m_lm = Gsv::LanguageManager::get_default();
    auto lan = m_lm->get_language("c");
    m_gsv->get_source_buffer()->set_language(lan);
    m_gsv->get_source_buffer()->set_highlight_syntax(true);
    m_style_scheme_manager = Gsv::StyleSchemeManager::get_default();
    // style
    auto scheme = m_style_scheme_manager->get_scheme("oblivion");
    m_gsv->get_source_buffer()->set_style_scheme(scheme);
    // TODO 增加字体可修改
    try {
        auto style_context = get_style_context();
        auto provider = Gtk::CssProvider::create();
        provider->load_from_path("../ui/css/main.css");
        auto display = Gdk::Display::get_default();
        auto screen = display->get_default_screen();
        style_context->add_provider(provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    }
    catch (CssProviderError &e) {
        LogErrorV(e.what().c_str());
    }
    m_main_build_notebook->set_current_page(STATIC_ANALYSIS_PAGE_ID);
    // 内容改变
    m_gsv->get_source_buffer()->signal_changed().connect([&]() {
        // 清空一次
        m_main_static_analysis_console->get_buffer()->set_text("");
        m_is_dirty = true;
        m_last_edit_time = pt::microsec_clock::local_time();
    });
        m_gsv->get_source_buffer()->property_cursor_position().signal_changed().connect(
        [&]() {
        auto buffer = m_gsv->get_source_buffer();
        auto v = buffer->property_cursor_position().get_value();
        auto iter = buffer->get_iter_at_offset(v);
        string msg = to_string(iter.get_line() + 1) + "行" + to_string(iter.get_line_index()) + "列";
        m_main_code_pos_label->set_text(msg);
        });

        // 静态分析检查线程
        auto current_state = m_state;
        boost::asio::post(threads,[&, current_state]() {
        while (1) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(1000));
        if (m_is_dirty) {
        // 输入完成后，1s再分析
        auto current_time = pt::microsec_clock::local_time();
        auto delta_time = current_time - m_last_edit_time;
        if (delta_time.total_milliseconds() > 1000 && current_state == M_STATUS::IN_EDIT) {
        signal_idle().connect_once([&]() {
        m_main_build_notebook->set_current_page(STATIC_ANALYSIS_PAGE_ID);
        });
                    analysis(new string(m_gsv->get_source_buffer()->begin(), m_gsv->get_source_buffer()->end()));
                    log("分析完成\n");
                    m_is_dirty = false;
                }
            }
        }
    });
}

void CompilerWindow::log(const char *str, const M_STATUS &state) {
    // 保存调用时log的state
    auto log_state = m_state;
    auto utf8_message = Glib::locale_to_utf8(string(str));
    signal_idle().connect_once([&, utf8_message, state, log_state]() {
        // 加个大锁，在UI线程执行
        log_mutex->lock();
        auto tmp_state = state;
        if (tmp_state == M_STATUS::IN_NONE) {
            tmp_state = log_state;
        }
        switch (tmp_state) {
            case M_STATUS::IN_EDIT:
                m_main_static_analysis_console->get_buffer()
                        ->insert(m_main_static_analysis_console->get_buffer()->end(), utf8_message);
                break;
            case M_STATUS::IN_RUNNING:
                m_main_runtime_console->get_buffer()
                        ->insert(m_main_runtime_console->get_buffer()->end(), utf8_message);
                break;
            case M_STATUS::IN_BUILD:
                m_main_build_console->get_buffer()
                        ->insert(m_main_build_console->get_buffer()->end(), utf8_message);
                break;
            default:
                break;
        }
        log_mutex->unlock();
    });
}

void CompilerWindow::setStatus(M_STATUS status) {
    this->m_state = status;
}

CompilerWindow::M_STATUS CompilerWindow::getMState() const {
    return m_state;
}

int CompilerWindow::buildSrc(const set<ArgsParser::Options> &opts,
                             const ustring &code,
                             const ustring &output_path, void (*onSuccess)(CompilerWindow *)) {
    auto window_pointer = this;
    if (getMState() == IN_EDIT) {
        setStatus(IN_BUILD);
        m_main_build_console->get_buffer()->set_text("");
        boost::asio::post(threads, [&, code, output_path, opts, window_pointer, onSuccess]() {
            m_main_build_notebook->set_current_page(BUILD_PAGE_ID);
            log("正在编译中\n", M_STATUS::IN_BUILD);
            auto code_str = new std::string(code);
            auto default_output_path = m_file->get_path() + ".o";
            int res = build(code_str, output_path.c_str(), opts);
            if (res == ROK) {
                log("编译完成\n", M_STATUS::IN_BUILD);
                if (onSuccess != nullptr) {
                    onSuccess(window_pointer);
                }
            } else {
                log("编译失败\n", M_STATUS::IN_BUILD);
            }
            setStatus(IN_EDIT);
            return res;
        });
    } else {
        setLatestMessage("正在编译，稍后再次尝试编译");
    }
    return false;
}

CompilerWindow::~CompilerWindow() noexcept {

}

bool KingtousCompletionProvider::match_vfunc(const RefPtr<const Gsv::CompletionContext> &context) const {
    // TODO 代码提示
}

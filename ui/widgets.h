//
// Created by kingtous on 2021/2/12.
// 用于创建各种窗口、ui
// mail: kingtous@qq.com
//

#ifndef SYSYPLUS_COMPILER_WIDGETS_H
#define SYSYPLUS_COMPILER_WIDGETS_H

#include <gtkmm.h>
#include <gtksourceviewmm/view.h>
#include <gtksourceviewmm/completionprovider.h>
#include <gtksourceviewmm/completionwords.h>
#include <gtksourceviewmm/styleschememanager.h>
#include <giomm/simpleactiongroup.h>
#include <vector>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <boost/chrono.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "compiler/Compiler.h"

#define WINDOW_NAME "SySyCompiler By Kingtous"
#define BUILD_PAGE_ID 0
#define RUNTIME_PAGE_ID 1
#define STATIC_ANALYSIS_PAGE_ID 2

using namespace Gtk;
using namespace Glib;
using namespace Gio;
namespace pt = boost::posix_time;

extern Glib::RefPtr<Gtk::Application> m_app;

/**
 * 向UI打印日志
 * @param Str
 */
void logOnUi(const char *Str);

/**
 * 初始化窗口
 */
int initWindow(int argc,
               char **argv,
               const char *glade_path,
               const char *window_name);

/**
 * 写入文件
 * @param file_ptr
 * @param content
 * @return
 */
long writeFile(RefPtr<File> file_ptr, const std::string &content);

/**
 *
 * @param file_ptr
 * @param buffer
 * @param sz
 * @return
 */
long writeFile(RefPtr<File> file_ptr, const void *buffer, long sz);

class CompilerWindow : public Gtk::ApplicationWindow {
public:
    enum M_STATUS {
        IN_NONE,
        IN_EDIT,
        IN_BUILD,
        IN_RUNNING,
    };
public:
    CompilerWindow();

    CompilerWindow(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);

    /**
     * 初始化
     */
    void init();

    /**
     * 初始化菜单
     */
    void initMenuBar();

    /**
     * 初始化代码编辑器
     */
    void initCodeForm();

    void on_menu_file_new_activate();

    void onFileOpen();

    void onFileExit();

    void setTitle(const ustring &str);

    void setLatestMessage(const ustring &msg);

    void log(const char *string, const M_STATUS &state = M_STATUS::IN_NONE);

    void setStatus(M_STATUS status);

    /**
     * 编译
     */
    int buildSrc(const set<ArgsParser::Options> &opts,
                 const ustring &code, const ustring &output_path, void (*onSuccess)(CompilerWindow*) = nullptr);

    M_STATUS getMState() const;

private:
    RefPtr<SimpleActionGroup> file_action_group;
    ImageMenuItem *m_menu_file_new;
    ImageMenuItem *m_menu_file_open;
    ImageMenuItem *m_menu_file_save;
    ImageMenuItem *m_menu_file_saveas;
    ImageMenuItem *m_menu_file_quit;
    ImageMenuItem *m_menu_help_about;
    ImageMenuItem *m_menu_build_compile;
    ImageMenuItem *m_menu_build_run;
    ImageMenuItem *m_menu_build_compile_asm;
    TextView *m_textview;
    AboutDialog *m_main_about;
    FileChooserDialog *m_main_filechooserdialog;
    Label *m_main_bottom_status_label;
    Label *m_main_code_pos_label;
    Notebook *m_main_build_notebook;
    TextView *m_main_runtime_console;
    TextView *m_main_build_console;
    TextView *m_main_static_analysis_console;
    ScrolledWindow *m_main_code_window;
    Expander *m_main_control_expander;
    // 暂存数据
    RefPtr<Gio::File> m_file;
    // 状态机
    volatile M_STATUS m_state;
    // buffer
    Gsv::View *m_gsv;
    RefPtr<Gsv::LanguageManager> m_lm;
    Pango::FontDescription *m_font_desc;
    // 1个守护线程，1个编译线程
    boost::asio::thread_pool threads{2};
    bool m_is_dirty = false;
    pt::ptime m_last_edit_time;
    Glib::Mutex *log_mutex;
    RefPtr<TextBuffer> m_tip_buffer;
    RefPtr<Gsv::CompletionWords> m_completion_words;
    RefPtr<Gsv::StyleSchemeManager> m_style_scheme_manager;
};

class KingtousCompletionProvider : public Gsv::CompletionProvider {
public:
private:
    bool match_vfunc(const RefPtr<const Gsv::CompletionContext> &context) const override;
};

#endif //SYSYPLUS_COMPILER_WIDGETS_H

//
// Created by kingtous on 2021/2/12.
// 用于创建各种窗口、ui
// mail: kingtous@qq.com
//

#ifndef SYSYPLUS_COMPILER_WIDGETS_H
#define SYSYPLUS_COMPILER_WIDGETS_H

#include <gtkmm.h>
#include <gtksourceviewmm/view.h>
#include <giomm/simpleactiongroup.h>

#include "compiler/Compiler.h"

#define WINDOW_NAME "SySyCompiler By Kingtous"

using namespace Gtk;
using namespace Glib;
using namespace Gio;

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
long writeFile(RefPtr<File> file_ptr,const std::string& content);

/**
 *
 * @param file_ptr
 * @param buffer
 * @param sz
 * @return
 */
long writeFile(RefPtr<File> file_ptr,const void* buffer,long sz);

class CompilerWindow : public Gtk::ApplicationWindow {
public:
    enum M_STATUS {
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

    void log(const char *string);

    void setStatus(M_STATUS status);

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
    TextView *m_textview;
    AboutDialog *m_main_about;
    FileChooserDialog *m_main_filechooserdialog;
    Label *m_main_bottom_status_label;
    Notebook *m_main_build_notebook;
    TextView *m_main_runtime_console;
    TextView *m_main_build_console;
    TextView *m_main_static_analysis_console;
    // 暂存数据
    RefPtr<Gio::File> m_file;
    // 状态机
    M_STATUS m_state;
};

class CompilerTextView : public Gsv::View {
public:
    CompilerTextView();

    CompilerTextView(BaseObjectType *cobject, const Glib::RefPtr<Gtk::Builder> &builder);
};

#endif //SYSYPLUS_COMPILER_WIDGETS_H

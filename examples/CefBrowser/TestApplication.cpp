#include "TestApplication.h"
#include "MainThread.h"

// duilib
#include "duilib/duilib_cef.h"

TestApplication::TestApplication()
{
}

TestApplication::~TestApplication()
{
}

TestApplication& TestApplication::Instance()
{
    static TestApplication self;
    return self;
}

int TestApplication::Run(int argc, char** argv)
{
    // 创建主线程
    MainThread thread;

    //必须在CefManager::Initialize前调用，设置DPI自适应属性，否则会导致显示不正常
    //初始化全局资源, 使用本地文件夹作为资源
    ui::FilePath resourcePath = ui::GlobalManager::GetDefaultResourcePath(true);
    ui::GlobalManager::Instance().Startup(ui::LocalFilesResParam(resourcePath), thread.GetDpiInitParam());

    //指定CEF模块相关路径
    //ui::FilePath cefBinDir = ui::FilePathUtil::JoinFilePath(ui::FilePathUtil::GetCurrentModuleDirectory(), ui::FilePath("cef.win7"));
    //ui::CefManager::GetInstance()->SetCefMoudlePath(cefBinDir.NativePath());

    //初始化CEF: 必须在GlobalManager初始化完成之后，因为初始化CEF过程中，会用到GlobalManager
    if (!ui::CefManager::GetInstance()->Initialize(true, _T("cef_browser"), argc, argv, nullptr)) {
        return 1;
    }

    //如果未启用CEF的消息循环，则需要启动一个定时器调用CEF的消息处理函数
    if (!ui::CefManager::GetInstance()->IsMultiThreadedMessageLoop()) {
        ui::CefManager::GetInstance()->ScheduleCefDoMessageLoopWork();
    }

    // 执行主线程循环
    thread.RunMessageLoop();

    // 清理 CEF
    ui::CefManager::GetInstance()->UnInitialize();
    return 0;
}

void TestApplication::AddMainWindow(ui::Window* pWindow)
{
    if (pWindow != nullptr) {
        ui::ControlPtrT<ui::Window> pMainWindow(pWindow);
        m_pMainWindows.push_back(pMainWindow);
    }
}

void TestApplication::RemoveMainWindow(ui::Window* pWindow)
{
    auto iter = m_pMainWindows.begin();
    while (iter != m_pMainWindows.end()) {
        if (*iter == pWindow) {
            iter = m_pMainWindows.erase(iter);
        }
        else {
            ++iter;
        }
    }
}

void TestApplication::SetActiveMainWindow(ui::Window* pWindow)
{
    m_pActiveWindow = pWindow;
}

void TestApplication::ActiveMainWindow()
{
    if (m_pActiveWindow != nullptr) {
        m_pActiveWindow->ShowWindow(ui::ShowWindowCommands::kSW_SHOW_NORMAL);
    }
    else {
        for (ui::ControlPtrT<ui::Window> pWindow : m_pMainWindows) {
            if ((pWindow != nullptr) && !pWindow->IsClosingWnd()) {
                pWindow->ShowWindow(ui::ShowWindowCommands::kSW_SHOW_NORMAL);
            }
        }
    }
}

void TestApplication::CloseMainWindow()
{
    for (ui::ControlPtrT<ui::Window> pWindow : m_pMainWindows) {
        if ((pWindow != nullptr) && !pWindow->IsClosingWnd()) {
            pWindow->CloseWnd();
        }
    }
}

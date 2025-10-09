#include "TestApplication.h"
#include "MainThread.h"
#include "CefForm.h"

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
    if (!ui::CefManager::GetInstance()->Initialize(kEnableOffScreenRendering, _T("cef"), argc, argv, nullptr)) {
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

void TestApplication::SetMainWindow(ui::Window* pWindow)
{
    m_pMainWindow = pWindow;
}

void TestApplication::ActiveMainWindow()
{
    if (m_pMainWindow != nullptr) {
        m_pMainWindow->ShowWindow(ui::ShowWindowCommands::kSW_SHOW_NORMAL);
    }
}

void TestApplication::CloseMainWindow()
{
    if ((m_pMainWindow != nullptr) && !m_pMainWindow->IsClosingWnd()) {
        m_pMainWindow->CloseWnd();
    }
}

#include <windows.h>

#include "base/at_exit.h"
#include "base/run_loop.h"
#include "base/message_loop/message_loop.h"
#include "base/time/time.h"
#include "base/bind.h"
#include "base/logging.h"
#include "base/command_line.h"

#include "app/app_d2d.h"

int WINAPI WinMain(HINSTANCE h_instance,
    HINSTANCE h_prev_instance,
    LPSTR cmd_line,
    int cmd_show){

    base::CommandLine::Init(0, nullptr);
    base::AtExitManager at_exit;

    logging::LoggingSettings settings;
    settings.logging_dest = logging::LOG_TO_ALL;
    settings.log_file = L"base.log";
    settings.delete_old = logging::DELETE_OLD_LOG_FILE;
    settings.lock_log = logging::LOCK_LOG_FILE;
    logging::InitLogging(settings);

    base::MessageLoopForUI main_loop;
    base::RunLoop run_loop;

    timeBeginPeriod(1);

    APPD2D app_d2d;
    if (SUCCEEDED(app_d2d.Initialize())) {
        app_d2d.Start();
        //main_loop.PostTask(FROM_HERE, base::Bind(&APPD2D::Start, base::Unretained(&app_d2d)));
        //base::TimeDelta duration = base::TimeDelta::FromSeconds(1);
        //main_loop.PostDelayedTask(FROM_HERE, base::Bind(&APPD2D::Start, base::Unretained(&app_d2d)), duration );
        run_loop.Run();
    }

    return 0;
}
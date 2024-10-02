#include "spotify_app.h"

SpotifyApp::SpotifyApp(SemaphoreHandle_t mutex) : Apps(mutex)
{
    clear();

    StopwatchApp *stopwatch_app = new StopwatchApp(screen_mutex_, "climate");
    add(0, stopwatch_app);

    SettingsApp *settings_app = new SettingsApp(screen_mutex_);
    settings_app->setOSConfigNotifier(os_config_notifier_);
    add(1, settings_app);

    updateMenu();
    setMotorNotifier(motor_notifier);

    menu->setMenuName("Spotify");
}
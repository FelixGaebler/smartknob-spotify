#include "spotify_app.h"

SpotifyApp::SpotifyApp(SemaphoreHandle_t mutex) : screen_mutex_(mutex)
{
    app_mutex_ = xSemaphoreCreateMutex();
    clear();

    StopwatchApp *stopwatch_app = new StopwatchApp(screen_mutex_, "stopwatch");
    add(0, stopwatch_app);

    setActive(0);
}

void SpotifyApp::add(uint8_t id, App *app)
{
    SemaphoreGuard lock(app_mutex_);
    apps.insert(std::make_pair(id, app));
}

void SpotifyApp::clear()
{
    SemaphoreGuard lock(app_mutex_);
    apps.clear();
}

EntityStateUpdate SpotifyApp::update(AppState state)
{
    SemaphoreGuard lock(app_mutex_);
    EntityStateUpdate new_state_update;

    new_state_update = active_app->updateStateFromKnob(state.motor_state);
    active_app->updateStateFromSystem(state);
    return new_state_update;
}

void SpotifyApp::render()
{
    active_app->render();
}

void SpotifyApp::setActive(int8_t id)
{
    SemaphoreGuard lock(app_mutex_);
    active_id = id;
    active_app = apps[id];
    render();
}

void SpotifyApp::setMotorNotifier(MotorNotifier *motor_notifier)
{
    this->motor_notifier = motor_notifier;
}

void SpotifyApp::triggerMotorConfigUpdate()
{
    motor_notifier->requestUpdate(active_app->getMotorConfig());
}

void SpotifyApp::handleNavigationEvent(NavigationEvent event)
{
    setActive(0);
    motor_notifier->requestUpdate(active_app->getMotorConfig());
}

void SpotifyApp::setOSConfigNotifier(OSConfigNotifier *os_config_notifier)
{
    os_config_notifier_ = os_config_notifier;
}

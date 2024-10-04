#include "spotify_app.h"

SpotifyApp::SpotifyApp(SemaphoreHandle_t mutex) : screen_mutex_(mutex)
{
    app_mutex_ = xSemaphoreCreateMutex();

    StopwatchApp *stopwatch_app = new StopwatchApp(screen_mutex_);
    apps.clear();
    apps.insert(std::make_pair(0, stopwatch_app));
    active_app = apps[0];
    render();
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
    motor_notifier->requestUpdate(active_app->getMotorConfig());
}
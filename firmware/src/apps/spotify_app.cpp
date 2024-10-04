#include "spotify_app.h"

SpotifyApp::SpotifyApp(SemaphoreHandle_t mutex)
{
    active_app = std::make_shared<StopwatchApp>(mutex);
    render();
}

EntityStateUpdate SpotifyApp::update(AppState state)
{
    EntityStateUpdate new_state_update = active_app->updateStateFromKnob(state.motor_state);
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
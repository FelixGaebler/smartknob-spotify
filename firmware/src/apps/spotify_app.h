#pragma once

#include "../notify/motor_notifier/motor_notifier.h"
#include "../navigation/navigation.h"
#include "apps/stopwatch/stopwatch.h"

class SpotifyApp
{
public:
    SpotifyApp(SemaphoreHandle_t mutex);

    EntityStateUpdate update(AppState state);
    void render();

    void setMotorNotifier(MotorNotifier *motor_notifier);
    void triggerMotorConfigUpdate();
    void handleNavigationEvent(NavigationEvent event);

private:
    std::shared_ptr<App> active_app = nullptr;

    PB_SmartKnobConfig root_level_motor_config;
    MotorNotifier *motor_notifier;
};

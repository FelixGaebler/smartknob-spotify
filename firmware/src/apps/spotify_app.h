#pragma once

#include <map>
#include "../app_config.h"
#include "../notify/motor_notifier/motor_notifier.h"
#include "../navigation/navigation.h"
#include "./notify/os_config_notifier/os_config_notifier.h"
#include "apps/stopwatch/stopwatch.h"

class SpotifyApp
{
public:
    SpotifyApp(SemaphoreHandle_t mutex);

    void add(uint8_t id, App *app);
    void clear();
    EntityStateUpdate update(AppState state);
    void render();
    void setActive(int8_t id);

    void setMotorNotifier(MotorNotifier *motor_notifier);
    void setOSConfigNotifier(OSConfigNotifier *os_config_notifier);
    void triggerMotorConfigUpdate();
    void handleNavigationEvent(NavigationEvent event);

private:
    SemaphoreHandle_t screen_mutex_;
    SemaphoreHandle_t app_mutex_;
    std::map<uint8_t, std::shared_ptr<App>> apps;

    int8_t active_id = 0;
    std::shared_ptr<App> active_app = nullptr;

    PB_SmartKnobConfig root_level_motor_config;
    MotorNotifier *motor_notifier;
    OSConfigNotifier *os_config_notifier_;
};

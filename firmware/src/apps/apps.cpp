#include "apps.h"

Apps::Apps(SemaphoreHandle_t mutex) : screen_mutex_(mutex)
{
    app_mutex_ = xSemaphoreCreateMutex();
}

void Apps::add(uint8_t id, App *app)
{
    SemaphoreGuard lock(app_mutex_);
    apps.insert(std::make_pair(id, app));
}

void Apps::clear()
{
    SemaphoreGuard lock(app_mutex_);
    apps.clear();
}

EntityStateUpdate Apps::update(AppState state)
{
    // TODO: update with AppState
    SemaphoreGuard lock(app_mutex_);
    EntityStateUpdate new_state_update;

    new_state_update = active_app->updateStateFromKnob(state.motor_state);
    active_app->updateStateFromSystem(state);
    return new_state_update;
}

void Apps::render()
{
    active_app->render();
};

void Apps::setActive(int8_t id)
{
    SemaphoreGuard lock(app_mutex_);
    active_id = 0;
    active_app = apps[0];
    render();
}

App *Apps::loadApp(uint8_t position, std::string app_slug, char *app_id, char *friendly_name, char *entity_id)
{
    return nullptr;
}
void Apps::updateMenu()
{
    {
        SemaphoreGuard lock(app_mutex_);
        menu = std::make_shared<MenuApp>(screen_mutex_);
    }
}

void Apps::setMotorNotifier(MotorNotifier *motor_notifier)
{
    this->motor_notifier = motor_notifier;
}
void Apps::triggerMotorConfigUpdate()
{
    motor_notifier->requestUpdate(active_app->getMotorConfig());
}

void Apps::handleNavigationEvent(NavigationEvent event)
{
    setActive(0);
    motor_notifier->requestUpdate(active_app->getMotorConfig());
}

std::shared_ptr<App> Apps::find(uint8_t id)
{
    return nullptr;
}
std::shared_ptr<App> Apps::find(char *app_id)
{
    return nullptr;
}

void Apps::setOSConfigNotifier(OSConfigNotifier *os_config_notifier)
{
    os_config_notifier_ = os_config_notifier;
}
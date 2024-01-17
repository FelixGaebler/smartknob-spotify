#pragma once
#include "apps.h"
#include "menu.h"

#include <typeinfo>
#include <iterator>

Apps::Apps()
{
    mutex = xSemaphoreCreateMutex();
}

void Apps::setSprite(TFT_eSprite *spr_)
{
    this->spr_ = spr_;
}

void Apps::add(uint8_t id, App *app)
{
    lock();
    char buf_[10];
    sprintf(buf_, "%d", id);

    // MenuApp *app = new MenuApp(spr_);

    apps.insert(std::make_pair(buf_, app));
    // ESP_LOGD("apps.cpp", ">>> inserted menu App");

    // apps.insert(apps.begin() + id, std::move(app), std::move(app));
    unlock();
}

void Apps::clear()
{
    lock();
    apps.clear();
    unlock();
}

EntityStateUpdate Apps::update(AppState state)
{
    // TODO: update with AppState
    lock();
    char buf_[10];
    sprintf(buf_, "%d", active_id);
    // ESP_LOGD("apps.cpp", ">>> pre-updated");
    EntityStateUpdate new_state_update = apps[buf_]->updateStateFromKnob(state.motor_state);
    apps[buf_]->updateStateFromSystem(state);
    // ESP_LOGD("apps.cpp", ">>> updated");

    unlock();
    return new_state_update;
}

TFT_eSprite *Apps::renderActive()
{
    // TODO: update with AppState
    lock();
    if (active_app != nullptr)
    {
        // ESP_LOGE("apps.cpp", "fast rendering");
        rendered_spr_ = active_app->render();
        // rendered_spr_ = spr_;
        unlock();
        return rendered_spr_;
    }
    // ESP_LOGE("apps.cpp", "slow rendering");

    char buf_[10];
    sprintf(buf_, "%d", active_id);
    if (apps[buf_] == nullptr)
    {
        rendered_spr_ = spr_;
        ESP_LOGE("apps.cpp", "null pointer instead of app");
        unlock();
        return rendered_spr_;
    }

    active_app = apps[buf_];

    rendered_spr_ = active_app->render();

    unlock();
    return rendered_spr_;
}

void Apps::setActive(uint8_t id)
{
    lock();
    active_id = id;
    char buf_[10];
    sprintf(buf_, "%d", active_id);
    if (apps[buf_] == nullptr)
    {
        // TODO: panic?
        ESP_LOGE("apps.cpp", "null pointer instead of app");
        unlock();
    }
    else
    {
        active_app = apps[buf_];
        unlock();
    }
}

void Apps::updateMenu()
{
    // re - generate new menu based on loaded apps
    MenuApp *menu_app = new MenuApp(spr_);

    std::map<std::string, std::shared_ptr<App>>::iterator it;

    uint16_t position = 0;

    for (it = apps.begin(); it != apps.end(); it++)
    {
        ESP_LOGD("apps.cpp", "menu add item %d", position);

        menu_app->add_item(
            position,
            MenuItem{
                it->second->friendly_name,
                1,
                spr_->color565(0, 255, 200),
                it->second->small_icon,
                it->second->big_icon,
            });

        position++;
    }

    add(0, menu_app);
}

// settings and menu apps kept aside for a reason. We will add them manually later
void Apps::loadApp(uint8_t position, std::string app_slug, std::string app_id, char entity_name[32])
{
    if (position < 1)
    {
        ESP_LOGE("apps.cpp", "can't load app at %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
        return;
    }

    ESP_LOGD("apps.cpp", "loading app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    if (app_slug.compare(APP_SLUG_CLIMATE) == 0)
    {
        ClimateApp *app = new ClimateApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_3D_PRINTER) == 0)
    {
        PrinterChamberApp *app = new PrinterChamberApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_BLINDS) == 0)
    {
        BlindsApp *app = new BlindsApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_LIGHT_DIMMER) == 0)
    {
        LightDimmerApp *app = new LightDimmerApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_LIGHT_SWITCH) == 0)
    {
        LightSwitchApp *app = new LightSwitchApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_MUSIC) == 0)
    {
        MusicApp *app = new MusicApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else if (app_slug.compare(APP_SLUG_STOPWATCH) == 0)
    {
        StopwatchApp *app = new StopwatchApp(this->spr_, app_id);
        add(position, app);
        ESP_LOGD("apps.cpp", "added app %d %s %s %s", position, app_slug.c_str(), app_id.c_str(), entity_name);
    }
    else
    {
        ESP_LOGE("apps.cpp", "can't find app with slug '%s'", app_slug);
    }
}

uint8_t Apps::navigationNext()
{
    lock();
    char buf_[10];
    sprintf(buf_, "%d", active_id);
    // ESP_LOGD("apps.cpp", ">>> pre-updated");
    uint8_t next = apps[buf_]->navigationNext();
    // ESP_LOGD("apps.cpp", ">>> updated");
    unlock();
    return next;
}

PB_SmartKnobConfig Apps::getActiveMotorConfig()
{
    lock();
    char buf_[10];
    sprintf(buf_, "%d", active_id);
    // ESP_LOGD("apps.cpp", ">>> pre-updated");
    PB_SmartKnobConfig motor_config = apps[buf_]->getMotorConfig();
    // ESP_LOGD("apps.cpp", ">>> updated");

    unlock();
    return motor_config;
}

// App *Apps::find(uint8_t id)
// {
//     // TODO: add protection with array size
//     return apps[id];
// }

void Apps::lock()
{
    xSemaphoreTake(mutex, portMAX_DELAY);
}
void Apps::unlock()
{
    xSemaphoreGive(mutex);
}
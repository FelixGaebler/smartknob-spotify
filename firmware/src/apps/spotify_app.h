#pragma once
#include "apps.h"

class SpotifyApp : public Apps
{
public:
    SpotifyApp(SemaphoreHandle_t mutex);
};
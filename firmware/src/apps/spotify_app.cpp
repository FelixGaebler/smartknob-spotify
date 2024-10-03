#include "spotify_app.h"

SpotifyApp::SpotifyApp(SemaphoreHandle_t mutex) : Apps(mutex)
{
    clear();

    StopwatchApp *stopwatch_app = new StopwatchApp(screen_mutex_, "stopwatch");
    add(0, stopwatch_app);

    setActive(0);
}
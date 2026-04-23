#include <App.h>

int main(int argc, char *argv[])
{
    App app = App(1920, 1080);
    app.run();

    app.destroy();

    return 0;
}

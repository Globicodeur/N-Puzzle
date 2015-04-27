#include "Application.hpp"

int main(int argc, char **argv) {

    Application app { argc, argv };

    try {
        app.run();
    }
    catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

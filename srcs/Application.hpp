#pragma once

class Application {

public:
            Application(int argc, char **argv);

    void    run(void);

    struct  UnknownHeuristic: public std::exception {

        UnknownHeuristic(const std::string & name):
            what_ { "Unknown heuristic: " + name }
        { }

        virtual const char * what() const noexcept {
            return what_.c_str();
        }

    private:
        std::string what_;
    };

};

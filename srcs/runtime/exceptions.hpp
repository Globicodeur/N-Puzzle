#pragma once

namespace runtime {

    namespace error {

        struct RuntimeError: public std::exception {

        protected:

            RuntimeError(const std::string & what):
                what_ { what }
            { }

            virtual const char * what() const noexcept {
                return what_.c_str();
            }

        private:

            std::string what_;

        };

        struct UnknownHeuristic: public RuntimeError {

            UnknownHeuristic(const std::string & name):
                RuntimeError { "Unknown heuristic: " + name }
            { }

        };

        struct UnknownStrategy: public RuntimeError {

            UnknownStrategy(const std::string & name):
                RuntimeError { "Unknown strategy: " + name }
            { }

        };

        struct UnknownAStarVariant: public RuntimeError {

            UnknownAStarVariant(const std::string & name):
                RuntimeError { "Unknown A* variant: " + name }
            { }

        };

    }

}

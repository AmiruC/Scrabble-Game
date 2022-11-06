#include <exception>

struct QuitGameExcept : public std::exception {
public:
    const char* what() const throw () {
        return "quit game";
    }
};
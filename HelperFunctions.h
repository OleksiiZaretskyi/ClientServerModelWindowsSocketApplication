#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

#include <cctype>

namespace WindowsSocketApp {

    inline bool is_vowel(char c) {
        switch (c | 0x20) {
            case 'a': case 'e': case 'i': case 'o': case 'u':
                return true;
            default:
                return false;
        }
    }

    inline bool is_consonant(char c) {
        return std::isalpha(c) && !is_vowel(c);
    }

}

#endif //HELPERFUNCTIONS_H

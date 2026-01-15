///
/// @file: NcursesSession.hpp
/// @description: A helper class for initializing and cleaning up ncurses
///
/// @date: 2026-01-14
/// @author: MoritzPirer
///

#ifndef NCURSES_SESSION_HPP
#define NCURSES_SESSION_HPP

class NcursesSession {
public:
    NcursesSession();
    NcursesSession(const NcursesSession&) = delete;
    ~NcursesSession();
};

#endif //NCURSES_SESSION_HPP

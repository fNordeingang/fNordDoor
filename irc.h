#ifndef IRC_H
#define IRC_H
#include <libircclient/libircclient.h>
irc_session_t* init_irc_session();
int connect_irc(irc_session_t*);
#endif

#include <map>
#include <string>

#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <libircclient/libircclient.h>

#include "irc.h"

typedef struct
{
	char    * channel;
	char    * nick;
	std::map <std::string, unsigned int> insolents;

} irc_ctx_t;



void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
	irc_cmd_join (session, ctx->channel, 0);
}


void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
	irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);

	if ( !origin || count != 2 )
		return;

	if ( strstr (params[1], "fNordDoor: alive?") == 0 )
		return;

	char nickbuf[128], text[256];

	irc_target_get_nick (origin, nickbuf, sizeof(nickbuf));

	sprintf (text, "yes, i'm alive!");
	irc_cmd_msg (session, params[0], text);
}

void connect_irc()
{
	irc_callbacks_t callbacks;
	irc_ctx_t ctx;

	const char* server   = "irc.freenode.net";
	const char* nickname = "fNordDoor";
	const char* channel  = "#fnordeingang";

  // Initialize the callbacks
	memset (&callbacks, 0, sizeof(callbacks));

  // Set up the callbacks we will use
	callbacks.event_connect = event_connect;
	callbacks.event_channel = event_channel;
	//callbacks.event_nick    = event_nick;
	//callbacks.event_numeric = event_numeric;

        // And create the IRC session; 0 means error
	irc_session_t * s = irc_create_session (&callbacks);

	if ( !s )
	{
		printf ("Could not create IRC session\n");
		return;
	}

	ctx.channel = (char*)channel;
	ctx.nick    = (char*)nickname;
	irc_set_ctx (s, &ctx);

        // Initiate the IRC server connection
	if ( irc_connect(s, server, 6667, 0, nickname, 0, 0) )
	{
		printf ("Could not connect: %s\n", irc_strerror (irc_errno(s)));
		return;
	}

        // and run into forever loop, generating events
	irc_run (s);

	return;
}

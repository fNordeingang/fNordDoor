#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <stdlib.h>

#include <libircclient/libircclient.h>

typedef struct
{
    char 	* channel;
    char 	* nick;

} irc_ctx_t;


irc_ctx_t ctx;

static bool is_reconnect = false;

void addlog (const char * fmt, ...)
{
    FILE * fp;
    char buf[1024];
    va_list va_alist;

    va_start (va_alist, fmt);
    vsnprintf (buf, sizeof(buf), fmt, va_alist);
    va_end (va_alist);

    printf ("%s\n", buf);

    if ( (fp = fopen ("irctest.log", "ab")) != 0 )
    {
        fprintf (fp, "%s\n", buf);
        fclose (fp);
    }
}


void dump_event (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    char buf[512];
    int cnt;

    buf[0] = '\0';

    for ( cnt = 0; cnt < count; cnt++ )
    {
        if ( cnt )
            strcat (buf, "|");

        strcat (buf, params[cnt]);
    }


    addlog ("Event \"%s\", origin: \"%s\", params: %d [%s]", event, origin ? origin : "NULL", cnt, buf);
}


void event_join (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    dump_event (session, event, origin, params, count);
    irc_cmd_user_mode (session, "+i");
    if(is_reconnect) {
      irc_cmd_msg (session, params[0], "Hi all, I reconnected, everthing is fine.");
    }
    else {
      irc_cmd_msg (session, params[0], "Hi all, I restared. Dont't know why.");
      is_reconnect = true;
    }
}


void event_connect (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    irc_ctx_t * ctx = (irc_ctx_t *) irc_get_ctx (session);
    dump_event (session, event, origin, params, count);

    irc_cmd_join (session, ctx->channel, 0);
}


void event_privmsg (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    dump_event (session, event, origin, params, count);

    printf ("'%s' said me (%s): %s\n", 
            origin ? origin : "someone",
            params[0], params[1] );
}


void dcc_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    static int count = 1;
    char buf[12];

    switch (status)
    {
        case LIBIRC_ERR_CLOSED:
            printf ("DCC %d: chat closed\n", id);
            break;

        case 0:
            if ( !data )
            {
                printf ("DCC %d: chat connected\n", id);
                irc_dcc_msg	(session, id, "Hehe");
            }
            else 
            {
                printf ("DCC %d: %s\n", id, data);
                sprintf (buf, "DCC [%d]: %d", id, count++);
                irc_dcc_msg	(session, id, buf);
            }
            break;

        default:
            printf ("DCC %d: error %s\n", id, irc_strerror(status));
            break;
    }
}


void dcc_file_recv_callback (irc_session_t * session, irc_dcc_t id, int status, void * ctx, const char * data, unsigned int length)
{
    if ( status == 0 && length == 0 )
    {
        printf ("File sent successfully\n");

        if ( ctx )
            fclose ((FILE*) ctx);
    }
    else if ( status )
    {
        printf ("File sent error: %d\n", status);

        if ( ctx )
            fclose ((FILE*) ctx);
    }
    else
    {
        if ( ctx )
            fwrite (data, 1, length, (FILE*) ctx);
        printf ("File sent progress: %d\n", length);
    }
}


void event_channel (irc_session_t * session, const char * event, const char * origin, const char ** params, unsigned int count)
{
    if ( count != 2 )
        return;

    printf ("'%s' said in channel %s: %s\n", 
            origin ? origin : "someone",
            params[0], params[1] );

    if ( !origin )
        return;
}


void irc_event_dcc_chat (irc_session_t * session, const char * nick, const char * addr, irc_dcc_t dccid)
{
    printf ("DCC chat [%d] requested from '%s' (%s)\n", dccid, nick, addr);

    irc_dcc_accept (session, dccid, 0, dcc_recv_callback);
}


void irc_event_dcc_send (irc_session_t * session, const char * nick, const char * addr, const char * filename, unsigned long size, irc_dcc_t dccid)
{
    FILE * fp;
    printf ("DCC send [%d] requested from '%s' (%s): %s (%lu bytes)\n", dccid, nick, addr, filename, size);

    if ( (fp = fopen ("file", "wb")) == 0 )
        abort();

    irc_dcc_accept (session, dccid, fp, dcc_file_recv_callback);
}

void event_numeric (irc_session_t * session, unsigned int event, const char * origin, const char ** params, unsigned int count)
{
    char buf[24];
    sprintf (buf, "%d", event);

    dump_event (session, buf, origin, params, count);
}

irc_session_t* init_irc_session() {
  irc_callbacks_t	callbacks;
  memset (&callbacks, 0, sizeof(callbacks));

  callbacks.event_connect = event_connect;
  callbacks.event_join = event_join;
  callbacks.event_nick = dump_event;
  callbacks.event_quit = dump_event;
  callbacks.event_part = dump_event;
  callbacks.event_mode = dump_event;
  callbacks.event_topic = dump_event;
  callbacks.event_kick = dump_event;
  callbacks.event_channel = event_channel;
  callbacks.event_privmsg = event_privmsg;
  callbacks.event_notice = dump_event;
  callbacks.event_invite = dump_event;
  callbacks.event_umode = dump_event;
  callbacks.event_ctcp_rep = dump_event;
  callbacks.event_ctcp_action = dump_event;
  callbacks.event_unknown = dump_event;
  callbacks.event_numeric = event_numeric;

  callbacks.event_dcc_chat_req = irc_event_dcc_chat;
  callbacks.event_dcc_send_req = irc_event_dcc_send;

  
  irc_session_t* door_session = irc_create_session (&callbacks);

  if ( !door_session )
  {
    printf ("Could not create session\n");
    return 0;
  }

  ctx.channel = (char*)"#fnorddoor";
  ctx.nick = (char*)"fNordDoor";

  irc_set_ctx (door_session, &ctx);


  // Initiate the IRC server connection
  if ( irc_connect (door_session, "irc.freenode.net", 6667, 0, "fNordDoor", 0, 0) )
  {
    printf ("Could not connect: %s\n", irc_strerror (irc_errno(door_session)));
  }

  return door_session;
}

int connect_irc(irc_session_t *door_session)
{
    // and run into forever loop, generating events
    if ( irc_run (door_session) )
    {
        printf ("Could not connect or I/O error: %s\n", irc_strerror (irc_errno(door_session)));
        return 1;
    }

    return 1;
}

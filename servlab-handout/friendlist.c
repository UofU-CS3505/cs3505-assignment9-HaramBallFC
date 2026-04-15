/*
 * friendlist.c - [Starting code for] a web-based friend-graph manager.
 *
 * Based on:
 *  tiny.c - A simple, iterative HTTP/1.0 Web server that uses the
 *      GET method to serve static and dynamic content.
 *   Tiny Web server
 *   Dave O'Hallaron
 *   Carnegie Mellon University
 */
#include "csapp.h"
#include "dictionary.h"
#include "more_string.h"

static void doit(int fd);
static dictionary_t *read_requesthdrs(rio_t *rp);
static void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *d);
static void clienterror(int fd, char *cause, char *errnum,
                        char *shortmsg, char *longmsg);
static void print_stringdictionary(dictionary_t *d);
static void serve_friends(int fd, dictionary_t *query);
static void serve_befriend(int fd, dictionary_t *query);
static void serve_unfriend(int fd, dictionary_t *query);
static void serve_introduce(int fd, dictionary_t *query);

/* Global friends table: maps username -> dictionary_t* (set of friends) */
static dictionary_t *friends_table;
static pthread_mutex_t lock;

/*
 * Thread handler for concurrent client connections
 */
static void *thread_handler(void *arg)
{
  int connfd = *(int *)arg;
  free(arg);
  Pthread_detach(Pthread_self());
  doit(connfd);
  Close(connfd);
  return NULL;
}

/*
 * Get or create the friend set for a user.
 * Must be called with lock held.
 */
static dictionary_t *get_or_create_friends(const char *user)
{
  dictionary_t *friends = dictionary_get(friends_table, user);
  if (!friends) {
    friends = make_dictionary(COMPARE_CASE_SENS, NULL);
    dictionary_set(friends_table, user, friends);
  }
  return friends;
}

/*
 * Return friends of user as a newline-terminated string.
 * Must be called with lock held. Caller must free result.
 */
static char *friends_to_string(const char *user)
{
  dictionary_t *friends = get_or_create_friends(user);
  const char **keys = dictionary_keys(friends);
  char *result = join_strings(keys, '\n');
  free(keys);
  return result;
}

int main(int argc, char **argv)
{
  int listenfd, *connfdp;
  char hostname[MAXLINE], port[MAXLINE];
  socklen_t clientlen;
  struct sockaddr_storage clientaddr;
  pthread_t tid;

  /* Check command line args */
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }

  listenfd = Open_listenfd(argv[1]);

  /* Don't kill the server if there's an error, because
     we want to survive errors due to a client. But we
     do want to report errors. */
  exit_on_error(0);

  /* Also, don't stop on broken connections: */
  Signal(SIGPIPE, SIG_IGN);

  /* Initialize global state */
  friends_table = make_dictionary(COMPARE_CASE_SENS, (free_proc_t)free_dictionary);
  pthread_mutex_init(&lock, NULL);

  while (1) {
    clientlen = sizeof(clientaddr);
    connfdp = malloc(sizeof(int));
    *connfdp = Accept(listenfd, (SA *)&clientaddr, &clientlen);
    if (*connfdp >= 0) {
      Getnameinfo((SA *) &clientaddr, clientlen, hostname, MAXLINE,
                  port, MAXLINE, 0);
      printf("Accepted connection from (%s, %s)\n", hostname, port);
      Pthread_create(&tid, NULL, thread_handler, connfdp);
    } else {
      free(connfdp);
    }
  }
}

/*
 * doit - handle one HTTP request/response transaction
 */
void doit(int fd)
{
  char buf[MAXLINE], *method, *uri, *version;
  rio_t rio;
  dictionary_t *headers, *query;

  /* Read request line and headers */
  Rio_readinitb(&rio, fd);
  if (Rio_readlineb(&rio, buf, MAXLINE) <= 0)
    return;
  printf("%s", buf);

  if (!parse_request_line(buf, &method, &uri, &version)) {
    clienterror(fd, method, "400", "Bad Request",
                "Friendlist did not recognize the request");
  } else {
    if (strcasecmp(version, "HTTP/1.0")
        && strcasecmp(version, "HTTP/1.1")) {
      clienterror(fd, version, "501", "Not Implemented",
                  "Friendlist does not implement that version");
    } else if (strcasecmp(method, "GET")
               && strcasecmp(method, "POST")) {
      clienterror(fd, method, "501", "Not Implemented",
                  "Friendlist does not implement that method");
    } else {
      headers = read_requesthdrs(&rio);

      /* Parse all query arguments into a dictionary */
      query = make_dictionary(COMPARE_CASE_SENS, free);
      parse_uriquery(uri, query);
      if (!strcasecmp(method, "POST"))
        read_postquery(&rio, headers, query);

      /* For debugging, print the dictionary */
      print_stringdictionary(query);

      /* Route to appropriate handler */
      if (starts_with("/friends", uri))
        serve_friends(fd, query);
      else if (starts_with("/befriend", uri))
        serve_befriend(fd, query);
      else if (starts_with("/unfriend", uri))
        serve_unfriend(fd, query);
      else if (starts_with("/introduce", uri))
        serve_introduce(fd, query);
      else
        clienterror(fd, uri, "404", "Not Found", "Request not recognized");

      /* Clean up */
      free_dictionary(query);
      free_dictionary(headers);
    }

    /* Clean up status line */
    free(method);
    free(uri);
    free(version);
  }
}

/*
 * read_requesthdrs - read HTTP request headers
 */
dictionary_t *read_requesthdrs(rio_t *rp)
{
  char buf[MAXLINE];
  dictionary_t *d = make_dictionary(COMPARE_CASE_INSENS, free);

  Rio_readlineb(rp, buf, MAXLINE);
  printf("%s", buf);
  while(strcmp(buf, "\r\n")) {
    Rio_readlineb(rp, buf, MAXLINE);
    printf("%s", buf);
    parse_header_line(buf, d);
  }

  return d;
}

void read_postquery(rio_t *rp, dictionary_t *headers, dictionary_t *dest)
{
  char *len_str, *type, *buffer;
  int len;

  len_str = dictionary_get(headers, "Content-Length");
  len = (len_str ? atoi(len_str) : 0);

  type = dictionary_get(headers, "Content-Type");

  buffer = malloc(len+1);
  Rio_readnb(rp, buffer, len);
  buffer[len] = 0;

  if (!strcasecmp(type, "application/x-www-form-urlencoded")) {
    parse_query(buffer, dest);
  }

  free(buffer);
}

static char *ok_header(size_t len, const char *content_type) {
  char *len_str, *header;

  header = append_strings("HTTP/1.0 200 OK\r\n",
                          "Server: Friendlist Web Server\r\n",
                          "Connection: close\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n",
                          "Content-type: ", content_type, "\r\n\r\n",
                          NULL);
  free(len_str);

  return header;
}

/*
 * serve_friends - GET /friends?user=<user>
 * Returns the friends of <user>, each on a newline-terminated line.
 */
static void serve_friends(int fd, dictionary_t *query)
{
  size_t len;
  char *body, *header, *user;

  user = dictionary_get(query, "user");
  if (!user) {
    clienterror(fd, "user", "400", "Bad Request", "Missing user parameter");
    return;
  }

  pthread_mutex_lock(&lock);
  body = friends_to_string(user);
  pthread_mutex_unlock(&lock);

  len = strlen(body);
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
}

/*
 * serve_befriend - GET /befriend?user=<user>&friends=<friends>
 * Adds each user in <friends> as a friend of <user> (symmetric).
 * Returns updated friends list for <user>.
 */
static void serve_befriend(int fd, dictionary_t *query)
{
  size_t len;
  char *body, *header, *user, *friends_str;
  char **friends_list;
  int i;

  user = dictionary_get(query, "user");
  friends_str = dictionary_get(query, "friends");
  if (!user || !friends_str) {
    clienterror(fd, "params", "400", "Bad Request", "Missing required parameters");
    return;
  }

  friends_list = split_string(friends_str, '\n');

  pthread_mutex_lock(&lock);

  for (i = 0; friends_list[i] != NULL; i++) {
    if (strlen(friends_list[i]) > 0) {
      dictionary_set(get_or_create_friends(user), friends_list[i], NULL);
      dictionary_set(get_or_create_friends(friends_list[i]), user, NULL);
    }
  }

  body = friends_to_string(user);
  pthread_mutex_unlock(&lock);

  len = strlen(body);
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
  for (i = 0; friends_list[i] != NULL; i++)
    free(friends_list[i]);
  free(friends_list);
}

/*
 * serve_unfriend - GET /unfriend?user=<user>&friends=<friends>
 * Removes each user in <friends> as a friend of <user> (symmetric).
 * Returns remaining friends list for <user>.
 */
static void serve_unfriend(int fd, dictionary_t *query)
{
  size_t len;
  char *body, *header, *user, *friends_str;
  char **friends_list;
  int i;

  user = dictionary_get(query, "user");
  friends_str = dictionary_get(query, "friends");
  if (!user || !friends_str) {
    clienterror(fd, "params", "400", "Bad Request", "Missing required parameters");
    return;
  }

  friends_list = split_string(friends_str, '\n');

  pthread_mutex_lock(&lock);

  for (i = 0; friends_list[i] != NULL; i++) {
    if (strlen(friends_list[i]) > 0) {
      dictionary_remove(get_or_create_friends(user), friends_list[i]);
      dictionary_remove(get_or_create_friends(friends_list[i]), user);
    }
  }

  body = friends_to_string(user);
  pthread_mutex_unlock(&lock);

  len = strlen(body);
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
  for (i = 0; friends_list[i] != NULL; i++)
    free(friends_list[i]);
  free(friends_list);
}

/*
 * serve_introduce - GET /introduce?user=<user>&friend=<friend>&host=<host>&port=<port>
 * Contacts server at host:port to get friends of <friend>,
 * then adds <friend> and all of <friend>'s friends as friends of <user> (symmetric).
 */
static void serve_introduce(int fd, dictionary_t *query)
{
  size_t len;
  char *body, *header;
  char *user, *friend_param, *host, *port_str;
  char *encoded_friend, *request;
  char buf[MAXLINE];
  char **remote_friends;
  int clientfd, i, ok;
  rio_t rio;
  char *version, *status, *desc;
  dictionary_t *response_headers;
  char *len_str;
  int content_len;
  char *response_body;

  user = dictionary_get(query, "user");
  friend_param = dictionary_get(query, "friend");
  host = dictionary_get(query, "host");
  port_str = dictionary_get(query, "port");

  if (!user || !friend_param || !host || !port_str) {
    clienterror(fd, "params", "400", "Bad Request", "Missing required parameters");
    return;
  }

  remote_friends = NULL;

  /* Connect to remote server and fetch friend's friends */
  clientfd = open_clientfd(host, port_str);
  if (clientfd >= 0) {
    encoded_friend = query_encode(friend_param);
    request = append_strings("GET /friends?user=", encoded_friend,
                             " HTTP/1.0\r\nHost: ", host, "\r\n\r\n", NULL);
    free(encoded_friend);

    Rio_writen(clientfd, request, strlen(request));
    free(request);

    Rio_readinitb(&rio, clientfd);

    if (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
      if (parse_status_line(buf, &version, &status, &desc)) {
        ok = !strcmp(status, "200");
        free(version);
        free(status);
        free(desc);

        /* Read response headers */
        response_headers = make_dictionary(COMPARE_CASE_INSENS, free);
        while (Rio_readlineb(&rio, buf, MAXLINE) > 0) {
          if (!strcmp(buf, "\r\n"))
            break;
          parse_header_line(buf, response_headers);
        }

        if (ok) {
          /* Read response body */
          len_str = dictionary_get(response_headers, "Content-length");
          content_len = (len_str ? atoi(len_str) : 0);
          response_body = malloc(content_len + 1);
          if (content_len > 0)
            Rio_readnb(&rio, response_body, content_len);
          response_body[content_len] = 0;

          remote_friends = split_string(response_body, '\n');
          free(response_body);
        }

        free_dictionary(response_headers);
      }
    }

    Close(clientfd);
  }

  /* Update local friends table */
  pthread_mutex_lock(&lock);

  if (remote_friends) {
    /* Add friend as friend of user (bidirectional), unless same person */
    if (strcmp(user, friend_param) != 0) {
      dictionary_set(get_or_create_friends(user), friend_param, NULL);
      dictionary_set(get_or_create_friends(friend_param), user, NULL);
    }

    /* Add each of friend's friends as friend of user (bidirectional) */
    for (i = 0; remote_friends[i] != NULL; i++) {
      if (strlen(remote_friends[i]) > 0 && strcmp(remote_friends[i], user) != 0) {
        dictionary_set(get_or_create_friends(user), remote_friends[i], NULL);
        dictionary_set(get_or_create_friends(remote_friends[i]), user, NULL);
      }
    }
  }

  body = friends_to_string(user);
  pthread_mutex_unlock(&lock);

  /* Send response */
  len = strlen(body);
  header = ok_header(len, "text/plain; charset=utf-8");
  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);

  if (remote_friends) {
    for (i = 0; remote_friends[i] != NULL; i++)
      free(remote_friends[i]);
    free(remote_friends);
  }
}

/*
 * clienterror - returns an error message to the client
 */
void clienterror(int fd, char *cause, char *errnum,
		 char *shortmsg, char *longmsg)
{
  size_t len;
  char *header, *body, *len_str;

  body = append_strings("<html><title>Friendlist Error</title>",
                        "<body bgcolor=""ffffff"">\r\n",
                        errnum, " ", shortmsg,
                        "<p>", longmsg, ": ", cause,
                        "<hr><em>Friendlist Server</em>\r\n",
                        NULL);
  len = strlen(body);

  /* Print the HTTP response */
  header = append_strings("HTTP/1.0 ", errnum, " ", shortmsg, "\r\n",
                          "Content-type: text/html; charset=utf-8\r\n",
                          "Content-length: ", len_str = to_string(len), "\r\n\r\n",
                          NULL);
  free(len_str);

  Rio_writen(fd, header, strlen(header));
  Rio_writen(fd, body, len);

  free(header);
  free(body);
}

static void print_stringdictionary(dictionary_t *d)
{
  int i, count;

  count = dictionary_count(d);
  for (i = 0; i < count; i++) {
    printf("%s=%s\n",
           dictionary_key(d, i),
           (const char *)dictionary_value(d, i));
  }
  printf("\n");
}

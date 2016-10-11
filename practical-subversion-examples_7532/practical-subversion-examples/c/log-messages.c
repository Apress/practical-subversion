#include <svn_client.h>

struct message_baton {
  const char *message;
  const char *file;
};

svn_error_t *
get_log_message (const char **log_msg,
                 const char **tmp_file,
                 apr_array_header_t *commit_items,
                 void *baton,
                 apr_pool_t *pool)
{
  struct message_baton *mb = baton;

  if (mb->message)
    *log_msg = apr_pstrdup (pool, mb->message);
  else if (mb->file)
    *tmp_file = apr_pstrdup (pool, mb->file);
  else
    return svn_error_create (SVN_ERR_CL_BAD_LOG_MESSAGE, NULL,
                             "User did not specify a log message");

  return SVN_NO_ERROR;
}

svn_error_t *
set_up_log_message_callback (svn_client_ctx_t *ctx,
                             const char *log_msg,
                             const char *tmp_file,
                             apr_pool_t *pool)
{
  struct message_baton *mb = apr_pcalloc (pool, sizeof (*mb));

  mb->message = log_msg;
  mb->file = tmp_file;

  ctx->log_msg_func = get_log_message;

  ctx->log_msg_baton = mb;

  return SVN_NO_ERROR;
}

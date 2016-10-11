#include <svn_types.h>
#include <svn_error.h>
#include <svn_error_codes.h>
#include <svn_client.h>

static volatile sig_atomic_t been_cancelled = FALSE;

static void
signal_handler (int unused)
{
  been_cancelled = TRUE;
}

svn_error_t *
cancellation_callback (void *baton)
{
  if (been_cancelled)
    return svn_error_create (SVN_ERR_CANCELLED, NULL, "Caught signal");
  else
    return SVN_NO_ERROR;
}

svn_error_t *
set_up_cancellation (svn_client_ctx_t *ctx, apr_pool_t *pool)
{
  apr_signal (SIGINT, signal_handler);

  ctx->cancel_func = cancellation_callback;

  /* in a more complex application with multiple operations in progress at 
   * once, we'd set this to whatever data the callback would need to look at 
   * to determine if the action this context is being used for was cancelled.
   */
  ctx->cancel_baton = NULL;

  return SVN_NO_ERROR;
}

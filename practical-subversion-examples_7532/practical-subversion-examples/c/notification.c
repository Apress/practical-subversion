#include <svn_client.h>
static void
notification_callback (void *baton,
		       const svn_wc_notify_t * notify, apr_pool_t * pool)
{
  switch (notify->action)
    {
    case svn_wc_notify_update_add:
      printf ("Adding %s\n", notify->path);
      break;

    case svn_wc_notify_update_delete:
      printf ("Deleting %s\n", notify->path);
      break;

    case svn_wc_notify_update_update:
      /* if this were real, we'd also pay attention to property changes,
       * which would mean we'd also be printing stuff out for directories,
       * but for this example only files will be enough.
       */
      if (notify->kind == svn_node_file)
	{
	  if (notify->content_state == svn_wc_notify_state_conflicted)
	    printf ("Conflict at %s\n", notify->path);
	  else if (notify->content_state == svn_wc_notify_state_merged)
	    printf ("Merged %s\n", notify->path);
	  else if (notify->content_state == svn_wc_notify_state_changed)
	    printf ("Updating %s\n", notify->path);
	}
      break;

    case svn_wc_notify_update_completed:
      printf ("Updated to %" SVN_REVNUM_T_FMT "\n", notify->revision);
      break;

    default:
      break;
    }
}

svn_error_t *
set_up_notification (svn_client_ctx_t * ctx, apr_pool_t * pool)
{
  ctx->notify_func2 = notification_callback;

  ctx->notify_baton2 = NULL;

  return SVN_NO_ERROR;
}

#include <svn_fs.h>
#include <svn_pools.h>
#include <svn_error.h>

svn_error_t *
print_node_history (svn_fs_t *fs,
                    svn_fs_root_t *root,
                    const char *path,
                    apr_pool_t *pool)
{
  apr_pool_t *newpool = svn_pool_create (pool);
  apr_pool_t *oldpool = svn_pool_create (pool);

  svn_fs_history_t *history;
  const char *history_path;
  svn_revnum_t history_rev;

  SVN_ERR (svn_fs_node_history (&history, root, path, pool));

  do
    {
      /* use two pools here, allocating out of one of them each time through 
       * the loop.  the history object needs to persist until the next 
       * iteration, so we can call svn_fs_history_prev on it.  then, once 
       * we're done with that history, we clear the pool it was allocated from.
       */

      SVN_ERR (svn_fs_history_location (&history_path, &history_rev,
                                        history, newpool));

      printf ("%s %" SVN_REVNUM_T_FMT "\n", history_path, history_rev);

      SVN_ERR (svn_fs_history_prev (&history, history, TRUE, newpool));

      svn_pool_clear (oldpool);

      {
        apr_pool_t *tmppool = oldpool;
        oldpool = newpool;
        newpool = tmppool;
      }
    }
  while (history);
}

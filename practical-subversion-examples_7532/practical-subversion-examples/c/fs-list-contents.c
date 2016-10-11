#include <svn_fs.h>
#include <svn_repos.h>
#include <svn_pools.h>

svn_error_t *
list_directory_contents (svn_repos_t *repos,
                         const char *path,
                         apr_pool_t *pool)
{
  svn_fs_root_t *root;
  svn_revnum_t rev;
  apr_hash_t *ents;

  /* figure out what the current HEAD is, so we can start by copying it. */
  SVN_ERR (svn_fs_youngest_rev (&rev, svn_repos_fs (repos), pool));

  SVN_ERR (svn_fs_revision_root (&root, svn_repos_fs (repos), rev, pool));

  SVN_ERR (svn_fs_dir_entries (&ents, root, path, pool));

  {
    apr_pool_t *subpool = svn_pool_create (pool);
    apr_hash_index_t *idx;
    const void *key;
    void *val;

    for (idx = apr_hash_first (pool, ents); idx; idx = apr_hash_next (idx))
      {
        svn_string_t *unparsed;
        svn_fs_dirent_t *dent;

        svn_pool_clear (subpool);

        apr_hash_this (idx, &key, NULL, &val);

        dent = val;

        unparsed = svn_fs_unparse_id (dent->id, subpool);
        
        printf ("%s %s\n", dent->name, unparsed->data);
      }

    svn_pool_destroy (subpool);
  }

  /* you don't really have to do this, the pool cleanup will take care of 
   * it for you, but hey, it doesn't hurt to speed up the process. */
  svn_fs_close_root (root);

  return SVN_NO_ERROR;
}

#include <svn_fs.h>
#include <svn_repos.h>

svn_error_t *
create_readme_file_impl (svn_fs_txn_t *txn, 
                         svn_repos_t *repos,
                         const char *contents,
                         apr_pool_t *pool)
{
  svn_revnum_t rev, new_rev;
  svn_fs_root_t *root;
  svn_stream_t *stream;

  /* grab the root of the txn */
  SVN_ERR (svn_fs_txn_root (&root, txn, pool));

  /* create a file named README in the top level of the txn */
  SVN_ERR (svn_fs_make_file (root, "README", pool));

  /* grab a writable stream for README, then write the contents to it */
  SVN_ERR (svn_fs_apply_text (&stream, root, "README",
                              NULL, /* we're not bothering with a checksum */
                              pool));

  {
    apr_size_t len = strlen (contents);

    SVN_ERR (svn_stream_write (stream, contents, &len));
  }

  SVN_ERR (svn_stream_close (stream));

  /* try to commit our change */
  SVN_ERR (svn_repos_fs_commit_txn (NULL,
                                    repos,
                                    &new_rev,
                                    txn,
                                    pool));
}

svn_error_t *
create_readme_file (svn_repos_t *repos, const char *contents, apr_pool_t *pool)
{
  svn_fs_txn_t *txn;
  svn_revnum_t rev;

  /* figure out what the current HEAD is, so we can start by copying it. */
  SVN_ERR (svn_fs_youngest_rev (&rev, svn_repos_fs (repos), pool));

  /* start our transaction */
  SVN_ERR (svn_repos_fs_begin_txn_for_commit (&txn, repos, rev, "example",
                                              "creating a readme file", pool));

  /* if anything else fails, we need to abort the transaction, so the guts 
   * of the code is in a helper function, and if that returns an error, we 
   * abort the transaction before throwing the error back upstream.
   */
  svn_error_t *err = create_readme_file_impl (txn, repos, contents, pool);

  if (err)
    {
      SVN_ERR (svn_fs_abort_txn (txn, pool));

      return err;
    }
  else
    return SVN_NO_ERROR;
}

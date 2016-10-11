#include <svn_error.h>

/* an example function that returns svn_error_t's in various ways*/
svn_error_t *
function_that_might_fail(apr_pool_t *pool)
{
  /* first, the most generic way of dealing with an error.  if this function
     returns any kind of error at all, SVN_ERR will immediately return that
     error to our calling function and let it deal with it. */
  SVN_ERR(another_function_that_might_fail(pool));

  if (something)
    {
      /* this case is similar, but instead of just using SVN_ERR, we use
         SVN_ERR_W, which allows us to specify a message that will be wrapped
         around any error that is returned.  so if this function returns
         an error, we'll create a new error, with "the first case" as its
         message and the error we're going to wrap as its parent.  when 
         the calling function receives the chain of errors, it will have both
         the lower-level error message that indicates that this function
         failed and our more specific error message wrapped around it, which
         indicates some more detail about how it was being used when it did
         fail. */
      SVN_ERR_W(a_third_function_that_might_fail(pool),
                "the first case");
    }
  else if (something_else)
    {
      /* in this case, we aren't reacting to an error we received from a
         function we called, instead we're returning an error we create
         ourselves. */
      return svn_error_createf (SVN_ERR_BAD_URL,
                                NULL,
                                "got bad URL '%s'",
                                bad_url);
    }
  else
    {
      /* finally, we have the case where we're calling a function that returns
         an error, but we don't particularly care if it failed or not.  perhaps
         we're already recovering from another error, and subsequent problems
         aren't really unexpected.  in this case, we need to use svn_error_clear
         to destroy it, or we'll get a memory leak. */
      svn_error_t *err = a_function_that_might_fail_but_we_dont_care(pool);

      if (err)
        svn_error_clear(err);
    }

  return SVN_NO_ERROR;
}

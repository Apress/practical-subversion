#include <svn_auth.h>
#include <svn_types.h>
#include <svn_error.h>
#include <svn_client.h>

/* prompt the user for a username and password */
static svn_error_t *
simple_prompt_callback (svn_auth_cred_simple_t **cred,
                        void *baton,
                        const char *realm,
                        const char *username,
                        svn_boolean_t may_save,
                        apr_pool_t *pool)
{
  svn_auth_cred_simple_t *ret = apr_pcalloc (pool, sizeof (*ret));
  size_t len;
  char *line;

  /* tell the user what the repository realm is, so he knows what username
     he should enter. */
  if (realm)
    printf ("Realm: %s\n", realm);

  /* if we were passed a username, that means subversion had one cached for
     this realm already, so use that. */
  if (username)
    ret->username = apr_pstrdup (pool, username);
  else
    {
      /* we didn't have a username, so prompt the user to enter one. */
      printf ("Username? : ");

      /* read in the username the user entered */
      line = fgetln (stdin, &len);

      /* assuming the user entered something copy it into our final
         svn_auth_cred_simple_t structure so we can return it. */
      if (line)
        {
          ret->username = apr_palloc (pool, len);
          snprintf (ret->username, len - 1, line);
        }
    }

  /* now we do the same thing for the user's password... */
  printf ("Password? : ");

  line = fgetln (stdin, &len);

  if (line)
    {
      ret->password = apr_palloc (pool, len);
      snprintf (ret->password, len - 1, line);
    }

  /* return the svn_auth_cred_simple_t by reference. */
  *cred = ret;

  return SVN_NO_ERROR;
}

/* similar to the previous function, but just prompts for a username */
static svn_error_t *
username_prompt_callback (svn_auth_cred_username_t **cred,
                          void *baton,
                          const char *realm,
                          svn_boolean_t may_save,
                          apr_pool_t *pool)
{
  svn_auth_cred_simple_t *ret = apr_pcalloc (pool, sizeof (*ret));
  size_t len;
  char *line;

  if (realm)
    printf ("Realm: %s\n", realm);

  printf ("Username? : ");

  line = fgetln (stdin, &len);

  if (line)
    {
      ret->username = apr_palloc (pool, len);
      snprintf (ret->username, len - 1, line);
    }

  *cred = ret;

  return SVN_NO_ERROR;
}

/* build authentication providers that use our prompting functions and add
   them to a client context. */
static svn_error_t *
set_up_auth (svn_client_ctx_t *ctx, apr_pool_t *pool)
{
  svn_auth_provider_object_t *provider;

  /* create an array to hold our two providers. */
  apr_array_header_t *providers
    = apr_array_make (pool, 2, sizeof (svn_auth_provider_object_t *));

  /* create the svn.simple provider. */
  svn_client_get_simple_prompt_provider (&provider,
                                         simple_prompt_callback,
                                         NULL, /* the baton goes here */
                                         2, /* number of times to retry */
                                         pool);

  /* add it to the array. */
  APR_ARRAY_PUSH (providers, svn_auth_provider_object_t *) = provider;

  /* create the svn.username provider. */
  svn_client_get_username_prompt_provider (&provider,
                                           username_prompt_callback,
                                           NULL, /* the baton goes here */
                                           2, /* number of times to retry */
                                           pool);

  /* add it to the array. */
  APR_ARRAY_PUSH (providers, svn_auth_provider_object_t *) = provider;

  /* now that we've got the array of providers, use them to create our auth
     baton. */
  svn_auth_open (&ctx->auth_baton, providers, pool);

  return SVN_NO_ERROR;
}

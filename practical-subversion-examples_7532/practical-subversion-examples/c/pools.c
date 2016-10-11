#include <svn_pools.h>
#include <apr_strings.h>

/* given a file number, return its path */
char * find_file (int i, apr_pool_t *pool);

/* returns 1 if a given file is important, 0 if it is not. */
int is_important_file (const char *file, apr_pool_t *pool);

/* look through files numbered 0 up to maximum.  for each one that is important,
   put its filename in the array we will return, otherwise leave its slot
   in the array as NULL. */
char **
find_important_files (int maximum, apr_pool_t *pool)
{
  int i;

  /* allocate an array of char *'s to hold potentially important files */
  char ** important_files = apr_pcalloc (pool, sizeof(char *) * maximum);

  /* create an iteration pool */
  apr_pool_t *subpool = svn_pool_create (pool);

  for (i = 0; i < maximum; ++i)
    {
      svn_pool_clear (subpool);

      /* get a file, allocated in subpool */
      char *file = find_file (i, subpool);

      if (is_important (file, subpool))
        {
          /* if file is important, strdup it into the main pool so it will
             persist beyond this iteration so we can return it. */
          important_files[i] = apr_pstrdup (pool, file);
        }
    }

  /* clean up our subpool so that the resources can be reused in the future */
  svn_pool_destroy (subpool);

  return important_files;
}

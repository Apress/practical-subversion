#include <apr_hash.h>

void
demonstrate_hash_tables (apr_pool_t *pool)
{
  /* create an empty hash table */
  apr_hash_t *hash = apr_hash_make (pool);

  /* add some items */
  apr_hash_set (hash, "foo", APR_HASH_KEY_STRING, "bar");
  apr_hash_set (hash, "baz", APR_HASH_KEY_STRING, "zot");

  {
    apr_hash_index_t *idx;
    const void *key;
    void *val;

    /* iterate over them */
    for (idx = apr_hash_first (pool, hash); idx; idx = apr_hash_next (idx))
      {
        /* and print them out */
        apr_hash_this (idx, &key, NULL, &val);
        printf ("key = '%s', val = '%s'\n",
                (const char *) key, (const char *) val);
      }
  }

  {
    /* get a specific value */
    void *val = apr_hash_get (hash, "foo", APR_HASH_KEY_STRING);
    if (val)
      {
        /* do something with it */
      }
  }

  /* remove the item at "foo" */
  apr_hash_set (hash, "foo", APR_HASH_KEY_STRING, NULL);
}

sub do_something {
  my $pool = SVN::Pool->new_default; # create a new default pool

  for (@_) {
    my $subpool = SVN::Pool->new_default_sub;

    # do something here that makes use of the default pool for allocation

    # now $subpool gets destroyed because it went out of scope, and the 
    # former default pool is reinstated
  }

  # now $pool is the default pool, so it will be used for any function that 
  # needs a pool.
}

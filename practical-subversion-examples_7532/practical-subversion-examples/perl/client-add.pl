sub add_file {
  my $target = shift;

  # first we create a client object
  my $client = SVN::Client->new (
    # specify a notification callback
    notify => sub {
      my ($path, $action, $nodetype, $mime, $state, $rev) = @_;

      # since all we're doing is adding, just print out something for
      # the add case
      if ($action == $SVN::Wc::Notify::Action::add) {
        print "added $path\n";
      }
    }
  );

  # recursive add target using the default pool
  $client->add ($target, 1);
}

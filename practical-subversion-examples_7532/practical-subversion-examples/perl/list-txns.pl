use SVN::Core;
use SVN::Repos;
use SVN::Fs;

sub list_txns {
  my $path = shift;

  # open the repository
  my $repos = SVN::Repos::open ($path);

  # grab the underlying filesystem object
  my $fs = $repos->fs;

  # list all the transactions
  my $txns = $fs->list_transactions;

  # and print them out
  for my $txn @{$txns} {
    print "$txn\n";
  }
}

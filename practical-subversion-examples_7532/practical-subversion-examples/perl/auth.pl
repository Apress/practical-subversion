sub build_auth_provider {
  # create an authentication baton with providers for svn.simple and
  # svn.username
  my $auth_baton = SVN::Core::auth_open ([
    SVN::Client::get_simple_provider (),
    SVN::Client::get_simple_prompt_provider (\&simple_prompt, 2),
    SVN::Client::get_username_provider ()
  ]);

  return $auth_baton;
}

sub simple_prompt {
  my ($cred, $realm, $def_username, $may_save, $pool) = @_;

  # show the user the realm and prompt for his username
  print "Realm = $realm\n";
  print "Username : ";

  # read in the username
  my $username = <>;
  chomp $username;

  # set the username in the credentials
  $cred->username($username);

  # prompt for the username
  print "Password : ";

  # read the password
  my $password = <>;
  chomp $password;

  # set the password in the credentials
  $cred->password($password);
}

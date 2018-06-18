#!../duck

!!include "vars.d" /* variables */

my($boo,$moo,$doo);

sub fish
{
  print ("The fish \nsaid `",$arg[1],"' ",$arg[0]," times.\n");
}
/* the main clause */

sub init
{
  print ("start\n----\n");

  $moo:="MOO!";
  print ("moo after set ",$moo);
  print ("\n");
  print (4.3,", ",3,", ",5,"\n");
  print ("after some prints $moo\n");

  $moo := PI * 2;
//  $boo := copy ( $moo );
  fish ($moo, "i love cows, they go moo...");
}

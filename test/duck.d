#!../duck

sub bw
{
  $a:=$arg[0];
  for($i:=#a-1;$i>=0;$i:=$i-1) $ret+=$a[$i];
  return $ret;
}

sub init
{
/*
  $a:="i like sheep, they go baa";

  print("$a\n");
  print(bw($a)+"\n\n");

  $a:=join("",split(",",$a));
  $a:=ucase(join("-",split(" ",$a)));

  print("$a\n");
*/
/*
  if(($file:=open(">> moo.txt"))==NULL)
  {
    print("noooooo\n");
    return;
  }
  print($file, "hello\n");
  close($file);

  if($file:=open("< moo.txt"))
  {
    print("moo.txt:\n");
    while($line:=readln($file))
      print("line: $line");
    close($file);
  }
*/
/*
  $m:=chomp(readln);
  print ("$m",,"MOO");
*/
/*
   print(sin(PI),"\n");
   print(PI,"\n");
   print(floor(PI+0.5)," f\n");
   print(ceil(PI+0.5)," c\n");
   print(round(PI+0.5)," r\n");
*/
/*
   $m:=("one","two");
   $m[3]:=?m;
   $p:=?m;
   print(@p[0]," ",#m,"\n");
*/

  $a:=0;
  do print(($a += 1)," "); while ($a<3);

}

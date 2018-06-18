#!../duck

sub init
{
  $dict:=open("</usr/share/dict/words");
  $new:=open(">words.txt");
  while($word:=readln($dict))
    print($new,$word);
  close($dict);
  close($new);
}

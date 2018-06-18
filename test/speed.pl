#!/usr/bin/perl

my($word);

open DICT, "</usr/share/dict/words";
open NEW, ">words.txt";
while($word=<DICT>)
{
  print(NEW $word);
}
close DICT;
close NEW;

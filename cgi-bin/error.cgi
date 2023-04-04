#!/usr/bin/perl
use strict;
use warnings;

print "Content-type: text/html\n\n";
print "<html><body>\n";
print "<h1>This is a test</h1>\n";

my $variable_with_typo = "Hello, world!";
print "<p>$varible_with_typo</p>\n"; # intentional typo

print "</body></html>\n";

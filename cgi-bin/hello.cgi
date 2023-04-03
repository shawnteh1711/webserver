#!/usr/bin/perl
use strict;
use warnings;
 
print "<h1>Environment Variables:</h1>\n";

print "<ul>\n";
print "<li>REQUEST_METHOD: $ENV{REQUEST_METHOD}</li>\n";
print "<li>QUERY_STRING: $ENV{QUERY_STRING}</li>\n";
print "<li>CONTENT_TYPE: $ENV{CONTENT_TYPE}</li>\n";
print "<li>CONTENT_LENGTH: $ENV{CONTENT_LENGTH}</li>\n";
print "<li>REMOTE_ADDR: $ENV{REMOTE_ADDR}</li>\n";
print "<li>SCRIPT_NAME: $ENV{SCRIPT_NAME}</li>\n";
print "<li>SCRIPT_PATH: $ENV{SCRIPT_PATH}</li>\n";
print "</ul>\n";

#!/usr/bin/perl
use strict;
use warnings;

print "<html>\n";
print "<head>\n";
print "<title>Environment Variables</title>\n";
print "<style>\n";
print "body { font-family: Arial, sans-serif; }\n";
print "h1 { color: #003366; }\n";
print "ul { list-style-type: none; }\n";
print "li { margin-bottom: 10px; }\n";
print "label { font-weight: bold; }\n";
print "</style>\n";
print "</head>\n";
print "<body>\n";
print "<h1>Environment Variables:</h1>\n";

print "<ul>\n";
print "<li>REQUEST_METHOD: $ENV{REQUEST_METHOD}</li>\n";
print "<li>QUERY_STRING: $ENV{QUERY_STRING}</li>\n";
print "<li>CONTENT_TYPE: $ENV{CONTENT_TYPE}</li>\n";
print "<li>CONTENT_LENGTH: $ENV{CONTENT_LENGTH}</li>\n";
print "<li>REMOTE_ADDR: $ENV{REMOTE_ADDR}</li>\n";
print "<li>SCRIPT_NAME: $ENV{SCRIPT_NAME}</li>\n";
print "<li>SCRIPT_PATH: $ENV{SCRIPT_PATH}</li>\n";
print "<li>HTTP_COOKIE: $ENV{HTTP_COOKIE}</li>\n";
print "<li>HTTP_COOKIES_MAP: $ENV{HTTP_COOKIES_MAP}</li>\n";
print "</ul>\n";


my $session_id = "";
if ($ENV{'REQUEST_METHOD'} eq 'POST') {
    my $form_data = $ENV{'QUERY_STRING'};
    $session_id = $form_data =~ /session_id=(.*?)(&|$)/ ? $1 : '';
}
# my $session_id = $ENV{'HTTP_COOKIES_MAP'}; # use HTTP_COOKIE instead of QUERY_STRING
# $session_id =~ s/.*session_id=([^;]+).*/$1/;


print "<h1>Enter session ID:</h1>\n";
print "<form method=\"post\" action=\"hello\">\n";
print "<input type=\"text\" name=\"session_id\" value=\"$session_id\"/>\n";
print "<input type=\"submit\" value=\"Submit\" />\n";

if ($session_id) {
    print "<h1>Session ID submitted:</h1>\n";
    print "<p>$session_id</p>\n";
}
print "</form>\n";
print "</html>\n";


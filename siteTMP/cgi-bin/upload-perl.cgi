#!/usr/bin/perl
use strict;
use warnings;
use CGI;

my $q = CGI->new();
print $q->header();
if ($q->param('upload')) {
    my $filename = $q->param('upload');
    my $fh = $q->upload('upload');
    if ($fh) {
        open(my $upload_file, '>', "/usr/share/nginx/html/upload/$filename") or die "Can't open file $filename: $!";
        binmode $upload_file;
        while (<$fh>) {
            print $upload_file $_;
        }
        close($upload_file);
        print "File $filename uploaded successfully!";
    } else {
        print "Error uploading file: " . $q->cgi_error();
    }
} else {
    print <<HTML;
    <html>
        <body>
            <form method="post" enctype="multipart/form-data">
                <input type="file" name="upload">
                <input type="submit" value="Upload">
            </form>
        </body>
    </html>
HTML
}

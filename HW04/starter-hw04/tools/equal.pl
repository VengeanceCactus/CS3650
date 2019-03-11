#!/usr/bin/perl
use 5.20.0;
use warnings FATAL => 'all';

use FindBin qw($Bin);
use lib $Bin;

use Matrix qw(read_matrix_from equal_matrix);

my ($m0f, $m1f) = @ARGV;
($m0f && $m1f) or die;

open my $m0h, "<", $m0f or die;
open my $m1h, "<", $m1f or die;

my $m0 = read_matrix_from($m0h) or die;
my $m1 = read_matrix_from($m1h) or die;

if (equal_matrix($m0, $m1)) {
    say "ok";
}
else {
    say "matrices not equal";
}

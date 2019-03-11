#!/usr/bin/perl
use 5.20.0;
use warnings FATAL => 'all';

use Getopt::Std;

BEGIN { use File::Basename; push @INC, dirname(__FILE__); }
use Matrix qw(gen_matrix ordered_matrix print_matrix);

our %opts;
getopts('o', \%opts);

my $NN = shift or die;
$NN = 0+$NN;

my $rows;
if ($opts{'o'}) {
    $rows = ordered_matrix($NN);
}
else {
    $rows = gen_matrix($NN);
}

print_matrix($rows);

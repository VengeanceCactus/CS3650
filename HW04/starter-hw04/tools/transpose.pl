#!/usr/bin/perl
use 5.20.0;
use warnings FATAL => 'all';

use FindBin qw($Bin);
use lib $Bin;

use Matrix qw(read_matrix transpose print_matrix);

my $m0 = read_matrix();
my $m1 = transpose($m0);
print_matrix($m1);


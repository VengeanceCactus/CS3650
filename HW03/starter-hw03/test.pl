#!/usr/bin/perl
use v5.16.0;
use warnings FATAL => 'all';
use autodie qw(:all);

use Test::Simple tests => 10;
use IO::Handle;

our $cmd = "sort";

sub ref_sort {
    my ($xs) = @_;
    my @ys = sort { $a <=> $b } @$xs;
    return \@ys;
}

sub run_test {
    my ($name, $nums) = @_;
    #say "input: @$nums";

    my $temp = "/tmp/nums.$$.txt";
    open my $ns, ">", $temp;
    $ns->say(scalar @$nums);
    for my $nn (@$nums) {
        $ns->say($nn);
    }
    close $ns;

    my @outs = ();
    open my $msort, "-|", qq{timeout -s 9 5 ./"$cmd" < "$temp"};
    while (<$msort>) {
        chomp;
        next unless /\d/;
        s/^\s+//;
        s/\s+$//;
        my @xs = split(/\s+/, $_);
        push @outs, \@xs;
    }
    system(qq{rm "$temp"});

    my ($nn, $sorted) = @outs;
    my $ground = ref_sort($nums);

    #say "ground: @$ground";
    #say "sorted: @$sorted";

    if (scalar @outs > 1 && scalar @$nn > 0) {
        ok($nn->[0] == scalar @$nums, "$name count correct");
        ok("@$ground" eq "@$sorted", "$name sorted");
    }
    else {
        ok(0, "$name: bad output");
        ok(0, "$name: bad output");
    }
}

run_test("1 to 5", [1,3,4,2,5]);
run_test("just 30", [30]);
run_test("30 numbers", [map {int(rand(100))} (1..30)]);
run_test("8 numbers", [80, 10, 70, 20, 50, 30, 50, 40]);

my $nn = 3 + int(rand(12));
run_test("random = $nn", [map {int(rand(100))} (1..$nn)]);


#!/usr/bin/perl
use v5.16.0;
use warnings FATAL => 'all';
use autodie qw(:all);

use Test::Simple tests => 25;
use IO::Handle;

our $ctr = 0;

sub run_test {
    my ($cmd, $ordered, $size) = @_;

    $ctr++;
    my $inp = "/tmp/matrix-inp.$$-$ctr.txt";
    my $out = "/tmp/matrix-out.$$-$ctr.txt";
    my $ref = "/tmp/matrix-ref.$$-$ctr.txt";

    $ordered = $ordered ? "-o" : "";

    system(qq{./tools/gen-matrix.pl $ordered $size > "$inp"});

    my @outs = ();
    my $rv = system(qq{timeout -s 9 5 ./"$cmd" < "$inp" > "$out"});

    system(qq{./tools/transpose.pl < "$inp" > "$ref"});
    my $ok = `./tools/equal.pl "$out" "$ref"`;

    if ($ok =~ /^ok$/) {
        return 1;
    }
    else {
        $ok =~ s/^/#/mg;
        say $ok;
        return 0;
    }
}

my $nn;

for my $ii (1..5) {
    ok(run_test("ctp", 0, 1), "c: single item $ii");
}

ok(run_test("ctp", 1, 2), "c: -o 2");
ok(run_test("ctp", 0, 2), "c: 2");
ok(run_test("ctp", 1, 7), "c: -o 7");
ok(run_test("ctp", 0, 7), "c: 7");
ok(run_test("ctp", 0, 34), "c: 34");

for (1..5) {
    $nn = 10 + int(rand 17);
    ok(run_test("ctp", 0, $nn), "c: $nn");
}

for my $ii (1..5) {
    ok(run_test("atp", 0, 1), "asm: single item $ii");
}

ok(run_test("atp", 1, 2), "a -o 2");
ok(run_test("atp", 0, 2), "a 2");
ok(run_test("atp", 1, 7), "a -o 7");
ok(run_test("atp", 0, 7), "a 7");
$nn = 10 + int(rand 17);
ok(run_test("atp", 0, $nn), "a $nn");

system("rm -f /tmp/matrix-*$$*");

package Matrix;
use 5.20.0;
use warnings FATAL => 'all';

require Exporter;
our @ISA = qw(Exporter);
our @EXPORT_OK = qw(
    read_matrix read_matrix_from print_matrix
    gen_matrix ordered_matrix transpose
    equal_matrix
);

use Clone qw(clone);

sub read_matrix {
    return read_matrix_from(*STDIN);
}

sub read_matrix_from {
    my ($fd) = @_;
    my $rows = [];
    while (<$fd>) {
        my @cols = split /\s+/;
        push @$rows, \@cols;
    }
    return $rows;
}

sub print_matrix {
    my ($rows) = @_;
    for my $cols (@$rows) {
        say(join(" ", @$cols));
    }
}

sub gen_matrix {
    my ($nn) = @_;
    my $rows = [];
    for (1..$nn) {
        my $cols = [];
        for (1..$nn) {
            my $xx = 1 + int(rand(199));
            push(@$cols, $xx);
        }
        push @$rows, $cols;
    }
    return $rows;
}

sub ordered_matrix {
    my ($nn) = @_;
    my $rows = [];
    for my $ii (1..$nn) {
        my $cols = [];
        for my $jj (1..$nn) {
            my $xx = 10*$ii + $jj;
            push(@$cols, $xx);
        }
        push @$rows, $cols;
    }
    return $rows;
}

sub transpose {
    my ($rows0) = @_;
    my $nn = scalar @$rows0;
    my $rows1 = clone($rows0);
    for (my $jj = 0; $jj < $nn; ++$jj) {
        if (scalar @{$rows0->[$jj]} != $nn) {
            die "Not a square matrix.";
        }
        for (my $ii = 0; $ii < $nn; ++$ii) {
            $rows1->[$jj][$ii] = $rows0->[$ii][$jj];
        }
    }
    return $rows1;
}

sub equal_matrix {
    my ($m0, $m1) = @_;
    if (scalar @$m0 != scalar @$m1) {
        say "not equal: row count";
        return 0;
    }
    my $nn = scalar @$m0;
    for (my $ii = 0; $ii < $nn; ++$ii) {
        my $row0 = $m0->[$ii];
        my $row1 = $m1->[$ii];
        if (scalar @$row0 != scalar @$row1) {
            say "not equal: row $ii size";
            return 0;
        }
        my $mm = scalar @$row0;
        for (my $jj = 0; $jj < $mm; ++$jj) {
            if ($row0->[$jj] != $row1->[$jj]) {
                my $xx = $row0->[$jj];
                my $yy = $row1->[$jj];
                say "not equal: $xx != $yy @ $ii,$jj";
                return 0;
            }
        }
    }
    return 1;
}

1;


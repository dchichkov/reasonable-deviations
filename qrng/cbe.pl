#!/usr/bin/perl

if ($#ARGV != 0) {
 print "usage cbe.pl <input file>\n";
 exit;
}

$N = 0; $TOTAL = 0;

open (FILE, $ARGV[0]);
chomp;

while (<FILE>) {
	chomp;
	($C, $M, $LB, $RB, $CC, $T, $S) = split(", ");
	$N = $N + (($M > 0.5)? 1 : -1);
	$TOTAL++;
}
print "N = $N, TOTAL = $TOTAL\n";
close (FILE);
exit;

#!/usr/bin/perl

use strict;
use IO::Socket;

my $line = '';
my $socket = new IO::Socket::INET(PeerAddr => $ARGV[0],
                                  PeerPort => '50003',
                                  Proto    => 'tcp');

while(1 > 0) {
        print $socket "GET value 0";

        if($line = <$socket>) {
                if($line = <$socket>) {
                        print "Kanal 0: $line";
                }
        }

#        print $socket "GET value 1";
#
#        if($line = <$socket>) {
#                if($line = <$socket>) {
#                        print "Kanal 1: $line";
#                }
#        }

        sleep(1);
}


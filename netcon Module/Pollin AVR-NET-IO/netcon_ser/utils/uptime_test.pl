#!/usr/bin/perl

use strict;
use IO::Socket;

my $line = '';
my $socket = new IO::Socket::INET(PeerAddr => $ARGV[0],
                                  PeerPort => '50003',
                                  Proto    => 'tcp');

while(1 > 0) {
        print $socket "get uptime";
        
        if(<$socket>) {
                if($line = <$socket>) {
                        print $line;
                }
        }

        sleep(1);
}


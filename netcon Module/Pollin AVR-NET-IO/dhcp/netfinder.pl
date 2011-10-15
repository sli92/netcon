#!/usr/bin/perl -w
#
# Datei:                netfinder.pl
# Author:               dev00
# Beschreibung:         Dient zum Aufspueren von netcon Modulen
#

use strict;
use IO::Socket::INET;
use Net::Address::IP::Local;

my $buffer;
my $remote_addr;
my $socket = IO::Socket::INET->new(PeerAddr => '255.255.255.255:6799',
                                   Proto => 'udp',
                                   LocalAddr => Net::Address::IP::Local->public,
                                   LocalPort => 6800,
                                   Reuse => 1,
                                   Broadcast => 1) or die "Can't bind: $@\n";

my $broadcast_socket = IO::Socket::INET->new(Proto => 'udp',
                                             LocalPort => 6800,
                                             Reuse => 1,
                                             Broadcast => 1) or die "Can't bind: $@\n";

$socket->send("bcnetcon");

while(1) {
        $broadcast_socket->recv($buffer, 128);

        $remote_addr = $broadcast_socket->peerhost();
        print "$remote_addr: $buffer\n";
}


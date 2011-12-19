#!/usr/bin/perl -w
#
# Datei:                modul.pl
# Author:               dev00
# Beschreibung:         Sendet eine netfind Antwort als Broadcast
#

use strict;
use IO::Socket::INET;
use Net::Address::IP::Local;

my $buffer;
my $remote_addr;
my $send_socket = IO::Socket::INET->new(PeerAddr => '255.255.255.255:50001',
                                        Proto => 'udp',
                                        LocalAddr => Net::Address::IP::Local->public,
                                        LocalPort => 50000,
                                        Reuse => 1,
                                        Broadcast => 1) or die "Can't bind: $@\n";


my $recv_socket = IO::Socket::INET->new(Proto => 'udp',
                                        LocalPort => 50000,
                                        Reuse => 1,
                                        Broadcast => 1) or die "Can't bind: $@\n";

while(1) {
        $recv_socket->recv($buffer, 128);
}


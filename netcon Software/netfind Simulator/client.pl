#!/usr/bin/perl -w
#
# Datei:                client.pl
# Author:               dev00
# Beschreibung:         Sendet eine netfind Anfrage als Broadcast
#

use strict;
use IO::Socket::INET;
use Net::Address::IP::Local;

my $buffer;
my $remote_addr;
my $send_socket = IO::Socket::INET->new(PeerAddr => '255.255.255.255:50000',
                                        Proto => 'udp',
                                        LocalAddr => Net::Address::IP::Local->public,
                                        LocalPort => 50001,
                                        Reuse => 1,
                                        Broadcast => 1) or die "Can't bind: $@\n";


my $recv_socket = IO::Socket::INET->new(Proto => 'udp',
                                             LocalPort => 50001,
                                             Reuse => 1,
                                             Broadcast => 1) or die "Can't bind: $@\n";

print "Fetching Modules:\n";

$send_socket->send("netfind\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF");

while(1) {
        $recv_socket->recv($buffer, 128);
}


//
// Copyright (C) 2008 Andras Varga
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
//

#ifndef __IROUTINGTABLE_H
#define __IROUTINGTABLE_H

#include <vector>
#include <omnetpp.h>
#include "INETDefs.h"
#include "IPAddress.h"

class IPv4Route;

/** Returned as the result of multicast routing */
struct MulticastRoute
{
    InterfaceEntry *interf;
    IPAddress gateway;
};
typedef std::vector<MulticastRoute> MulticastRoutes;


/**
 * A C++ interface to abstract the functionality of RoutingTable.
 * Referring to RoutingTable via this interface makes it possible to
 * transparently replace RoutingTable with a different implementation,
 * without any change to the base INET.
 *
 * @see RoutingTable, IPv4Route
 */
class INET_API IRoutingTable
{
  public:
    virtual ~RoutingTable() {};

    /**
     * For debugging
     */
    virtual void printRoutingTable() const = 0;

    /** @name Interfaces */
    //@{
    virtual void configureInterfaceForIPv4(InterfaceEntry *ie) = 0;

    /**
     * Returns an interface given by its address. Returns NULL if not found.
     */
    virtual InterfaceEntry *getInterfaceByAddress(const IPAddress& address) const = 0;
    //@}

    /**
     * IP forwarding on/off
     */
    virtual bool isIPForwardingEnabled() = 0;

    /**
     * Returns routerId.
     */
    virtual IPAddress getRouterId() = 0;

    /**
     * Sets routerId.
     */
    virtual void setRouterId(IPAddress a) = 0;

    /** @name Routing functions (query the route table) */
    //@{
    /**
     * Checks if the address is a local one, i.e. one of the host's.
     */
    virtual bool isLocalAddress(const IPAddress& dest) const = 0;

    /**
     * The routing function.
     */
    virtual const IPv4Route *findBestMatchingRoute(const IPAddress& dest) const = 0;

    /**
     * Convenience function based on findBestMatchingRoute().
     *
     * Returns the interface Id to send the packets with dest as
     * destination address, or -1 if destination is not in routing table.
     */
    virtual InterfaceEntry *getInterfaceForDestAddr(const IPAddress& dest) const = 0;

    /**
     * Convenience function based on findBestMatchingRoute().
     *
     * Returns the gateway to send the destination. Returns null address
     * if the destination is not in routing table or there is
     * no gateway (local delivery).
     */
    virtual IPAddress getGatewayForDestAddr(const IPAddress& dest) const = 0;
    //@}

    /** @name Multicast routing functions */
    //@{

    /**
     * Checks if the address is in one of the local multicast group
     * address list.
     */
    virtual bool isLocalMulticastAddress(const IPAddress& dest) const = 0;

    /**
     * Returns routes for a multicast address.
     */
    virtual MulticastRoutes getMulticastRoutesFor(const IPAddress& dest) const = 0;
    //@}

    /** @name Route table manipulation */
    //@{

    /**
     * Returns the total number of routes (unicast, multicast, plus the
     * default route).
     */
    virtual int getNumRoutes() const = 0;

    /**
     * Returns the kth route. The returned route cannot be modified;
     * you must delete and re-add it instead. This rule is emphasized
     * by returning a const pointer.
     */
    virtual const IPv4Route *getRoute(int k) const = 0;

    /**
     * Find first routing entry with the given parameters.
     */
    virtual const IPv4Route *findRoute(const IPAddress& target, const IPAddress& netmask,
        const IPAddress& gw, int metric = 0, const char *dev = NULL) const = 0;

    /**
     * Adds a route to the routing table. Note that once added, routes
     * cannot be modified; you must delete and re-add them instead.
     */
    virtual void addRoute(const IPv4Route *entry) = 0;

    /**
     * Deletes the given route from the routing table.
     * Returns true if the route was deleted correctly, false if it was
     * not in the routing table.
     */
    virtual bool deleteRoute(const IPv4Route *entry) = 0;

    /**
     * Utility function: Returns a vector of all addresses of the node.
     */
    virtual std::vector<IPAddress> gatherAddresses() const = 0;
    //@}

};

#endif

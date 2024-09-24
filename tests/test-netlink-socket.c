/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <config.h>

#include <stdlib.h>
#include <linux/netfilter/nfnetlink.h>
#include <linux/rtnetlink.h>
#include <linux/if.h>

#include "netlink.h"
#include "netlink-socket.h"
#include "ovstest.h"

static int
try_find_link(const char *netns) {
    struct ofpbuf request, *reply;
    int err;
    ofpbuf_init(&request, 0);
    nl_msg_put_nlmsghdr(&request, 0, RTM_GETLINK, NLM_F_REQUEST);
    ofpbuf_put_zeros(&request, sizeof(struct ifinfomsg));
    nl_msg_put_string(&request, IFLA_IFNAME, "vrf1337");
    err = nl_ns_transact(netns, NETLINK_ROUTE, &request, &reply);
    ofpbuf_uninit(&request);
    ofpbuf_delete(reply);
    return err;
}

static void
test_netlink_socket_main(int argc OVS_UNUSED, char *argv[] OVS_UNUSED)
{
    ovs_assert(try_find_link(NULL) != 0);
    ovs_assert(try_find_link("ns1337") == 0);
}

OVSTEST_REGISTER("test-netlink-socket", test_netlink_socket_main);

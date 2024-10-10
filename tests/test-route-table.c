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

#include "route-table.h"
#include "ovstest.h"

static void
count_br_test(const struct route_table_msg *msg, void *data)
{
    int *count = data;
    if (msg->rd.plen != 16) {
        return;
    }

    for (int i = 0; i < msg->rd.n_nexthops; i++) {
        if (!strcmp(msg->rd.nexthops[i].ifname, "br-test")) {
            *count = *count + 1;
        }
    }
}

static void
test_route_table_main(int argc OVS_UNUSED, char *argv[] OVS_UNUSED)
{
    int count = 0;
    route_table_dump_one_table("ns1337", 0, count_br_test, &count);
    ovs_assert(count == 2);
}

OVSTEST_REGISTER("test-route-table", test_route_table_main);

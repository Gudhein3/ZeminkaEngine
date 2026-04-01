#include <zeminka/engine.h>
#include <stdio.h>

typedef struct {
    void *data;
    bool is_freeless;
    size_t intf_idx;
} Ent__Data;

typedef struct {
    Ent__Data *items;
    size_t count, capacity;
} Ents;

typedef struct {
    ZEEnt_intf *items;
    size_t count, capacity;
} Intfs;

Ents ent_list = {0};
Intfs intfs_list = {0};

static ZEEnt_ent ent_current = -1;
static f64 lastest_pu = 0;

void ZEEnt_register_intf(ZEEnt_intf intf) {
    da_append(&intfs_list, intf);
}

ZEEnt_ent ZEEnt_add(s32 intf_id, void *_Nullable arg) {
    size_t pos = 0;

    ZEEnt_intf *intf = NULL;
    size_t intf_idx;

    for (intf_idx = 0; intf_idx < intfs_list.count; ++intf_idx) {
        if (intfs_list.items[intf_idx].intf_id == intf_id) {
            intf = &intfs_list.items[intf_idx];
            goto intf_found;
        }
    }

    ZELog(ZELOG_FATAL, "Failed to add a entity with interface id #%d: The interface is not in the list.\n", intf_id);

intf_found:

    Ent__Data ent;

    ent.data = intf->alloc(pos, arg);
    ent.is_freeless = true;
    ent.intf_idx = intf_idx;

    if (ent_list.count + 1 >= ent_list.capacity) {
        for (size_t i = 0; i < ent_list.count; ++i) {
            if (!ent_list.items[i].is_freeless) {
                ent_list.items[i] = ent;
                pos = i;
                goto appended;
            }
        }
    }

    da_append(&ent_list, ent);
    pos = ent_list.count-1;

appended:

    return pos;
}

void ZEEnt_destroy(ZEEnt_ent id) {
    if (id == ent_current) {
        ZELog(ZELOG_FATAL, "Entity system: entity %d tried to destroy itself\n", id);
    }
    if (id < 0 || id > ent_list.count || !ent_list.items[id].is_freeless) return;
    intfs_list.items[ent_list.items[id].intf_idx].dealloc(ent_list.items[id].data, id);
    ent_list.items[id].is_freeless = false;
}

void *_Nullable ZEEnt_getdata(ZEEnt_ent id) {
    if (id < 0 || id > ent_list.count || !ent_list.items[id].is_freeless) return NULL;
    return ent_list.items[id].data;
}

s32 ZEEnt_get_intfid(ZEEnt_ent ent) {
    if (ent < 0 || ent > ent_list.count || !ent_list.items[ent].is_freeless) return -1;
    return intfs_list.items[ent_list.items[ent].intf_idx].intf_id;
}

void ZEEnt_update(void) {
    if (ent_current != -1) return; // Entities shouldn't call update loop
    bool isFixed = ((s32)(ZEdeltaTime*10.)) % 3 == 0;
    if (isFixed) {
        if (lastest_pu != 0) {
            ZEdeltaTime30Hz = ZEsystemTime-lastest_pu;
            ZEdeltaTime = ZEdeltaTime30Hz;
        }
        lastest_pu = ZEsystemTime;
    }
    for (size_t i = 0; i < ent_list.count; ++i) {
        if (ent_list.items[i].is_freeless) {
            ent_current = i;
            ZEEnt_intf *intf = &intfs_list.items[ent_list.items[i].intf_idx];
            intf->onmsg(ent_list.items[i].data, i, -1, ZEENT_MSG_UPDATE, NULL);
            if (isFixed) {
                intf->onmsg(ent_list.items[i].data, i, -1, ZEENT_MSG_30Hz_UPDATE, NULL);
            }
            intf->onmsg(ent_list.items[i].data, i, -1, ZEENT_MSG_RENDER, NULL);
        }
    }
    ent_current = -1;
}

void ZEEnt_send(ZEEnt_Msg_Kind msg_kind, ZEEnt_ent callee, void *msg_data) {
    if (callee < 0 || callee > ent_list.count || !ent_list.items[callee].is_freeless) return;
    ZEEnt_ent caller = ent_current;
    ent_current = callee;
    intfs_list.items[ent_list.items[callee].intf_idx].onmsg(ent_list.items[callee].data, callee, caller, msg_kind, msg_data);
    ent_current = caller;
}

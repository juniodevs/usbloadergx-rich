#ifndef _DISCORD_RPC_H_
#define _DISCORD_RPC_H_

#include <gctypes.h>

#ifdef __cplusplus
extern "C" {
#endif

// Command 0x00: Update Presence
void Discord_UpdatePresence(const char* titleId);

// Command 0x01: Clear Presence
void Discord_ClearPresence();

#ifdef __cplusplus
}
#endif

#endif

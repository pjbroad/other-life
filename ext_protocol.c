#include <SDL_types.h>
#include "errors.h"
#include "ext_protocol.h"
#include "multiplayer.h"
#include "stats.h"
#include "text.h"

void handle_extended_protocol_message(const Uint8 *in_data, int data_length)
{
	char info[1024];
	int i;

	switch (in_data[0])
	{
		case OL_GENERIC_DATA:
			{
				Uint8 action;
				Uint16 group, subgroup, start, data_type, a_length, a_width;

				action = in_data[3];
				group = SDL_SwapLE16(*(Uint16 *)(in_data+4));
				subgroup = SDL_SwapLE16(*(Uint16 *)(in_data+6));
				start = SDL_SwapLE16(*(Uint16 *)(in_data+8));
				data_type = SDL_SwapLE16(*(Uint16 *)(in_data+10));
				a_length = SDL_SwapLE16(*(Uint16 *)(in_data+12));
				a_width = SDL_SwapLE16(*(Uint16 *)(in_data+14));
#if 0
				safe_snprintf(info, sizeof(info), "Got OL_GENERIC_DATA message. LENGTH: %u/%i, Action: %u, GROUP: %u, SUBGROUP: %u, Start Entry: %u, Data type: %u, ARRAY Length: %u, ARRAY Width: %u",SDL_SwapLE16(*(Uint16 *)(in_data+1)), data_length, action, group, subgroup, start, data_type, a_length, a_width);
				LOG_TO_CONSOLE(c_green1, info);
				for(i=0; i<SDL_SwapLE16(*(Uint16 *)(in_data+12)); i++)
				{
					safe_snprintf(info, sizeof(info), "%u", SDL_SwapLE32(*(Uint32 *)(in_data+16+i*4)));
					LOG_TO_CONSOLE(c_green1, info);
				}
#endif
				if(action == 1 && group == 0 && subgroup == 16)
				{
					if(a_length*a_width > data_length-16)
					{
						LOG_WARNING("CAUTION: Possibly forged x-attrib packet received.\n");
						break;
					}
					if (a_length > 1)
						get_xattribs(a_length, (Uint16 *)(in_data+16));
					else
						get_partial_xattribs(start, (Uint16 *)(in_data+16));
				}
			}
			break;
		case OL_EXTENDED_PACKET:
#if 0
			safe_snprintf(info, sizeof(info), "Got OL_EXTENDED_PACKET message. LENGTH: %u/%i, Ext Type: %u",SDL_SwapLE16(*(Uint16 *)(in_data+1)), data_length, SDL_SwapLE16(*(Uint16 *)(in_data+3)));
			LOG_TO_CONSOLE(c_green1, info);
#endif
			break;
		case OL_COMPRESSED_PACKET:
#if 0
			safe_snprintf(info, sizeof(info), "Got OL_COMPRESSED_PACKET message. LENGTH: %u/%i, FLAGS: %u",SDL_SwapLE16(*(Uint16 *)(in_data+1)), data_length, in_data[3]);
			LOG_TO_CONSOLE(c_green1, info);
#endif
			break;
		case OL_OLD_EL_PACKET:
			{
				Uint8 *new_data;
#if 0
				safe_snprintf(info, sizeof(info), "Got OL_OLD_EL_PACKET message. LENGTH: %u/%i, EL Packet Type: %u",SDL_SwapLE16(*(Uint16 *)(in_data+1)), data_length, in_data[3]);
				LOG_TO_CONSOLE(c_green1, info);
#endif
				new_data = malloc(data_length-1);
				new_data[0] = in_data[3];
				if(SDL_BYTEORDER == SDL_LIL_ENDIAN)
					*((Uint16 *)(new_data+1)) = data_length-3;
				else
					*((Uint16 *)(new_data+1)) = SDL_Swap16(data_length-3);
				if(data_length > 4)
					memcpy(&new_data[3], &in_data[4], data_length-3);
#if 0
				safe_snprintf(info, sizeof(info), "Generated EL message. LENGTH: %u/%i, EL Packet Type: %u",SDL_SwapLE16(*(Uint16 *)(new_data+1)), data_length-1, new_data[0]);
				LOG_TO_CONSOLE(c_green1, info);
#endif
				process_message_from_server(new_data, data_length-1);
				free(new_data);
			}
			break;
		default:
			process_message_from_server(in_data, data_length);
	}
}
/*!
 * \file
 * \ingroup ext_protocol
 * \brief OL extended protocol handling.
 */
#ifndef __EXT_PROTOCOL_H__
#define __EXT_PROTOCOL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
	OL specific commands
*/
#define OL_GENERIC_DATA 222
#define OL_EXTENDED_PACKET 227
#define OL_COMPRESSED_PACKET 228
#define OL_OLD_EL_PACKET 229

/*!
 * \ingroup ext_protocol
 * \brief       generic OL extended protocol handler.
 *
 *      Generic OL extended protocol handler
 *
 */
void handle_extended_protocol_message(const Uint8 *in_data, int data_length);


#ifdef __cplusplus
} // extern "C"
#endif

#endif

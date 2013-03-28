//////////////////////////////// state.h ////////////////////////////
//
//
//
//
//

#ifndef STATE_H_
#define STATE_H_

/*

STATE			EVENT			ACTION					STATE

INIT							act_start				WF_SOF
WF_SOF			tmr_timout		act_frame_started		WF_UNI
				rx_pin_falling	act_reset_timer			WF_SOF

WF_UNI			tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_uni			WF_SEQ

WF_SEQ			tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_seq			WF_TYPE

WF_TYPE			tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_type		WF_TTL

WF_TTL			tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_type		WF_LEN

WF_LEN			tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_len			WF_PAYLOAD

WF_PAYLOAD		tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_payload		WF_PAYLOAD		counter > 0
														WF_TS-LO		counter = 0, T = 1
														WF_CRC-LO		counter = 0, T = 0

WF_TS-LO		tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_ts_lo		WF_TS-HI

WF_TS-HI		tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_ts_hi		WF_CRC-LO

WF_CRC-LO		tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_crc_lo		WF_CRC-HI

WF_CRC-HI		tmr_timout		act_frame_error			WF_SOF
				rx_char			act_rx_char_crc_hi		PARSE_FRAME

PARSE_FRAME		frame_error		act_frame_error			WF_SOF
				frame_ok		act_frame_ok			WF_SOF

*/

#endif /* STATE_H_ */
